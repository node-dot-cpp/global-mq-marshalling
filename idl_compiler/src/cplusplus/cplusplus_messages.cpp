/* -------------------------------------------------------------------------------
* Copyright (c) 2020, OLogN Technologies AG
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

std::string impl_generateMessageClassName( MessageParameterType::KIND type, const std::string& name )
{
	if(type == MessageParameterType::KIND::STRUCT)
		return fmt::format( "publishable_{}_{}", "STRUCT", name );
	else if(type == MessageParameterType::KIND::DISCRIMINATED_UNION)
		return fmt::format( "publishable_{}_{}", "DISCRIMINATED_UNION", name );
	else
		assert(false);
}

const char* impl_generatePrimitiveProcessorName( MessageParameterType::KIND type )
{
	switch ( type )
	{
		case MessageParameterType::KIND::INTEGER:
			return "globalmq::marshalling2::Int64Processor";
			break;
		case MessageParameterType::KIND::UINTEGER:
			return "globalmq::marshalling2::UInt64Processor";
			break;
		case MessageParameterType::KIND::REAL:
			return "globalmq::marshalling2::DoubleProcessor";
			break;
		case MessageParameterType::KIND::CHARACTER_STRING:
			return "globalmq::marshalling2::StringProcessor";
			break;
		default:
			assert(false);
			return "";
	}
}

std::string impl_generateMessageParseFunctionRetType( CompositeType& s )
{
	if ( s.type == CompositeType::Type::message )
		return fmt::format( "structures::{}::{}_{}", s.scopeName.c_str(), s.type2string(), s.name );
	
//	assert( false );
//	if ( s.type == CompositeType::Type::structure )
	return fmt::format( "structures::{}", s.name );
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
	fprintf( header, "\t\t%s( composer, std::forward<Args>( args )... );\n", impl_generateComposeFunctionName(*(scope.objectList[0])).c_str() );
	for ( size_t i=1; i<scope.objectList.size(); ++i )
	{
		fprintf( header, "\telse if constexpr ( msgID::id == %s::id )\n", scope.objectList[i]->name.c_str() );
		fprintf( header, "\t\t%s( composer, std::forward<Args>( args )... );\n", impl_generateComposeFunctionName(*(scope.objectList[i])).c_str() );
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

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "\tusing arg_%d_type = globalmq::marshalling2::NamedParameterWithProcessor<%s::Name, %s>;\n", count, paramNameToNameTagType( param.name ).c_str(), impl_generatePrimitiveProcessorName(param.type.kind) );
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf( header, "\tusing arg_%d_type = globalmq::marshalling2::NamedParameterWithProcessor<%s::Name, %s>;\n", count, paramNameToNameTagType( param.name ).c_str(), impl_generateMessageClassName( param.type.kind, param.type.name ).c_str() );
				break;
			case MessageParameterType::KIND::VECTOR:
				switch ( param.type.vectorElemKind )
				{
					case MessageParameterType::KIND::INTEGER:
					case MessageParameterType::KIND::UINTEGER:
					case MessageParameterType::KIND::REAL:
					case MessageParameterType::KIND::CHARACTER_STRING:
						fprintf( header, "\tusing arg_%d_type = globalmq::marshalling2::NamedParameterWithProcessor<%s::Name, globalmq::marshalling2::MessageVectorProcessor<%s>>;\n", count, paramNameToNameTagType( param.name ).c_str(), impl_generatePrimitiveProcessorName(param.type.vectorElemKind) );
						break;
					case MessageParameterType::KIND::STRUCT:
					case MessageParameterType::KIND::DISCRIMINATED_UNION:
						fprintf( header, "\tusing arg_%d_type = globalmq::marshalling2::NamedParameterWithProcessor<%s::Name, globalmq::marshalling2::MessageVectorProcessor<%s>>;\n", count, paramNameToNameTagType( param.name ).c_str(), impl_generateMessageClassName( param.type.vectorElemKind, param.type.name ).c_str() );
						break;
					default:
						assert( false ); // unexpected
						break;
				}
				break;
			case MessageParameterType::KIND::DICTIONARY:
			{
				switch ( param.type.dictionaryValueKind )
				{
					case MessageParameterType::KIND::INTEGER:
					case MessageParameterType::KIND::UINTEGER:
					case MessageParameterType::KIND::REAL:
					case MessageParameterType::KIND::CHARACTER_STRING:
					// case MessageParameterType::KIND::BYTE_ARRAY:
					// case MessageParameterType::KIND::BLOB:
					// case MessageParameterType::KIND::ENUM:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::DictionaryOfSympleTypes<%s, %s>, %s::Name>;\n", count, paramTypeToLibType(param.type.dictionaryKeyKind), paramTypeToLibType(param.type.dictionaryValueKind), paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::STRUCT:
						if ( param.type.isNonExtendable )
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::DictionaryOfNonextMessageTypes, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						else
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::DictionaryOfMessageType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::DISCRIMINATED_UNION:
						if ( param.type.isNonExtendable )
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::DictionaryOfNonextDiscriminatedUnionTypes, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						else
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::DictionaryOfDiscriminatedUnionType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					default:
						assert( false ); // unexpected
						break;
				}
				break;
			}
			default:
			{
				assert( false ); // unexpected
				break;
			}
		}
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

//void impl_generateParamCallBlockForComposingGmq_MemberIterationBlock( FILE* header, CompositeType& s, int& count, const char* offset )
//{
//	assert( s.type != CompositeType::Type::discriminated_union );
//	for ( auto& it : s.getMembers() )
//	{
//		assert( it != nullptr );
//
//		MessageParameter& param = *it;
//		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
//			continue;
//		++count;
//
//		switch ( param.type.kind )
//		{
//			case MessageParameterType::KIND::INTEGER:
//				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), count );
//				break;
//			case MessageParameterType::KIND::UINTEGER:
//				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), count );
//				break;
//			case MessageParameterType::KIND::REAL:
//			{
//				FloatingParts parts(param.type.numericalDefault);
//				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, FloatingDefault<%lldll,%lldll>, int, 0>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", parts.fraction, parts.exponent, count );
//				break;
//			}
//			case MessageParameterType::KIND::CHARACTER_STRING:
//				if ( param.type.hasDefault )
//					fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, false, nodecpp::string, const ::globalmq::marshalling::impl::StringLiteralForComposing*, &%s::default_%d>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, impl_MessageNameToDefaultsNamespaceName(s.name).c_str(), count, count );
//				else
//					fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, true, uint64_t, uint64_t, (uint64_t)0>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
//				break;
//			case MessageParameterType::KIND::BYTE_ARRAY:
//				break;
//			case MessageParameterType::KIND::BLOB:
//				break;
//			case MessageParameterType::KIND::ENUM:
//				break;
//			case MessageParameterType::KIND::VECTOR:
//				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), count );
//				break;
//			case MessageParameterType::KIND::DICTIONARY: // TODO: revise
//				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), count );
//				break;
//			case MessageParameterType::KIND::EXTENSION:
//				break; // TODO: treatment
//			case MessageParameterType::KIND::STRUCT:
//				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", count );
//				break; // TODO: treatment
//			case MessageParameterType::KIND::DISCRIMINATED_UNION:
//				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", count );
//				break;
//			default:
//			{
//				assert( false ); // unexpected
//				break;
//			}
//		}
//	}
//}
//
//void impl_generateParamCallBlockForComposingGmq( FILE* header, CompositeType& s, const char* offset )
//{
//	int count = 0;
//	if ( s.isDiscriminatedUnion() )
//	{
//		for ( auto& it: s.getDiscriminatedUnionCases() )
//		{
//			assert( it != nullptr );
//			CompositeType& cs = *it;
//			assert( cs.type == CompositeType::Type::discriminated_union_case );
//			impl_generateParamCallBlockForComposingGmq_MemberIterationBlock( header, cs, count, offset );
//		}
//	}
//	else
//		impl_generateParamCallBlockForComposingGmq_MemberIterationBlock( header, s, count, offset );
//
//}

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
	//size_t protoCount = s.protoList.size();
	//if ( protoCount == 1 )
	//{
	//	switch ( *(s.protoList.begin()) )
	//	{
	//		case Proto::gmq:
	//		{
	//			fprintf( header, "\tstatic_assert( ComposerT::proto == Proto::GMQ, \"this %s assumes only GMQ protocol\" );\n", s.type2string() );
	//			impl_generateParamCallBlockForComposingGmq( header, s, "\t" );
	//			break;
	//		}
	//		case Proto::json:
	//		{
				//fprintf( header, "\tstatic_assert( ComposerT::proto == Proto::JSON, \"this %s assumes only JSON protocol\" );\n", s.type2string() );
				impl_generateParamCallBlockForComposingJson( header, s, "\t", composerType);
	//			break;
	//		}
	//		default:
	//			assert( false );
	//	}
	//}
	//else
	//{
	//	size_t processedProtoCount = 0;
	//	// if present, keep GMQ first!
	//	if ( s.protoList.find( Proto::gmq ) != s.protoList.end() )
	//	{
	//		++processedProtoCount;
	//		fprintf( header, 
	//			"\tif constexpr( ComposerT::proto == Proto::GMQ )\n"
	//			"\t{\n" );
	//		impl_generateParamCallBlockForComposingGmq( header, s, "\t\t" );
	//		fprintf( header, "\t}\n" );
	//	}
	//	// then add the rest
	//	for ( auto it:s.protoList )
	//	{
	//		++processedProtoCount;
	//		switch ( it )
	//		{
	//			case Proto::gmq:
	//				break; // already done
	//			case Proto::json:
	//			{
	//				// NOTE: currently we have only two protocols; if more, we need to be a bit more delicate with 'else if' constructions
	//				if ( processedProtoCount == 0 )
	//				{
	//					fprintf( header, 
	//						"\tif constexpr ( ComposerT::proto == Proto::JSON )\n"
	//						"\t{\n" );
	//				}
	//				else
	//				{
	//					if ( processedProtoCount == protoCount )
	//						fprintf( header, 
	//							"\telse\n"
	//							"\t{\n"
	//							"\t\tstatic_assert( ComposerT::proto == Proto::JSON );\n" );
	//					else
	//						fprintf( header, 
	//							"\telse if constexpr ( ComposerT::proto == Proto::JSON )\n"
	//							"\t{\n" );
	//				}
	//				impl_generateParamCallBlockForComposingJson( header, s, "\t\t" );
	//				fprintf( header, "\n\t}\n" );
	//				break;
	//			}
	//			default:
	//				assert( false );
	//		}
	//	}
	//}
}

void impl_generateComposeFunction( FILE* header, CompositeType& s, const std::string& composerType )
{
	assert( s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union );
	fprintf( header, "template<typename ... Args>\n"
	"void %s(%s& composer, Args&& ... args)\n"
	"{\n", impl_generateComposeFunctionName( s ).c_str(), composerType.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForComposing( header, s, composerType);


	fprintf( header, "}\n\n" );
}

void impl_generateParseFunctionForMessagesAndAliasingStructs( FILE* header, Root& root, CompositeType& s, const std::string& parserType )
{
	fprintf( header, "inline\n" );
	fprintf( header, "%s %s(%s& parser)\n", impl_generateMessageParseFunctionRetType(s).c_str(), impl_generateParseFunctionName( s ).c_str(), parserType.c_str());
	fprintf( header, "{\n" );

	fprintf( header, "\tusing T = %s;\n", impl_generateMessageParseFunctionRetType(s).c_str() );
	fprintf( header, "\tT t;\n" );

	fprintf( header, "\tparser.structBegin();\n" );
	fprintf( header, "\n" );

	impl_generateParseFunctionBodyForPublishableStructStateSyncOrMessageInDepth( header, root, s, parserType );

	fprintf( header, "\tparser.structEnd();\n" );

	fprintf( header, "\treturn t;\n" );

	fprintf( header, "}\n\n" );
}

void generateMessage( FILE* header, Root& root, CompositeType& s, const GenerationConfig& config)
{
	bool checked = impl_checkParamNameUniqueness(s);
	checked = impl_checkFollowingExtensionRules(s) && checked;
	if ( !checked )
		throw std::exception();

	impl_generateMessageCommentBlock( header, s );
	impl_GenerateMessageDefaults( header, s );

	for(auto& each : config.composerNames)
		impl_generateComposeFunction( header, s, each);

	if ( s.type == CompositeType::Type::message )
		for (auto& each : config.parserNames)
			impl_generateParseFunctionForMessagesAndAliasingStructs( header, root, s, each );

}

void generateMessageAlias( FILE* header, Root& root, CompositeType& s, const GenerationConfig& config)
{
	assert( s.aliasIdx < root.structs.size() );
	CompositeType& alias = *(root.structs[s.aliasIdx]);
	assert( s.aliasOf == alias.name );

	bool checked = impl_checkFollowingExtensionRules(s);
	if ( !checked )
		throw std::exception();

	fprintf( header, "//**********************************************************************\n" );
	fprintf( header, "// %s \"%s\" %sTargets: ", s.type2string(), s.name.c_str(), s.isNonExtendable ? "NONEXTENDABLE " : "" );
	for ( auto t:s.protoList )
		switch ( t )
		{
			case Proto::gmq: fprintf( header, "GMQ " ); break;
			case Proto::json: fprintf( header, "JSON " ); break;
			default: assert( false );
		}
	fprintf( header, "(Alias of %s)\n", s.aliasOf.c_str() );
	fprintf( header, "\n" );
	fprintf( header, "//**********************************************************************\n\n" );

	// compose function
	for (auto& each : config.composerNames)
	{
		fprintf(header, "template<typename ... Args>\n"
			"void %s_%s_compose(%s& composer, Args&& ... args)\n", s.type2string(), s.name.c_str(), each.c_str());
		fprintf(header, "{\n");
		fprintf(header, "\t%s_%s_compose(composer, std::forward<Args>( args )...);\n", impl_kindToString(MessageParameterType::KIND::STRUCT), s.aliasOf.c_str());
		fprintf(header, "}\n\n");
	}
	// parse-by-param function
	for (auto& each : config.parserNames)
	{
		fprintf(header, "template<typename ... Args>\n"
			"void %s_%s_parse(%s& p, Args&& ... args)\n", s.type2string(), s.name.c_str(), each.c_str());
		fprintf(header, "{\n");
		fprintf(header, "\t%s_%s_parse(p, std::forward<Args>( args )...);\n", impl_kindToString(MessageParameterType::KIND::STRUCT), s.aliasOf.c_str());
		fprintf(header, "}\n\n");

		// parse function
		fprintf(header, "inline\n");
		fprintf(header, "structures::%s::%s_%s %s_%s_parse(%s& p)\n", s.scopeName.c_str(), s.type2string(), s.name.c_str(), s.type2string(), s.name.c_str(), each.c_str());
		fprintf(header, "{\n");
		fprintf(header, "\treturn static_cast<structures::%s::%s_%s>(%s(p));\n", s.scopeName.c_str(), s.type2string(), s.name.c_str(), impl_generateParseFunctionName(alias).c_str());
		fprintf(header, "}\n\n");
	}
}

}//namespace cplusplus