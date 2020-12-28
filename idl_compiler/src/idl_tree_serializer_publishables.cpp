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

const char* paramTypeToLibType( MessageParameterType::KIND kind )
{
	switch( kind )
	{
		case MessageParameterType::KIND::INTEGER: return "::m::impl::SignedIntegralType";
		case MessageParameterType::KIND::UINTEGER: return "::m::impl::UnsignedIntegralType";
		case MessageParameterType::KIND::REAL: return "::m::impl::RealType";
		case MessageParameterType::KIND::CHARACTER_STRING: return "::m::impl::StringType";
		case MessageParameterType::KIND::STRUCT: return "::m::impl::StructType";
		default: return nullptr;
	}
}

string impl_parameterTypeToDescriptiveString( Root& s, const MessageParameterType& type )
{
	switch ( type.kind )
	{
		case MessageParameterType::KIND::INTEGER: return "INTEGER";
		case MessageParameterType::KIND::UINTEGER: return "UINTEGER";
		case MessageParameterType::KIND::REAL: return "REAL";
		case MessageParameterType::KIND::CHARACTER_STRING: return "CHARACTER_STRING";
		case MessageParameterType::KIND::BYTE_ARRAY: return "BYTE_ARRAY";
		case MessageParameterType::KIND::BLOB: return "BLOB";

		case MessageParameterType::KIND::STRUCT: 
			assert( type.messageIdx <= s.structs.size() );
			return fmt::format( "STRUCT {}", s.structs[type.messageIdx]->name );
		case MessageParameterType::KIND::VECTOR: 
			if ( type.vectorElemKind == MessageParameterType::KIND::STRUCT )
			{
				assert( type.messageIdx <= s.structs.size() );
				return fmt::format( "VECTOR<STRUCT {}>", s.structs[type.messageIdx]->name );
			}
			else if ( type.vectorElemKind == MessageParameterType::KIND::ENUM )
				return fmt::format( "VECTOR<ENUM {}>", type.name );
			else
				return fmt::format( "VECTOR<{}>", impl_kindToString( type.vectorElemKind ) );

		case MessageParameterType::KIND::ENUM: return ( "ENUM {}", type.name );
		case MessageParameterType::KIND::UNDEFINED: return "UNDEFINED";
		case MessageParameterType::KIND::EXTENSION: return "EXTENSION";
		default: assert( false ); return "";
	}
}

const char* paramTypeToLeafeParser( MessageParameterType::KIND kind )
{
	switch( kind )
	{
		case MessageParameterType::KIND::INTEGER: return "publishableParseLeafeInteger";
		case MessageParameterType::KIND::UINTEGER: return "publishableParseLeafeUnsignedInteger";
		case MessageParameterType::KIND::REAL: return "publishableParseLeafeReal";
		case MessageParameterType::KIND::CHARACTER_STRING: return "publishableParseLeafeString";
		default: return nullptr;
	}
}

const char* paramTypeToParser( MessageParameterType::KIND kind )
{
	switch( kind )
	{
		case MessageParameterType::KIND::INTEGER: return "publishableParseInteger";
		case MessageParameterType::KIND::UINTEGER: return "publishableParseUnsignedInteger";
		case MessageParameterType::KIND::REAL: return "publishableParseReal";
		case MessageParameterType::KIND::CHARACTER_STRING: return "publishableParseString";
		default: return nullptr;
	}
}

std::string impl_generateComposeFunctionNameForStructMemeberOfPublishable( MessageParameter& s )
{
	assert( s.type.kind == MessageParameterType::KIND::STRUCT );
	return fmt::format( "publishable_{}_{}_compose", CompositeType::type2string( CompositeType::Type::structure ), s.type.name );
}

std::string impl_generateComposeFunctionNameForPublishableStruct( CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure );
	return fmt::format( "publishable_{}_{}_compose", s.type2string(), s.name );
}

std::string impl_generateParseFunctionNameForStructMemeberOfPublishable( MessageParameter& s )
{
	assert( s.type.kind == MessageParameterType::KIND::STRUCT );
	return fmt::format( "publishable_{}_{}_parse", CompositeType::type2string( CompositeType::Type::structure ), s.type.name );
}

std::string impl_generateParseFunctionNameForPublishableStruct( CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure );
	return fmt::format( "publishable_{}_{}_parse", s.type2string(), s.name );
}

std::string impl_generatePublishableStructName( CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure );
	return fmt::format( "publishable_{}_{}", s.type2string(), s.name );
}

std::string impl_generatePublishableStructName( MessageParameter& s )
{
	assert( s.type.kind == MessageParameterType::KIND::STRUCT );
	return fmt::format( "publishable_{}_{}", CompositeType::type2string( CompositeType::Type::structure ), s.type.name );
}

void impl_GeneratePublishableStateMemberPresenceCheckingBlock( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable || ( s.type == CompositeType::Type::structure && s.isStruct4Publishing ) );
	for ( auto& it : s.members )
	{
		assert( it != nullptr );
		fprintf( header, "\tstatic constexpr bool has_%s = has_%s_member<T>;\n", it->name.c_str(), it->name.c_str() );
		fprintf( header, "\tstatic_assert( has_%s, \"type T must have member T::%s of a type corresponding to IDL type %s\" );\n", it->name.c_str(), it->name.c_str(), impl_parameterTypeToDescriptiveString( root, it->type ).c_str() );
	}
	fprintf( header, "\n" );
}

void impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( FILE* header, Root& root, CompositeType& s, const char* offset )
{
	/* Short summary:
	*  template<typename StateT> ...
	*	has_void_update_notifier_call_for_%s            notifyUpdated_%s<T>()
	*	has_update_notifier_call_for_%s                 notifyUpdated_%s<T, MemberT>(MemberT&)
	*	// for vectors
	*	has_element_updated_void_notifier_call_for_%s   notifyElementUpdated_%s<T>()
	*	has_element_updated_notifier_call_for_%s        notifyElementUpdated_%s<T>(index_type_for_array_notifiers)
	*	has_full_element_updated_notifier_call_for_%s   notifyElementUpdated_%s<T, MemberT>(index_type_for_array_notifiers, MemberT)
	*	has_void_insert_notifier_call_for_%s            notifyInserted_%s<T>()
	*	has_insert_notifier_call_for_%s                 notifyInserted_%s<T>(index_type_for_array_notifiers, index_type_for_array_notifiers)
	*	has_void_erased_notifier_call_for_%s            notifyErased_%s<T>()
	*	has_erased_notifier_call2_for_%s                notifyErased_%s<T>(index_type_for_array_notifiers, index_type_for_array_notifiers)
	*	has_erased_notifier_call3_for_%s                notifyErased_%s<T, MemberT>(index_type_for_array_notifiers, index_type_for_array_notifiers, MemberT>()	
	*/
	assert( s.type == CompositeType::Type::publishable || ( s.type == CompositeType::Type::structure && s.isStruct4Publishing ) );
	for ( auto& member : s.members )
	{
		assert( member != nullptr );
		fprintf( header, "%sstatic constexpr bool has_void_update_notifier_for_%s = has_void_update_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
		fprintf( header, "%sstatic constexpr bool has_update_notifier_for_%s = has_update_notifier_call_for_%s<T, decltype(T::%s)>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
		/*if ( member->type.kind == MessageParameterType::KIND::VECTOR )
		{
			fprintf( header, "%sstatic constexpr bool has_element_updated_void_notifier_for_%s = has_element_updated_void_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_element_updated_notifier_for_%s = has_element_updated_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%susing %sT = decltype(T::%s);\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_full_element_updated_notifier_for_%s = has_full_element_updated_notifier_call_for_%s<T, %sT&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_void_insert_notifier_for_%s = has_void_insert_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_insert_notifier_for_%s = has_insert_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_void_erased_notifier_for_%s = has_void_erased_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_erased_notifier2_for_%s = has_erased_notifier_call2_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_erased_notifier3_for_%s = has_erased_notifier_call3_for_%s<T, GMQ_COLL vector<%sT>&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
		}
		else*/
			fprintf( header, "%sstatic constexpr bool has_any_notifier_for_%s = has_void_update_notifier_for_%s || has_update_notifier_for_%s;\n", 
				offset, member->name.c_str(), member->name.c_str(), member->name.c_str()
			);
	}
	fprintf( header, "\n" );
}

void impl_GeneratePublishableStateMemberGetter( FILE* header, Root& root, CompositeType& s, MessageParameter& param )
{
	assert( s.type == CompositeType::Type::publishable || ( s.type == CompositeType::Type::structure && s.isStruct4Publishing ) );
	if ( param.type.isNumericType() )
		fprintf( header, "\tauto get_%s() { return t.%s; }\n", param.name.c_str(), param.name.c_str() );
	else if ( param.type.kind == MessageParameterType::KIND::VECTOR )
	{
		if ( param.type.vectorElemKind == MessageParameterType::KIND::STRUCT )
		{
			assert( root.structs.size() > param.type.messageIdx );
			fprintf( header, "\tauto get_%s() { return m::VectorOfStructRefWrapper<%s_RefWrapper<typename decltype(T::%s)::value_type>, decltype(T::%s)>(t.%s); }\n", 
				param.name.c_str(), root.structs[param.type.messageIdx]->name.c_str(), param.name.c_str(), param.name.c_str(), param.name.c_str() );
		}
		else
			fprintf( header, "\tauto get_%s() { return m::VectorOfSimpleTypeRefWrapper(t.%s); }\n", param.name.c_str(), param.name.c_str() );
	}
	else
		fprintf( header, "\tconst auto& get_%s() { return t.%s; }\n", param.name.c_str(), param.name.c_str() );
}

void impl_GeneratePublishableStateMemberGetter4Set( FILE* header, Root& root, const char* rootName, MessageParameter& param, size_t idx )
{
	if ( param.type.kind == MessageParameterType::KIND::STRUCT )
	{
		string rootType;
		string addr;
		if ( rootName == nullptr )
		{
			rootType = "RootT";
			addr = "address";
		}
		else
		{
			rootType = fmt::format( "{}_Wrapper", rootName );
			addr = "GMQ_COLL vector<size_t>()";
		}
		assert( param.type.messageIdx < root.structs.size() );
		fprintf( header, "\tauto get4set_%s() { return %s_RefWrapper4Set<decltype(T::%s), %s>(t.%s, *this, %s, %zd); }\n", 
			param.name.c_str(), root.structs[param.type.messageIdx]->name.c_str(), param.name.c_str(), rootType.c_str(), param.name.c_str(), addr.c_str(), idx );
	}
	else if ( param.type.kind == MessageParameterType::KIND::VECTOR )
	{
		const char* libType = paramTypeToLibType( param.type.vectorElemKind );
		assert( libType != nullptr );
		switch( param.type.vectorElemKind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
//					fprintf( header, "\tauto get4set_%s() { return m::VectorOfSimpleTypeRefWrapper4Set<decltype(T::%s), impl::%s, %s_Wrapper>(t.%s, *this, GMQ_COLL vector<size_t>(), %zd); }\n", 
//						param.name.c_str(), param.name.c_str(), libType, rootName, param.name.c_str(), idx );
				fprintf( header, "\tauto get4set_%s() { return m::VectorRefWrapper4Set<decltype(T::%s), %s, %s_Wrapper>(t.%s, *this, GMQ_COLL vector<size_t>(), %zd); }\n", 
					param.name.c_str(), param.name.c_str(), libType, rootName, param.name.c_str(), idx );
				break;
			case MessageParameterType::KIND::STRUCT:
				assert( param.type.messageIdx < root.structs.size() );
				fprintf( header, 
					"\tauto get4set_%s() { return m::VectorOfStructRefWrapper4Set<decltype(T::%s), %s, %s_Wrapper, %s_RefWrapper4Set<typename decltype(T::%s)::value_type, %s_Wrapper>>"
					"(t.%s, *this, GMQ_COLL vector<size_t>(), %zd); }\n", 
					param.name.c_str(), param.name.c_str(),
					impl_generatePublishableStructName( *(root.structs[param.type.messageIdx]) ).c_str(), 
					rootName, 
					root.structs[param.type.messageIdx]->name.c_str(), 
					param.name.c_str(),
					rootName, 
					param.name.c_str(), idx );
				break;
			default:
				assert( false ); // not (yet) implemented
		}
	}
}


void impl_generateComposeFunctionForPublishableStruct( FILE* header, Root& root, CompositeType& obj )
{
	fprintf( header, 
		"\ttemplate<class ComposerT, class T>\n"
		"\tstatic\n"
		"\tvoid compose( ComposerT& composer, const T& t )\n"
		"\t{\n"
	);

	const char* composer = "composer";

	for ( size_t i=0; i<obj.members.size(); ++i )
	{
		auto& it = obj.members[i];
		assert( it != nullptr );
		auto member = *it;
		const char* addSepar = i + 1 < obj.members.size() ? "true": "false";
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "\t\tm::impl::publishableStructComposeInteger( %s, t.%s, \"%s\", %s );\n", composer, member.name.c_str(), member.name.c_str(), addSepar );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "\t\tm::impl::publishableStructComposeUnsignedInteger( %s, t.%s, \"%s\", %s );\n", composer, member.name.c_str(), member.name.c_str(), addSepar );
				break;
			case MessageParameterType::KIND::REAL:
				fprintf( header, "\t\tm::impl::publishableStructComposeReal( %s, t.%s, \"%s\", %s );\n", composer, member.name.c_str(), member.name.c_str(), addSepar );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "\t\tm::impl::publishableStructComposeString( %s, t.%s, \"%s\", %s );\n", composer, member.name.c_str(), member.name.c_str(), addSepar );
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf( header, "\t\t//assert( false ); // NOT IMPLEMENTED (YET);\n", composer, member.name.c_str() );
				break;
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "\t\tm::impl::composePublishableStructBegin( %s, \"%s\" );\n", composer, member.name.c_str() );
				fprintf( header, "\t\t%s::compose( %s, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), composer, member.name.c_str() );
				fprintf( header, "\t\tm::impl::composePublishableStructEnd( %s, %s );\n", composer, addSepar );
				break;
			default:
				assert( false ); // not implemented (yet)
		}
	}

	fprintf( header, "\t}\n" );
}

void impl_generateContinueParsingFunctionForPublishableStruct( FILE* header, Root& root, CompositeType& obj )
{
	fprintf( header, 
		"\ttemplate<class ParserT, class T, class RetT = void>\n"
		"\tstatic\n"
		"\tRetT parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset )\n"
		"\t{\n"
		"\t\t//****  ContinueParsing  **************************************************************************************************************************************************************\n" 
		"\t\tstatic_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );\n"
		"\t\tconstexpr bool reportChanges = std::is_same<RetT, bool>::value;\n"
		"\t\tbool changed = false;\n"
	);
	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, obj, "\t\t" );
	fprintf( header, 
		"\t\tGMQ_ASSERT( addr.size() );\n"
		"\t\tswitch ( addr[offset] )\n"
		"\t\t{\n"
	);

	for ( size_t i=0; i<obj.members.size(); ++i )
	{
		fprintf( header, "\t\t\tcase %zd:\n", i );
		assert( obj.members[i] != nullptr );
		auto& member = *(obj.members[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				fprintf( header, "\t\t\t\tassert( addr.size() == offset + 1 );\n" );
				fprintf( header, 
					"\t\t\t\t\tif constexpr( has_any_notifier_for_%s|| reportChanges )\n"
					"\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tdecltype(T::%s) newVal;\n"
					"\t\t\t\t\t\tm::impl::%s<ParserT, decltype(T::%s)>( parser, &newVal );\n",
					member.name.c_str(), paramTypeToLeafeParser( member.type.kind ), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tbool currentChanged = newVal != t.%s;\n"
					"\t\t\t\t\t\tif ( currentChanged )\n"
					"\t\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr ( reportChanges )\n"
					"\t\t\t\t\t\t\t\tchanged = true;\n"
					"\t\t\t\t\t\t\tif constexpr ( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr ( has_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s( t.%s );\n",
					member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tt.%s = newVal;\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t}\n"
					"\t\t\t\t\telse\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\tm::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s) );\n",
					paramTypeToLeafeParser( member.type.kind ), member.name.c_str(), member.name.c_str()
				);
				break;
			}
			case  MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "\t\t\t\tif ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child\n" );
				fprintf( header, "\t\t\t\t{\n" );
				fprintf( header, "\t\t\t\t\tm::impl::publishableParseLeafeStructBegin( parser );\n" );
				fprintf( header,
					"\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tdecltype(T::%s) temp_%s;\n"
					"\t\t\t\t\t\t%s::copy<decltype(T::%s), decltype(T::%s)>( t.%s, temp_%s );\n", 
					member.name.c_str(), member.name.c_str(), impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() 
				);
				fprintf( header, 
					"\t\t\t\t\t\tif ( changedCurrent )\n"
					"\t\t\t\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s( temp_%s );\n",
					member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr ( reportChanges )\n"
					"\t\t\t\t\t\t\t\tchanged = true;\n"
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t}\n"
				);
				fprintf( header, 
					"\t\t\t\t\telse if constexpr( has_void_update_notifier_for_%s || reportChanges )\n"
					"\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, "\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::Size), bool>( parser, t.%s );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tif ( changedCurrent )\n"
					"\t\t\t\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr ( reportChanges )\n"
					"\t\t\t\t\t\t\t\tchanged = true;\n"
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t}\n"
					"\t\t\t\t\telse\n"
					"\t\t\t\t\t{\n" 
					"\t\t\t\t\t\t%s::parse( parser, t.%s );\n"
					"\t\t\t\t\t}\n"
					"\t\t\t\t\tm::impl::publishableParseLeafeStructEnd( parser );\n"
					"\t\t\t\t}\n" ,
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\telse // let child continue parsing\n"
					"\t\t\t\t{\n"
				);


				fprintf( header,
					"\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tdecltype(T::%s) temp_%s;\n"
					"\t\t\t\t\t\t%s::copy<decltype(T::%s), decltype(T::%s)>( t.%s, temp_%s );\n", 
					member.name.c_str(), member.name.c_str(), impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s, addr, offset + 1 );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() 
				);
				fprintf( header, 
					"\t\t\t\t\t\tif ( changedCurrent )\n"
					"\t\t\t\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s( temp_%s );\n",
					member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr ( reportChanges )\n"
					"\t\t\t\t\t\t\t\tchanged = true;\n"
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t}\n"
				);
				fprintf( header, 
					"\t\t\t\t\telse if constexpr( has_void_update_notifier_for_%s || reportChanges )\n"
					"\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, "\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::Size), bool>( parser, t.%s, addr, offset + 1 );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tif ( changedCurrent )\n"
					"\t\t\t\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr ( reportChanges )\n"
					"\t\t\t\t\t\t\t\tchanged = true;\n"
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t}\n"
					"\t\t\t\t\telse\n"
					"\t\t\t\t\t{\n" 
					"\t\t\t\t\t\t%s::parse( parser, t.%s, addr, offset + 1 );\n"
					"\t\t\t\t\t}\n"
					"\t\t\t\t}\n" ,
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str()
				);
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				const char* libType = paramTypeToLibType( member.type.vectorElemKind );
				fprintf( header, 
					"\t\t\t\t\t\tif ( addr.size() == 1 )\n"
					"\t\t\t\t\t\t{\n"
					"\t\t\t\t\t\t\tm::impl::publishableParseLeafeValueBegin( parser );\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\t\t\t\tVectorOfSimpleTypeBody::parse<ComposerT, decltype(T::%s), impl::%s>( parser, t.%s );\n", 
					member.name.c_str(), 
					member.type.vectorElemKind == MessageParameterType::KIND::STRUCT ? impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() : libType,
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tm::impl::parseStateUpdateBlockEnd( parser );\n"
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t\telse\n"
					"\t\t\t\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\t}\n"
				);
				fprintf( header, "\t\t\t\tassert( addr.size() > offset + 1 );\n" );
				fprintf( header, 
					"\t\t\t\t\t\tif ( addr.size() == 1 )\n"
					"\t\t\t\t\t\t\tthrow std::exception(); // bad format, TODO: ...\n"
					"\t\t\t\t\t\t// TODO: forward to child\n"
				);
				break;
			}
			default:
				assert( false );
		}
		fprintf( header, "\t\t\t\tbreak;\n" );
	}

	fprintf( header, "\t\t\tdefault:\n" );
	fprintf( header, "\t\t\t\tthrow std::exception(); // unexpected\n" );
	fprintf( header, 
		"\t\t}\n"
		"\t\tif constexpr ( reportChanges )\n"
		"\t\t\treturn changed;\n"
		"\t}\n"
	);
}

void impl_generateParseFunctionForPublishableStruct( FILE* header, Root& root, CompositeType& obj )
{
	fprintf( header, 
		"\ttemplate<class ParserT, class T, class RetT = void>\n"
		"\tstatic\n"
		"\tRetT parse( ParserT& parser, T& t )\n"
		"\t{\n"
		"\t\t//****  ParseFunctionForPublishableStruct  **************************************************************************************************************************************************************\n" 
		"\t\tstatic_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );\n"
		"\t\tconstexpr bool reportChanges = std::is_same<RetT, bool>::value;\n"
		"\t\tbool changed = false;\n"
	);

	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, obj, "\t\t" );

	for ( size_t i=0; i<obj.members.size(); ++i )
	{
		assert( obj.members[i] != nullptr );
		auto& member = *(obj.members[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				fprintf( header, 
					"\t\tif constexpr( has_any_notifier_for_%s || reportChanges )\n"
					"\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\tdecltype(T::%s) newVal;\n"
					"\t\t\tm::impl::%s<ParserT, decltype(T::%s)>( parser, &newVal, \"%s\" );\n",
					member.name.c_str(), 
					paramTypeToParser( member.type.kind ), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\tbool currentChanged = newVal != t.%s;\n"
					"\t\t\tif ( currentChanged )\n"
					"\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\tif constexpr ( reportChanges )\n"
					"\t\t\t\t\tchanged = true;\n"
					"\t\t\t\tif constexpr ( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\tif constexpr ( has_update_notifier_for_%s )\n"
					"\t\t\t\t\tt.notifyUpdated_%s( t.%s );\n",
					member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\tt.%s = newVal;\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t}\n"
					"\t\t}\n"
					"\t\telse\n"
				);
				fprintf( header, 
					"\t\t\tm::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s), \"%s\" );\n",
					paramTypeToParser( member.type.kind ), member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, "\n" );
				break;
			}
			case  MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "\t\tm::impl::parsePublishableStructBegin( parser, \"%s\" );\n", member.name.c_str() );

				fprintf( header, 
					"\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\tdecltype(T::%s) temp_%s;\n"
					"\t\t\t%s::copy<decltype(T::%s), decltype(T::%s)>( t.%s, temp_%s );\n", 
					member.name.c_str(), member.name.c_str(), impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, "\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::Size), bool>( parser, t.%s );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\tif ( changedCurrent )\n"
					"\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t\t\t\tt.notifyUpdated_%s( temp_%s );\n",
					member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\tif constexpr ( reportChanges )\n"
					"\t\t\t\t\tchanged = true;\n"
					"\t\t\t}\n"
					"\t\t}\n"
				);
				fprintf( header, 
					"\t\telse if constexpr( has_void_update_notifier_for_%s || reportChanges )\n"
					"\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, "\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::Size), bool>( parser, t.%s );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\tif ( changedCurrent )\n"
					"\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\tif constexpr ( reportChanges )\n"
					"\t\t\t\t\tchanged = true;\n"
					"\t\t\t}\n"
					"\t\t}\n"
					"\t\telse\n"
					"\t\t\t%s::parse( parser, t.%s );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str()
				);

				fprintf( header, "\t\tm::impl::parsePublishableStructEnd( parser );\n" );
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				assert( member.type.messageIdx < root.structs.size() );
				fprintf( header, 
					"\t\t\t\t\tif ( addr.size() > 1 )\n"
				);
				const char* libType = paramTypeToLibType( member.type.vectorElemKind );
				fprintf( header, 
					"\t\t\t\t\t\tVectorOfSimpleTypeBody::parse<ParserT, decltype(T::%s), %s, %s>( parser, t.%s, addr, 1 );\n", 
					member.name.c_str(),
					libType, 
					member.type.vectorElemKind == MessageParameterType::KIND::STRUCT ? impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() : libType,
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\telse\n"
					"\t\t\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\tm::impl::publishableParseLeafeVectorBegin( parser );\n"
					"\t\t\t\t\t\tVectorOfSimpleTypeBody::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n" 
					"\t\t\t\t\t\tm::impl::publishableParseLeafeVectorEnd( parser );\n",
					member.name.c_str(),
					member.type.vectorElemKind == MessageParameterType::KIND::STRUCT ? impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() : libType,
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t}\n"
				);
				break;
			}
			default:
				assert( false );
		}
	}

	fprintf( header, 
		"\n"
		"\t\tif constexpr ( reportChanges )\n"
		"\t\t\treturn changed;\n"
		"\t}\n"
	);
}

void impl_generatePublishableStructForwardDeclaration( FILE* header, Root& root, CompositeType& obj )
{
	fprintf( header, 
		"struct %s;\n",
		impl_generatePublishableStructName( obj ).c_str()
	);
}

void impl_GeneratePublishableStructCopyFn( FILE* header, Root& root, CompositeType& s )
{
	fprintf( header, 
//		"struct Publishable_%s_Copier {\n"
		"\ttemplate<typename UserT>\n"
		"\tstatic void copy(const UserT& src, UserT& dst) {\n"
	);

	for ( size_t i=0; i<s.members.size(); ++i )
	{
		assert( s.members[i] != nullptr );
		auto& member = *(s.members[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, 
					"\t\tdst.%s = src.%s;\n",
					member.name.c_str(), member.name.c_str()
				);
				break;
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "\t\t%s::copy( src.%s, dst.%s );\n",
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str()
				);
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				assert( member.type.messageIdx < root.structs.size() );
				switch ( member.type.kind )
				{
					case MessageParameterType::KIND::INTEGER:
					case MessageParameterType::KIND::UINTEGER:
					case MessageParameterType::KIND::REAL:
					case MessageParameterType::KIND::CHARACTER_STRING:
						fprintf( header, "\t\timpl::copyVector<declval(UserT::%s), impl::%s>( src.%s, dst.%s );\n",
							s.name.c_str(), paramTypeToLibType( member.type.vectorElemKind ), member.name.c_str(), member.name.c_str()
						);
						break;
					case MessageParameterType::KIND::STRUCT:
						fprintf( header, "\t\timpl::copyVector<declval(UserT::%s), Publishable_%s_Copier>( src.%s, dst.%s );\n",
							member.name.c_str(), s.name.c_str(), member.name.c_str(), member.name.c_str()
						);
						break;
					default:
						assert( false ); // TODO: revise or add cases
				}
			}
			default:
				assert( false ); // TODO: revise or add cases
		}
	}

	fprintf( header, 
		"\t}\n"
//		"};\n\n"
	);
}

void impl_generatePublishableStruct( FILE* header, Root& root, CompositeType& obj )
{
	fprintf( header, 
		"struct %s : public impl::StructType\n"
		"{\n",
		impl_generatePublishableStructName( obj ).c_str()
	);

	impl_generateComposeFunctionForPublishableStruct( header, root, obj );
	fprintf( header, "\n" );
	impl_generateParseFunctionForPublishableStruct( header, root, obj );
	fprintf( header, "\n" );
	impl_generateContinueParsingFunctionForPublishableStruct( header, root, obj );
	fprintf( header, "\n" );
	impl_GeneratePublishableStructCopyFn( header, root, obj );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStateMemberSetter( FILE* header, Root& root, bool forRoot, const char* rootName, MessageParameter& param, size_t idx )
{
	assert( (forRoot && rootName != nullptr) || (forRoot == false && rootName == nullptr) );
	const char* composer = forRoot ? "*composer" : "root.getComposer()";
	const char* addrVector = forRoot ? "GMQ_COLL vector<size_t>()" : "address";
	if ( !forRoot )
		rootName = "RootT";
	fprintf( header, 
		"\tvoid set_%s( decltype(T::%s) val) { \n"
		"\t\tt.%s = val; \n",
		param.name.c_str(), param.name.c_str(), param.name.c_str()
	);
//	if ( forRoot )
	fprintf( header, 
		"\t\tm::impl::composeAddressInPublishable( %s, %s, %zd );\n",
		composer, addrVector, idx
	);
//	else
//	fprintf( header, "\treturn;\n", composer );
	switch ( param.type.kind )
	{
		case MessageParameterType::KIND::INTEGER:
			fprintf( header, "\t\tm::impl::publishableComposeLeafeInteger( %s, t.%s );\n", composer, param.name.c_str() );
			break;
		case MessageParameterType::KIND::UINTEGER:
			fprintf( header, "\t\tm::impl::publishableComposeLeafeUnsignedInteger( %s, t.%s );\n", composer, param.name.c_str() );
			break;
		case MessageParameterType::KIND::REAL:
			fprintf( header, "\t\tm::impl::publishableComposeLeafeReal( %s, t.%s );\n", composer, param.name.c_str() );
			break;
		case MessageParameterType::KIND::CHARACTER_STRING:
			fprintf( header, "\t\tm::impl::publishableComposeLeafeString( %s, t.%s );\n", composer, param.name.c_str() );
			break;
		case MessageParameterType::KIND::VECTOR:
			fprintf( header, "\t\tm::impl::publishableComposeLeafeValueBegin( %s );\n", composer );
			switch ( param.type.vectorElemKind )
			{
				case MessageParameterType::KIND::INTEGER:
					fprintf( header, "\t\tVectorOfSimpleTypeBody::compose<ComposerT, decltype(T::%s), impl::SignedIntegralType>( %s, t.%s );\n", param.name.c_str(), composer, param.name.c_str() );
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf( header, "\t\tVectorOfSimpleTypeBody::compose<ComposerT, decltype(T::%s), impl::UnsignedIntegralType>( %s, t.%s );\n", param.name.c_str(), composer, param.name.c_str() );
					break;
				case MessageParameterType::KIND::REAL:
					fprintf( header, "\t\tVectorOfSimpleTypeBody::compose<ComposerT, decltype(T::%s), impl::RealType>( %s, t.%s );\n", param.name.c_str(), composer, param.name.c_str() );
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf( header, "\t\tVectorOfSimpleTypeBody::compose<ComposerT, decltype(T::%s), impl::StringType>( %s, t.%s );\n", param.name.c_str(), composer, param.name.c_str() );
					break;
				case MessageParameterType::KIND::VECTOR:
					assert( false ); // unexpected
					break;
				case MessageParameterType::KIND::STRUCT:
					assert( param.type.messageIdx < root.structs.size() );
					fprintf( header, "\t\tm::impl::publishableComposeLeafeStructBegin( %s );\n", composer );
		//			fprintf( header, "\t\tm::impl::composePublishableStructBegin( %s, \"%s\" );\n", composer, param.name.c_str() );
					fprintf( header, "\t\t%s::compose( %s, t.%s );\n", impl_generatePublishableStructName( *(root.structs[param.type.messageIdx]) ).c_str(), composer, param.name.c_str() );
		//			fprintf( header, "\t\tm::impl::composePublishableStructEnd( %s, %s );\n", composer, "false" );
					fprintf( header, "\t\tm::impl::publishableComposeLeafeStructEnd( %s );\n", composer );
					break;
				default:
					assert( false ); // not implemented (yet)
			}

//			fprintf( header, "\t\t//assert( false ); // NOT IMPLEMENTED (YET);\n", composer, param.name.c_str() );
			fprintf( header, "\t\tm::impl::composeStateUpdateBlockEnd( %s );\n", composer );
			break;
		case MessageParameterType::KIND::STRUCT:
			fprintf( header, "\t\tm::impl::publishableComposeLeafeStructBegin( %s );\n", composer );
//			fprintf( header, "\t\tm::impl::composePublishableStructBegin( %s, \"%s\" );\n", composer, param.name.c_str() );
			fprintf( header, "\t\t%s::compose( %s, t.%s );\n", impl_generatePublishableStructName( param ).c_str(), composer, param.name.c_str() );
//			fprintf( header, "\t\tm::impl::composePublishableStructEnd( %s, %s );\n", composer, "false" );
			fprintf( header, "\t\tm::impl::publishableComposeLeafeStructEnd( %s );\n", composer );
			break;
		default:
			assert( false ); // not implemented (yet)
	}
	
	fprintf( header, "\t}\n" );
}

void impl_GeneratePublishableStateMemberAccessors( FILE* header, Root& root, CompositeType& s, bool allowSeters )
{
	assert( s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure );
	bool forRoot = s.type == CompositeType::Type::publishable;
	const char* rootName = forRoot ? s.name.c_str() : nullptr;
//	const char* rootName = forRoot ? s.name.c_str() : "RootT";
	for ( size_t i=0; i<s.members.size(); ++i )
	{
		auto& it = s.members[i];
		assert( it != nullptr );
		impl_GeneratePublishableStateMemberGetter( header, root, s, *it );
		if ( allowSeters )
		{
			impl_GeneratePublishableStateMemberSetter( header, root, forRoot, rootName, *it, i );
			impl_GeneratePublishableStateMemberGetter4Set( header, root, rootName, *it, i );
		}
	}
}

void impl_GenerateApplyUpdateMessageMemberFn( FILE* header, Root& root, CompositeType& s )
{
	fprintf( header, 
		"\ttemplate<typename ParserT>\n"
		"\tvoid applyMessageWithUpdates(ParserT& parser) {\n"
		"\t\t//****  ApplyUpdateMessageMemberFn  **************************************************************************************************************************************************************\n" 
		"\t\tm::impl::parseStateUpdateMessageBegin( parser );\n"
		"\t\tGMQ_COLL vector<size_t> addr;\n"
		"\t\twhile( impl::parseAddressInPublishable<ParserT, GMQ_COLL vector<size_t>>( parser, addr ) )\n"
		"\t\t{\n"
		"\t\t\tGMQ_ASSERT( addr.size() );\n"
		"\t\t\tswitch ( addr[0] )\n"
		"\t\t\t{\n"
	);

	for ( size_t i=0; i<s.members.size(); ++i )
	{
		fprintf( header, "\t\t\t\tcase %zd:\n", i );
		assert( s.members[i] != nullptr );
		auto& member = *(s.members[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, 
					"\t\t\t\t\tif ( addr.size() > 1 )\n"
					"\t\t\t\t\t\tthrow std::exception(); // bad format, TODO: ...\n"
				);
				fprintf( header, 
					"\t\t\t\t\tif constexpr( has_any_notifier_for_%s )\n"
					"\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tdecltype(T::%s) oldVal = t.%s;\n"
					"\t\t\t\t\t\tm::impl::publishableParseLeafeInteger<ParserT, decltype(T::%s)>( parser, &(t.%s) );\n",
					member.name.c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tif ( oldVal != t.%s )\n"
					"\t\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr ( has_void_update_notifier_call_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr ( has_update_notifier_call_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s( oldVal );\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t}\n"
					"\t\t\t\t\telse\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\tm::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s) );\n",
					paramTypeToLeafeParser( member.type.kind ), member.name.c_str(), member.name.c_str()
				);
				break;
			case MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "\t\t\t\t\tif ( addr.size() == 1 ) // we have to parse and apply changes of this child\n" );
				fprintf( header, "\t\t\t\t\t{\n" );
				fprintf( header, "\t\t\t\t\t\tm::impl::publishableParseLeafeStructBegin( parser );\n" );
				fprintf( header, "\n" );

				fprintf( header,
					"\t\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tdecltype(T::%s) temp_%s;\n"
					"\t\t\t\t\t\t\t%s::copy<decltype(T::%s), decltype(T::%s)>( t.%s, temp_%s );\n", 
					member.name.c_str(), member.name.c_str(), impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() 
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif ( changedCurrent )\n"
					"\t\t\t\t\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s( temp_%s );\n"
					"\t\t\t\t\t\t\t}\n"
					"\t\t\t\t\t\t}\n",
					member.name.c_str(), member.name.c_str()
				);

				fprintf( header,
					"\t\t\t\t\t\telse if constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() 
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif ( changedCurrent )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n"
					"\t\t\t\t\t\t}\n"
					"\n",
					member.name.c_str()
				);

				fprintf( header, "\t\t\t\t\t\telse\n" );
				fprintf( header, "\t\t\t\t\t\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );

				fprintf( header, "\n" );
				fprintf( header, "\t\t\t\t\t\tm::impl::publishableParseLeafeStructEnd( parser );\n" );
				fprintf( header, "\t\t\t\t\t}\n" );
				fprintf( header, "\t\t\t\t\telse // let child continue parsing\n" );
				fprintf( header, "\t\t\t\t\t{\n" );

				fprintf( header,
					"\t\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tdecltype(T::%s) temp_%s;\n"
					"\t\t\t\t\t\t\t%s::copy<decltype(T::%s), decltype(T::%s)>( t.%s, temp_%s );\n", 
					member.name.c_str(), member.name.c_str(), impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s, addr, 1 );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() 
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif ( changedCurrent )\n"
					"\t\t\t\t\t\t\t{\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s( temp_%s );\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\t}\n"
					"\t\t\t\t\t\t}\n"
				);
				fprintf( header, 
					"\t\t\t\t\t\telse if constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, "\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::Size), bool>( parser, t.%s, addr, 1 );\n", 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif ( changedCurrent )\n"
					"\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t\telse\n"
					"\t\t\t\t\t\t\t%s::parse( parser, t.%s, addr, 1 );\n"
					"\t\t\t\t\t}\n",
//					"\t\t\t\t}\n" 
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str()
				);

				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				assert( member.type.messageIdx < root.structs.size() );
				fprintf( header, 
					"\t\t\t\t\tif ( addr.size() > 1 )\n"
				);
				const char* libType = paramTypeToLibType( member.type.vectorElemKind );
				fprintf( header, 
					"\t\t\t\t\t\tVectorOfSimpleTypeBody::parse<ParserT, decltype(T::%s), %s, %s>( parser, t.%s, addr, 1 );\n", 
					member.name.c_str(),
					libType, 
					member.type.vectorElemKind == MessageParameterType::KIND::STRUCT ? impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() : libType,
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\telse\n"
					"\t\t\t\t\t{\n"
				);
fprintf( header, "//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" );
				fprintf( header, 
					"\t\t\t\t\t\tm::impl::publishableParseLeafeVectorBegin( parser );\n"
					"\n"
				);

				/*fprintf( header, 
					"\t\t\t\t\t\tVectorOfSimpleTypeBody::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n", 
					member.name.c_str(),
					member.type.vectorElemKind == MessageParameterType::KIND::STRUCT ? impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() : libType,
					member.name.c_str()
				);*/

				fprintf( header,
					"\t\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tdecltype(T::%s) temp_%s;\n"
					"\t\t\t\t\t\t\timpl::copyVector<decltype(T::%s), impl::%s>( t.%s, temp_%s );\n", 
					member.name.c_str(), member.name.c_str(), 
					member.name.c_str(), 
					member.type.vectorElemKind == MessageParameterType::KIND::STRUCT ? impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() : libType,
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tVectorOfSimpleTypeBody::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n", 
					member.name.c_str(),
					member.type.vectorElemKind == MessageParameterType::KIND::STRUCT ? impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() : libType,
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n"
					"\t\t\t\t\t\t\t\t\tt.notifyUpdated_%s( temp_%s );\n",
					member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t}\n"
				);

				fprintf( header,
					"\t\t\t\t\t\telse if constexpr( has_void update_notifier_for_%s )\n"
					"\t\t\t\t\t\t{\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tVectorOfSimpleTypeBody::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n", 
					member.name.c_str(),
					member.type.vectorElemKind == MessageParameterType::KIND::STRUCT ? impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() : libType,
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tt.notifyUpdated_%s();\n"
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t\telse\n",
					member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tVectorOfSimpleTypeBody::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n", 
					member.name.c_str(),
					member.type.vectorElemKind == MessageParameterType::KIND::STRUCT ? impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() : libType,
					member.name.c_str()
				);

				fprintf( header, 
					"\n"
					"\t\t\t\t\t\tm::impl::publishableParseLeafeVectorEnd( parser );\n"
				);
				fprintf( header, 
					"\t\t\t\t\t}\n"
				);
				break;
			}
			default:
				assert( false );
		}
		fprintf( header, "\t\t\t\t\tbreak;\n" );
	}
	fprintf( header, 
		"\t\t\t\tdefault:\n"
		"\t\t\t\t\tthrow std::exception(); // bad format, TODO: ...\n"
		"\t\t\t}\n"
		"\t\t\taddr.clear();\n"
		"\t\t}\n"
		"\t}\n"
	);
}

void impl_GeneratePublishableStateWrapper( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, 
		"template<class T, class ComposerT>\n"
		"class %s_Wrapper\n"
		"{\n"
		"\tT t;\n"
		"\tComposerT* composer;\n",
		s.name.c_str()
	);

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );
	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, s, "\t" );

	fprintf( header, 
		"\npublic:\n" 
		"\ttemplate<class ... ArgsT>\n"
		"\tpublishable_sample_Wrapper( ArgsT ... args ) : t( std::forward<ArgsT>( args )... ) {}\n"
		"\tconst T& getState() { return t; }\n"
	);
	fprintf( header, 
		"\tComposerT& getComposer() { return *composer; }\n"
		"\tvoid resetComposer( ComposerT* composer_ ) {\n"
		"\t\tcomposer = composer_; \n"
		"\t\tm::impl::composeStateUpdateMessageBegin<ComposerT>( *composer );\n"
		"\t}\n"
		"\tvoid finalizeComposing() {\n"
		"\t\tm::impl::composeStateUpdateMessageEnd( *composer );\n"
		"\t}\n"
	);

	impl_GenerateApplyUpdateMessageMemberFn( header, root, s );

	impl_GeneratePublishableStateMemberAccessors( header, root, s, true );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStructWrapperForwardDeclaration( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure );

	fprintf( header, 
		"template<class T> class %s_RefWrapper;\n",
		s.name.c_str()
	);
}

void impl_GeneratePublishableStructWrapper( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure );

	fprintf( header, 
		"template<class T>\n"
		"class %s_RefWrapper\n"
		"{\n"
		"\tT& t;\n",
		s.name.c_str()
	);

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );
//	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, s, "\t" );

	fprintf( header, 
		"\npublic:\n" 
		"\t%s_RefWrapper( T& actual ) : t( actual ) {}\n" ,
		s.name.c_str()
	);

	impl_GeneratePublishableStateMemberAccessors( header, root, s, false );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStructWrapper4SetForwardDeclaration( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure );

	fprintf( header, 
		"template<class T, class RootT> class %s_RefWrapper4Set;\n",
		s.name.c_str()
	);
}

void impl_GeneratePublishableStructWrapper4Set( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure );

	fprintf( header, 
		"template<class T, class RootT>\n"
		"class %s_RefWrapper4Set\n"
		"{\n"
		"\tT& t;\n"
		"\tRootT& root;\n"
		"\tGMQ_COLL vector<size_t> address;\n",
		s.name.c_str()
	);

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );

	fprintf( header, 
		"\npublic:\n" 
		"\t%s_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {\n"
		"\t\taddress = address_;\n"
		"\t\taddress.push_back (idx );\n"
		"\t}\n",
		s.name.c_str()
	);

	impl_GeneratePublishableStateMemberAccessors( header, root, s, true );

	fprintf( header, "};\n\n" );
}

void impl_generatePublishableCommentBlock( FILE* header, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable );
	fprintf( header, "//**********************************************************************\n" );
	fprintf( header, "// %s %s (%zd parameters)\n", s.type2string(), s.name.c_str(), s.members.size() );

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

		assert( !param.type.hasDefault );
		fprintf( header, "\n" );
	}

	fprintf( header, "//**********************************************************************\n\n" );
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
			"template<typename StateT, typename MemberT> concept has_update_notifier_call_for_%s = requires { { std::declval<StateT>().notifyUpdated_%s(std::declval<MemberT>()) }; };\n",
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
				"template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_%s = requires { { std::declval<StateT>().notifyElementUpdated_%s(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };\n",
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
				"template<typename StateT> concept has_erased_notifier_call2_for_%s = requires { { std::declval<StateT>().notifyErased_%s(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_%s = requires { { std::declval<StateT>().notifyErased_%s(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };\n",
				name.c_str(), name.c_str()
			);
		}
	}
	fprintf( header, "\n" );
}

void generatePublishable( FILE* header, Root& root, CompositeType& s )
{
	bool checked = impl_checkParamNameUniqueness(s);
	checked = impl_checkFollowingExtensionRules(s) && checked;
	if ( !checked )
		throw std::exception();

	impl_generatePublishableCommentBlock( header, s );
	impl_GeneratePublishableStateWrapper( header, root, s );
}


