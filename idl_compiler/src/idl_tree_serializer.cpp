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
	for ( auto& it : s.members )
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
		assert( it->type == CompositeType::Type::structure );
		if ( it->isStruct4Messaging )
			impl_CollectParamNamesFromMessage( params, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}
}

template<class CompositeObjPtrT>
bool impl_checkCompositeTypeNameUniqueness(vector<CompositeObjPtrT>& coll, const char* typeName)
{
	bool ok = true;
	std::map<string, Location> names;
	for ( auto& it : coll )
	{
		auto ins = names.insert( std::make_pair( it->name, it->location ) );
		if ( !ins.second )
		{
			fprintf( stderr, "%s name \"%s\" has already been used, see %s : %d\n", typeName, it->name.c_str(), ins.first->second.fileName.c_str(), ins.first->second.lineNumber );
			ok = false;
		}
	}
	return ok;
}


bool impl_checkCompositeTypeNameUniqueness(Root& s)
{
	bool ok = true;
	for ( auto& scope : s.scopes )
		ok = impl_checkCompositeTypeNameUniqueness(scope->objectList, "MESSAGE") && ok;
	ok = impl_checkCompositeTypeNameUniqueness(s.publishables, "PUBLISHABLE") && ok;
	ok = impl_checkCompositeTypeNameUniqueness(s.structs, "STRUCT") && ok;
	return ok;
}



bool impl_checkParamNameUniqueness(CompositeType& s)
{
	bool ok = true;
	std::map<string, Location> names;
	for ( auto& it : s.members )
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
	for ( size_t i=0; i<s.members.size(); ++i )
	{
		auto& msg = *(s.members[i]);
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

	for ( auto& param : ct.members )
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
		assert( it->type == CompositeType::Type::structure );
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
		fprintf( header, "using %s = impl::MessageName<%lld>;\n", msg->name.c_str(), msg->numID );
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

void collectMemberNamesFromPublishableObjects( vector<unique_ptr<CompositeType>> &structs, set<string>& names )
{
	for ( auto& s : structs )
	{
		assert( s != nullptr );
		assert( s->type == CompositeType::structure || s->type == CompositeType::publishable );
		if ( s->type == CompositeType::publishable || ( s->type == CompositeType::structure && s->isStruct4Publishing ) )
			for ( auto& member : s->members )
			{
				assert( member != nullptr );
				names.insert( member->name );
			}
	}
}

void collectVectorMemberNamesFromPublishableObjects( vector<unique_ptr<CompositeType>> &structs, set<string>& names )
{
	for ( auto& s : structs )
	{
		assert( s != nullptr );
		assert( s->type == CompositeType::structure || s->type == CompositeType::publishable );
		if ( s->type == CompositeType::publishable || ( s->type == CompositeType::structure && s->isStruct4Publishing ) )
			for ( auto& member : s->members )
			{
				assert( member != nullptr );
				if ( member->type.kind == MessageParameterType::KIND::VECTOR )
					names.insert( member->name );
			}
	}
}

void generateNotifierPresenceTesterBlock( FILE* header, Root& root )
{
	set<string> names;
	collectMemberNamesFromPublishableObjects( root.publishables, names );
	collectMemberNamesFromPublishableObjects( root.structs, names );
	set<string> namesOfVectors;
	collectVectorMemberNamesFromPublishableObjects( root.publishables, namesOfVectors );
	collectVectorMemberNamesFromPublishableObjects( root.structs, namesOfVectors );

	fprintf( header, "// member update notifier presence checks\n" );
	fprintf( header, "using index_type_for_array_notifiers = size_t&;\n" );
	for ( auto& name : names )
	{
		fprintf( header, 
			"template<typename T> concept has_void_update_notifier_call_for_%s = requires(T t) { { t.notifyUpdated_%s() }; };\n",
			name.c_str(), name.c_str()
		);
		fprintf( header, 
			"template<typename StateT, typename NodeT> concept has_update_notifier_call_for_%s = requires { { std::declval<StateT>().notifyUpdated_%s(std::declval<NodeT>()) }; };\n",
			name.c_str(), name.c_str()
		);

		if ( namesOfVectors.find( name ) != namesOfVectors.end() )
		{
			fprintf( header, 
				"template<typename T> concept has_element_updated_void_notifier_call_for_%s = requires(T t) { { t.notifyElementUpdated_%s() }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT> concept has_element_updated_notifier_call_for_%s = requires { { std::declval<StateT>().notifyElementUpdated_%s(std::declval<index_type_for_array_notifiers>()) }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename NodeT> concept has_full_element_updated_notifier_call_for_%s = requires { { std::declval<StateT>().notifyElementUpdated_%s(std::declval<index_type_for_array_notifiers>(), std::declval<NodeT>()) }; };\n",
				name.c_str(), name.c_str()
			);

			fprintf( header, 
				"template<typename T> concept has_void_insert_notifier_call_for_%s = requires(T t) { { t.notifyInserted_%s() }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT> concept has_insert_notifier_call_for_%s = requires { { std::declval<StateT>().notifyInserted_%s(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };\n",
				name.c_str(), name.c_str()
			);

			fprintf( header, 
				"template<typename T> concept has_void_erased_notifier_call_for_%s = requires(T t) { { t.notifyErased_%s() }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT> concept has_erased_notifier_call_for_%s = requires { { std::declval<StateT>().notifyErased_%s(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename NodeT> concept has_erased_notifier_call_for_%s = requires { { std::declval<StateT>().notifyErased_%s(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<NodeT>()) }; };\n",
				name.c_str(), name.c_str()
			);
		}
	}
	fprintf( header, "\n" );
}

void orderStructsByDependency( vector<unique_ptr<CompositeType>> &structs, vector<CompositeType*>& out )
{
	size_t processed = 0;
	vector<CompositeType*> tmpStructs;
	while ( processed < structs.size() )
	{
		for ( auto& s : structs )
			if ( s->dependsOnCnt != -1 )
				for ( auto& member : s->members )
					if ( member->type.kind == MessageParameterType::KIND::STRUCT )
						structs[member->type.messageIdx]->dependsOnCnt = 1;
		for ( auto& s : structs )
			if ( s->dependsOnCnt == 0 )
			{
				tmpStructs.push_back( s.get() );
				s->dependsOnCnt = -1;
				++processed;
			}
		for ( auto& s : structs )
			if ( s->dependsOnCnt != -1 )
				s->dependsOnCnt = 0;
	}
	for ( size_t i=0; i<tmpStructs.size(); ++i )
		out.push_back( tmpStructs[tmpStructs.size()-1-i] );
	assert( out.size() == structs.size() );
}

void generateRoot( const char* fileName, FILE* header, Root& s )
{
	bool ok = impl_checkCompositeTypeNameUniqueness(s);
	ok = impl_processScopes(s) && ok;
	ok = impl_processCompositeTypeNamesInMessagesAndPublishables(s) && ok;
	if ( !ok )
		throw std::exception();

	std::set<string> msgParams;
	impl_CollectMessageParamNamesFromRoot( msgParams, s );

	std::set<string> publishableMembers;
	impl_CollectPublishableMemberNamesFromRoot( publishableMembers, s );

	fprintf( header, "#ifndef %s_guard\n"
		"#define %s_guard\n"
		"\n"
		"#include <marshalling.h>\n"
		"#include <publishable_impl.h>\n"
		"\n"
		"namespace m {\n\n",
		fileName, fileName );

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
		assert( it->type == CompositeType::Type::structure );
		if ( it->isStruct4Publishing )
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
		assert( it->type == CompositeType::Type::structure );
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
		generatePublishable( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}

	for ( auto& it : structsOrderedByDependency )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure );
		if ( it->isStruct4Publishing )
		{
			impl_GeneratePublishableStructWrapper( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
			impl_GeneratePublishableStructWrapper4Set( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
			impl_GeneratePublishableStructCopyFn( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
		}
	}

	for ( auto& it : s.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure );
		if ( it->isStruct4Messaging )
			generateMessage( header, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}

	fprintf( header, "\n"
		"} // namespace m\n"
		"\n"
		"#endif // %s_guard\n",
		fileName );
}

