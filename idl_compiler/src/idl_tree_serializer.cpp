/* -------------------------------------------------------------------------------
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

#include "idl_tree_serializer.h"
#include "raiistdiofile.h"


const char* impl_kindToString( MessageParameterType::KIND kind )
{
	switch ( kind )
	{
		case MessageParameterType::KIND::ENUM: return "ENUM";
		case MessageParameterType::KIND::UNDEFINED: return "UNDEFINED";
		case MessageParameterType::KIND::INTEGER: return "INTEGER";
		case MessageParameterType::KIND::UINTEGER: return "UINTEGER";
		case MessageParameterType::KIND::REAL: return "REAL";
		case MessageParameterType::KIND::CHARACTER_STRING: return "CHARACTER_STRING";
		case MessageParameterType::KIND::BYTE_ARRAY: return "BYTE_ARRAY";
		case MessageParameterType::KIND::BLOB: return "BLOB";
		case MessageParameterType::KIND::VECTOR: return "VECTOR";
		case MessageParameterType::KIND::STRUCT: return "STRUCT";
		case MessageParameterType::KIND::DISCRIMINATED_UNION: return "DISCRIMINATED_UNION";
		case MessageParameterType::KIND::EXTENSION: return "EXTENSION";
		default: assert( false ); return "";
	}
}

string paramNameToNameTagType( string name )
{
	return fmt::format( "{}_Type", name );
}

string paramNameToNameTagStruct( string name )
{
	return fmt::format( "{}_Struct", name );
}

string paramNameToNameObject( string name )
{
	return name;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <set>




void impl_CollectParamNamesFromeMessageParameter( std::set<string>& params, MessageParameter& s )
{
	params.insert( s.name );
}

void impl_CollectParamNamesFromMessage( std::set<string>& params, CompositeType& s )
{
	for ( auto& it : s.getMembers() )
	{
		assert( it != nullptr );
		if ( it->type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		impl_CollectParamNamesFromeMessageParameter( params, *(dynamic_cast<MessageParameter*>(&(*(it)))) );
	}
}

void impl_CollectMessageParamNamesFromRoot( std::set<string>& params, Root& s )
{
	for ( auto& it : s.messages )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::message );
		impl_CollectParamNamesFromMessage( params, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}
	for ( auto& it : s.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->isStruct4Messaging )
			impl_CollectParamNamesFromMessage( params, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}
}

template<class CompositeObjPtrT>
bool impl_checkCompositeTypeNameUniqueness(vector<CompositeObjPtrT>& coll)
{
	struct PrevInstaceInfo
	{
		Location location;
		string typeStr;
	};

	bool ok = true;
	std::map<string, PrevInstaceInfo> names;
	for ( auto& it : coll )
	{
		string typeStr = it->type2string();
		auto ins = names.insert( std::make_pair( it->name, PrevInstaceInfo({it->location, typeStr}) ) );
		if ( !ins.second )
		{
			if ( typeStr == ins.first->second.typeStr )
				fprintf( stderr, "line %d: %s \"%s\" has already been defined, see %s : %d\n", it->location.lineNumber, typeStr.c_str(), it->name.c_str(), ins.first->second.location.fileName.c_str(), ins.first->second.location.lineNumber );
			else
				fprintf( stderr, "line %d: %s \"%s\" has already been defined as %s, see %s : %d\n", it->location.lineNumber, typeStr.c_str(), it->name.c_str(), ins.first->second.typeStr.c_str(), ins.first->second.location.fileName.c_str(), ins.first->second.location.lineNumber );
			ok = false;
		}
	}
	return ok;
}


bool impl_checkCompositeTypeNameUniqueness(Root& s)
{
	bool ok = true;
	for ( auto& scope : s.scopes )
		ok = impl_checkCompositeTypeNameUniqueness(scope->objectList) && ok;
	ok = impl_checkCompositeTypeNameUniqueness(s.publishables) && ok;
	ok = impl_checkCompositeTypeNameUniqueness(s.structs) && ok;
	return ok;
}


bool impl_checkDiscriminatedUnions(Root& s)
{
	bool ok = true;
	for ( auto& du : s.structs )
		if ( du->type == CompositeType::Type::discriminated_union )
		{
			auto& cases = du->getDiscriminatedUnionCases();
			std::map<string, Location> labels;
			for ( auto& ducase: cases )
			{
				assert( ducase->type == CompositeType::Type::discriminated_union_case );
				auto ins = labels.insert( std::make_pair( ducase->name, ducase->location ) );
				if ( !ins.second )
				{
					fprintf( stderr, "line %d: CASE \"%s\" has already been defined, see %s : %d\n", ducase->location.lineNumber, ducase->name.c_str(), ins.first->second.fileName.c_str(), ins.first->second.lineNumber );
					ok = false;
				}
			}
			std::map<uint64_t, Location> values;
			for ( auto& ducase: cases )
			{
				assert( ducase->type == CompositeType::Type::discriminated_union_case );
				auto ins = values.insert( std::make_pair( ducase->numID, ducase->location ) );
				if ( !ins.second )
				{
					fprintf( stderr, "line %d: CASE VALUE \'%lld\' has already been used, see %s : %d\n", ducase->location.lineNumber, ducase->numID, ins.first->second.fileName.c_str(), ins.first->second.lineNumber );
					ok = false;
				}
			}
			std::map<string, Location> fieldNames;
			for ( auto& ducase: cases )
			{
				assert( ducase->type == CompositeType::Type::discriminated_union_case );
				auto& members = ducase->getMembers();
				for ( auto& m: members )
				{
					auto ins = fieldNames.insert( std::make_pair( m->name, m->location ) );
					if ( !ins.second )
					{
						fprintf( stderr, "line %d: Name \'%s\' has already been defined within this DISCRIMINATED UNION, see %s : %d\n", m->location.lineNumber, m->name.c_str(), ins.first->second.fileName.c_str(), ins.first->second.lineNumber );
						ok = false;
					}
				}
			}
		}
	return ok;
}



bool impl_checkParamNameUniqueness(CompositeType& s)
{
	bool ok = true;
	std::map<string, Location> names;
	for ( auto& it : s.getMembers() )
	{
		if ( it->type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		auto ins = names.insert( std::make_pair( it->name, it->location ) );
		if ( !ins.second )
		{
			fprintf( stderr, "%s parameter \"%s\" has already been used within this %s, see %s : %d\n", s.type2string(), it->name.c_str(), s.type2string(), ins.first->second.fileName.c_str(), ins.first->second.lineNumber );
			ok = false;
		}
	}
	return ok;
}


bool impl_checkFollowingExtensionRules(CompositeType& s)
{
	bool ok = true;
	bool extMarkFound = false;
	std::map<string, Location> names;
	for ( size_t i=0; i<s.getMembers().size(); ++i )
	{
		auto& msg = *(s.getMembers()[i]);
		if ( msg.type.kind != MessageParameterType::KIND::EXTENSION )
		{
			if ( msg.type.hasDefault && !extMarkFound )
			{
				fprintf( stderr, "File %s, line %d: %s parameter \"%s\" with default value cannot appear before extension blocks\n", msg.location.fileName.c_str(), msg.location.lineNumber, s.type2string(), msg.name.c_str() );
				ok = false;
			}
			else if ( (!msg.type.hasDefault) && extMarkFound )
			{
				fprintf( stderr, "File %s, line %d: %s parameter \"%s\" with no default value cannot appear inside extension blocks\n", msg.location.fileName.c_str(), msg.location.lineNumber, s.type2string(), msg.name.c_str() );
				ok = false;
			}
		}
		else
			extMarkFound = true;
	}
	return ok;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void impl_propagateParentPropsToStruct( CompositeType& parent, CompositeType& memberOrArrayElementType )
{
	assert( memberOrArrayElementType.type == CompositeType::Type::structure );
	switch ( parent.type )
	{
		case CompositeType::Type::message:
			memberOrArrayElementType.isStruct4Messaging = true;
			memberOrArrayElementType.protoList.insert( parent.protoList.begin(), parent.protoList.end() );
			break;
		case CompositeType::Type::publishable:
			memberOrArrayElementType.isStruct4Publishing = true;
			break;
		case CompositeType::Type::structure:
		case CompositeType::Type::discriminated_union:
			memberOrArrayElementType.isStruct4Messaging = memberOrArrayElementType.isStruct4Messaging || parent.isStruct4Messaging;
			memberOrArrayElementType.isStruct4Publishing = memberOrArrayElementType.isStruct4Publishing || parent.isStruct4Publishing;
			memberOrArrayElementType.protoList.insert( parent.protoList.begin(), parent.protoList.end() );
			break;
		default:
			assert( false );
	}
}

bool impl_processCompositeTypeNamesInMessagesAndPublishables(Root& s, CompositeType& ct, std::vector<CompositeType*>& stack )
{
	if ( !ct.processingOK )
		return false;

	for ( size_t i=0; i<stack.size(); ++i )
	{
		if ( &ct == stack[i] )
		{
			fprintf( stderr, "Error: cyclic dependency\n" );
			for ( size_t j=i; j<stack.size(); ++j )
			{
				stack[j]->processingOK = false;
				fprintf( stderr, "    File \"%s\", line %d: %s %s depends on ...\n", stack[j]->location.fileName.c_str(), stack[j]->location.lineNumber, stack[j]->type2string(), stack[j]->name.c_str() );
			}
			ct.processingOK = false;
			fprintf( stderr, "    File \"%s\", line %d: %s %s\n", ct.location.fileName.c_str(), ct.location.lineNumber, ct.type2string(), ct.name.c_str() );
			return false;
		}
	}

	stack.push_back( &ct );

	bool ok = true;

	for ( auto& param : ct.getMembers() )
	{
		if ( param->type.kind == MessageParameterType::KIND::VECTOR )
		{
			if ( param->type.vectorElemKind == MessageParameterType::KIND::STRUCT ) // existance and extentability
			{
				param->type.messageIdx = (size_t)(-1);
				for ( size_t i=0; i<s.structs.size(); ++i )
					if ( param->type.name == s.structs[i]->name )
					{
						param->type.messageIdx = i;
						if ( param->type.isNonExtendable && !s.structs[i]->isNonExtendable )
						{
							fprintf( stderr, "%s, line %d: %s \"%s\" is not declared as NONEXTENDABLE (see %s declaration at %s, line %d)\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( param->type.kind ), param->type.name.c_str(), ct.type2string(), s.messages[i]->location.fileName.c_str(), s.messages[i]->location.lineNumber );
							ok = false;
						}
						impl_propagateParentPropsToStruct( ct, *(s.structs[i]) );
						impl_processCompositeTypeNamesInMessagesAndPublishables(s, *(s.structs[i]), stack );
						break;
					}
				if ( param->type.messageIdx == (size_t)(-1) )
				{
					fprintf( stderr, "%s, line %d: %s name \"%s\" not found\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( MessageParameterType::KIND::STRUCT ), param->type.name.c_str() );
					ok = false;
				}
			}
		}
		else if ( param->type.kind == MessageParameterType::KIND::STRUCT ) // extentability only
		{
			param->type.messageIdx = (size_t)(-1);
			for ( size_t i=0; i<s.structs.size(); ++i )
				if ( param->type.name == s.structs[i]->name )
				{
					param->type.messageIdx = i;
					if ( param->type.isNonExtendable && !s.structs[i]->isNonExtendable )
					{
						fprintf( stderr, "%s, line %d: %s \"%s\" is not declared as NONEXTENDABLE (see %s declaration at %s, line %d)\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( param->type.kind ), param->type.name.c_str(), ct.type2string(), s.structs[i]->location.fileName.c_str(), s.structs[i]->location.lineNumber );
						ok = false;
					}
					impl_propagateParentPropsToStruct( ct, *(s.structs[i]) );
					impl_processCompositeTypeNamesInMessagesAndPublishables(s, *(s.structs[i]), stack );
					break;
				}
			if ( param->type.messageIdx == (size_t)(-1) )
			{
				fprintf( stderr, "%s, line %d: %s name \"%s\" not found\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( param->type.kind ), param->type.name.c_str() );
				ok = false;
			}
		}
	}

	stack.pop_back();

	return ok;
}

bool impl_processCompositeTypeNamesInMessagesAndPublishables(Root& s, CompositeType& ct )
{
	if ( !ct.processingOK )
		return false;

	std::vector<CompositeType*> stack;
	bool ok = true;

	if ( ct.isAlias )
	{
		stack.push_back( &ct );
		size_t structIdx = (size_t)(-1);
		for ( size_t i=0; i<s.structs.size(); ++i )
			if ( ct.aliasOf == s.structs[i]->name )
			{
				structIdx = i;
				if ( ct.isNonExtendable && !s.structs[i]->isNonExtendable )
				{
					fprintf( stderr, "%s, line %d: %s \"%s\" is not declared as NONEXTENDABLE (see %s declaration at %s, line %d)\n", ct.location.fileName.c_str(), ct.location.lineNumber, ct.type2string(), ct.name.c_str(), s.structs[i]->type2string(), s.structs[i]->location.fileName.c_str(), s.structs[i]->location.lineNumber );
					ok = false;
				}
				impl_propagateParentPropsToStruct( ct, *(s.structs[i]) );
				impl_processCompositeTypeNamesInMessagesAndPublishables(s, *(s.structs[i]), stack );
				break;
			}
		if ( structIdx == (size_t)(-1) )
		{
			fprintf( stderr, "%s, line %d: %s \"%s\" not found\n", ct.location.fileName.c_str(), ct.location.lineNumber, ct.type2string(), ct.name.c_str() );
			ok = false;
		}
		return ok;
	}
	else
	{
		return impl_processCompositeTypeNamesInMessagesAndPublishables( s, ct, stack );
	}

}
bool impl_processCompositeTypeNamesInMessagesAndPublishables(Root& r)
{
	bool ok = true;
	for ( auto& s : r.messages )
		ok = impl_processCompositeTypeNamesInMessagesAndPublishables( r, *s ) && ok;
	for ( auto& s : r.publishables )
		ok = impl_processCompositeTypeNamesInMessagesAndPublishables( r, *s ) && ok;
//	for ( auto& s : r.structs )
//		ok = impl_processCompositeTypeNamesInMessagesAndPublishables( r, *s ) && ok;
	return ok;
}

void impl_CollectPublishableMemberNamesFromRoot( std::set<string>& params, Root& s )
{
	for ( auto& it : s.publishables )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::publishable );
		impl_CollectParamNamesFromMessage( params, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}
	for ( auto& it : s.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->isStruct4Publishing )
			impl_CollectParamNamesFromMessage( params, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}
}

void impl_insertScopeDetails( FILE* header, Scope& scope )
{
	assert( scope.objectList.size() != 0 );
	fprintf( header, "//  %s\n", scope.name.c_str() );
	fprintf( header, "//  {\n" );
	for ( auto msg : scope.objectList )
		fprintf( header, "//    %s\n", msg->name.c_str() );
	fprintf( header, "//  }\n" );
}

void impl_insertScopeList( FILE* header, Root& r )
{
	fprintf( header, "//////////////////////////////////////////////////////////////\n" );
	fprintf( header, "//\n" );
	fprintf( header, "//  Scopes:\n" );
	fprintf( header, "//\n" );
	for ( auto& s : r.scopes )
	{
		impl_insertScopeDetails( header, *s );
		fprintf( header, "//\n" );
	}
	fprintf( header, "//////////////////////////////////////////////////////////////\n\n" );
}

void impl_generateScopeEnum( FILE* header, Scope& scope )
{
	for ( auto msg : scope.objectList )
	{
		fprintf( header, "using %s = ::globalmq::marshalling::impl::MessageName<%lld>;\n", msg->name.c_str(), msg->numID );
	}
	fprintf( header, "\n" );
}

void generateMessageParamNameBlock( FILE* header, const std::set<string>& params )
{
	// types
	for ( auto name : params )
	{
		fprintf( header, "using %s = NamedParameter<struct %s>;\n", paramNameToNameTagType( name ).c_str(), paramNameToNameTagStruct( name ).c_str() );
	}
	fprintf( header, "\n" );

	// objects
	for ( auto name : params )
	{
		fprintf( header, "constexpr %s::TypeConverter %s;\n", paramNameToNameTagType( name ).c_str(), paramNameToNameObject( name ).c_str() );
	}
	fprintf( header, "\n\n" );
}

void generatePublishableMemberNameBlock( FILE* header, const std::set<string>& names )
{
	fprintf( header, "// member name presence checkers\n" );
	for ( auto name : names )
		fprintf( header, "template<typename T> concept has_%s_member = requires { { T::%s }; };\n", name.c_str(), name.c_str() );
	fprintf( header, "\n\n" );
}

void orderStructsByDependency( vector<unique_ptr<CompositeType>> &structs, vector<CompositeType*>& out )
{
	size_t processed = 0;
	vector<CompositeType*> tmpStructs;
	while ( processed < structs.size() )
	{
		for ( auto& s : structs )
			if ( s->type == CompositeType::Type::structure && s->dependsOnCnt != -1 ) // TODO: DU
				for ( auto& member : s->getMembers() )
					if ( member->type.kind == MessageParameterType::KIND::STRUCT )
						structs[member->type.messageIdx]->dependsOnCnt = 1;
		for ( auto& s : structs )
			if ( s->type == CompositeType::Type::structure && s->dependsOnCnt == 0 )
			{
				tmpStructs.push_back( s.get() );
				s->dependsOnCnt = -1;
				++processed;
			}
			else if ( s->type == CompositeType::Type::discriminated_union && s->dependsOnCnt == 0 ) // TODO: DU
			{
				tmpStructs.push_back( s.get() );
				s->dependsOnCnt = -1;
				++processed;
			}
		for ( auto& s : structs )
			if ( s->type == CompositeType::Type::structure && s->dependsOnCnt != -1 )
				s->dependsOnCnt = 0;
			else if ( s->type == CompositeType::Type::discriminated_union && s->dependsOnCnt != -1 ) // TODO: DU
				s->dependsOnCnt = 0;
	}
	for ( size_t i=0; i<tmpStructs.size(); ++i )
		out.push_back( tmpStructs[tmpStructs.size()-1-i] );
	assert( out.size() == structs.size() );
}

void addLibAliasingBlock( FILE* header )
{
	fprintf( header, "// Useful aliases:\n" );
	fprintf( header, "//     (note: since clang apparently too often requires providing template arguments for aliased type ctors we use wrappers instead of type aliasing)\n" );
	fprintf( header, "using Buffer = globalmq::marshalling::Buffer;\n" );
	fprintf( header, "using FileReadBuffer = globalmq::marshalling::FileReadBuffer;\n" );
	fprintf( header, "template<class BufferT>\n" );
	fprintf( header, "class GmqComposer : public globalmq::marshalling::GmqComposer<BufferT> { public: GmqComposer( BufferT& buff_ ) : globalmq::marshalling::GmqComposer<BufferT>( buff_ ) {} };\n" );
	fprintf( header, "template<class BufferT>\n" );
	fprintf( header, "class GmqParser : public globalmq::marshalling::GmqParser<BufferT> { public: /*GmqParser( BufferT& buff_ ) : globalmq::marshalling::GmqParser<BufferT>( buff_ ) {}*/ GmqParser( typename BufferT::ReadIteratorT& iter ) : globalmq::marshalling::GmqParser<BufferT>( iter ) {} GmqParser( const GmqParser<BufferT>& other ) : globalmq::marshalling::GmqParser<BufferT>( other ) {} GmqParser& operator = ( const GmqParser<BufferT>& other ) { globalmq::marshalling::GmqParser<BufferT>::operator = ( other ); return *this; }};\n" );
	fprintf( header, "template<class BufferT>\n" );
	fprintf( header, "class JsonComposer : public globalmq::marshalling::JsonComposer<BufferT> { public: JsonComposer( BufferT& buff_ ) : globalmq::marshalling::JsonComposer<BufferT>( buff_ ) {} };\n" );
	fprintf( header, "template<class BufferT>\n" );
	fprintf( header, "class JsonParser : public globalmq::marshalling::JsonParser<BufferT> { public: /*JsonParser( BufferT& buff_ ) : globalmq::marshalling::JsonParser<BufferT>( buff_ ) {}*/ JsonParser( typename BufferT::ReadIteratorT& iter ) : globalmq::marshalling::JsonParser<BufferT>( iter ) {} JsonParser( const JsonParser<BufferT>& other ) : globalmq::marshalling::JsonParser<BufferT>( other ) {} JsonParser& operator = ( const JsonParser<BufferT>& other ) { globalmq::marshalling::JsonParser<BufferT>::operator = ( other ); return *this; } };\n" );
	fprintf( header, "template<class T>\n" );
	fprintf( header, "class SimpleTypeCollectionWrapper : public globalmq::marshalling::SimpleTypeCollectionWrapper<T> { public: SimpleTypeCollectionWrapper( T& coll ) : globalmq::marshalling::SimpleTypeCollectionWrapper<T>( coll ) {} };\n" );
	fprintf( header, "template<class LambdaSize, class LambdaNext>\n" );
	fprintf( header, "class CollectionWrapperForComposing : public globalmq::marshalling::CollectionWrapperForComposing<LambdaSize, LambdaNext> { public: CollectionWrapperForComposing(LambdaSize &&lsize, LambdaNext &&lnext) : globalmq::marshalling::CollectionWrapperForComposing<LambdaSize, LambdaNext>(std::forward<LambdaSize>(lsize), std::forward<LambdaNext>(lnext)) {} };\n" );
	fprintf( header, "template<class LambdaCompose>\n" );
	fprintf( header, "class MessageWrapperForComposing : public globalmq::marshalling::MessageWrapperForComposing<LambdaCompose> { public: MessageWrapperForComposing(LambdaCompose &&lcompose) : globalmq::marshalling::MessageWrapperForComposing<LambdaCompose>( std::forward<LambdaCompose>(lcompose) ) {} };\n" );
	fprintf( header, "template<class LambdaSize, class LambdaNext>\n" );
	fprintf( header, "class CollectionWrapperForParsing : public globalmq::marshalling::CollectionWrapperForParsing<LambdaSize, LambdaNext> { public: CollectionWrapperForParsing(LambdaSize &&lsizeHint, LambdaNext &&lnext) : globalmq::marshalling::CollectionWrapperForParsing<LambdaSize, LambdaNext>(std::forward<LambdaSize>(lsizeHint), std::forward<LambdaNext>(lnext)) {} };\n" );
	fprintf( header, "template<class LambdaParse>\n" );
	fprintf( header, "class MessageWrapperForParsing : public globalmq::marshalling::MessageWrapperForParsing<LambdaParse> { public: MessageWrapperForParsing(LambdaParse &&lparse) : globalmq::marshalling::MessageWrapperForParsing<LambdaParse>(std::forward<LambdaParse>(lparse)) {} };\n" );
	fprintf( header, "template<typename msgID_, class LambdaHandler>\n" );
	fprintf( header, "MessageHandler<msgID_, LambdaHandler> makeMessageHandler( LambdaHandler &&lhandler ) { return globalmq::marshalling::makeMessageHandler<msgID_, LambdaHandler>(std::forward<LambdaHandler>(lhandler)); }\n" );
	fprintf( header, "template<class LambdaHandler>\n" );
	fprintf( header, "DefaultMessageHandler<LambdaHandler> makeDefaultMessageHandler( LambdaHandler &&lhandler ) { return globalmq::marshalling::makeDefaultMessageHandler<LambdaHandler>(std::forward<LambdaHandler>(lhandler)); }\n" );
	fprintf( header, "\n" );
}

void generateStateConcentratorFactory( FILE* header, Root& root )
{
	fprintf( header, "template<class InputBufferT, class ComposerT>\n" );
	fprintf( header, "class StateConcentratorFactory : public ::globalmq::marshalling::StateConcentratorFactoryBase<InputBufferT, ComposerT>\n" );
	fprintf( header, "{\n" );
	fprintf( header, "public:\n" );
	fprintf( header, "\tvirtual StateConcentratorBase<InputBufferT, ComposerT>* createConcentrator( uint64_t typeID )\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\tswitch( typeID )\n" );
	fprintf( header, "\t\t{\n" );

	for ( auto& it : root.publishables )
	{
		auto& obj_1 = it;
		assert( obj_1 != nullptr );
		assert( typeid( *(obj_1) ) == typeid( CompositeType ) );
		assert( obj_1->type == CompositeType::Type::publishable );
		fprintf( header, "\t\t\tcase %lld:\n", obj_1->numID );
		fprintf( header, "\t\t\t\treturn new %s_WrapperForConcentrator<%s, InputBufferT, ComposerT>;\n", obj_1->name.c_str(), obj_1->name.c_str() );
	}
	fprintf( header, "\t\t\tdefault:\n" );
	fprintf( header, "\t\t\t\treturn nullptr;\n" );
	fprintf( header, "\t\t}\n" );
	fprintf( header, "\t}\n" );
	fprintf( header, "};\n" );
}

void generateRoot( const char* fileName, uint32_t fileChecksum, FILE* header, const char* metascope, std::string platformPrefix, std::string classNotifierName, Root& s )
{
	bool ok = impl_checkCompositeTypeNameUniqueness(s);
	ok = impl_checkDiscriminatedUnions(s) && ok;
	ok = impl_processScopes(s) && ok;
	ok = impl_processCompositeTypeNamesInMessagesAndPublishables(s) && ok;
	if ( !ok )
		throw std::exception();

	std::set<string> msgParams;
	impl_CollectMessageParamNamesFromRoot( msgParams, s );

	std::set<string> publishableMembers;
	impl_CollectPublishableMemberNamesFromRoot( publishableMembers, s );

	fprintf( header, "#ifndef %s_%08x_guard\n"
		"#define %s_%08x_guard\n"
		"\n"
		"#include <marshalling.h>\n"
		"#include <publishable_impl.h>\n"
		"using namespace globalmq::marshalling;\n"
		"namespace %s {\n\n"
		"#ifdef METASCOPE_%s_ALREADY_DEFINED\n"
		"#error metascope must reside in a single idl file\n"
		"#endif\n"
		"#define METASCOPE_%s_ALREADY_DEFINED\n\n",
		fileName, fileChecksum, fileName, fileChecksum, metascope, metascope, metascope );

	addLibAliasingBlock( header );

	impl_insertScopeList( header, s );

	generateMessageParamNameBlock( header, msgParams );
	generatePublishableMemberNameBlock( header, publishableMembers );
	generateNotifierPresenceTesterBlock( header, s );

	vector<CompositeType*> structsOrderedByDependency;
	orderStructsByDependency( s.structs, structsOrderedByDependency );

	for ( auto& it : s.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->type == CompositeType::Type::structure && it->isStruct4Publishing )
		{
			impl_generatePublishableStructForwardDeclaration( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
			impl_GeneratePublishableStructWrapperForwardDeclaration( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
			impl_GeneratePublishableStructWrapper4SetForwardDeclaration( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
			fprintf( header, "\n" );
		}
	}

	fprintf( header, "\n" );

	for ( auto it : structsOrderedByDependency )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->isStruct4Publishing )
			impl_generatePublishableStruct( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}

	for ( auto& scope : s.scopes )
	{
		fprintf( header, "namespace %s {\n\n", scope->name.c_str() );

		impl_generateScopeEnum( header, *scope );
		impl_generateScopeHandler( header, *scope );
		impl_generateScopeComposerForwardDeclaration( header, *scope );

		for ( auto it : scope->objectList )
		{
			assert( it != nullptr );
			assert( typeid( *(it) ) == typeid( CompositeType ) );
			assert( it->type == CompositeType::Type::message );
			if ( !it->isAlias )
				generateMessage( header, *it );
			else
				generateMessageAlias( header, *it );
		}

		impl_generateScopeComposer( header, *scope );

		fprintf( header, "} // namespace %s \n\n", scope->name.c_str() );
	}

	for ( auto& it : s.publishables )
	{
		auto& obj_1 = it;
		assert( obj_1 != nullptr );
		assert( typeid( *(obj_1) ) == typeid( CompositeType ) );
		assert( obj_1->type == CompositeType::Type::publishable );
		generatePublishable( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))), platformPrefix, classNotifierName );
	}

	fprintf( header, "//===============================================================================\n" );
	fprintf( header, "// Publishable c-structures\n" );
	fprintf( header, "// Use them as-is or copy and edit member types as necessary\n\n" );
	for ( auto it : structsOrderedByDependency )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->isStruct4Publishing )
			generatePublishableAsCStruct( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}

	for ( auto& it : s.publishables )
	{
		auto& obj_1 = it;
		assert( obj_1 != nullptr );
		assert( typeid( *(obj_1) ) == typeid( CompositeType ) );
		assert( obj_1->type == CompositeType::Type::publishable );
		generatePublishableAsCStruct( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}
	fprintf( header, "\n//===============================================================================\n\n" );

	if ( !s.publishables.empty() )
	{
		generateStateConcentratorFactory( header, s );
		fprintf( header, "\n//===============================================================================\n\n" );
	}

	for ( auto& it : structsOrderedByDependency )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->isStruct4Publishing )
		{
			impl_GeneratePublishableStructWrapper( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
			impl_GeneratePublishableStructWrapper4Set( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
		}
	}

	for ( auto& it : s.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->isStruct4Messaging )
			generateMessage( header, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}

	fprintf( header, "\n"
		"} // namespace %s\n"
		"\n"
		"#endif // %s_%08x_guard\n",
		metascope,
		fileName, fileChecksum );
}

uint32_t adler32( uint8_t* buff, size_t sz ) 
{
    uint32_t a = 1;
	uint32_t b = 0;
    
    for (size_t i=0; i < sz; ++i)
    {
        a = (a + buff[i]) % 65521;
        b = (b + a) % 65521;
    }
    
    return (b << 16) | a;
}

uint32_t idlFileChecksum( std::string path )
{
	RaiiStdioFile file( fopen( path.c_str(), "rb" ) );
	int res = fseek( file, 0, SEEK_END );
	if ( res != 0 )
		throw std::exception();

	long sz = ftell( file );
	if ( sz == -1 )
		throw std::exception();
	
	res = fseek( file, 0, SEEK_SET );
	if ( res != 0 )
		throw std::exception();

	uint8_t* buff = new uint8_t[sz];

	fread( buff, 1, sz, file );
	uint32_t ret = adler32( buff, sz );
	delete [] buff;

	return ret;
}