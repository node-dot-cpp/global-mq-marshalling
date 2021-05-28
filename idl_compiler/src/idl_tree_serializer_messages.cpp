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

#include "idl_tree_serializer.h"

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

bool impl_processScopes( Root& r )
{
	bool ok = true;
	for ( auto& s : r.scopes )
	{
		if ( s->proto == Proto::undefined )
		{
			fprintf( stderr, "File %s, line %d: Scope \"%s\" misses protocol\n", s->location.fileName.c_str(), s->location.lineNumber, s->name.c_str() );
			ok = false;
		}
	}
	
	// each message must be from one of declared scopes
	for ( auto& it : r.messages )
	{
		if ( it->numID == CompositeType::invalid_num_id )
		{
			// todo report
			ok = false;
			continue;
		}

		bool found = false;
		for ( auto& s : r.scopes )
		{
			if ( it->scopeName == s->name )
			{
//				s->objectList.push_back( &(*it) );
				assert( it->protoList.empty() );
				it->protoList.insert( s->proto );
				s->objectList.push_back( &(*it) );
				found = true;
				break;
			}
		}

		if ( !found )
		{
			ok = false;
			fprintf( stderr, "File %s, line %d: Scope \"%s\" has not been declared\n", it->location.fileName.c_str(), it->location.lineNumber, it->scopeName.c_str() );

			/*{
				Scope* scope = new Scope;
				scope->name = it->scopeName;
				scope->objectList.push_back( &(*it) );
				r.scopes.push_back( unique_ptr<Scope>(scope) );
			}*/
		}
	}
	return ok;
}

void impl_generateScopeHandler( FILE* header, Scope& scope )
{
	fprintf( header, 
		"template<class ParserT, class ... HandlersT >\n"
		"void implHandleMessage( ParserT& parser, HandlersT ... handlers )\n"
		"{\n"
		"\tuint64_t msgID;\n\n"
	);
	switch ( scope.proto )
	{
		case Proto::gmq:
			fprintf( header, 
//				"\tGmqParser parser( buffer );\n"
				"\tstatic_assert( ParserT::proto == Proto::GMQ, \"According to IDL GMQ parser is expected\" );\n"
				"\tparser.parseUnsignedInteger( &msgID );\n"
			);
			break;
		case Proto::json:
			fprintf( header, 
//				"\tJsonParser parser( buffer );\n"
				"\tstatic_assert( ParserT::proto == Proto::JSON, \"According to IDL JSON parser is expected\" );\n"
				"\tparser.skipDelimiter(\'{\');\n"
				"\tstd::string key;\n"
				"\tparser.readKey(&key);\n"
				"\tif (key != \"msgid\")\n"
				"\t\tthrow std::exception(); // bad format\n"
				"\tparser.readUnsignedIntegerFromJson(&msgID);\n"
				"\tparser.skipSpacesEtc();\n"
				"\tif (!parser.isDelimiter(\',\'))\n"
				"\t\tthrow std::exception(); // bad format\n"
				"\tparser.skipDelimiter(\',\');\n"
				"\tparser.readKey(&key);\n"
				"\tif (key != \"msgbody\")\n"
				"\t\tthrow std::exception(); // bad format\n"
				"\tJsonParser p( parser );\n\n"
			);
			break;
		default:
			assert( false );
	}
	fprintf( header, 
		"\tswitch ( msgID )\n"
		"\t{\n"
	);
	for ( auto msg : scope.objectList )
		fprintf( header, "\t\tcase %s::id: ::globalmq::marshalling::impl::implHandleMessage<%s>( parser, handlers... ); break;\n", msg->name.c_str(), msg->name.c_str() );
	fprintf( header, "\t\tdefault: ::globalmq::marshalling::impl::implHandleMessage<::globalmq::marshalling::impl::UnknownMessageName>( parser, handlers... ); break;\n" );
	fprintf( header, "\t}\n\n" );
	switch ( scope.proto )
	{
		case Proto::gmq: break;
		case Proto::json:
			fprintf( header, 
				"\tif (!parser.isDelimiter(\'}\'))\n"
				"\t\tthrow std::exception(); // bad format\n"
				"\tparser.skipDelimiter(\'}\');\n"
			);
			break;
		default:
			assert( false );
	}
	fprintf( header, "}\n\n" );


	fprintf( header, 
		"template<class BufferT, class ... HandlersT >\n"
		"void handleMessage( BufferT& buffer, HandlersT ... handlers )\n"
		"{\n"
		"\tauto riter = buffer.getReadIter();\n"
	);
	switch ( scope.proto )
	{
		case Proto::gmq:
			fprintf( header, 
				"\tGmqParser<BufferT> parser( riter );\n"
			);
			break;
		case Proto::json:
			fprintf( header, 
				"\tJsonParser<BufferT> parser( riter );\n"
			);
			break;
		default:
			assert( false );
	}
	fprintf( header, "\timplHandleMessage( parser, handlers... );\n" );
	fprintf( header, "}\n\n" );


	fprintf( header, 
		"template<class ReadIteratorT, class ... HandlersT >\n"
		"void handleMessage2( ReadIteratorT& riter, HandlersT ... handlers )\n"
		"{\n"
	);
	switch ( scope.proto )
	{
		case Proto::gmq:
			fprintf( header, 
				"\tGmqParser<typename ReadIteratorT::BufferT> parser( riter );\n"
			);
			break;
		case Proto::json:
			fprintf( header, 
				"\tJsonParser<typename ReadIteratorT::BufferT> parser( riter );\n"
			);
			break;
		default:
			assert( false );
	}
	fprintf( header, "\timplHandleMessage( parser, handlers... );\n" );
	fprintf( header, "}\n\n" );
}

void impl_generateScopeComposerForwardDeclaration( FILE* header, Scope& scope )
{
	fprintf( header, 
		"template<typename msgID, class BufferT, typename ... Args>\n"
		"void composeMessage( BufferT& buffer, Args&& ... args );\n\n"
	);
}

void impl_generateScopeComposer( FILE* header, Scope& scope )
{
	assert( scope.objectList.size() != 0 );
	fprintf( header, 
		"template<typename msgID, class BufferT, typename ... Args>\n"
		"void composeMessage( BufferT& buffer, Args&& ... args )\n"
		"{\n"
		"\tstatic_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );\n" 
	);
	switch ( scope.proto )
	{
		case Proto::gmq: 
			fprintf( header, 
				"\tglobalmq::marshalling::GmqComposer composer( buffer );\n"
				"\t::globalmq::marshalling::impl::composeUnsignedInteger( composer, msgID::id );\n"
			);
			break;
		case Proto::json: 
			fprintf( header, 
				"\tglobalmq::marshalling::JsonComposer composer( buffer );\n"
				"\tcomposer.buff.append( \"{\\n  \", sizeof(\"{\\n  \") - 1 );\n"
				"\t::globalmq::marshalling::impl::json::composeNamedSignedInteger( composer, \"msgid\", msgID::id);\n"
				"\tcomposer.buff.append( \",\\n  \", sizeof(\",\\n  \") - 1 );\n"
				"\t::globalmq::marshalling::impl::json::addNamePart( composer, \"msgbody\" );\n"
			);
			break;
		default:
			assert( false );
	}

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
	switch ( scope.proto )
	{
		case Proto::gmq: break;
		case Proto::json: 
			fprintf( header, "\tcomposer.buff.append( \"\\n}\", 2 );\n"	);
			break;
		default:
			assert( false );
			break;
	}
	fprintf( header, 
		"}\n\n" );
}

void impl_GenerateMessageDefaults( FILE* header, CompositeType& s )
{
	int count = 0; // let's see whether we need this block at all
	for ( auto& it : s.members )
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

	if ( count == 0 )
		return;

	fprintf( header, "namespace %s {\n", impl_MessageNameToDefaultsNamespaceName(s.name).c_str() );

	count = 0;
	for ( auto& it : s.members )
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

	fprintf( header, "} // namespace Message_%s_defaults\n\n", s.name.c_str() );
}

void impl_generateParamTypeLIst( FILE* header, CompositeType& s )
{
	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::UnsignedIntegralType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::REAL:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::StringType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::ByteArrayType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::BLOB:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::BlobType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::ENUM:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::EnumType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::VECTOR:
				switch ( param.type.vectorElemKind )
				{
					case MessageParameterType::KIND::INTEGER:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::SignedIntegralType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::UINTEGER:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::UnsignedIntegralType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::REAL:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::RealType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::CHARACTER_STRING:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::StringType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::BYTE_ARRAY:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::ByteArrayType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::BLOB:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::BlobType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::ENUM:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::EnumType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::STRUCT:
						if ( param.type.isNonExtendable )
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfNonextMessageTypes, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						else
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					default:
						assert( false ); // unexpected
						break;
				}
				break;
			case MessageParameterType::KIND::STRUCT:
				if ( param.type.isNonExtendable )
					fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::NonextMessageType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				else
					fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			default:
			{
				assert( false ); // unexpected
				break;
			}
		}
	}

	fprintf( header, "\n" );
}

void impl_generateParamCallBlockForComposingGmq( FILE* header, CompositeType& s, const char* offset )
{
	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), count );
				break;
			case MessageParameterType::KIND::REAL:
			{
				FloatingParts parts(param.type.numericalDefault);
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, FloatingDefault<%lldll,%lldll>, int, 0>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", parts.fraction, parts.exponent, count );
				break;
			}
			case MessageParameterType::KIND::CHARACTER_STRING:
				if ( param.type.hasDefault )
					fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, false, nodecpp::string, const ::globalmq::marshalling::impl::StringLiteralForComposing*, &%s::default_%d>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, impl_MessageNameToDefaultsNamespaceName(s.name).c_str(), count, count );
				else
					fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, true, uint64_t, uint64_t, (uint64_t)0>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), count );
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: treatment
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", count );
				break; // TODO: treatment
			default:
			{
				assert( false ); // unexpected
				break;
			}
		}
	}
}

void impl_generateParamCallBlockForParsingGmq( FILE* header, CompositeType& s, const char* offset )
{
	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::REAL:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: treatment
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "%s::globalmq::marshalling::impl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break; // TODO: ...
			default:
			{
				assert( false ); // unexpected
				break;
			}
		}
	}
}

void impl_generateMatchCountBlock( FILE* header, CompositeType& s )
{
	fprintf( header, "\tconstexpr size_t matchCount = " );

	int count = 0;
	for ( auto& it : s.members )
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

	fprintf( header, ";\n" );
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

void impl_generateMessageCommentBlock( FILE* header, CompositeType& s )
{
	assert( s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure );
	fprintf( header, "//**********************************************************************\n" );
	fprintf( header, "// %s \"%s\" %sTargets: ", s.type2string(), s.name.c_str(), s.isNonExtendable ? "NONEXTENDABLE " : "" );
	for ( auto t:s.protoList )
		switch ( t )
		{
			case Proto::gmq: fprintf( header, "GMQ " ); break;
			case Proto::json: fprintf( header, "JSON " ); break;
			default: assert( false );
		}
	fprintf( header, "(%zd parameters)\n", s.members.size() );

	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );
		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;
			
		if ( param.type.kind == MessageParameterType::KIND::VECTOR )
		{
			if ( param.type.vectorElemKind == MessageParameterType::KIND::STRUCT )
				fprintf( header, "// %d. %s<%s%s %s>", count, impl_kindToString( param.type.kind ), param.type.isNonExtendable ? "NONEXTENDABLE " : " ", impl_kindToString( param.type.vectorElemKind ), param.type.name.c_str() );
			else
				fprintf( header, "// %d. %s<%s>", count, impl_kindToString( param.type.kind ), impl_kindToString( param.type.vectorElemKind ) );
			fprintf( header, " %s", param.name.c_str() );
		}
		else if ( param.type.kind == MessageParameterType::KIND::STRUCT )
		{
			fprintf( header, "// %d. %s %s%s", count, impl_kindToString( param.type.kind ), param.type.isNonExtendable ? "NONEXTENDABLE " : "", param.type.name.c_str() );
			fprintf( header, " %s", param.name.c_str() );
		}
		else
			fprintf( header, "// %d. %s %s", count, impl_kindToString( param.type.kind ), param.name.c_str() );

		if ( param.type.hasDefault )
		{
			switch ( param.type.kind )
			{
				case MessageParameterType::KIND::ENUM: fprintf( header, " (DEFAULT: %s::%s)", param.type.name.c_str(), param.type.stringDefault.c_str() ); break;
				case MessageParameterType::KIND::INTEGER: fprintf( header, " (DEFAULT: %lld)", (int64_t)(param.type.numericalDefault) ); break;
				case MessageParameterType::KIND::UINTEGER: fprintf( header, " (DEFAULT: %lld)", (uint64_t)(param.type.numericalDefault) ); break;
				case MessageParameterType::KIND::REAL: fprintf( header, " (DEFAULT: %f)", param.type.numericalDefault ); break;
				case MessageParameterType::KIND::CHARACTER_STRING: fprintf( header, " (DEFAULT: \"%s\")", param.type.stringDefault.c_str() ); break;
			}
		}
		else
			fprintf( header, " (REQUIRED)" );
		fprintf( header, "\n" );
	}

	fprintf( header, "\n" );
	fprintf( header, "//**********************************************************************\n\n" );
}

void impl_generateParamCallBlockForComposingJson( FILE* header, CompositeType& s, const char* offset )
{
	fprintf( header, "%scomposer.buff.append( \"{\\n  \", sizeof(\"{\\n  \") - 1 );\n", offset );
	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "%s::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "%s::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::REAL:
			{
				FloatingParts parts(param.type.numericalDefault);
//				fprintf( header, "%s::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_%d_type, %s, double, double, %f>(\"%s\", arg_%d_type::nameAndTypeID, composer, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", param.type.numericalDefault, param.name.c_str(), count );
				fprintf( header, "%s::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_%d_type, %s, FloatingDefault<%lldll,%lldll>, int, 0>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", parts.fraction, parts.exponent, param.name.c_str(), count );
				break;
			}
			case MessageParameterType::KIND::CHARACTER_STRING:
				if ( param.type.hasDefault )
					fprintf( header, "%s::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_%d_type, false, nodecpp::string, const ::globalmq::marshalling::impl::StringLiteralForComposing*, &%s::default_%d>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, impl_MessageNameToDefaultsNamespaceName(s.name).c_str(), count, param.name.c_str(), count );
				else
					fprintf( header, "%s::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_%d_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf( header, "%s::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: ...
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "%s::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_%d_type, %s, int64_t, int64_t, (int64_t)(0)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", param.name.c_str(), count );
				break; // TODO: ...
			default:
			{
				assert( false ); // unexpected
				break;
			}
		}
	
		if ( count != s.members.size() )
			fprintf( header, "%scomposer.buff.append( \",\\n  \", 4 );\n", offset );
	}

	fprintf( header, "%scomposer.buff.append( \"\\n}\", 2 );\n", offset );
}

void impl_generateParamCallBlockForParsingJson( FILE* header, CompositeType& s, const char* offset )
{
	fprintf( header, "%sp.skipDelimiter( \'{\' );\n", offset );
	fprintf( header, "%sfor ( ;; )\n", offset );
	fprintf( header, "%s{\n", offset );
	fprintf( header, "%s\tstd::string key;\n", offset );
	fprintf( header, "%s\tp.readKey( &key );\n", offset );

	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;

		fprintf( header, "%s\t%s( key == \"%s\" )\n", offset, count == 1 ? "if " : "else if ", param.name.c_str() );
		fprintf( header, "%s\t\t::globalmq::marshalling::impl::json::parseJsonParam<ParserT, arg_%d_type, false>(arg_%d_type::nameAndTypeID, p, args...);\n", offset, count, count );
	}

	fprintf( header, "%s\tp.skipSpacesEtc();\n", offset );
	fprintf( header, "%s\tif ( p.isDelimiter( \',\' ) )\n", offset );
	fprintf( header, "%s\t{\n", offset );
	fprintf( header, "%s\t\tp.skipDelimiter( \',\' );\n", offset );
	fprintf( header, "%s\t\tcontinue;\n", offset );
	fprintf( header, "%s\t}\n", offset );
	fprintf( header, "%s\tif ( p.isDelimiter( \'}\' ) )\n", offset );
	fprintf( header, "%s\t{\n", offset );
	fprintf( header, "%s\t\tp.skipDelimiter( \'}\' );\n", offset );
	fprintf( header, "%s\t\tbreak;\n", offset );
	fprintf( header, "%s\t}\n", offset );
	fprintf( header, "%s\tthrow std::exception(); // bad format\n", offset );

	fprintf( header, "%s}\n", offset );
}

void impl_generateParamCallBlockForComposing( FILE* header, CompositeType& s )
{
	size_t protoCount = s.protoList.size();
	if ( protoCount == 1 )
	{
		switch ( *(s.protoList.begin()) )
		{
			case Proto::gmq:
			{
				fprintf( header, "\tstatic_assert( ComposerT::proto == Proto::GMQ, \"this %s assumes only GMQ protocol\" );\n", s.type2string() );
				impl_generateParamCallBlockForComposingGmq( header, s, "\t" );
				break;
			}
			case Proto::json:
			{
				fprintf( header, "\tstatic_assert( ComposerT::proto == Proto::JSON, \"this %s assumes only JSON protocol\" );\n", s.type2string() );
				impl_generateParamCallBlockForComposingJson( header, s, "\t" );
				break;
			}
			default:
				assert( false );
		}
	}
	else
	{
		size_t processedProtoCount = 0;
		// if present, keep GMQ first!
		if ( s.protoList.find( Proto::gmq ) != s.protoList.end() )
		{
			++processedProtoCount;
			fprintf( header, 
				"\tif constexpr( ComposerT::proto == Proto::GMQ )\n"
				"\t{\n" );
			impl_generateParamCallBlockForComposingGmq( header, s, "\t\t" );
			fprintf( header, "\t}\n" );
		}
		// then add the rest
		for ( auto it:s.protoList )
		{
			++processedProtoCount;
			switch ( it )
			{
				case Proto::gmq:
					break; // already done
				case Proto::json:
				{
					// NOTE: currently we have only two protocols; if more, we need to be a bit more delicate with 'else if' constructions
					if ( processedProtoCount == 0 )
					{
						fprintf( header, 
							"\tif constexpr ( ComposerT::proto == Proto::JSON )\n"
							"\t{\n" );
					}
					else
					{
						if ( processedProtoCount == protoCount )
							fprintf( header, 
								"\telse\n"
								"\t{\n"
								"\t\tstatic_assert( ComposerT::proto == Proto::JSON );\n" );
						else
							fprintf( header, 
								"\telse if constexpr ( ComposerT::proto == Proto::JSON )\n"
								"\t{\n" );
					}
					impl_generateParamCallBlockForComposingJson( header, s, "\t\t" );
					fprintf( header, "\n\t}\n" );
					break;
				}
				default:
					assert( false );
			}
		}
	}
}

void impl_generateParamCallBlockForParsing( FILE* header, CompositeType& s )
{
	size_t protoCount = s.protoList.size();
	if ( protoCount == 1 )
	{
		switch ( *(s.protoList.begin()) )
		{
			case Proto::gmq:
			{
				fprintf( header, "\tstatic_assert( ParserT::proto == Proto::GMQ, \"this %s assumes only GMQ protocol\" );\n", s.type2string() );
				impl_generateParamCallBlockForParsingGmq( header, s, "\t" );
				break;
			}
			case Proto::json:
			{
				fprintf( header, "\tstatic_assert( ParserT::proto == Proto::JSON, \"this %s assumes only JSON protocol\" );\n", s.type2string() );
				impl_generateParamCallBlockForParsingJson( header, s, "\t" );
				break;
			}
			default:
				assert( false );
		}
	}
	else
	{
		size_t processedProtoCount = 0;
		// if present, keep GMQ first!
		if ( s.protoList.find( Proto::gmq ) != s.protoList.end() )
		{
			++processedProtoCount;
			fprintf( header, 
				"\tif constexpr( ParserT::proto == Proto::GMQ )\n"
				"\t{\n" );
			impl_generateParamCallBlockForParsingGmq( header, s, "\t\t" );
			fprintf( header, "\t}\n" );
		}
		// then add the rest
		for ( auto it:s.protoList )
		{
			++processedProtoCount;
			switch ( it )
			{
				case Proto::gmq:
					break; // already done
				case Proto::json:
				{
					// NOTE: currently we have only two protocols; if more, we need to be a bit more delicate with 'else if' constructions
					if ( processedProtoCount == 0 )
					{
						fprintf( header, 
							"\tif constexpr ( ParserT::proto == Proto::JSON )\n"
							"\t{\n" );
					}
					else
					{
						if ( processedProtoCount == protoCount )
							fprintf( header, 
								"\telse\n"
								"\t{\n"
								"\t\tstatic_assert( ParserT::proto == Proto::JSON );\n" );
						else
							fprintf( header, 
								"\telse if constexpr ( ParserT::proto == Proto::JSON )\n"
								"\t{\n" );
					}
					impl_generateParamCallBlockForParsingJson( header, s, "\t\t" );
					fprintf( header, "\t}\n" );
					break;
				}
				default:
					assert( false );
			}
		}
	}
}

void impl_generateComposeFunction( FILE* header, CompositeType& s )
{
	assert( s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure );
	fprintf( header, "template<class ComposerT, typename ... Args>\n"
	"void %s(ComposerT& composer, Args&& ... args)\n"
	"{\n", impl_generateComposeFunctionName( s ).c_str() );
	fprintf( header, "\tstatic_assert( std::is_base_of<ComposerBase, ComposerT>::value, \"Composer must be one of GmqComposer<> or JsonComposer<>\" );\n\n" );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForComposing( header, s );


	fprintf( header, "}\n\n" );
}

void impl_generateParseFunction( FILE* header, CompositeType& s )
{
	fprintf( header, "template<class ParserT, typename ... Args>\n"
	"void %s(ParserT& p, Args&& ... args)\n"
	"{\n", impl_generateParseFunctionName( s ).c_str() );
	fprintf( header, "\tstatic_assert( std::is_base_of<ParserBase, ParserT>::value, \"Parser must be one of GmqParser<> or JsonParser<>\" );\n\n" );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForParsing( header, s );

	fprintf( header, "}\n\n" );
}

void generateMessage( FILE* header, CompositeType& s )
{
	bool checked = impl_checkParamNameUniqueness(s);
	checked = impl_checkFollowingExtensionRules(s) && checked;
	if ( !checked )
		throw std::exception();

	impl_generateMessageCommentBlock( header, s );
	impl_GenerateMessageDefaults( header, s );

	impl_generateComposeFunction( header, s );
	impl_generateParseFunction( header, s );
}

void generateMessageAlias( FILE* header, CompositeType& s )
{
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
	fprintf( header, "template<class ComposerT, typename ... Args>\n"
	"void %s_%s_compose(ComposerT& composer, Args&& ... args)\n"
	"{\n", s.type2string(), s.name.c_str() );
	fprintf( header, "\t%s_%s_compose(composer, std::forward<Args>( args )...);\n", impl_kindToString( MessageParameterType::KIND::STRUCT ), s.aliasOf.c_str() );
	fprintf( header, "}\n\n" );

	// compose function
	fprintf( header, "template<class ParserT, typename ... Args>\n"
	"void %s_%s_parse(ParserT& p, Args&& ... args)\n"
	"{\n", s.type2string(), s.name.c_str() );
	fprintf( header, "\t%s_%s_parse(p, std::forward<Args>( args )...);\n", impl_kindToString( MessageParameterType::KIND::STRUCT ), s.aliasOf.c_str() );
	fprintf( header, "}\n\n" );
}
