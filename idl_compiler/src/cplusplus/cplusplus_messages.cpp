/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
* Copyright (c) 2020-2021, OLogN Technologies AG
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*	 * Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	 * Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in the
*	   documentation and/or other materials provided with the distribution.
*	 * Neither the name of the OLogN Technologies AG nor the
*	   names of its contributors may be used to endorse or promote products
*	   derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL OLogN Technologies AG BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------------------*/

#include "cplusplus_idl_tree_serializer.h"
#include "idl_tree_common.h"

namespace cplusplus
{


string impl_MessageNameToDefaultsNamespaceName( string name )
{
	return fmt::format( "Message_{}_defaults", name );
}

std::string impl_generateComposeFunctionName( CompositeType& s )
{
	return fmt::format( "{}_{}_compose", s.type2string(), s.name );
}

std::string impl_generateParseFunctionName( CompositeType& s )
{
	return fmt::format( "{}_{}_parse", s.type2string(), s.name );
}


void impl_generateScopeHandler( FILE* header, Scope& scope, const std::string& parserType )
{
	fprintf( header, 
		"template<class ... HandlersT >\n"
		"void implHandleMessage( %s& parser, HandlersT ... handlers )\n"
		"{\n", parserType.c_str()
	);


	

	fprintf(header, "\tparser.structBegin();\n");
	fprintf(header, "\tparser.namedParamBegin(\"msgid\");\n");
	fprintf(header, "\tuint64_t msgID = parser.parseUnsignedInteger();\n");
	fprintf(header, "\tparser.nextElement();\n");
	fprintf(header, "\tparser.namedParamBegin(\"msgbody\");\n");

	fprintf( header, "\tbool ok = false;\n" );
	fprintf( header, "\n" );

	fprintf( header, "\tswitch ( msgID )\n" );
	fprintf( header, "\t{\n" );
	
	for ( auto msg : scope.objectList )
		fprintf( header, "\t\tcase %s::id: ok = ::globalmq::marshalling::impl::implHandleMessage<%s>( parser, handlers... ); break;\n", msg->name.c_str(), msg->name.c_str() );
	fprintf( header, "\t\tdefault: ::globalmq::marshalling::impl::implHandleMessage<::globalmq::marshalling::impl::UnknownMessageName>( parser, handlers... ); break;\n" );
	fprintf( header, "\t}\n\n" );

	fprintf( header, "\t/*if (!ok) return;\n" );
	fprintf( header, "\tparser.structEnd();*/\n" );

	fprintf( header, "}\n\n" );


	fprintf( header, 
		"template<class ... HandlersT >\n"
		"void handleMessage( %s& parser, HandlersT ... handlers )\n"
		"{\n", parserType.c_str()
	);
	fprintf( header, "\timplHandleMessage( parser, handlers... );\n" );
	fprintf( header, "}\n\n" );
}

void impl_generateScopeComposerForwardDeclaration( FILE* header, Scope& scope, const std::string& composerType)
{
	fprintf( header, 
		"template<typename msgID, typename ... Args>\n"
		"void composeMessage( %s& composer, Args&& ... args );\n\n", composerType.c_str()
	);
}

void impl_generateScopeComposer( FILE* header, Scope& scope, const std::string& composerType)
{
	assert( scope.objectList.size() != 0 );
	fprintf( header, 
		"template<typename msgID, typename ... Args>\n"
		"void composeMessage( %s& composer, Args&& ... args )\n"
		"{\n"
		"\tstatic_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );\n", composerType.c_str()
	);

	fprintf(header,
		"\tcomposer.structBegin();\n"
		"\tcomposer.namedParamBegin(\"msgid\");\n"
		"\tcomposer.composeUnsignedInteger(msgID::id);\n"
		"\n"
		"\tcomposer.nextElement();\n"
		"\n"
		"\tcomposer.namedParamBegin(\"msgbody\");\n"
	);

	fprintf( header, "\tif constexpr ( msgID::id == %s::id )\n", scope.objectList[0]->name.c_str() );
	fprintf( header, "\t\tMESSAGE_%s_compose( composer, std::forward<Args>( args )... );\n", scope.objectList[0]->name.c_str() );
	for ( size_t i=1; i<scope.objectList.size(); ++i )
	{
		fprintf( header, "\telse if constexpr ( msgID::id == %s::id )\n", scope.objectList[i]->name.c_str() );
		fprintf( header, "\t\tMESSAGE_%s_compose( composer, std::forward<Args>( args )... );\n", scope.objectList[i]->name.c_str() );
	}
	fprintf( header, 
		"\telse\n"
		"\t\tstatic_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, \"unexpected value of msgID\" ); // note: should be just static_assert(false,\"...\"); but it seems that in this case clang asserts yet before looking at constexpr conditions\n" );


	fprintf(header, "\tcomposer.structEnd();\n");
	fprintf( header, 
		"}\n\n" );
}

void impl_GenerateMessageDefaults_MemberIterationBlock_GetCount( FILE* header, CompositeType& s, int& count )
{
	assert( s.type != CompositeType::Type::discriminated_union );

	for ( auto& it : s.getMembers() )
	{
		if ( it != nullptr )
		{
			MessageParameter& param = *it;
			if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
				continue;
			if ( param.type.hasDefault )
				switch ( param.type.kind )
				{
					case MessageParameterType::KIND::CHARACTER_STRING:
					{
						++count;
						break;
					}
					case MessageParameterType::KIND::INTEGER:
					case MessageParameterType::KIND::UINTEGER:
					case MessageParameterType::KIND::REAL:
						break;
					case MessageParameterType::KIND::BYTE_ARRAY:
					case MessageParameterType::KIND::BLOB:
					case MessageParameterType::KIND::ENUM:
					case MessageParameterType::KIND::VECTOR:
					case MessageParameterType::KIND::DICTIONARY:
					{
						break; // unsupported (yet)
					}
					case MessageParameterType::KIND::EXTENSION:
						assert( false ); // inapplicable
						break;
					default:
					{
						assert( false ); // unexpected
						break;
					}
				}
		}
	}
}

void impl_GenerateMessageDefaults_MemberIterationBlock( FILE* header, CompositeType& s, int& count )
{
	assert( s.type != CompositeType::Type::discriminated_union );
	for ( auto& it : s.getMembers() )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;

		if ( param.type.hasDefault )
			switch ( param.type.kind )
			{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
					break; // will be added right to respective calls
				case MessageParameterType::KIND::VECTOR:
				case MessageParameterType::KIND::DICTIONARY:
					break; // TODO: revise
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					fprintf( header, "static constexpr ::globalmq::marshalling::impl::StringLiteralForComposing default_%d = { \"%s\", sizeof( \"%s\" ) - 1};\n", count, param.type.stringDefault.c_str(), param.type.stringDefault.c_str() );
					break;
				}
				case MessageParameterType::KIND::ENUM:
				{
//					assert( false ); // unsupported (yet)
					break;
				}
				case MessageParameterType::KIND::EXTENSION:
					assert( false ); // inapplicable
					break;
				case MessageParameterType::KIND::BYTE_ARRAY:
				case MessageParameterType::KIND::BLOB:
				default:
				{
					assert( false ); // unexpected
					break;
				}
			}
	}
}

void impl_GenerateMessageDefaults( FILE* header, CompositeType& s )
{
	int count = 0; // let's see whether we need this block at all

	if ( s.isDiscriminatedUnion() )
	{
		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_GenerateMessageDefaults_MemberIterationBlock_GetCount( header, cs, count );
		}
	}
	else
		impl_GenerateMessageDefaults_MemberIterationBlock_GetCount( header, s, count );

	if ( count == 0 )
		return;

	fprintf( header, "namespace %s {\n", impl_MessageNameToDefaultsNamespaceName(s.name).c_str() );

	count = 0;
	if ( s.isDiscriminatedUnion() )
	{
		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_GenerateMessageDefaults_MemberIterationBlock( header, cs, count );
		}
	}
	else
		impl_GenerateMessageDefaults_MemberIterationBlock( header, s, count );

	fprintf( header, "} // namespace Message_%s_defaults\n\n", s.name.c_str() );
}

void impl_generateParamTypeLIst_MemberIterationBlock( FILE* header, CompositeType& s, int& count )
{
	assert( s.type != CompositeType::Type::discriminated_union );
	for ( auto& it : s.getMembers() )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;

		fprintf( header, "\tusing arg_%d_type = globalmq::marshalling2::NamedParameterWithProcessor<%s::Name, %s>;\n", count, paramNameToNameTagType( param.name ).c_str(), getTypeProcessor(param.type).c_str() );

	}
}

void impl_generateParamTypeLIst( FILE* header, CompositeType& s )
{
	int count = 0;
	if ( s.isDiscriminatedUnion() )
	{
		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_generateParamTypeLIst_MemberIterationBlock( header, cs, count );
		}
	}
	else
		impl_generateParamTypeLIst_MemberIterationBlock( header, s, count );

	fprintf( header, "\n" );
}

void impl_generateMatchCountBlock_MemberIterationBlock( FILE* header, CompositeType& s, int& count )
{
	assert( s.type != CompositeType::Type::discriminated_union );
	for ( auto& it : s.getMembers() )
	{
		assert( it != nullptr );
		MessageParameter& param = *it;
			
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;

		if ( count )
			fprintf( header, " + \n\t\t" );

		++count;

		fprintf( header, "isMatched(arg_%d_type::nameAndTypeID, Args::nameAndTypeID...)", count );
	}
}

void impl_generateMatchCountBlock( FILE* header, CompositeType& s )
{
	fprintf( header, "\tconstexpr size_t matchCount = " );

	int count = 0;
	if ( s.isDiscriminatedUnion() )
	{
		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_generateMatchCountBlock_MemberIterationBlock( header, cs, count );
		}
	}
	else
		impl_generateMatchCountBlock_MemberIterationBlock( header, s, count );

	if ( count )
		fprintf( header, ";\n" );
	else
		fprintf( header, "0;\n" );
}

void impl_addParamStatsCheckBlock( FILE* header, CompositeType& s )
{
	impl_generateMatchCountBlock( header, s );
	fprintf( header, 
		"\tconstexpr size_t argCount = sizeof ... (Args);\n"
		"\tif constexpr ( argCount != 0 )\n"
		"\t\tensureUniqueness(args.nameAndTypeID...);\n"
		"\tstatic_assert( argCount == matchCount, \"unexpected arguments found\" );\n\n" );
}


void impl_generateParamCallBlockForComposingJson( FILE* header, CompositeType& s, const char* offset, const std::string& composerType )
{
	fprintf( header, "%scomposer.structBegin();\n", offset );
	int count = 0;
	for ( auto& it : s.getMembers() )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;

		if ( count != 0 )
		{
			fprintf( header, "\n" );
			fprintf( header, "%scomposer.nextElement();\n", offset );
			fprintf( header, "\n" );
		}
		++count;

		fprintf( header, "%scomposer.namedParamBegin(\"%s\");\n", offset, param.name.c_str() );

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "%s::globalmq::marshalling2::composeParam2<%s, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, composerType.c_str(), count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "%s::globalmq::marshalling2::composeParam2<%s, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, composerType.c_str(), count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::REAL:
			{
				FloatingParts parts(param.type.numericalDefault);
//				fprintf( header, "%s::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_%d_type, %s, double, double, %f>(\"%s\", arg_%d_type::nameAndTypeID, composer, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", param.type.numericalDefault, param.name.c_str(), count );
				fprintf( header, "%s::globalmq::marshalling2::composeParam2<%s, arg_%d_type, %s, FloatingDefault<%lldll,%lldll>, int, 0>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, composerType.c_str(), count, param.type.hasDefault ? "false" : "true", parts.fraction, parts.exponent, param.name.c_str(), count );
				break;
			}
			case MessageParameterType::KIND::CHARACTER_STRING:
				if ( param.type.hasDefault )
					fprintf( header, "%s::globalmq::marshalling2::composeParam2<%s, arg_%d_type, false, nodecpp::string, const ::globalmq::marshalling::impl::StringLiteralForComposing*, &%s::default_%d>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, composerType.c_str(), count, impl_MessageNameToDefaultsNamespaceName(s.name).c_str(), count, param.name.c_str(), count );
				else
					fprintf( header, "%s::globalmq::marshalling2::composeParam2<%s, arg_%d_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, composerType.c_str(), count, param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf( header, "%s::globalmq::marshalling2::composeParam2<%s, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, composerType.c_str(), count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::DICTIONARY: // TODO: revise
				fprintf( header, "%s::globalmq::marshalling2::composeParam2<%s, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, composerType.c_str(), count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: ...
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "%s::globalmq::marshalling2::composeParam2<%s, arg_%d_type, %s, int64_t, int64_t, (int64_t)(0)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, composerType.c_str(), count, param.type.hasDefault ? "false" : "true", param.name.c_str(), count );
				break; // TODO: ...
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf( header, "%s::globalmq::marshalling2::composeParam2<%s, arg_%d_type, %s, int64_t, int64_t, (int64_t)(0)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, composerType.c_str(), count, param.type.hasDefault ? "false" : "true", param.name.c_str(), count );
				break;
			default:
			{
				assert( false ); // unexpected
				break;
			}
		}
	}

	fprintf( header, "%scomposer.structEnd();\n", offset );
}

void impl_generateParamCallBlockForComposing( FILE* header, CompositeType& s, const std::string& composerType )
{
	impl_generateParamCallBlockForComposingJson( header, s, "\t", composerType);
}

void impl_generateComposeFunction( FILE* header, CompositeType& s, const std::string& messageName, const std::string& composerType )
{
	assert( s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union );
	fprintf( header, "template<typename ... Args>\n"
	"void MESSAGE_%s_compose(%s& composer, Args&& ... args)\n"
	"{\n", messageName.c_str(), composerType.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForComposing( header, s, composerType);


	fprintf( header, "}\n\n" );
}

void impl_generateParseFunctionForMessagesAndAliasingStructs( FILE* header, CompositeType& s, const std::string& messageName, const std::string& parserType )
{
	fprintf( header, "inline\n" );
	fprintf( header, "%s MESSAGE_%s_parse(%s& parser)\n", getGeneratedTypeName(s).c_str(), messageName.c_str(), parserType.c_str());
	fprintf( header, "{\n" );

	fprintf( header, "\tusing T = %s;\n", getGeneratedTypeName(s).c_str() );
	fprintf( header, "\tT t;\n" );
 
	fprintf( header, "\t%s::parseForStateSyncOrMessageInDepth(parser, t);\n", getHelperClassName(s).c_str() );

	fprintf( header, "\treturn t;\n" );

	fprintf( header, "}\n\n" );
}

void generateMessage( FILE* header, Root& root, CompositeType& s, const std::string& messageName, const GenerationConfig& config)
{
	impl_generateMessageCommentBlock( header, s );
	impl_GenerateMessageDefaults( header, s );

	for(auto& each : config.composerNames)
		impl_generateComposeFunction( header, s, messageName, each);

	for (auto& each : config.parserNames)
		impl_generateParseFunctionForMessagesAndAliasingStructs( header, s, messageName, each );

}

void generateMessageAlias( FILE* header, Root& root, CompositeType& s, const GenerationConfig& config)
{
	assert( s.aliasIdx < root.structs.size() );
	CompositeType& alias = *(root.structs[s.aliasIdx]);
	assert( s.aliasOf == alias.name );

	generateMessage(header, root, alias, s.name, config);
}

}//namespace cplusplus