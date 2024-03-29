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

#include "idl_tree_common.h"
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
		case MessageParameterType::KIND::DICTIONARY: return "DICTIONARY";
		case MessageParameterType::KIND::STRUCT: return "STRUCT";
		case MessageParameterType::KIND::DISCRIMINATED_UNION: return "DISCRIMINATED_UNION";
		case MessageParameterType::KIND::EXTENSION: return "EXTENSION";
		default: assert( false ); return "";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <set>




void impl_CollectParamNamesFromeMessageParameter( std::set<string>& params, MessageParameter& s )
{
	params.insert( s.name );
}

void impl_CollectParamNamesFromMessage( std::set<string>& params, std::set<string>& caseParams, CompositeType& s )
{
	if ( s.type == CompositeType::Type::discriminated_union )
	{
		for ( auto& cc : s.getDiscriminatedUnionCases() )
		{
			assert( cc != nullptr );
			impl_CollectParamNamesFromMessage( params, caseParams, *(dynamic_cast<CompositeType*>(&(*(cc)))) );
		}
	}
	else
	{
		bool isCase = s.type == CompositeType::Type::discriminated_union_case;
		for ( auto& it : s.getMembers() )
		{
			assert( it != nullptr );
			if ( it->type.kind == MessageParameterType::KIND::EXTENSION )
				continue;
			impl_CollectParamNamesFromeMessageParameter( params, *(dynamic_cast<MessageParameter*>(&(*(it)))) );
			if ( isCase )
				impl_CollectParamNamesFromeMessageParameter( caseParams, *(dynamic_cast<MessageParameter*>(&(*(it)))) );
		}
	}
}

void impl_CollectMessageParamNamesFromRoot( std::set<string>& params, std::set<string>& caseParams, Root& s )
{
	for ( auto& it : s.messages )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::message );
		impl_CollectParamNamesFromMessage( params, caseParams, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}
	for ( auto& it : s.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->isStruct4Messaging )
			impl_CollectParamNamesFromMessage( params, caseParams, *(dynamic_cast<CompositeType*>(&(*(it)))) );
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
				if ( ducase->name == "unknown" )
				{
					fprintf( stderr, "line %d: identifier \'unknown\' is reserved and cannot be used as a CASE name\n", ducase->location.lineNumber );
					ok = false;
				}
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
				if ( ducase->numID == 0 )
				{
					fprintf( stderr, "line %d: CASE VALUE \'0\' is reserved and cannot be used as a CASE VALUE\n", ducase->location.lineNumber );
					ok = false;
				}
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



bool impl_checkParamNameUniqueness(CompositeType& s, std::map<string, Location>& names)
{
	assert( s.type != CompositeType::Type::discriminated_union );

	bool ok = true;
	// if ( s.type == CompositeType::Type::discriminated_union )
	// {
	// 	for ( auto& it : s.getDiscriminatedUnionCases() )
	// 	{
	// 		assert( it != nullptr );
	// 		impl_checkParamNameUniqueness( *it );
	// 	}
	// }
	// else
	// {
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
	// }
	return ok;
}

bool impl_checkParamNameUniqueness(CompositeType& s)
{
	std::map<string, Location> names;
	if ( s.type == CompositeType::Type::discriminated_union )
	{
		bool ok = true;
		for ( auto& it : s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			ok = impl_checkParamNameUniqueness( *it, names ) && ok;
		}
		return ok;
	}
	else
		return impl_checkParamNameUniqueness( s, names );
}

bool checkParamNameUniqueness(Root& root)
{
	bool ok = true;

	for ( auto& it : root.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		ok = impl_checkParamNameUniqueness(*it) && ok;
	}

	for ( auto& it : root.messages )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::message );
		ok = impl_checkParamNameUniqueness(*it) && ok;
	}

	for ( auto& it : root.publishables )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::publishable );
		ok = impl_checkParamNameUniqueness(*it) && ok;
	}

	return ok;
}


bool impl_checkFollowingExtensionRules(CompositeType& s, bool& extMarkFound, std::map<string, Location>& names)
{
	bool ok = true;
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

bool impl_checkFollowingExtensionRules(CompositeType& s)
{
	// TODO: revise logic around discriminated unions
	bool ok = true;
	bool extMarkFound = false;
	std::map<string, Location> names;
	if ( s.type == CompositeType::Type::discriminated_union )
	{
		bool ok = true;
		for ( auto& it : s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			ok = impl_checkFollowingExtensionRules( *it, extMarkFound, names ) && ok;
		}
		return ok;
	}
	else
		return impl_checkFollowingExtensionRules( s, extMarkFound, names );
}

bool checkFollowingExtensionRules(Root& root)
{
	bool ok = true;
	for ( auto& it : root.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		ok = impl_checkFollowingExtensionRules(*it) && ok;
	}

	for ( auto& it : root.messages )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::message );
		ok = impl_checkFollowingExtensionRules(*it) && ok;
	}

	for ( auto& it : root.publishables )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::publishable );
		ok = impl_checkFollowingExtensionRules(*it) && ok;
	}

	return ok;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void impl_propagateParentPropsToStruct( CompositeType& parent, CompositeType& memberOrArrayElementType )
{
	assert( memberOrArrayElementType.type == CompositeType::Type::structure || memberOrArrayElementType.type == CompositeType::Type::discriminated_union );
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
		case CompositeType::Type::discriminated_union_case:
			memberOrArrayElementType.isStruct4Messaging = memberOrArrayElementType.isStruct4Messaging || parent.isStruct4Messaging;
			memberOrArrayElementType.isStruct4Publishing = memberOrArrayElementType.isStruct4Publishing || parent.isStruct4Publishing;
			memberOrArrayElementType.protoList.insert( parent.protoList.begin(), parent.protoList.end() );
			break;
		default:
			assert( false );
	}
}

bool impl_processCompositeTypeNamesInParams(Root& s, CompositeType& parent, MessageParameter& param, std::vector<CompositeType*>& stack )
{
	bool ok = true;

	if ( param.type.kind == MessageParameterType::KIND::VECTOR )
	{
		if ( param.type.vectorElemKind == MessageParameterType::KIND::STRUCT || param.type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION ) // existance and extentability
		{
			param.type.structIdx = (size_t)(-1);
			for ( size_t i=0; i<s.structs.size(); ++i )
				if ( param.type.name == s.structs[i]->name )
				{
					param.type.structIdx = i;
					if ( param.type.isNonExtendable && !s.structs[i]->isNonExtendable )
					{
						fprintf( stderr, "%s, line %d: %s \"%s\" is not declared as NONEXTENDABLE (see %s declaration at %s, line %d)\n", param.location.fileName.c_str(), param.location.lineNumber, impl_kindToString( param.type.kind ), param.type.name.c_str(), parent.type2string(), s.messages[i]->location.fileName.c_str(), s.messages[i]->location.lineNumber );
						ok = false;
					}
					impl_propagateParentPropsToStruct( parent, *(s.structs[i]) );
//					impl_processCompositeTypeNamesInMessagesAndPublishables(s, *(s.structs[i]), stack, true );
					break;
				}
			if ( param.type.structIdx == (size_t)(-1) )
			{
				fprintf( stderr, "%s, line %d: %s name \"%s\" not found\n", param.location.fileName.c_str(), param.location.lineNumber, impl_kindToString( MessageParameterType::KIND::STRUCT ), param.type.name.c_str() );
				ok = false;
			}
		}
	}
	else if (param.type.kind == MessageParameterType::KIND::DICTIONARY)
	{
		if (param.type.dictionaryValueKind == MessageParameterType::KIND::STRUCT || param.type.dictionaryValueKind == MessageParameterType::KIND::DISCRIMINATED_UNION) // existance and extentability
		{
			param.type.structIdx = (size_t)(-1);
			for (size_t i = 0; i < s.structs.size(); ++i)
			{
				if (param.type.name == s.structs[i]->name)
				{
					param.type.structIdx = i;
					if (param.type.isNonExtendable && !s.structs[i]->isNonExtendable)
					{
						fprintf(stderr, "%s, line %d: %s \"%s\" is not declared as NONEXTENDABLE (see %s declaration at %s, line %d)\n", param.location.fileName.c_str(), param.location.lineNumber, impl_kindToString(param.type.kind), param.type.name.c_str(), parent.type2string(), s.messages[i]->location.fileName.c_str(), s.messages[i]->location.lineNumber);
						ok = false;
					}
					impl_propagateParentPropsToStruct(parent, *(s.structs[i]));
					//					impl_processCompositeTypeNamesInMessagesAndPublishables(s, *(s.structs[i]), stack, true );
					break;
				}
			}
			if (param.type.structIdx == (size_t)(-1))
			{
				fprintf(stderr, "%s, line %d: %s name \"%s\" not found\n", param.location.fileName.c_str(), param.location.lineNumber, impl_kindToString(MessageParameterType::KIND::STRUCT), param.type.name.c_str());
				ok = false;
			}
		}
	}
	else if ( param.type.kind == MessageParameterType::KIND::STRUCT || param.type.kind == MessageParameterType::KIND::DISCRIMINATED_UNION ) // extentability only
	{
		param.type.structIdx = (size_t)(-1);
		for ( size_t i=0; i<s.structs.size(); ++i )
			if ( param.type.name == s.structs[i]->name )
			{
				param.type.structIdx = i;
				if ( param.type.isNonExtendable && !s.structs[i]->isNonExtendable )
				{
					fprintf( stderr, "%s, line %d: %s \"%s\" is not declared as NONEXTENDABLE (see %s declaration at %s, line %d)\n", param.location.fileName.c_str(), param.location.lineNumber, impl_kindToString( param.type.kind ), param.type.name.c_str(), parent.type2string(), s.structs[i]->location.fileName.c_str(), s.structs[i]->location.lineNumber );
					ok = false;
				}
				impl_propagateParentPropsToStruct( parent, *(s.structs[i]) );
				impl_processCompositeTypeNamesInMessagesAndPublishables(s, *(s.structs[i]), stack );
				break;
			}
		if ( param.type.structIdx == (size_t)(-1) )
		{
			fprintf( stderr, "%s, line %d: %s name \"%s\" not found\n", param.location.fileName.c_str(), param.location.lineNumber, impl_kindToString( param.type.kind ), param.type.name.c_str() );
			ok = false;
		}
	}

	return ok;
}

bool impl_processCompositeTypeNamesInMessagesAndPublishables(Root& s, CompositeType& ct, std::vector<CompositeType*>& stack, bool isCollectionElementType )
{
	if ( !ct.processingOK )
		return false;

	if ( !isCollectionElementType ) // do dependency checking
	{
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
	}

	stack.push_back( &ct );

	bool ok = true;

	if ( ct.type != CompositeType::Type::discriminated_union )
	{
		for ( auto& param : ct.getMembers() )
			ok = impl_processCompositeTypeNamesInParams( s, ct, *param, stack ) && ok;
	}
	else
	{
		for ( auto& cs : ct.getDiscriminatedUnionCases() )
		{
			cs->isStruct4Messaging = cs->isStruct4Messaging || ct.isStruct4Messaging;
			cs->isStruct4Publishing = cs->isStruct4Publishing || ct.isStruct4Publishing;
			for ( auto& param : cs->getMembers() )
				ok = impl_processCompositeTypeNamesInParams( s, ct, *param, stack ) && ok;
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
		ct.aliasIdx = structIdx;
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
	for ( auto& s : r.structs )
		ok = impl_processCompositeTypeNamesInMessagesAndPublishables( r, *s ) && ok;
	return ok;
}

void impl_CollectPublishableMemberNamesFromRoot( std::set<string>& params, std::set<string>& caseParams, Root& s )
{
	for ( auto& it : s.publishables )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::publishable );
		impl_CollectParamNamesFromMessage( params, caseParams, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}
	for ( auto& it : s.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->isStruct4Publishing )
			impl_CollectParamNamesFromMessage( params, caseParams, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	}
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

void impl_generateMessageCommentBlock_MemberIterationBlock( FILE* header, CompositeType& s, const char* offset )
{
	assert( s.type != CompositeType::Type::discriminated_union );

	fprintf( header, "(%zd parameters)\n", s.getMembers().size() );

	int count = 0;
	for ( auto& it : s.getMembers() )
	{
		assert( it != nullptr );
		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;
			
		if ( param.type.kind == MessageParameterType::KIND::VECTOR )
		{
			if ( param.type.vectorElemKind == MessageParameterType::KIND::STRUCT || param.type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
				fprintf( header, "//%s%d. %s<%s%s %s>", offset, count, impl_kindToString( param.type.kind ), param.type.isNonExtendable ? "NONEXTENDABLE " : " ", impl_kindToString( param.type.vectorElemKind ), param.type.name.c_str() );
			else
				fprintf( header, "//%s%d. %s<%s>", offset, count, impl_kindToString( param.type.kind ), impl_kindToString( param.type.vectorElemKind ) );
			fprintf( header, " %s", param.name.c_str() );
		}
		else if ( param.type.kind == MessageParameterType::KIND::STRUCT || param.type.kind == MessageParameterType::KIND::DISCRIMINATED_UNION )
		{
			fprintf( header, "//%s%d. %s %s%s", offset, count, impl_kindToString( param.type.kind ), param.type.isNonExtendable ? "NONEXTENDABLE " : "", param.type.name.c_str() );
			fprintf( header, " %s", param.name.c_str() );
		}
		else
			fprintf( header, "//%s%d. %s %s", offset, count, impl_kindToString( param.type.kind ), param.name.c_str() );

		if ( param.type.hasDefault )
		{
			switch ( param.type.kind )
			{
				case MessageParameterType::KIND::ENUM: fprintf( header, " (DEFAULT: %s::%s)", param.type.name.c_str(), param.type.stringDefault.c_str() ); break;
				case MessageParameterType::KIND::INTEGER: fprintf( header, " (DEFAULT: %lld)", (int64_t)(param.type.numericalDefault) ); break;
				case MessageParameterType::KIND::UINTEGER: fprintf( header, " (DEFAULT: %lld)", (uint64_t)(param.type.numericalDefault) ); break;
				case MessageParameterType::KIND::REAL: fprintf( header, " (DEFAULT: %f)", param.type.numericalDefault ); break;
				case MessageParameterType::KIND::CHARACTER_STRING: fprintf( header, " (DEFAULT: \"%s\")", param.type.stringDefault.c_str() ); break;
				default:
					fprintf( header, " (REQUIRED)" );
					break;
			}
		}
		else
			fprintf( header, " (REQUIRED)" );
		fprintf( header, "\n" );
	}
}

void impl_generateMessageCommentBlock( FILE* header, CompositeType& s )
{
	assert( s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union );
	fprintf( header, "//**********************************************************************\n" );
	fprintf( header, "// %s \"%s\" %sTargets: ", s.type2string(), s.name.c_str(), s.isNonExtendable ? "NONEXTENDABLE " : "" );
	for ( auto t:s.protoList )
		switch ( t )
		{
			case Proto::gmq: fprintf( header, "GMQ " ); break;
			case Proto::json: fprintf( header, "JSON " ); break;
			default: assert( false );
		}
	if ( s.isDiscriminatedUnion() )
	{
		fprintf( header, "(%zd cases)\n", s.getDiscriminatedUnionCases().size() );
		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			fprintf( header, "//  CASE %s (%zd parameters)", cs.name.c_str(), cs.getMembers().size() );
			impl_generateMessageCommentBlock_MemberIterationBlock( header, cs, "    " );
		}
	}
	else
	{
		impl_generateMessageCommentBlock_MemberIterationBlock( header, s, "  " );
	}

	fprintf( header, "//**********************************************************************\n\n" );
}


void orderStructsByDependency( std::vector<unique_ptr<CompositeType>> &structs, std::vector<CompositeType*>& out, std::unordered_set<size_t>& collElementTypes )
{
	size_t processed = 0;
	std::vector<CompositeType*> tmpStructs;
	while ( processed < structs.size() )
	{
		for ( auto& s : structs )
			if ( s->type == CompositeType::Type::structure && s->dependsOnCnt != -1 )
				for ( auto& member : s->getMembers() )
				{
					if ( member->type.kind == MessageParameterType::KIND::STRUCT || member->type.kind == MessageParameterType::KIND::DISCRIMINATED_UNION )
						structs[member->type.structIdx]->dependsOnCnt = 1;
					else if ( member->type.kind == MessageParameterType::KIND::VECTOR )
					{
						if ( member->type.vectorElemKind == MessageParameterType::KIND::STRUCT || member->type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
						{
							assert( member->type.structIdx < structs.size() );
							collElementTypes.insert( member->type.structIdx );
							structs[member->type.structIdx]->isStruct4Publishing = structs[member->type.structIdx]->isStruct4Publishing || s->isStruct4Publishing;
							structs[member->type.structIdx]->isStruct4Messaging = structs[member->type.structIdx]->isStruct4Messaging || s->isStruct4Messaging;
						}
					}
					else if (member->type.kind == MessageParameterType::KIND::DICTIONARY)
					{
						if (member->type.dictionaryValueKind == MessageParameterType::KIND::STRUCT || member->type.dictionaryValueKind == MessageParameterType::KIND::DISCRIMINATED_UNION)
						{
							assert(member->type.structIdx < structs.size());
							collElementTypes.insert(member->type.structIdx);
							structs[member->type.structIdx]->isStruct4Publishing = structs[member->type.structIdx]->isStruct4Publishing || s->isStruct4Publishing;
							structs[member->type.structIdx]->isStruct4Messaging = structs[member->type.structIdx]->isStruct4Messaging || s->isStruct4Messaging;
						}
					}
				}
			else if ( s->type == CompositeType::Type::discriminated_union && s->dependsOnCnt != -1 )
				for ( auto& cs : s->getDiscriminatedUnionCases() )
					for ( auto& member : cs->getMembers() )
					{
						if ( member->type.kind == MessageParameterType::KIND::STRUCT || member->type.kind == MessageParameterType::KIND::DISCRIMINATED_UNION )
							structs[member->type.structIdx]->dependsOnCnt = 1;
						else if ( member->type.kind == MessageParameterType::KIND::VECTOR )
						{
							if ( member->type.vectorElemKind == MessageParameterType::KIND::STRUCT || member->type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
							{
								assert( member->type.structIdx < structs.size() );
								collElementTypes.insert( member->type.structIdx );
								structs[member->type.structIdx]->isStruct4Publishing = structs[member->type.structIdx]->isStruct4Publishing || s->isStruct4Publishing;
								structs[member->type.structIdx]->isStruct4Messaging = structs[member->type.structIdx]->isStruct4Messaging || s->isStruct4Messaging;
							}
						}
						else if (member->type.kind == MessageParameterType::KIND::DICTIONARY)
						{
							if (member->type.dictionaryValueKind == MessageParameterType::KIND::STRUCT || member->type.dictionaryValueKind == MessageParameterType::KIND::DISCRIMINATED_UNION)
							{
								assert(member->type.structIdx < structs.size());
								collElementTypes.insert(member->type.structIdx);
								structs[member->type.structIdx]->isStruct4Publishing = structs[member->type.structIdx]->isStruct4Publishing || s->isStruct4Publishing;
								structs[member->type.structIdx]->isStruct4Messaging = structs[member->type.structIdx]->isStruct4Messaging || s->isStruct4Messaging;
							}
						}
						//						else if ( member->type.kind == MessageParameterType::KIND::VECTOR && ( member->type.vectorElemKind == MessageParameterType::KIND::STRUCT || member->type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION ) )
//							structs[member->type.structIdx]->dependsOnCnt = 1;
					}
		for ( auto& s : structs )
			if ( ( s->type == CompositeType::Type::structure || s->type == CompositeType::Type::discriminated_union ) && s->dependsOnCnt == 0 )
			{
				tmpStructs.push_back( s.get() );
				s->dependsOnCnt = -1;
				++processed;
			}
		for ( auto& s : structs )
			if ( ( s->type == CompositeType::Type::structure || s->type == CompositeType::Type::discriminated_union ) && s->dependsOnCnt != -1 )
				s->dependsOnCnt = 0;
	}
	for ( size_t i=0; i<tmpStructs.size(); ++i )
		out.push_back( tmpStructs[tmpStructs.size()-1-i] );
	assert( out.size() == structs.size() );

	for (auto& s : structs)
		s->dependsOnCnt = 0;
}

bool impl_isResolved(CompositeType& s, const std::vector<bool> alreadyResolved)
{
	assert( s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union ||
			s.type == CompositeType::Type::discriminated_union_case);

	if(s.isDiscriminatedUnion())
	{
		for ( auto& cs : s.getDiscriminatedUnionCases() )
		{
			if(!impl_isResolved(*cs, alreadyResolved))
				return false;
		}
	}
	else
	{
		for ( auto& member : s.getMembers() )
		{
			if( member->type.kind == MessageParameterType::KIND::STRUCT ||
				member->type.kind == MessageParameterType::KIND::DISCRIMINATED_UNION )
			{
				if(!alreadyResolved.at(member->type.structIdx))
					return false;
			}
		}
	}
	return true;
}


std::vector<CompositeType*> orderStructsByDependency2( const std::vector<unique_ptr<CompositeType>> &structs )
{
	size_t processed = 0;
	std::vector<CompositeType*> result;
	std::vector<bool> resolved(structs.size(), false);

	bool stillChanging = true;
	while(stillChanging)
	{
		stillChanging = false;
		for(size_t i = 0; i != structs.size(); ++i)
		{
			if(resolved[i])
				continue;

			bool currentResolved = impl_isResolved(*structs[i], resolved);
			if(currentResolved)
			{
				resolved[i] = true;
				stillChanging = true;
				result.push_back(structs[i].get());
			}
		}
	}
	if(result.size() != structs.size())
		throw std::logic_error("Internal error at 'orderStructsByDependency2'");

	return result;
}

void impl_generatePublishableCommentBlock_MemberIterationBlock( FILE* header, CompositeType& s, int& count )
{
	assert( s.type != CompositeType::Type::discriminated_union );
	for ( auto& it : s.getMembers() )
	{
		assert( it != nullptr );
		MessageParameter& param = *it;
		if ( param.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		++count;
			
		if ( param.type.kind == MessageParameterType::KIND::VECTOR )
		{
			if ( param.type.vectorElemKind == MessageParameterType::KIND::STRUCT || param.type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
				fprintf( header, "// %d. %s<%s%s %s>", count, impl_kindToString( param.type.kind ), param.type.isNonExtendable ? "NONEXTENDABLE " : " ", impl_kindToString( param.type.vectorElemKind ), param.type.name.c_str() );
			else
				fprintf( header, "// %d. %s<%s>", count, impl_kindToString( param.type.kind ), impl_kindToString( param.type.vectorElemKind ) );
			fprintf( header, " %s", param.name.c_str() );
		}
		else if ( param.type.kind == MessageParameterType::KIND::DICTIONARY )
		{
			if ( param.type.vectorElemKind == MessageParameterType::KIND::STRUCT || param.type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
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

		assert( !param.type.hasDefault );
		fprintf( header, "\n" );
	}
}

void impl_generatePublishableCommentBlock( FILE* header, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable );
	fprintf( header, "//**********************************************************************\n" );

	int count = 0;
	if ( s.isDiscriminatedUnion() )
	{
		fprintf( header, "// %s %s (%zd cases)\n", s.type2string(), s.name.c_str(), s.getDiscriminatedUnionCases().size() );
		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			fprintf( header, "// CASE %s (%zd parameters)\n", cs.name.c_str(), cs.getMembers().size() );
			impl_generatePublishableCommentBlock_MemberIterationBlock( header, cs, count );
		}
	}
	else
	{
		fprintf( header, "// %s %s (%zd parameters)\n", s.type2string(), s.name.c_str(), s.getMembers().size() );
		impl_generatePublishableCommentBlock_MemberIterationBlock( header, s, count );
	}

	fprintf( header, "//**********************************************************************\n\n" );
}


void processDiscriminatedUnionCaseParams( Root& s )
{

	for ( auto& it : s.structs )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->type == CompositeType::Type::discriminated_union )
		{
			CompositeType& du = *(dynamic_cast<CompositeType*>(&(*(it))));
			for ( auto& duit: du.getDiscriminatedUnionCases() )
			{
				assert( duit != nullptr );
				auto& cs = *duit;
				assert( typeid( cs ) == typeid( CompositeType ) );
				assert( cs.type == CompositeType::Type::discriminated_union_case );
				for ( auto& mbit: cs.getMembers() )
				{
					mbit->duCaseParam = true;
					mbit->caseName = cs.name;
				}
			}
		}
	}
}

void preprocessRoot( Root& s )
{
	bool ok = impl_checkCompositeTypeNameUniqueness(s);
	ok = impl_checkDiscriminatedUnions(s) && ok;

	ok = checkParamNameUniqueness(s) && ok;
	ok = checkFollowingExtensionRules(s) && ok;

	ok = impl_processScopes(s) && ok;
	ok = impl_processCompositeTypeNamesInMessagesAndPublishables(s) && ok;
	if (!ok)
		throw std::exception();

	processDiscriminatedUnionCaseParams( s );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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