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
		case MessageParameterType::KIND::INTEGER: return "::globalmq::marshalling::impl::SignedIntegralType";
		case MessageParameterType::KIND::UINTEGER: return "::globalmq::marshalling::impl::UnsignedIntegralType";
		case MessageParameterType::KIND::REAL: return "::globalmq::marshalling::impl::RealType";
		case MessageParameterType::KIND::CHARACTER_STRING: return "::globalmq::marshalling::impl::StringType";
		case MessageParameterType::KIND::STRUCT: return "::globalmq::marshalling::impl::StructType";
		default: return nullptr;
	}
}

string vectorElementTypeToLibTypeOrTypeProcessor( const MessageParameterType& type, Root& root )
{
	switch( type.vectorElemKind )
	{
		case MessageParameterType::KIND::INTEGER: return "::globalmq::marshalling::impl::SignedIntegralType";
		case MessageParameterType::KIND::UINTEGER: return "::globalmq::marshalling::impl::UnsignedIntegralType";
		case MessageParameterType::KIND::REAL: return "::globalmq::marshalling::impl::RealType";
		case MessageParameterType::KIND::CHARACTER_STRING: return "::globalmq::marshalling::impl::StringType";
		case MessageParameterType::KIND::STRUCT: 
			assert( type.messageIdx < root.structs.size() );
			return fmt::format( "publishable_STRUCT_{}", root.structs[type.messageIdx]->name );
		default: 
			assert( false );
			return "";
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
	*	has_void_insert_notifier_call_for_%s            notifyInserted_%s<T>()
	*	has_insert_notifier_call_for_%s                 notifyInserted_%s<T>(index_type_for_array_notifiers, index_type_for_array_notifiers)
	*	has_void_erased_notifier_call_for_%s            notifyErased_%s<T>()
	*	has_erased_notifier_call2_for_%s                notifyErased_%s<T>(index_type_for_array_notifiers, index_type_for_array_notifiers)
	*	has_erased_notifier_call3_for_%s                notifyErased_%s<T, MemberT>(index_type_for_array_notifiers, index_type_for_array_notifiers, MemberT>()	
	*	has_element_updated_void_notifier_call_for_%s   notifyElementUpdated_%s<T>()
	*	has_element_updated_notifier_call_for_%s        notifyElementUpdated_%s<T>(index_type_for_array_notifiers)
	*	has_full_element_updated_notifier_call_for_%s   notifyElementUpdated_%s<T, MemberT>(index_type_for_array_notifiers, MemberT)
	*/
	assert( s.type == CompositeType::Type::publishable || ( s.type == CompositeType::Type::structure && s.isStruct4Publishing ) );
	for ( auto& member : s.members )
	{
		assert( member != nullptr );
		fprintf( header, "%sstatic constexpr bool has_void_update_notifier_for_%s = has_void_update_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
		fprintf( header, "%sstatic constexpr bool has_update_notifier_for_%s = has_update_notifier_call_for_%s<T, decltype(T::%s)>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
		fprintf( header, "%sstatic constexpr bool has_any_notifier_for_%s = has_void_update_notifier_for_%s || has_update_notifier_for_%s;\n", 
			offset, member->name.c_str(), member->name.c_str(), member->name.c_str()
		);
		if ( member->type.kind == MessageParameterType::KIND::VECTOR )
		{
			fprintf( header, "%susing %sT = decltype(T::%s);\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_void_insert_notifier_for_%s = has_void_insert_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_insert_notifier2_for_%s = has_insert_notifier_call2_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_insert_notifier3_for_%s = has_insert_notifier_call3_for_%s<T, GMQ_COLL vector<%sT>&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_void_erased_notifier_for_%s = has_void_erased_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_erased_notifier2_for_%s = has_erased_notifier_call2_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_erased_notifier3_for_%s = has_erased_notifier_call3_for_%s<T, GMQ_COLL vector<%sT>&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_void_element_updated_notifier_for_%s = has_element_updated_void_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_element_updated_notifier_for_%s = has_element_updated_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_full_element_updated_notifier_for_%s = has_full_element_updated_notifier_call_for_%s<T, %sT&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
		}
	}
	fprintf( header, "%sstatic constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;", offset );
	fprintf( header, "\n" );
}

void impl_generateApplyUpdateForSimpleType( FILE* header, MessageParameter& member, bool addReportChanges, bool isLeafe )
{
	string parseProcessorType = isLeafe ? paramTypeToLeafeParser( member.type.kind ) : paramTypeToParser( member.type.kind );
	if ( addReportChanges )
		fprintf( header, "\t\t\t\t\tif constexpr( has_any_notifier_for_%s || reportChanges )\n", member.name.c_str() );
	else
		fprintf( header, "\t\t\t\t\tif constexpr( has_any_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\tdecltype(T::%s) oldVal = t.%s;\n", member.name.c_str(), member.name.c_str() );
	if ( isLeafe )
		fprintf( header, "\t\t\t\t\t\tglobalmq::marshalling::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s) );\n", paramTypeToLeafeParser( member.type.kind ), member.name.c_str(), member.name.c_str() );
	else
		fprintf( header, "\t\t\t\t\t\tglobalmq::marshalling::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s), \"%s\" );\n", paramTypeToParser( member.type.kind ), member.name.c_str(), member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\tbool currentChanged = oldVal != t.%s;\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\tif ( currentChanged )\n" );
	fprintf( header, "\t\t\t\t\t\t{\n" );
	if ( addReportChanges )
	{
		fprintf( header, "\t\t\t\t\t\t\tif constexpr ( reportChanges )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\tchanged = true;\n" );
	}
	fprintf( header, "\t\t\t\t\t\t\tif constexpr ( has_void_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tif constexpr ( has_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\tt.notifyUpdated_%s( oldVal );\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\telse\n" );
	if ( isLeafe )
		fprintf( header, "\t\t\t\t\t\tglobalmq::marshalling::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s) );\n", paramTypeToLeafeParser( member.type.kind ), member.name.c_str(), member.name.c_str() );
	else
		fprintf( header, "\t\t\t\t\t\tglobalmq::marshalling::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s), \"%s\" );\n", paramTypeToParser( member.type.kind ), member.name.c_str(), member.name.c_str(), member.name.c_str() );
}

void impl_generateApplyUpdateForStructItself( FILE* header, MessageParameter& member, bool addReportChanges )
{
	fprintf( header, "\t\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\tdecltype(T::%s) temp_%s;\n", member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t%s::copy<decltype(T::%s), decltype(T::%s)>( t.%s, temp_%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tif ( changedCurrent )\n" );
	fprintf( header, "\t\t\t\t\t\t\t{\n" );
	if ( addReportChanges )
	{
		fprintf( header, "\t\t\t\t\t\t\t\tif constexpr ( reportChanges )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\tchanged = true;\n" );
	}
	fprintf( header, "\t\t\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\tt.notifyUpdated_%s( temp_%s );\n", member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\telse if constexpr( has_void_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tif ( changedCurrent )\n" );
	fprintf( header, "\t\t\t\t\t\t\t{\n" );
	if ( addReportChanges )
	{
		fprintf( header, "\t\t\t\t\t\t\t\tif constexpr ( reportChanges )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\tchanged = true;\n" );
	}
	fprintf( header, "\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\t\t\t\t\t\telse\n" );
	fprintf( header, "\t\t\t\t\t\t{\n" );
	if ( addReportChanges )
	{
		fprintf( header, "\t\t\t\t\t\t\t\tif constexpr ( reportChanges )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\tchanged = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t\telse\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
	}
	else
		fprintf( header, "\t\t\t\t\t\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t}\n" );
}

void impl_generateApplyUpdateForFurtherProcessingInStruct( FILE* header, MessageParameter& member, bool addOffsetInAddr, bool addReportChanges, bool forwardAddress )
{
	assert( (!addOffsetInAddr) || (addOffsetInAddr && forwardAddress) );
	const char* offsetPlusStr = addOffsetInAddr ? "offset + " : "";
	fprintf( header, "\t\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\tdecltype(T::%s) temp_%s;\n", member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t%s::copy<decltype(T::%s), decltype(T::%s)>( t.%s, temp_%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str(), member.name.c_str() );
	if ( forwardAddress )
		fprintf( header, "\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s, addr, %s1 );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str(), offsetPlusStr );
	else
		fprintf( header, "\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tif ( changedCurrent )\n" );
	fprintf( header, "\t\t\t\t\t\t\t{\n" );
	if ( addReportChanges )
	{
		fprintf( header, "\t\t\t\t\t\t\t\tif constexpr ( reportChanges )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\tchanged = true;\n" );
	}
	fprintf( header, "\t\t\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\tt.notifyUpdated_%s( temp_%s );\n", member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\telse if constexpr( has_void_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t{\n" );
	if ( forwardAddress )
		fprintf( header, "\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::Size), bool>( parser, t.%s, addr, %s1 );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), offsetPlusStr );
	else
		fprintf( header, "\t\t\t\t\t\t\tbool changedCurrent = %s::parse<ParserT, decltype(T::Size), bool>( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tif ( changedCurrent )\n" );
	fprintf( header, "\t\t\t\t\t\t\t{\n" );
	if ( addReportChanges )
	{
		fprintf( header, "\t\t\t\t\t\t\t\tif constexpr ( reportChanges )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\tchanged = true;\n" );
	}
	fprintf( header, "\t\t\t\t\t\t\t\tt.notifyUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t}\n" );
	if ( addReportChanges )
	{
		fprintf( header, "\t\t\t\t\t\telse if constexpr ( reportChanges )\n" );
		if ( forwardAddress )
			fprintf( header, "\t\t\t\t\t\t\tchanged = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s, addr, %s1 );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str(), offsetPlusStr );
		else
			fprintf( header, "\t\t\t\t\t\t\tchanged = %s::parse<ParserT, decltype(T::%s), bool>( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\telse\n" );
		if ( forwardAddress )
			fprintf( header, "\t\t\t\t\t\t\t%s::parse( parser, t.%s, addr, %s1 );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), offsetPlusStr );
		else
			fprintf( header, "\t\t\t\t\t\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
	}
	else
	{
		fprintf( header, "\t\t\t\t\t\telse\n" );
		if ( forwardAddress )
			fprintf( header, "\t\t\t\t\t\t\t%s::parse( parser, t.%s, addr, %s1 );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), offsetPlusStr );
		else
			fprintf( header, "\t\t\t\t\t\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
	}
}

void impl_generateApplyUpdateForFurtherProcessingInVector( FILE* header, Root& root, MessageParameter& member, bool addOffsetInAddr, bool addReportChanges/*, bool forwardAddress*/ )
{
//	assert( (!addOffsetInAddr) || (addOffsetInAddr && forwardAddress) );
	const char* offsetPlusStr = addOffsetInAddr ? "offset + " : "";

	fprintf( header, "\t\t\t\t\tdecltype(T::%s) oldVectorVal;\n",  member.name.c_str() );
	fprintf( header, "\t\t\t\t\tbool currentChanged = false;\n" );
	fprintf( header, "\t\t\t\t\tconstexpr bool alwaysCollectChanges = has_any_notifier_for_%s;\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\tif constexpr( alwaysCollectChanges )\n" );
	fprintf( header, "\t\t\t\t\t\timpl::copyVector<decltype(T::%s), %s>( t.%s, oldVectorVal );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );

//fprintf( header, "//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" );

	const char* libType = paramTypeToLibType( member.type.vectorElemKind );
	assert( member.type.messageIdx < root.structs.size() );
	fprintf( header, "\t\t\t\t\tif ( addr.size() > %s1 ) // one of actions over elements of the vector\n", offsetPlusStr );
	fprintf( header, "\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\tsize_t pos = addr[%s1];\n", offsetPlusStr );
	fprintf( header, "\t\t\t\t\t\tif ( pos >= t.%s.size() )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tthrow std::exception();\n" );

	fprintf( header, "\t\t\t\t\t\tif ( addr.size() > %s2 ) // update for a member of a particular vector element\n", offsetPlusStr );
	fprintf( header, "\t\t\t\t\t\t{\n" );

	if ( member.type.vectorElemKind == MessageParameterType::KIND::STRUCT )
	{
		fprintf( header, "\t\t\t\t\t\t\ttypename decltype(T::%s)::value_type& value = t.%s[pos];\n", member.name.c_str(), member.name.c_str() );

		fprintf( header, "\t\t\t\t\t\t\tif constexpr ( has_full_element_updated_notifier_for_%s )\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t{\n" );
		fprintf( header, "\t\t\t\t\t\t\t\ttypename decltype(T::%s)::value_type oldValue;\n", member.name.c_str() );
		switch ( member.type.vectorElemKind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "\t\t\t\t\t\t\t\toldValue = value;\n" );
				break;
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "\t\t\t\t\t\t\t\t%s::copy( value, oldValue );\n", impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() );
				break;
		}
		fprintf( header, "\t\t\t\t\t\t\t\tcurrentChanged = %s::parse<ParserT, typename decltype(T::%s)::value_type, bool>( parser, value, addr, %s2 );\n", impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str(), member.name.c_str(), offsetPlusStr );
		fprintf( header, "\t\t\t\t\t\t\t\tif ( currentChanged )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t{\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s( pos, oldValue );\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_element_updated_notifier_for_%s )\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_void_element_updated_notifier_for_%s )\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t\t}\n" );
		fprintf( header, "\t\t\t\t\t\t\t}\n" );

		fprintf( header, "\t\t\t\t\t\t\telse if constexpr ( has_element_updated_notifier_for_%s )\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t{\n" );
		fprintf( header, "\t\t\t\t\t\t\t\tcurrentChanged = %s::parse<ParserT, typename decltype(T::%s)::value_type, bool>( parser, value, addr, %s2 );\n", impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str(), member.name.c_str(), offsetPlusStr );
		fprintf( header, "\t\t\t\t\t\t\t\tif ( currentChanged )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t{\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s( pos );\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_void_element_updated_notifier_for_%s )\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t\t}\n" );
		fprintf( header, "\t\t\t\t\t\t\t}\n" );

		fprintf( header, "\t\t\t\t\t\t\telse if constexpr ( has_void_element_updated_notifier_for_%s )\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t{\n" );
		fprintf( header, "\t\t\t\t\t\t\t\tcurrentChanged = %s::parse<ParserT, typename decltype(T::%s)::value_type, bool>( parser, value, addr, %s2 );\n", impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str(), member.name.c_str(), offsetPlusStr );
		fprintf( header, "\t\t\t\t\t\t\t\tif ( currentChanged )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", member.name.c_str() );
		fprintf( header, "\t\t\t\t\t\t\t}\n" );

		fprintf( header, "\t\t\t\t\t\t\telse\n" );
		fprintf( header, "\t\t\t\t\t\t\t{\n" );
		fprintf( header, "\t\t\t\t\t\t\t\tif constexpr ( alwaysCollectChanges )\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\tcurrentChanged = %s::parse<ParserT, typename decltype(T::%s)::value_type, bool>( parser, value, addr, %s2 );\n", impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str(), member.name.c_str(), offsetPlusStr );
		fprintf( header, "\t\t\t\t\t\t\t\telse\n" );
		fprintf( header, "\t\t\t\t\t\t\t\t\t%s::parse<ParserT, typename decltype(T::%s)::value_type>( parser, value, addr, %s2 );\n", impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str(), member.name.c_str(), offsetPlusStr );
		fprintf( header, "\t\t\t\t\t\t\t}\n" );
	}
	else
		fprintf( header, "\t\t\t\t\t\t\tthrow std::exception(); // deeper address is unrelated to simple type of vector elements (IDL type of t.%s elements is %s)\n", member.name.c_str(), impl_kindToString( member.type.vectorElemKind ) );
				
	fprintf( header, "\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\telse // update of one or more elelments as a whole\n" );
	fprintf( header, "\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\tsize_t action;\n" );
	fprintf( header, "\t\t\t\t\t\t\timpl::parseActionInPublishable( parser, action );\n" );
	fprintf( header, "\t\t\t\t\t\t\tswitch ( action )\n" );
	fprintf( header, "\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\tcase ActionOnVector::remove_at:\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_erased_notifier3_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tdecltype(T::%s) oldVal;\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\timpl::copyVector<decltype(T::%s), %s>( t.%s, oldVal );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.%s.erase( t.%s.begin() + pos );\n", member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.notifyErased_%s( pos, oldVal );\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_erased_notifier2_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.%s.erase( t.%s.begin() + pos );\n", member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.notifyErased_%s( pos );\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_void_erased_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.%s.erase( t.%s.begin() + pos );\n", member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.notifyErased_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( alwaysCollectChanges )\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tcurrentChanged = true;\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\tbreak;\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t\t\tcase ActionOnVector::update_at:\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\timpl::publishableParseLeafeValueBegin( parser );\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\ttypename decltype(T::%s)::value_type& value = t.%s[pos];\n", member.name.c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_full_element_updated_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\ttypename decltype(T::%s)::value_type oldValue;\n", member.name.c_str() );

	switch ( member.type.vectorElemKind )
	{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			fprintf( header, "\t\t\t\t\t\t\t\t\t\toldValue = value;\n" );
			break;
		case MessageParameterType::KIND::STRUCT:
			fprintf( header, "\t\t\t\t\t\t\t\t\t\t%s::copy( value, oldValue );\n", impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str() );
			break;
	}
				
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tcurrentChanged = PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::%s), %s, bool>( parser, value );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tif ( currentChanged )\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s( pos, oldValue );\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\tif constexpr ( has_element_updated_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\tif constexpr ( has_void_element_updated_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t}\n"  );
				
	fprintf( header, "\t\t\t\t\t\t\t\t\telse if constexpr ( has_element_updated_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tcurrentChanged = PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::%s), %s, bool>( parser, value );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tif ( currentChanged )\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s( pos );\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\tif constexpr ( has_void_element_updated_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t}\n" );

	fprintf( header, "\t\t\t\t\t\t\t\t\telse if constexpr ( has_void_element_updated_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tcurrentChanged = PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::%s), %s, bool>( parser, value );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tif ( currentChanged )\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t}\n" );

	fprintf( header, "\t\t\t\t\t\t\t\t\telse\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tif constexpr ( alwaysCollectChanges )\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\tcurrentChanged = PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::%s), %s, bool>( parser, value );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\telse\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\t\tPublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::%s), %s>( parser, value );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t}\n" );

	fprintf( header, "\t\t\t\t\t\t\t\t\tbreak;\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t\t\tcase ActionOnVector::insert_single_before:\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\timpl::publishableParseLeafeValueBegin( parser );\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\ttypename decltype(T::%s)::value_type value;\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\tPublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::%s), %s>( parser, value );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );

	fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_insert_notifier3_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tdecltype(T::%s) oldVal;\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\timpl::copyVector<decltype(T::%s), %s>( t.%s, oldVal );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
				
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.notifyInserted_%s( pos, oldVal );\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t}\n" );

	fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_insert_notifier2_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.notifyInserted_%s( pos );\n", member.name.c_str() );
				
	fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( has_void_insert_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tt.notifyInserted_%s();\n", member.name.c_str() );
				
	fprintf( header, "\t\t\t\t\t\t\t\t\tt.%s.insert( t.%s.begin() + pos, value );\n", member.name.c_str(), member.name.c_str() );
				
	fprintf( header, "\t\t\t\t\t\t\t\t\tif constexpr ( alwaysCollectChanges )\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\t\tcurrentChanged = true;\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\tbreak;\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t\t\tdefault:\n" );
	fprintf( header, "\t\t\t\t\t\t\t\t\tthrow std::exception();\n" );
	fprintf( header, "\t\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\t\timpl::parseStateUpdateBlockEnd( parser );\n" );
	fprintf( header, "\t\t\t\t\t\t}\n" );
			
//fprintf( header, "//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" );
	fprintf( header, "\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\telse // replacement of the whole vector\n" );
	fprintf( header, "\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\tglobalmq::marshalling::impl::publishableParseLeafeVectorBegin( parser );\n" );
	fprintf( header, "\n" );
	fprintf( header, "\t\t\t\t\t\tif constexpr( alwaysCollectChanges )\n" );
	fprintf( header, "\t\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\t\tPublishableVectorProcessor::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tcurrentChanged = !impl::isSameVector<decltype(T::%s), %s>( oldVectorVal, t.%s );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t}\n" );
	fprintf( header, "\t\t\t\t\t\telse\n" );
	fprintf( header, "\t\t\t\t\t\t\tPublishableVectorProcessor::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
	fprintf( header, "\n" );
	fprintf( header, "\t\t\t\t\t\tglobalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );\n" );
	fprintf( header, "\t\t\t\t\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\t\t\t\t\tif ( currentChanged )\n" );
	fprintf( header, "\t\t\t\t\t{\n" );
	fprintf( header, "\t\t\t\t\t\tif constexpr( has_void_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tt.notifyUpdated_%s();\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\tif constexpr( has_update_notifier_for_%s )\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t\t\tt.notifyUpdated_%s( oldVectorVal );\n", member.name.c_str() );
	fprintf( header, "\t\t\t\t\t}\n" );

	if ( addReportChanges )
	{
		fprintf( header, "\n" );
		fprintf( header, "\t\t\t\t\tif constexpr( alwaysCollectChanges )\n" );
		fprintf( header, "\t\t\t\t\t\treturn currentChanged;\n" );
	}
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
			fprintf( header, "\tauto get_%s() { return globalmq::marshalling::VectorOfStructRefWrapper<%s_RefWrapper<typename decltype(T::%s)::value_type>, decltype(T::%s)>(t.%s); }\n", 
				param.name.c_str(), root.structs[param.type.messageIdx]->name.c_str(), param.name.c_str(), param.name.c_str(), param.name.c_str() );
		}
		else
			fprintf( header, "\tauto get_%s() { return globalmq::marshalling::VectorOfSimpleTypeRefWrapper(t.%s); }\n", param.name.c_str(), param.name.c_str() );
	}
	else
		fprintf( header, "\tconst auto& get_%s() { return t.%s; }\n", param.name.c_str(), param.name.c_str() );
}

void impl_GeneratePublishableStateMemberGetter4Set( FILE* header, Root& root, const char* rootName, MessageParameter& param, size_t idx )
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
		rootType = fmt::format( "{}_WrapperForPublisher", rootName );
		addr = "GMQ_COLL vector<size_t>()";
	}
	if ( param.type.kind == MessageParameterType::KIND::STRUCT )
	{
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
				fprintf( header, "\tauto get4set_%s() { return globalmq::marshalling::VectorRefWrapper4Set<decltype(T::%s), %s, %s>(t.%s, *this, GMQ_COLL vector<size_t>(), %zd); }\n", 
					param.name.c_str(), param.name.c_str(), libType, rootType.c_str(), param.name.c_str(), idx );
				break;
			case MessageParameterType::KIND::STRUCT:
				assert( param.type.messageIdx < root.structs.size() );
				fprintf( header, 
					"\tauto get4set_%s() { return globalmq::marshalling::VectorOfStructRefWrapper4Set<decltype(T::%s), %s, %s, %s_RefWrapper4Set<typename decltype(T::%s)::value_type, %s>>"
					"(t.%s, *this, %s, %zd); }\n", 
					param.name.c_str(), param.name.c_str(),
					impl_generatePublishableStructName( *(root.structs[param.type.messageIdx]) ).c_str(), 
					rootType.c_str(), 
					root.structs[param.type.messageIdx]->name.c_str(), 
					param.name.c_str(),
					rootType.c_str(), 
					param.name.c_str(), addr.c_str(), idx );
				break;
			default:
				assert( false ); // not (yet) implemented
		}
	}
}


void impl_generateComposeFunctionForPublishableStruct( FILE* header, Root& root, CompositeType& obj )
{
	if ( obj.type == CompositeType::Type::structure )
		fprintf( header, 
			"\ttemplate<class ComposerT, class T>\n"
			"\tstatic\n"
			"\tvoid compose( ComposerT& composer, const T& t )\n"
			"\t{\n"
		);
	else if ( obj.type == CompositeType::Type::publishable )
		fprintf( header, 
			"\ttemplate<class ComposerT>\n"
			"\tvoid compose( ComposerT& composer )\n"
			"\t{\n"
			"\t\tglobalmq::marshalling::impl::composeStructBegin( composer );\n"
			"\n"
		);
	else
		assert( false );

	const char* composer = "composer";

	for ( size_t i=0; i<obj.members.size(); ++i )
	{
		auto& it = obj.members[i];
		assert( it != nullptr );
		auto& member = *it;
		const char* addSepar = i + 1 < obj.members.size() ? "true": "false";
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "\t\tglobalmq::marshalling::impl::publishableStructComposeInteger( %s, t.%s, \"%s\", %s );\n", composer, member.name.c_str(), member.name.c_str(), addSepar );
				fprintf( header, "\n" );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "\t\tglobalmq::marshalling::impl::publishableStructComposeUnsignedInteger( %s, t.%s, \"%s\", %s );\n", composer, member.name.c_str(), member.name.c_str(), addSepar );
				fprintf( header, "\n" );
				break;
			case MessageParameterType::KIND::REAL:
				fprintf( header, "\t\tglobalmq::marshalling::impl::publishableStructComposeReal( %s, t.%s, \"%s\", %s );\n", composer, member.name.c_str(), member.name.c_str(), addSepar );
				fprintf( header, "\n" );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "\t\tglobalmq::marshalling::impl::publishableStructComposeString( %s, t.%s, \"%s\", %s );\n", composer, member.name.c_str(), member.name.c_str(), addSepar );
				fprintf( header, "\n" );
				break;
			case MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "\t\tglobalmq::marshalling::impl::composePublishableStructBegin( %s, \"%s\" );\n", composer, member.name.c_str() );
				fprintf( header, "\t\t%s::compose( %s, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), composer, member.name.c_str() );
				fprintf( header, "\t\tglobalmq::marshalling::impl::composePublishableStructEnd( %s, %s );\n", composer, addSepar );
				fprintf( header, "\n" );
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				switch ( member.type.vectorElemKind )
				{
					case MessageParameterType::KIND::INTEGER:
						fprintf( header, "\t\tPublishableVectorProcessor::compose<ComposerT, decltype(T::%s), impl::SignedIntegralType>( %s, t.%s, \"%s\", %s );\n", member.name.c_str(), composer, member.name.c_str(), member.name.c_str(), addSepar );
						break;
					case MessageParameterType::KIND::UINTEGER:
						fprintf( header, "\t\tPublishableVectorProcessor::compose<ComposerT, decltype(T::%s), impl::UnsignedIntegralType>( %s, t.%s, \"%s\", %s );\n", member.name.c_str(), composer, member.name.c_str(), member.name.c_str(), addSepar );
						break;
					case MessageParameterType::KIND::REAL:
						fprintf( header, "\t\tPublishableVectorProcessor::compose<ComposerT, decltype(T::%s), impl::RealType>( %s, t.%s, \"%s\", %s );\n", member.name.c_str(), composer, member.name.c_str(), member.name.c_str(), addSepar );
						break;
					case MessageParameterType::KIND::CHARACTER_STRING:
						fprintf( header, "\t\tPublishableVectorProcessor::compose<ComposerT, decltype(T::%s), impl::StringType>( %s, t.%s, \"%s\", %s );\n", member.name.c_str(), composer, member.name.c_str(), member.name.c_str(), addSepar );
						break;
					case MessageParameterType::KIND::VECTOR:
						assert( false ); // unexpected
						break;
					case MessageParameterType::KIND::STRUCT:
						assert( member.type.messageIdx < root.structs.size() );
						fprintf( header, "\t\tPublishableVectorProcessor::compose<ComposerT, decltype(T::%s), %s>( %s, t.%s, \"%s\", %s );\n", member.name.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.messageIdx]) ).c_str(), composer, member.name.c_str(), member.name.c_str(), addSepar );
						break;
					default:
						assert( false ); // not implemented (yet)
				}
				fprintf( header, "\n" );
				break;
			}
			default:
				assert( false ); // not implemented (yet)
		}
	}

	if ( obj.type == CompositeType::Type::publishable )
		fprintf( header, 
			"\n"
			"\t\tglobalmq::marshalling::impl::composeStructEnd( composer );\n" );

	fprintf( header, "\t}\n" );
}

void impl_generateContinueParsingFunctionForPublishableStruct( FILE* header, Root& root, CompositeType& obj )
{
	fprintf( header, 
		"\ttemplate<class ParserT, class T, class RetT = void>\n"
		"\tstatic\n"
		"\tRetT parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset )\n"
		"\t{\n"
//		"\t\t//****  ContinueParsing  **************************************************************************************************************************************************************\n" 
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
				fprintf( header, "\t\t\t\t\tif ( addr.size() > offset + 1 )\n" );
				fprintf( header, "\t\t\t\t\t\tthrow std::exception(); // bad format, TODO: ...\n" );
				impl_generateApplyUpdateForSimpleType( header, member, true, true );
				break;
			}
			case  MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "\t\t\t\tif ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child\n" );
				fprintf( header, "\t\t\t\t{\n" );
				fprintf( header, "\t\t\t\t\tglobalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );\n" );

				impl_generateApplyUpdateForStructItself( header, member, true );

				fprintf( header, "\t\t\t\t\tglobalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );\n" );
				fprintf( header, "\t\t\t\t}\n" );
				fprintf( header, "\t\t\t\telse // let child continue parsing\n" );
				fprintf( header, "\t\t\t\t{\n" );

				impl_generateApplyUpdateForFurtherProcessingInStruct( header, member, true, true, true );

				fprintf( header, 
					"\t\t\t\t}\n" 
				);
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				fprintf( header, "\t\t\t\t{\n" );
				impl_generateApplyUpdateForFurtherProcessingInVector( header, root, member, false, false );
				fprintf( header, "\t\t\t\t}\n" );
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
	assert( obj.type == CompositeType::Type::structure );
	fprintf( header, 
		"\ttemplate<class ParserT, class T, class RetT = void>\n"
		"\tstatic\n"
		"\tRetT parse( ParserT& parser, T& t )\n"
		"\t{\n"
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
				impl_generateApplyUpdateForSimpleType( header, member, true, false );
				fprintf( header, "\n" );
				break;
			}
			case  MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "\t\tglobalmq::marshalling::impl::parsePublishableStructBegin( parser, \"%s\" );\n", member.name.c_str() );

				impl_generateApplyUpdateForFurtherProcessingInStruct( header, member, false, true, false );

				fprintf( header, "\t\tglobalmq::marshalling::impl::parsePublishableStructEnd( parser );\n" );
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				assert( member.type.messageIdx < root.structs.size() );
				
				fprintf( header, "\t\tif constexpr( reportChanges )\n" );
				fprintf( header, "\t\t{\n" );
				fprintf( header, "\t\t\tdecltype(T::%s) oldVectorVal;\n", member.name.c_str() );
				fprintf( header, "\t\t\timpl::copyVector<decltype(T::%s), %s>( t.%s, oldVectorVal );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
				fprintf( header, "\t\t\tPublishableVectorProcessor::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
				fprintf( header, "\t\t\tbool currentChanged = !impl::isSameVector<decltype(T::%s), %s>( oldVectorVal, t.%s );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
				fprintf( header, "\t\t\tchanged = changed || currentChanged;\n" );
				fprintf( header, "\t\t}\n" );
				fprintf( header, "\t\telse\n" );
				fprintf( header, "\t\t{\n" );
				fprintf( header, "\t\t\tglobalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", member.name.c_str() );
				fprintf( header, "\t\t\tPublishableVectorProcessor::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
				fprintf( header, "\t\t}\n" );
				fprintf( header, "\n" );

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

void impl_generateParseFunctionForPublishableState( FILE* header, Root& root, CompositeType& obj )
{
	assert( obj.type == CompositeType::Type::publishable );
	fprintf( header, "\ttemplate<class ParserT>\n" );
	fprintf( header, "\tvoid parse( ParserT& parser )\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\tglobalmq::marshalling::impl::parseStructBegin( parser );\n" );
	fprintf( header, "\n" );

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
//				impl_generateApplyUpdateForSimpleType( header, member, true, false );
				fprintf( header, "\t\tglobalmq::marshalling::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s), \"%s\" );\n", paramTypeToParser( member.type.kind ), member.name.c_str(), member.name.c_str(), member.name.c_str() );
				fprintf( header, "\n" );
				break;
			}
			case  MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "\t\tglobalmq::marshalling::impl::parsePublishableStructBegin( parser, \"%s\" );\n", member.name.c_str() );
//				impl_generateApplyUpdateForFurtherProcessingInStruct( header, member, false, true, false );
				fprintf( header, "\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
				fprintf( header, "\t\tglobalmq::marshalling::impl::parsePublishableStructEnd( parser );\n" );
				fprintf( header, "\n" );
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				assert( member.type.messageIdx < root.structs.size() );
				
				fprintf( header, "\t\tglobalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", member.name.c_str() );
				fprintf( header, "\t\tPublishableVectorProcessor::parse<ParserT, decltype(T::%s), %s>( parser, t.%s );\n", member.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str() );
				fprintf( header, "\n" );

				break;
			}
			default:
				assert( false );
		}
	}

	fprintf( header, "\t\tglobalmq::marshalling::impl::parseStructEnd( parser );\n" );
	fprintf( header, "\n" );
	fprintf( header, "\t\tif constexpr ( has_full_update_notifier )\n" );
	fprintf( header, "\t\t\tt.notifyFullyUpdated();\n" );
	fprintf( header, "\t}\n" );
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
				fprintf( header, "\t\timpl::copyVector<declval(UserT::%s), %s>( src.%s, dst.%s );\n",
					s.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str(), member.name.c_str()
				);
				break;
			}
			default:
				assert( false ); // TODO: revise or add cases
		}
	}

	fprintf( header, 
		"\t}\n"
	);
}

void impl_GeneratePublishableStructIsSameFn( FILE* header, Root& root, CompositeType& s )
{
	fprintf( header, 
		"\ttemplate<typename UserT>\n"
		"\tstatic bool isSame(const UserT& s1, const UserT& s2) {\n"
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
					"\t\tif ( s1.%s != s2.%s ) return false;\n",
					member.name.c_str(), member.name.c_str()
				);
				break;
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "\t\tif( ! %s::isSame( s1.%s, s2.%s ) ) return false;\n",
					impl_generatePublishableStructName( member ).c_str(), member.name.c_str(), member.name.c_str()
				);
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				fprintf( header, "\t\tif ( !impl::isSameVector<declval(UserT::%s), %s>( s1.%s, s2.%s ) ) return false;\n",
					s.name.c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), member.name.c_str(), member.name.c_str()
				);
				break;
			}
			default:
				assert( false ); // TODO: revise or add cases
		}
	}

	fprintf( header, 
		"\t\treturn true;\n"
		"\t}\n"
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
	fprintf( header, "\n" );
	impl_GeneratePublishableStructIsSameFn( header, root, obj );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStateMemberSetter( FILE* header, Root& root, bool forRoot/*, const char* rootName*/, MessageParameter& param, size_t idx )
{
//	assert( (forRoot && rootName != nullptr) || (forRoot == false && rootName == nullptr) );
	const char* composer = forRoot ? "composer" : "root.getComposer()";
	const char* composerType = forRoot ? "ComposerT" : "decltype(root.getComposer())";
	const char* addrVector = forRoot ? "GMQ_COLL vector<size_t>()" : "address";

	fprintf( header, 
		"\tvoid set_%s( decltype(T::%s) val) { \n"
		"\t\tt.%s = val; \n",
		param.name.c_str(), param.name.c_str(), param.name.c_str()
	);

	fprintf( header, 
		"\t\tglobalmq::marshalling::impl::composeAddressInPublishable( %s, %s, %zd );\n",
		composer, addrVector, idx
	);

	switch ( param.type.kind )
	{
		case MessageParameterType::KIND::INTEGER:
			fprintf( header, "\t\tglobalmq::marshalling::impl::publishableComposeLeafeInteger( %s, t.%s );\n", composer, param.name.c_str() );
			break;
		case MessageParameterType::KIND::UINTEGER:
			fprintf( header, "\t\tglobalmq::marshalling::impl::publishableComposeLeafeUnsignedInteger( %s, t.%s );\n", composer, param.name.c_str() );
			break;
		case MessageParameterType::KIND::REAL:
			fprintf( header, "\t\tglobalmq::marshalling::impl::publishableComposeLeafeReal( %s, t.%s );\n", composer, param.name.c_str() );
			break;
		case MessageParameterType::KIND::CHARACTER_STRING:
			fprintf( header, "\t\tglobalmq::marshalling::impl::publishableComposeLeafeString( %s, t.%s );\n", composer, param.name.c_str() );
			break;
		case MessageParameterType::KIND::VECTOR:
		{
			fprintf( header, "\t\tglobalmq::marshalling::impl::publishableComposeLeafeValueBegin( %s );\n", composer );
			switch ( param.type.vectorElemKind )
			{
				case MessageParameterType::KIND::INTEGER:
					fprintf( header, "\t\tPublishableVectorProcessor::compose<%s, decltype(T::%s), impl::SignedIntegralType>( %s, t.%s );\n", composerType, param.name.c_str(), composer, param.name.c_str() );
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf( header, "\t\tPublishableVectorProcessor::compose<%s, decltype(T::%s), impl::UnsignedIntegralType>( %s, t.%s );\n", composerType, param.name.c_str(), composer, param.name.c_str() );
					break;
				case MessageParameterType::KIND::REAL:
					fprintf( header, "\t\tPublishableVectorProcessor::compose<%s, decltype(T::%s), impl::RealType>( %s, t.%s );\n", composerType, param.name.c_str(), composer, param.name.c_str() );
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf( header, "\t\tPublishableVectorProcessor::compose<%s, decltype(T::%s), impl::StringType>( %s, t.%s );\n", composerType, param.name.c_str(), composer, param.name.c_str() );
					break;
				case MessageParameterType::KIND::VECTOR:
					assert( false ); // unexpected
					break;
				case MessageParameterType::KIND::STRUCT:
					assert( param.type.messageIdx < root.structs.size() );
					fprintf( header, "\t\tglobalmq::marshalling::impl::publishableComposeLeafeStructBegin( %s );\n", composer );
					fprintf( header, "\t\t%s::compose( %s, t.%s );\n", impl_generatePublishableStructName( *(root.structs[param.type.messageIdx]) ).c_str(), composer, param.name.c_str() );
					fprintf( header, "\t\tglobalmq::marshalling::impl::publishableComposeLeafeStructEnd( %s );\n", composer );
					break;
				default:
					assert( false ); // not implemented (yet)
			}
			fprintf( header, "\t\tglobalmq::marshalling::impl::composeStateUpdateBlockEnd( %s );\n", composer );
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		{
			fprintf( header, "\t\tglobalmq::marshalling::impl::publishableComposeLeafeStructBegin( %s );\n", composer );
			fprintf( header, "\t\t%s::compose( %s, t.%s );\n", impl_generatePublishableStructName( param ).c_str(), composer, param.name.c_str() );
			fprintf( header, "\t\tglobalmq::marshalling::impl::publishableComposeLeafeStructEnd( %s );\n", composer );
			break;
		}
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
			impl_GeneratePublishableStateMemberSetter( header, root, forRoot/*, rootName*/, *it, i );
			impl_GeneratePublishableStateMemberGetter4Set( header, root, rootName, *it, i );
		}
	}
}

void impl_GenerateApplyUpdateMessageMemberFn( FILE* header, Root& root, CompositeType& s )
{
	fprintf( header, 
		"\ttemplate<typename ParserT>\n"
		"\tvoid applyMessageWithUpdates(ParserT& parser)\n"
		"\t{\n"
		"\t\tglobalmq::marshalling::impl::parseStateUpdateMessageBegin( parser );\n"
		"\t\tGMQ_COLL vector<size_t> addr;\n"
		"\t\twhile( impl::parseAddressInPublishable<ParserT, GMQ_COLL vector<size_t>>( parser, addr ) )\n"
		"\t\t{\n"
		"\t\t\tGMQ_ASSERT( addr.size() );\n"
		"\t\t\tswitch ( addr[0] )\n"
		"\t\t\t{\n"
	);

	for ( size_t i=0; i<s.members.size(); ++i )
	{
		fprintf( header, 
			"\t\t\t\tcase %zd:\n"
			"\t\t\t\t{\n", 
			i
		);
		assert( s.members[i] != nullptr );
		auto& member = *(s.members[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				fprintf( header, "\t\t\t\t\tif ( addr.size() > 1 )\n" );
				fprintf( header, "\t\t\t\t\t\tthrow std::exception(); // bad format, TODO: ...\n" );
				impl_generateApplyUpdateForSimpleType( header, member, false, true );
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "\t\t\t\t\tif ( addr.size() == 1 ) // we have to parse and apply changes of this child\n" );
				fprintf( header, "\t\t\t\t\t{\n" );
				fprintf( header, "\t\t\t\t\t\tglobalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );\n" );
				fprintf( header, "\n" );

				impl_generateApplyUpdateForStructItself( header, member, false );

				fprintf( header, "\n" );
				fprintf( header, "\t\t\t\t\t\tglobalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );\n" );
				fprintf( header, "\t\t\t\t\t}\n" );
				fprintf( header, "\t\t\t\t\telse // let child continue parsing\n" );
				fprintf( header, "\t\t\t\t\t{\n" );

				impl_generateApplyUpdateForFurtherProcessingInStruct( header, member, false, false, true );

				fprintf( header, "\t\t\t\t\t}\n" );

				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				fprintf( header, "\t\t\t\t{\n" );
				impl_generateApplyUpdateForFurtherProcessingInVector( header, root, member, false, false );
				fprintf( header, "\t\t\t\t}\n" );
				fprintf( header, "\n" ); 

				break;
			}
			default:
				assert( false );
		}

		fprintf( header, "\t\t\t\t\tbreak;\n" ); 
		fprintf( header, "\t\t\t\t}\n" );
	}

	fprintf( header, "\t\t\t\tdefault:\n" );
	fprintf( header, "\t\t\t\t\tthrow std::exception(); // bad format, TODO: ...\n" );
	fprintf( header, "\t\t\t}\n" );
	fprintf( header, "\t\t\taddr.clear();\n" );
	fprintf( header, "\t\t}\n" );
	fprintf( header, "\t}\n" );
}

void impl_GeneratePublishableStateWrapperForPublisher( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, "template<class T, class ComposerT>\n" );
	fprintf( header, "class %s_WrapperForPublisher : public globalmq::marshalling::PublisherBase<ComposerT>\n", s.name.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tT t;\n" );
	fprintf( header, "\tusing BufferT = typename ComposerT::BufferType;\n" );
	fprintf( header, "\tBufferT buffer;\n" );
	fprintf( header, "\tComposerT composer;\n" );

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );
//	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, s, "\t" );

	fprintf( header, "\npublic:\n" );
	fprintf( header, "\ttemplate<class ... ArgsT>\n" );
	fprintf( header, "\t%s_WrapperForPublisher( ArgsT ... args ) : t( std::forward<ArgsT>( args )... ), composer( buffer ) {}\n", s.name.c_str() );
	fprintf( header, "\tconst T& getState() { return t; }\n" );
	fprintf( header, "\tComposerT& getComposer() { return composer; }\n" );
	fprintf( header, "\tvoid startTick( BufferT&& buff ) { buffer = std::move( buff ); composer.reset(); globalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( composer );}\n" );
	fprintf( header, "\tBufferT&& endTick() { globalmq::marshalling::impl::composeStateUpdateMessageEnd( composer ); return std::move( buffer ); }\n" );
//	fprintf( header, "\tvoid resetComposer( ComposerT* composer_ ) {\n" );
//	fprintf( header, "\t\tcomposer = composer_; \n" );
//	fprintf( header, "\t\tglobalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( *composer );\n" );
//	fprintf( header, "\t}\n" );
//	fprintf( header, "\tvoid finalizeComposing() {\n" );
//	fprintf( header, "\t\tglobalmq::marshalling::impl::composeStateUpdateMessageEnd( *composer );\n" );
//	fprintf( header, "\t}\n" );
	fprintf( header, "\tconst char* name() {\n" );
	fprintf( header, "\t\treturn \"%s\";\n", s.name.c_str() );
	fprintf( header, "\t}\n" );

	impl_GeneratePublishableStateMemberAccessors( header, root, s, true );
	fprintf( header, "\n" );
	impl_generateComposeFunctionForPublishableStruct( header, root, s );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStatePlatformWrapperForPublisher( FILE* header, Root& root, CompositeType& s, std::string platformPrefix, std::string classNotifierName )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, "template<class T>\n" );
	fprintf( header, "class %s_%sWrapperForPublisher : public %s_WrapperForPublisher<T, typename %s::ComposerT>\n", s.name.c_str(), platformPrefix.c_str(), s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tusing ComposerT = typename %s::ComposerT;\n", classNotifierName.c_str() );
	fprintf( header, "public:\n" );
	fprintf( header, "\tusing BufferT = typename %s::ComposerT::BufferType;\n", classNotifierName.c_str() );
	fprintf( header, "\ttemplate<class ... ArgsT>\n" );
	fprintf( header, "\t%s_%sWrapperForPublisher( ArgsT ... args ) : %s_WrapperForPublisher<T, typename %s::ComposerT>( std::forward<ArgsT>( args )... )\n", s.name.c_str(), platformPrefix.c_str(), s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t{ \n" );
	fprintf( header, "\t\t%s::registerPublisher( this );\n", classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual ~%s_%sWrapperForPublisher()\n", s.name.c_str(), platformPrefix.c_str() );
	fprintf( header, "\t{ \n" );
	fprintf( header, "\t\t%s::unregisterPublisher( this );\n", classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );

//	fprintf( header, "\tvirtual ComposerT& getComposer() { return %s_WrapperForPublisher<T, ComposerT>::getComposer(); }\n", s.name.c_str() );
//	fprintf( header, "\tvirtual void resetComposer( ComposerT* composer_ ) { %s_WrapperForPublisher<T, ComposerT>::resetComposer( composer_ ); }\n", s.name.c_str() );
//	fprintf( header, "\tvirtual void finalizeComposing() { %s_WrapperForPublisher<T, ComposerT>::finalizeComposing(); }\n", s.name.c_str() );
	fprintf( header, "\tvirtual void startTick( BufferT&& buff ) { %s_WrapperForPublisher<T, ComposerT>::startTick( std::move( buff ) ); }\n", s.name.c_str() );
	fprintf( header, "\tvirtual BufferT&& endTick() { return  %s_WrapperForPublisher<T, ComposerT>::endTick(); }\n", s.name.c_str() );
	fprintf( header, "\tvirtual void generateStateSyncMessage(ComposerT& composer) { %s_WrapperForPublisher<T, ComposerT>::compose(composer); }\n", s.name.c_str() );
	fprintf( header, "\tvirtual const char* name() { return %s_WrapperForPublisher<T, ComposerT>::name(); }\n", s.name.c_str() );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStateWrapperForSubscriber( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, "template<class T, class RegistrarT>\n" );
	fprintf( header, "class %s_WrapperForSubscriber : public globalmq::marshalling::SubscriberBase<typename RegistrarT::BufferT>\n", s.name.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tT t;\n" );

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );
	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, s, "\t" );

	fprintf( header, "\npublic:\n" );
	fprintf( header, "\ttemplate<class ... ArgsT>\n" );
	fprintf( header, "\t%s_WrapperForSubscriber( ArgsT ... args ) : t( std::forward<ArgsT>( args )... ) {}\n", s.name.c_str() );
	fprintf( header, "\tconst T& getState() { return t; }\n" );
	fprintf( header, "\tvirtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename RegistrarT::BufferT>& parser ) { applyMessageWithUpdates(parser); }\n" );
	fprintf( header, "\tvirtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename RegistrarT::BufferT>& parser ) { applyMessageWithUpdates(parser); }\n" );
	fprintf( header, "\tvirtual const char* name() { return \"%s\"; }\n", s.name.c_str() );
	fprintf( header, "\n" );

	impl_GenerateApplyUpdateMessageMemberFn( header, root, s );
	fprintf( header, "\n" );
	fprintf( header, "\n" );
	impl_generateParseFunctionForPublishableState( header, root, s );

	impl_GeneratePublishableStateMemberAccessors( header, root, s, false );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStatePlatformWrapperForSubscriber( FILE* header, Root& root, CompositeType& s, std::string platformPrefix, std::string classNotifierName )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, "template<class T>\n" );
	fprintf( header, "class %s_%sWrapperForSubscriber : public %s_WrapperForSubscriber<T, %s>\n", s.name.c_str(), platformPrefix.c_str(), s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "public:\n" );
	fprintf( header, "\ttemplate<class ... ArgsT>\n" );
	fprintf( header, "\t%s_%sWrapperForSubscriber( ArgsT ... args ) : %s_WrapperForSubscriber<T, %s>( std::forward<ArgsT>( args )... )\n", s.name.c_str(), platformPrefix.c_str(), s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t{ \n" );
	fprintf( header, "\t\t%s::registerSubscriber( this );\n", classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual ~%s_%sWrapperForSubscriber()\n", s.name.c_str(), platformPrefix.c_str() );
	fprintf( header, "\t{ \n" );
	fprintf( header, "\t\t%s::unregisterSubscriber( this );\n", classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename %s::BufferT>& parser ) \n", classNotifierName.c_str() );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t%s_WrapperForSubscriber<T, %s>::applyMessageWithUpdates(parser);\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename %s::BufferT>& parser )\n", classNotifierName.c_str() );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t%s_WrapperForSubscriber<T, %s>::applyMessageWithUpdates(parser);\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<typename %s::BufferT>& parser ) \n", classNotifierName.c_str() );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t%s_WrapperForSubscriber<T, %s>::parse(parser);\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<typename %s::BufferT>& parser )\n", classNotifierName.c_str() );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t%s_WrapperForSubscriber<T, %s>::parse(parser);\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\tvirtual const char* name()\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\treturn %s_WrapperForSubscriber<T, %s>::name();\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\tvoid subscribe()\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t%s::subscribe( this );\n", classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
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
	fprintf( header, "template<typename T> concept has_full_update_notifier_call = requires(T t) { { t.notifyFullyUpdated() }; };\n" );
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
				"template<typename StateT> concept has_insert_notifier_call2_for_%s = requires { { std::declval<StateT>().notifyInserted_%s(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_%s = requires { { std::declval<StateT>().notifyInserted_%s(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };\n",
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

void generatePublishable( FILE* header, Root& root, CompositeType& s, std::string platformPrefix, std::string classNotifierName )
{
	bool checked = impl_checkParamNameUniqueness(s);
	checked = impl_checkFollowingExtensionRules(s) && checked;
	if ( !checked )
		throw std::exception();

	impl_generatePublishableCommentBlock( header, s );
	impl_GeneratePublishableStateWrapperForPublisher( header, root, s );
	impl_GeneratePublishableStatePlatformWrapperForPublisher( header, root, s, platformPrefix, classNotifierName );
	impl_GeneratePublishableStateWrapperForSubscriber( header, root, s );
	impl_GeneratePublishableStatePlatformWrapperForSubscriber( header, root, s, platformPrefix, classNotifierName );
}


