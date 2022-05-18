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
#include "idl_tree_common.h"

namespace cpptemplates
{

inline
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

inline
std::string impl_generatePublishableStructName( MessageParameter& s )
{
	if ( s.type.kind == MessageParameterType::KIND::STRUCT )
		return fmt::format( "publishable_{}_{}", CompositeType::type2string( CompositeType::Type::structure ), s.type.name );
	else if ( s.type.kind == MessageParameterType::KIND::DISCRIMINATED_UNION )
		return fmt::format( "publishable_{}_{}", CompositeType::type2string( CompositeType::Type::discriminated_union ), s.type.name );
	else
	{
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
			assert( type.structIdx <= s.structs.size() );
			return fmt::format( "STRUCT {}", s.structs[type.structIdx]->name );
		case MessageParameterType::KIND::DISCRIMINATED_UNION: 
			assert( type.structIdx <= s.structs.size() );
			return fmt::format( "DISCRIMINATED_UNION {}", s.structs[type.structIdx]->name );
		case MessageParameterType::KIND::VECTOR: 
			if ( type.vectorElemKind == MessageParameterType::KIND::STRUCT )
			{
				assert( type.structIdx <= s.structs.size() );
				return fmt::format( "VECTOR<STRUCT {}>", s.structs[type.structIdx]->name );
			}
			else if ( type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
			{
				assert( type.structIdx <= s.structs.size() );
				return fmt::format( "VECTOR<DISCRIMINATED_UNION {}>", s.structs[type.structIdx]->name );
			}
			else if ( type.vectorElemKind == MessageParameterType::KIND::ENUM )
				return fmt::format( "VECTOR<ENUM {}>", type.name );
			else
				return fmt::format( "VECTOR<{}>", impl_kindToString( type.vectorElemKind ) );
		case MessageParameterType::KIND::DICTIONARY: 
			if ( type.dictionaryValueKind == MessageParameterType::KIND::STRUCT )
			{
				assert( type.structIdx <= s.structs.size() );
				return fmt::format( "DICTIONARY<{}, STRUCT {}>", impl_kindToString( type.dictionaryKeyKind ), s.structs[type.structIdx]->name );
			}
			else if ( type.dictionaryValueKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
			{
				assert( type.structIdx <= s.structs.size() );
				return fmt::format( "DICTIONARY<{}, DISCRIMINATED_UNION {}>", impl_kindToString( type.dictionaryKeyKind ), s.structs[type.structIdx]->name );
			}
			else if ( type.dictionaryValueKind == MessageParameterType::KIND::ENUM )
				return fmt::format( "DICTIONARY<{}, ENUM {}>", impl_kindToString( type.dictionaryKeyKind ), type.name );
			else
				return fmt::format( "DICTIONARY<{}, {}>", impl_kindToString( type.dictionaryKeyKind ), impl_kindToString( type.dictionaryValueKind ) );

		case MessageParameterType::KIND::ENUM: return fmt::format( "ENUM {}", type.name );
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

std::string impl_generateComposeFunctionNameForStructMemeberOfPublishable( MessageParameter& s )
{
	if ( s.type.kind == MessageParameterType::KIND::STRUCT )
		return fmt::format( "publishable_{}_{}_compose", CompositeType::type2string( CompositeType::Type::structure ), s.type.name );
	else if ( s.type.kind == MessageParameterType::KIND::DISCRIMINATED_UNION )
		return fmt::format( "publishable_{}_{}_compose", CompositeType::type2string( CompositeType::Type::discriminated_union ), s.type.name );
	else
	{
		assert( false );
		return "";
	}
}

std::string impl_generateComposeFunctionNameForPublishableStruct( CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure || s.isDiscriminatedUnion() );
	return fmt::format( "publishable_{}_{}_compose", s.type2string(), s.name );
}

std::string impl_generateParseFunctionNameForStructMemeberOfPublishable( MessageParameter& s )
{
	if ( s.type.kind == MessageParameterType::KIND::STRUCT )
		return fmt::format( "publishable_{}_{}_parse", CompositeType::type2string( CompositeType::Type::structure ), s.type.name );
	else if ( s.type.kind == MessageParameterType::KIND::DISCRIMINATED_UNION )
		return fmt::format( "publishable_{}_{}_parse", CompositeType::type2string( CompositeType::Type::discriminated_union ), s.type.name );
	else
	{
		assert( false );
		return "";
	}
}

std::string impl_generateParseFunctionNameForPublishableStruct( CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure || s.isDiscriminatedUnion() );
	return fmt::format( "publishable_{}_{}_parse", s.type2string(), s.name );
}

std::string impl_generatePublishableStructName( CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union );
	return fmt::format( "publishable_{}_{}", s.type2string(), s.name );
}

void impl_GeneratePublishableStateMemberPresenceCheckingBlock( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable || ( ( s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union ) && s.isStruct4Publishing ) );
	if ( s.isDiscriminatedUnion() )
	{
		// TODO: revise DU
		/*for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			for ( auto& it : cs.getMembers() )
			{
				assert( it != nullptr );
				fprintf( header, "\tstatic constexpr bool has_%s = has_%s_getter<T>;\n", it->name.c_str(), it->name.c_str() );
				fprintf( header, "\tstatic_assert( has_%s, \"type T must have member function T::get_%s() of a return type corresponding to IDL type %s\" );\n", it->name.c_str(), it->name.c_str(), impl_parameterTypeToDescriptiveString( root, it->type ).c_str() );
				fprintf( header, "\tstatic constexpr bool has_%s = has_%s_setter<T>;\n", it->name.c_str(), it->name.c_str() );
				fprintf( header, "\tstatic_assert( has_%s, \"type T must have void member function T::set_%s with a single input parameter of a type corresponding to IDL type %s\" );\n", it->name.c_str(), it->name.c_str(), impl_parameterTypeToDescriptiveString( root, it->type ).c_str() );
			}
		}*/
	}
	else
	{
		for ( auto& it : s.getMembers() )
		{
			assert( it != nullptr );
			fprintf( header, "\tstatic constexpr bool has_%s = has_%s_member<T>;\n", it->name.c_str(), it->name.c_str() );
			fprintf( header, "\tstatic_assert( has_%s, \"type T must have member T::%s of a type corresponding to IDL type %s\" );\n", it->name.c_str(), it->name.c_str(), impl_parameterTypeToDescriptiveString( root, it->type ).c_str() );
		}
	}
	fprintf( header, "\n" );
}

void impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock_MemberIterationBlock( FILE* header, Root& root, CompositeType& s, const char* offset )
{
	// NOTE: in case of updates here revise impl_GenerateDiscriminatedUnionVariantUpdateNotifierPresenceCheckingBlock() as well
	assert( s.type != CompositeType::Type::discriminated_union );
	for ( auto& member : s.getMembers() )
	{
		assert( member != nullptr );
		fprintf( header, "%sstatic constexpr bool has_void_update_notifier_for_%s = has_void_update_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
		fprintf( header, "%sstatic constexpr bool has_update_notifier_for_%s = has_update_notifier_call_for_%s<T, %s>;\n", offset, member->name.c_str(), member->name.c_str(), impl_templateMemberTypeName( "T", *member).c_str() );
		fprintf( header, "%sstatic constexpr bool has_any_notifier_for_%s = has_void_update_notifier_for_%s || has_update_notifier_for_%s;\n", 
			offset, member->name.c_str(), member->name.c_str(), member->name.c_str()
		);
		if ( member->type.kind == MessageParameterType::KIND::VECTOR )
		{
			fprintf( header, "%susing %sT = %s;\n", offset, member->name.c_str(), impl_templateMemberTypeName( "T", *member).c_str() );
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
		else if ( member->type.kind == MessageParameterType::KIND::DICTIONARY )
		{
			fprintf( header, "%susing %sT = %s;\n", offset, member->name.c_str(), impl_templateMemberTypeName( "T", *member).c_str() );
			fprintf( header, "%s//// TODO: revise notifier list !!!!!!!!!!!!!!!!!!!!!!!!!\n", offset );
			fprintf( header, "%sstatic constexpr bool has_void_insert_notifier_for_%s = has_void_insert_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_insert_notifier2_for_%s = has_insert_notifier_call2_for_%s<T, typename %sT::key_type&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_insert_notifier3_for_%s = has_insert_notifier_call3_for_%s<T, typename %sT::key_type&, typename %sT::mapped_type&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str(), member->name.c_str() );

			fprintf( header, "%sstatic constexpr bool has_void_removed_notifier_for_%s = has_void_removed_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_removed_notifier2_for_%s = has_removed_notifier_call2_for_%s<T, typename %sT::key_type>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_removed_notifier3_for_%s = has_removed_notifier_call3_for_%s<T, typename %sT::key_type, typename %sT::mapped_type&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str(), member->name.c_str() );

			fprintf( header, "%sstatic constexpr bool has_void_value_updated_notifier_for_%s = has_value_updated_void_notifier_call_for_%s<T>;\n", offset, member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_value_updated_notifier_for_%s = has_value_updated_notifier_call_for_%s<T, typename %sT::key_type&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str() );
			fprintf( header, "%sstatic constexpr bool has_full_value_updated_notifier_for_%s = has_full_value_updated_notifier_call_for_%s<T, typename %sT::key_type&, typename %sT::mapped_type&>;\n", offset, member->name.c_str(), member->name.c_str(), member->name.c_str(), member->name.c_str() );
		}
	}
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
	*	// for dictionaries
	*	has_void_insert_notifier_call_for_%s            notifyInserted_%s<T>()
	*	has_insert_notifier_call_for_%s                 notifyInserted_%s<T>(index_type_for_array_notifiers, index_type_for_array_notifiers)
	*	has_void_removed_notifier_call_for_%s           notifyRemoved_%s<T>()
	*	has_removed_notifier_call2_for_%s               notifyRemoved_%s<T>(index_type_for_array_notifiers, index_type_for_array_notifiers)
	*	has_removed_notifier_call3_for_%s               notifyRemoved_%s<T, MemberT>(index_type_for_array_notifiers, index_type_for_array_notifiers, MemberT>()	
	*	has_value_updated_void_notifier_call_for_%s     notifyValueUpdated_%s<T>()
	*	has_value_updated_notifier_call_for_%s          notifyValueUpdated_%s<T>(index_type_for_array_notifiers)
	*	has_full_value_updated_notifier_call_for_%s     notifyValueUpdated_%s<T, MemberT>(index_type_for_array_notifiers, MemberT)
	*/
	assert( s.type == CompositeType::Type::publishable || ( ( s.type == CompositeType::Type::structure || s.isDiscriminatedUnion() ) && s.isStruct4Publishing ) ); // TODO: revise DU
	if ( s.isDiscriminatedUnion() )
	{
		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock_MemberIterationBlock( header, root, cs, offset );
		}
	}
	else
		impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock_MemberIterationBlock( header, root, s, offset );
	fprintf( header, "%sstatic constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;\n", offset );
	fprintf( header, "%sstatic constexpr bool has_update_notifier = has_update_notifier_call<T>;\n", offset );
	fprintf( header, "\n" );
}

void impl_generateApplyUpdateForSimpleType( FILE* header, MessageParameter& member, bool addReportChanges, bool isLeafe, std::string offset )
{
	string parseProcessorType = isLeafe ? paramTypeToLeafeParser( member.type.kind ) : paramTypeToParser( member.type.kind );
	if ( addReportChanges )
		fprintf( header, "%sif constexpr( has_any_notifier_for_%s || reportChanges || has_update_notifier )\n", offset.c_str(), member.name.c_str() );
	else
		fprintf( header, "%sif constexpr( has_any_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\t%s oldVal = t.%s;\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	if ( isLeafe )
		fprintf( header, "%s\t::globalmq::marshalling::impl::%s<ParserT, %s>( parser, &(t.%s) );\n", offset.c_str(), paramTypeToLeafeParser( member.type.kind ), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	else
		fprintf( header, "%s\t::globalmq::marshalling::impl::%s<ParserT, %s>( parser, &(t.%s), \"%s\" );\n", offset.c_str(), paramTypeToParser( member.type.kind ), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str() );
	fprintf( header, "%s\tbool currentChanged = oldVal != t.%s;\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\tif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	if ( addReportChanges )
	{
		fprintf( header, "%s\t\tif constexpr ( reportChanges || has_update_notifier )\n", offset.c_str() );
		fprintf( header, "%s\t\t\tchanged = true;\n", offset.c_str() );
	}
	fprintf( header, "%s\t\tif constexpr ( has_void_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\tt.notifyUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\tif constexpr ( has_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\tt.notifyUpdated_%s( oldVal );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );
	fprintf( header, "%selse\n", offset.c_str() );
	if ( isLeafe )
		fprintf( header, "%s\t::globalmq::marshalling::impl::%s<ParserT, %s>( parser, &(t.%s) );\n", offset.c_str(), paramTypeToLeafeParser( member.type.kind ), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	else
		fprintf( header, "%s\t::globalmq::marshalling::impl::%s<ParserT, %s>( parser, &(t.%s), \"%s\" );\n", offset.c_str(), paramTypeToParser( member.type.kind ), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str() );
}

void impl_generateApplyUpdateForSimpleTypeNoNotifiers( FILE* header, MessageParameter& member, bool isLeafe, std::string offset )
{
	if ( isLeafe )
		fprintf( header, "%s::globalmq::marshalling::impl::%s<ParserT, %s>( parser, &(t.%s) );\n", offset.c_str(), paramTypeToLeafeParser( member.type.kind ), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	else
		fprintf( header, "%s::globalmq::marshalling::impl::%s<ParserT, %s>( parser, &(t.%s), \"%s\" );\n", offset.c_str(), paramTypeToParser( member.type.kind ), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str() );
}

void impl_generateApplyUpdateForStructItself( FILE* header, MessageParameter& member, bool addReportChanges, std::string offset )
{
	fprintf( header, "%sif constexpr( has_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\t%s temp_%s;\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), member.name.c_str() );
//	fprintf( header, "%s\t%s::copy<%s, %s>( t.%s, temp_%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_templateMemberTypeName( "T", member).c_str(), member.name.c_str(), member.name.c_str() );
	fprintf( header, "%s\t%s::copy<%s>( t.%s, temp_%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str() );
	fprintf( header, "%s\tbool changedCurrent = %s::parse<ParserT, %s, bool>( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\tif ( changedCurrent )\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	if ( addReportChanges )
	{
		fprintf( header, "%s\t\tif constexpr ( reportChanges || has_update_notifier )\n", offset.c_str() );
		fprintf( header, "%s\t\t\tchanged = true;\n", offset.c_str() );
	}
	fprintf( header, "%s\t\tif constexpr( has_void_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\tt.notifyUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\tif constexpr( has_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\tt.notifyUpdated_%s( temp_%s );\n", offset.c_str(), member.name.c_str(), member.name.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );

	fprintf( header, "%selse if constexpr( has_void_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\tbool changedCurrent = %s::parse<ParserT, %s, bool>( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\tif ( changedCurrent )\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	if ( addReportChanges )
	{
		fprintf( header, "%s\t\tif constexpr ( reportChanges || has_update_notifier )\n", offset.c_str() );
		fprintf( header, "%s\t\t\tchanged = true;\n", offset.c_str() );
	}
	fprintf( header, "%s\t\tt.notifyUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );

	fprintf( header, "%selse\n", offset.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	if ( addReportChanges )
	{
		fprintf( header, "%s\tif constexpr ( reportChanges || has_update_notifier )\n", offset.c_str() );
		fprintf( header, "%s\t{\n", offset.c_str() );
		fprintf( header, "%s\t\tbool changedCurrent = %s::parse<ParserT, %s, bool>( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
		fprintf( header, "%s\t\tif ( changedCurrent )\n", offset.c_str() );
		fprintf( header, "%s\t\t\tchanged = true;\n", offset.c_str() );
		fprintf( header, "%s\t}\n", offset.c_str() );

		fprintf( header, "%s\telse\n", offset.c_str() );
		fprintf( header, "%s\t\t%s::parse( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	}
	else
		fprintf( header, "%s\t%s::parse( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s}\n", offset.c_str() );
}

void impl_generateApplyUpdateForStructItselfNoNotifiers( FILE* header, MessageParameter& member, std::string offset )
{
	fprintf( header, "%s%s::parse( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
}

void impl_generateApplyUpdateForFurtherProcessingInStruct( FILE* header, MessageParameter& member, bool addOffsetInAddr, bool addReportChanges, bool forwardAddress, std::string offset )
{
	assert( (!addOffsetInAddr) || (addOffsetInAddr && forwardAddress) );
	const char* offsetPlusStr = addOffsetInAddr ? "offset + " : "";
	fprintf( header, "%sif constexpr( has_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\t%s temp_%s;\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), member.name.c_str() );
//	fprintf( header, "%s\t%s::copy<%s, %s>( t.%s, temp_%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str() );
	fprintf( header, "%s\t%s::copy<%s>( t.%s, temp_%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str() );
	if ( forwardAddress )
		fprintf( header, "%s\tbool changedCurrent = %s::parse<ParserT, %s, bool>( parser, t.%s, addr, %s1 );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), offsetPlusStr );
	else
		fprintf( header, "%s\tbool changedCurrent = %s::parse<ParserT, %s, bool>( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\tif ( changedCurrent )\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
//	fprintf( header, "%s\t\tchanged = true;\n", offset.c_str() );
	if ( addReportChanges )
	{
		fprintf( header, "%s\t\tif constexpr ( reportChanges || has_update_notifier )\n", offset.c_str() );
		fprintf( header, "%s\t\t\tchanged = true;\n", offset.c_str() );
	}
	fprintf( header, "%s\t\tif constexpr( has_void_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\tt.notifyUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\tif constexpr( has_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\tt.notifyUpdated_%s( temp_%s );\n", offset.c_str(), member.name.c_str(), member.name.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );

	fprintf( header, "%selse if constexpr( has_void_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	if ( forwardAddress )
		fprintf( header, "%s\tbool changedCurrent = %s::parse<ParserT, %s, bool>( parser, t.%s, addr, %s1 );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), offsetPlusStr );
	else
		fprintf( header, "%s\tbool changedCurrent = %s::parse<ParserT, %s, bool>( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\tif ( changedCurrent )\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	if ( addReportChanges )
	{
		fprintf( header, "%s\t\tif constexpr ( reportChanges || has_update_notifier )\n", offset.c_str() );
		fprintf( header, "%s\t\t\tchanged = true;\n", offset.c_str() );
	}
	fprintf( header, "%s\t\tt.notifyUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );

	if ( addReportChanges )
	{
		fprintf( header, "%selse if constexpr ( reportChanges || has_update_notifier )\n", offset.c_str() );
		fprintf( header, "%s{\n", offset.c_str() );
		if ( forwardAddress )
			fprintf( header, "%s\tbool changedCurrent = %s::parse<ParserT, %s, bool>( parser, t.%s, addr, %s1 );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), offsetPlusStr );
		else
			fprintf( header, "%s\tbool changedCurrent = %s::parse<ParserT, %s, bool>( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
		
		fprintf( header, "%s\tif ( changedCurrent )\n", offset.c_str() );
		fprintf( header, "%s\t\tchanged = true;\n", offset.c_str() );



		fprintf( header, "%s}\n", offset.c_str() );
		fprintf( header, "%selse\n", offset.c_str() );
		if ( forwardAddress )
			fprintf( header, "%s\t%s::parse( parser, t.%s, addr, %s1 );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), offsetPlusStr );
		else
			fprintf( header, "%s\t%s::parse( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	}
	else
	{
		fprintf( header, "%selse\n", offset.c_str() );
		if ( forwardAddress )
			fprintf( header, "%s\t%s::parse( parser, t.%s, addr, %s1 );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), offsetPlusStr );
		else
			fprintf( header, "%s\t%s::parse( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	}
}

void impl_generateApplyUpdateForFurtherProcessingInStructNoNotifiers( FILE* header, MessageParameter& member, bool addOffsetInAddr, bool forwardAddress, std::string offset )
{
	assert( (!addOffsetInAddr) || (addOffsetInAddr && forwardAddress) );
	const char* offsetPlusStr = addOffsetInAddr ? "offset + " : "";

	if ( forwardAddress )
		fprintf( header, "%s%s::parse( parser, t.%s, addr, %s1 );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), offsetPlusStr );
	else
		fprintf( header, "%s%s::parse( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
}

void impl_generateApplyUpdateForFurtherProcessingInDictionary( FILE* header, Root& root, MessageParameter& member, bool addOffsetInAddr, bool addReportChanges, std::string offset )
{
//	assert( (!addOffsetInAddr) || (addOffsetInAddr && forwardAddress) );
	const char* offsetPlusStr = addOffsetInAddr ? "offset + " : "";

	fprintf( header, "%s%s oldDictionaryVal;\n", offset.c_str(),  impl_templateMemberTypeName( "T", member).c_str() );
	fprintf( header, "%sbool currentChanged = false;\n", offset.c_str() );
	fprintf( header, "%sconstexpr bool alwaysCollectChanges = reportChanges || has_any_notifier_for_%s;\n", offset.c_str(), member.name.c_str() );
//	fprintf( header, "%sif constexpr( alwaysCollectChanges )\n", offset.c_str() );
//	fprintf( header, "%s\t::globalmq::marshalling::impl::copyDictionary<%s, %s, %s>( t.%s, oldDictionaryVal );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );

fprintf( header, "%s//~~~~~~~~~~XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n", offset.c_str() );

	const char* libType = paramTypeToLibType( member.type.dictionaryValueKind );
	fprintf( header, "%sif ( addr.size() > %s1 ) // one of actions over elements of the dictionary\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\tsize_t action = addr[%s1];\n", offset.c_str(), offsetPlusStr );

	fprintf( header, "%s\tif ( addr.size() > %s2 ) // update for a value of a particular dictionary element\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s\t{\n", offset.c_str() );

	if ( member.type.dictionaryValueKind == MessageParameterType::KIND::STRUCT || member.type.dictionaryValueKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
	{
		fprintf( header, "%s\t\ttypename %s::value_type& value = t.%s[pos];\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	}
	else
		fprintf( header, "%s\t\tthrow std::exception(); // deeper address is unrelated to simple type of dictionary values (IDL type of t.%s elements is %s)\n", offset.c_str(), member.name.c_str(), impl_kindToString( member.type.dictionaryValueKind ) );
				
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s\telse // update of one or more elelments as a whole\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	fprintf( header, "%s\t\tswitch ( action )\n", offset.c_str() );
	fprintf( header, "%s\t\t{\n", offset.c_str() );

	fprintf( header, "%s\t\t\tcase ActionOnDictionary::remove:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::key_type key;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableDictionaryProcessor::parseKey<ParserT, %s, %s>( parser, key );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\tauto f = t.%s.find( key );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tif ( f == t.%s.end() )\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\t\tthrow std::exception();\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::key_type oldVal;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\toldVal = f->second;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tt.%s.erase( key );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tif constexpr ( has_removed_notifier3_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\tt.notifyRemoved_%s( key, oldVal );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\tif constexpr ( has_removed_notifier2_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\tt.notifyRemoved_%s( key );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\tif constexpr ( has_void_removed_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\tt.notifyRemoved_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\tif constexpr ( alwaysCollectChanges )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tcurrentChanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\tcase ActionOnDictionary::update_value:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::key_type key;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableDictionaryProcessor::parseKey<ParserT, %s, %s>( parser, key );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\tauto f = t.%s.find( key );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tif ( f == t.%s.end() )\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\t\tthrow std::exception();\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\ttypename %s::mapped_type& value = f->second;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );

	fprintf( header, "%s\t\t\t\ttypename %s::mapped_type oldValue;\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str() );
	switch ( member.type.dictionaryValueKind )
	{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			fprintf( header, "%s\t\t\t\toldValue = value;\n", offset.c_str() );
			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			assert( member.type.structIdx < root.structs.size() );
			fprintf( header, "%s\t\t\t\t%s::copy( value, oldValue );\n", offset.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str() );
			break;
		default:
			fprintf( header, "%s\t\t\t\tstatic_assert(false);\n", offset.c_str() );
			break;
	}
				
	fprintf( header, "%s\t\t\t\tif constexpr ( has_full_value_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\t\tcurrentChanged = PublishableDictionaryProcessor::parseValueAndCompare<ParserT, %s, %s>( parser, value, oldValue );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t\tif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tchanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyValueUpdated_%s( key, oldValue );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tif constexpr ( has_value_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\t\tt.notifyValueUpdated_%s( key );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tif constexpr ( has_void_value_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\t\tt.notifyValueUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str()  );
				
	fprintf( header, "%s\t\t\t\telse if constexpr ( has_value_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tcurrentChanged = PublishableDictionaryProcessor::parseValueAndCompare<ParserT, %s, %s>( parser, value, oldValue );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t\tif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tchanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyValueUpdated_%s( key );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tif constexpr ( has_void_value_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\t\tt.notifyValueUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\telse if constexpr ( has_void_value_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tcurrentChanged = PublishableDictionaryProcessor::parseValueAndCompare<ParserT, %s, %s>( parser, value, oldValue );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t\tif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tchanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyValueUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\telse\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tif constexpr ( alwaysCollectChanges )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tcurrentChanged = PublishableDictionaryProcessor::parseValueAndCompare<ParserT, %s, %s>( parser, value, oldValue );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t\telse\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tPublishableDictionaryProcessor::parseValue<ParserT, %s, %s>( parser, value );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\tcase ActionOnDictionary::insert:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::key_type key;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableDictionaryProcessor::parseKey<ParserT, %s, %s>( parser, key );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::mapped_type value;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableDictionaryProcessor::parseValue<ParserT, %s, %s>( parser, value );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );

	fprintf( header, "%s\t\t\t\tif constexpr ( has_insert_notifier3_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t%s oldVal;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\t\t::globalmq::marshalling::impl::copyDictionary<%s, %s, %s>( t.%s, oldVal );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\t\tt.%s.insert( std::make_pair( key, value ) );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				
	fprintf( header, "%s\t\t\t\t\tt.notifyInserted_%s( key, oldVal );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\tif constexpr ( has_insert_notifier2_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyInserted_%s( key );\n", offset.c_str(), member.name.c_str() );
				
	fprintf( header, "%s\t\t\t\t\tif constexpr ( has_void_insert_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyInserted_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\telse\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\t\tt.%s.insert( std::make_pair( key, value ) );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\t\tif constexpr ( has_insert_notifier2_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyInserted_%s( key );\n", offset.c_str(), member.name.c_str() );
				
	fprintf( header, "%s\t\t\t\t\tif constexpr ( has_void_insert_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyInserted_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );
				
	fprintf( header, "%s\t\t\t\tif constexpr ( alwaysCollectChanges )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tcurrentChanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\tdefault:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tthrow std::exception();\n", offset.c_str() );
	fprintf( header, "%s\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t::globalmq::marshalling::impl::parseStateUpdateBlockEnd( parser );\n", offset.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
			
//fprintf( header, "//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );
	fprintf( header, "%selse // replacement of the whole dictionary\n", offset.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\t::globalmq::marshalling::impl::publishableParseLeafeDictionaryBegin( parser );\n", offset.c_str() );
	fprintf( header, "\n" );
	fprintf( header, "%s\tif constexpr( alwaysCollectChanges )\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	fprintf( header, "%s\t\tPublishableDictionaryProcessor::parse<ParserT, %s, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\tcurrentChanged = !::globalmq::marshalling::impl::isSameDictionary<%s, %s>( oldDictionaryVal, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s\telse\n", offset.c_str() );
	fprintf( header, "%s\t\tPublishableDictionaryProcessor::parse<ParserT, %s, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "\n" );
	fprintf( header, "%s\t::globalmq::marshalling::impl::publishableParseLeafeDictionaryEnd( parser );\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );
	fprintf( header, "\n" );
	fprintf( header, "%sif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\tchanged = true;\n", offset.c_str() );
	fprintf( header, "%s\tif constexpr( has_void_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\tt.notifyUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\tif constexpr( has_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\tt.notifyUpdated_%s( oldDictionaryVal );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );

	if ( addReportChanges )
	{
		fprintf( header, "\n" );
		fprintf( header, "%sif constexpr( alwaysCollectChanges )\n", offset.c_str() );
		fprintf( header, "%s\treturn currentChanged;\n", offset.c_str() );
	}
}

void impl_generateApplyUpdateForFurtherProcessingInVector( FILE* header, Root& root, MessageParameter& member, bool addOffsetInAddr, std::string offset )
{
//	assert( (!addOffsetInAddr) || (addOffsetInAddr && forwardAddress) );
	const char* offsetPlusStr = addOffsetInAddr ? "offset + " : "";

	fprintf( header, "%s%s oldVectorVal;\n", offset.c_str(),  impl_templateMemberTypeName( "T", member).c_str() );
	fprintf( header, "%sbool currentChanged = false;\n", offset.c_str() );
	fprintf( header, "%sconstexpr bool alwaysCollectChanges = reportChanges || has_any_notifier_for_%s;\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%sif constexpr( alwaysCollectChanges )\n", offset.c_str() );
	fprintf( header, "%s\t::globalmq::marshalling::impl::copyVector<%s, %s>( t.%s, oldVectorVal );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );

//fprintf( header, "//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n", offset.c_str() );

	const char* libType = paramTypeToLibType( member.type.vectorElemKind );
	fprintf( header, "%sif ( addr.size() > %s1 ) // one of actions over elements of the vector\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\tsize_t pos = addr[%s1];\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s\tif ( pos > t.%s.size() )\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\tthrow std::exception();\n", offset.c_str() );

	fprintf( header, "%s\tif ( addr.size() > %s2 ) // update for a member of a particular vector element\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s\t{\n", offset.c_str() );

	if ( member.type.vectorElemKind == MessageParameterType::KIND::STRUCT || member.type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
	{
		assert( member.type.structIdx < root.structs.size() );
		fprintf( header, "%s\t\ttypename %s::value_type& value = t.%s[pos];\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );

		fprintf( header, "%s\t\tif constexpr ( has_full_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t{\n", offset.c_str() );
		fprintf( header, "%s\t\t\ttypename %s::value_type oldValue;\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str() );
		// switch ( member.type.vectorElemKind )
		// {
		// 	case MessageParameterType::KIND::INTEGER:
		// 	case MessageParameterType::KIND::UINTEGER:
		// 	case MessageParameterType::KIND::REAL:
		// 	case MessageParameterType::KIND::CHARACTER_STRING:
		// 		fprintf( header, "%s\t\t\toldValue = value;\n", offset.c_str() );
		// 		break;
		// 	case MessageParameterType::KIND::STRUCT:
		// 	case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf( header, "%s\t\t\t%s::copy( value, oldValue );\n", offset.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str() );
		// 		break;
		// 	default:
		// 		fprintf( header, "%s\t\t\t\tstatic_assert(false);\n", offset.c_str() );
		// 		break;
		// }
		fprintf( header, "%s\t\t\tcurrentChanged = %s::parse<ParserT, typename %s::value_type, bool>( parser, value, addr, %s2 );\n", offset.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), offsetPlusStr );
		fprintf( header, "%s\t\t\tif ( currentChanged )\n", offset.c_str() );
		fprintf( header, "%s\t\t\t{\n", offset.c_str() );
		fprintf( header, "%s\t\t\t\tchanged = true;\n", offset.c_str() );
		fprintf( header, "%s\t\t\t\tt.notifyElementUpdated_%s( pos, oldValue );\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t\t\tif constexpr ( has_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t\t\t\tt.notifyElementUpdated_%s();\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t\t\tif constexpr ( has_void_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t\t\t\tt.notifyElementUpdated_%s();\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t\t}\n", offset.c_str() );
		fprintf( header, "%s\t\t}\n", offset.c_str() );

		fprintf( header, "%s\t\telse if constexpr ( has_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t{\n", offset.c_str() );
		fprintf( header, "%s\t\t\tcurrentChanged = %s::parse<ParserT, typename %s::value_type, bool>( parser, value, addr, %s2 );\n", offset.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), offsetPlusStr );
		fprintf( header, "%s\t\t\tif ( currentChanged )\n", offset.c_str() );
		fprintf( header, "%s\t\t\t{\n", offset.c_str() );
		fprintf( header, "%s\t\t\t\tchanged = true;\n", offset.c_str() );
		fprintf( header, "%s\t\t\t\tt.notifyElementUpdated_%s( pos );\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t\t\tif constexpr ( has_void_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t\t\t\tt.notifyElementUpdated_%s();\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t\t}\n", offset.c_str() );
		fprintf( header, "%s\t\t}\n", offset.c_str() );

		fprintf( header, "%s\t\telse if constexpr ( has_void_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t{\n", offset.c_str() );
		fprintf( header, "%s\t\t\tcurrentChanged = %s::parse<ParserT, typename %s::value_type, bool>( parser, value, addr, %s2 );\n", offset.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), offsetPlusStr );
		fprintf( header, "%s\t\t\tif ( currentChanged )\n", offset.c_str() );
		fprintf( header, "%s\t\t\t{\n", offset.c_str() );
		fprintf( header, "%s\t\t\t\tchanged = true;\n", offset.c_str() );
		fprintf( header, "%s\t\t\t\tt.notifyElementUpdated_%s();\n", offset.c_str(), member.name.c_str() );
		fprintf( header, "%s\t\t\t}\n", offset.c_str() );
		fprintf( header, "%s\t\t}\n", offset.c_str() );

		fprintf( header, "%s\t\telse\n", offset.c_str() );
		fprintf( header, "%s\t\t{\n", offset.c_str() );
		fprintf( header, "%s\t\t\tif constexpr ( alwaysCollectChanges )\n", offset.c_str() );
		fprintf( header, "%s\t\t\t\tcurrentChanged = %s::parse<ParserT, typename %s::value_type, bool>( parser, value, addr, %s2 );\n", offset.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), offsetPlusStr );
		fprintf( header, "%s\t\t\telse\n", offset.c_str() );
		fprintf( header, "%s\t\t\t\t%s::parse<ParserT, typename %s::value_type>( parser, value, addr, %s2 );\n", offset.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), offsetPlusStr );
		fprintf( header, "%s\t\t}\n", offset.c_str() );
	}
	else
		fprintf( header, "%s\t\tthrow std::exception(); // deeper address is unrelated to simple type of vector elements (IDL type of t.%s elements is %s)\n", offset.c_str(), member.name.c_str(), impl_kindToString( member.type.vectorElemKind ) );
				
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s\telse // update of one or more elelments as a whole\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	fprintf( header, "%s\t\tsize_t action;\n", offset.c_str() );
	fprintf( header, "%s\t\t::globalmq::marshalling::impl::parseActionInPublishable( parser, action );\n", offset.c_str() );
	fprintf( header, "%s\t\tswitch ( action )\n", offset.c_str() );
	fprintf( header, "%s\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\tcase ActionOnVector::remove_at:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t%s oldVal;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\t::globalmq::marshalling::impl::copyVector<%s, %s>( t.%s, oldVal );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tt.%s.erase( t.%s.begin() + pos );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tif constexpr ( has_erased_notifier3_for_%s )\n", offset.c_str(), member.name.c_str() );
//	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tt.notifyErased_%s( pos, oldVal );\n", offset.c_str(), member.name.c_str() );
//	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tif constexpr ( has_erased_notifier2_for_%s )\n", offset.c_str(), member.name.c_str() );
//	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
//	fprintf( header, "%s\t\t\t\t\tt.%s.erase( t.%s.begin() + pos );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\t\tt.notifyErased_%s( pos );\n", offset.c_str(), member.name.c_str() );
//	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tif constexpr ( has_void_erased_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
//	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
//	fprintf( header, "%s\t\t\t\t\tt.%s.erase( t.%s.begin() + pos );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\t\tt.notifyErased_%s();\n", offset.c_str(), member.name.c_str() );
//	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tif constexpr ( alwaysCollectChanges )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tcurrentChanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\tcase ActionOnVector::update_at:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::value_type& value = t.%s[pos];\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );

	fprintf( header, "%s\t\t\t\ttypename %s::value_type oldValue;\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str() );
	switch ( member.type.vectorElemKind )
	{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			fprintf( header, "%s\t\t\t\toldValue = value;\n", offset.c_str() );
			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			assert( member.type.structIdx < root.structs.size() );
			fprintf( header, "%s\t\t\t\t%s::copy( value, oldValue );\n", offset.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str() );
			break;
		default:
			fprintf( header, "%s\t\t\t\tstatic_assert(false);\n", offset.c_str() );
			break;
	}
				
	fprintf( header, "%s\t\t\t\tif constexpr ( has_full_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\t\tcurrentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, %s, %s>( parser, value, oldValue );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t\tif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tchanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyElementUpdated_%s( pos, oldValue );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tif constexpr ( has_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\t\tt.notifyElementUpdated_%s( pos );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tif constexpr ( has_void_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str()  );
				
	fprintf( header, "%s\t\t\t\telse if constexpr ( has_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tcurrentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, %s, %s>( parser, value, oldValue );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t\tif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tchanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyElementUpdated_%s( pos );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tif constexpr ( has_void_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\telse if constexpr ( has_void_element_updated_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tcurrentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, %s, %s>( parser, value, oldValue );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t\tif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tchanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tt.notifyElementUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\telse\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tif constexpr ( alwaysCollectChanges )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tcurrentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, %s, %s>( parser, value, oldValue );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t\t\tchanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\telse\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t\tPublishableVectorProcessor::parseSingleValue<ParserT, %s, %s>( parser, value );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\tcase ActionOnVector::insert_single_before:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::value_type value;\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableVectorProcessor::parseSingleValue<ParserT, %s, %s>( parser, value );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );

	fprintf( header, "%s\t\t\t\tif constexpr ( has_insert_notifier3_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\t%s oldVal;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\t\t::globalmq::marshalling::impl::copyVector<%s, %s>( t.%s, oldVal );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				
	fprintf( header, "%s\t\t\t\t\tt.notifyInserted_%s( pos, oldVal );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\tif constexpr ( has_insert_notifier2_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\tt.notifyInserted_%s( pos );\n", offset.c_str(), member.name.c_str() );
				
	fprintf( header, "%s\t\t\t\tif constexpr ( has_void_insert_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\t\t\t\tt.notifyInserted_%s();\n", offset.c_str(), member.name.c_str() );
				
	fprintf( header, "%s\t\t\t\tt.%s.insert( t.%s.begin() + pos, value );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				
	fprintf( header, "%s\t\t\t\tif constexpr ( alwaysCollectChanges )\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t\tcurrentChanged = true;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\tdefault:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tthrow std::exception();\n", offset.c_str() );
	fprintf( header, "%s\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t::globalmq::marshalling::impl::parseStateUpdateBlockEnd( parser );\n", offset.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
			
//fprintf( header, "//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );
	fprintf( header, "%selse // replacement of the whole vector\n", offset.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\t::globalmq::marshalling::impl::publishableParseLeafeVectorBegin( parser );\n", offset.c_str() );
	fprintf( header, "\n" );
	fprintf( header, "%s\tif constexpr( alwaysCollectChanges )\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	fprintf( header, "%s\t\tPublishableVectorProcessor::parse<ParserT, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\tcurrentChanged = !::globalmq::marshalling::impl::isSameVector<%s, %s>( oldVectorVal, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s\telse\n", offset.c_str() );
	fprintf( header, "%s\t\tPublishableVectorProcessor::parse<ParserT, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "\n" );
	fprintf( header, "%s\t::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );
	fprintf( header, "\n" );
	fprintf( header, "%sif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\tchanged = true;\n", offset.c_str() );
	fprintf( header, "%s\tif constexpr( has_void_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\tt.notifyUpdated_%s();\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\tif constexpr( has_update_notifier_for_%s )\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s\t\tt.notifyUpdated_%s( oldVectorVal );\n", offset.c_str(), member.name.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );

}

void impl_generateApplyUpdateForFurtherProcessingInDictionaryNoNotifiers( FILE* header, Root& root, MessageParameter& member, bool addOffsetInAddr, std::string offset )
{
//	assert( (!addOffsetInAddr) || (addOffsetInAddr && forwardAddress) );
	const char* offsetPlusStr = addOffsetInAddr ? "offset + " : "";

	const char* libType = paramTypeToLibType( member.type.vectorElemKind );
	assert( member.type.structIdx < root.structs.size() );
	fprintf( header, "%sif ( addr.size() > %s1 ) // one of actions over elements of the dictionary\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\tsize_t action = addr[%s1];\n", offset.c_str(), offsetPlusStr );

	fprintf( header, "%s\tif ( addr.size() > %s2 ) // update for a value of a particular dictionary element\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s\t{\n", offset.c_str() );

	if ( member.type.dictionaryValueKind == MessageParameterType::KIND::STRUCT || member.type.dictionaryValueKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
	{
		fprintf( header, "%s\t\tthrow std::exception(); // so far unsupported and, therefore, unexpected\n", offset.c_str() );
	}
	else
		fprintf( header, "%s\t\tthrow std::exception(); // deeper address is unrelated to simple type of dictionary values (IDL type of t.%s elements is %s)\n", offset.c_str(), member.name.c_str(), impl_kindToString( member.type.dictionaryValueKind ) );
				
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s\telse // update of one or more elelments as a whole\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	fprintf( header, "%s\t\tswitch ( action )\n", offset.c_str() );
	fprintf( header, "%s\t\t{\n", offset.c_str() );

	fprintf( header, "%s\t\t\tcase ActionOnDictionary::remove:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::key_type key;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableDictionaryProcessor::parseKey<ParserT, %s, %s>( parser, key );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\tauto f = t.%s.find( key );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tif ( f == t.%s.end() )\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\t\tthrow std::exception();\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tt.%s.erase( key );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\tcase ActionOnDictionary::update_value:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::key_type key;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableDictionaryProcessor::parseKey<ParserT, %s, %s>( parser, key );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\tauto f = t.%s.find( key );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tif ( f == t.%s.end() )\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\t\tthrow std::exception();\n", offset.c_str() );

	fprintf( header, "%s\t\t\t\ttypename %s::mapped_type& value = f->second;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableDictionaryProcessor::parseValue<ParserT, %s, %s>( parser, value );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\tcase ActionOnDictionary::insert:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::key_type key;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableDictionaryProcessor::parseKey<ParserT, %s, %s>( parser, key );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::mapped_type value;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableDictionaryProcessor::parseValue<ParserT, %s, %s>( parser, value );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\tt.%s.insert( std::make_pair( key, value ) );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );
	
	fprintf( header, "%s\t\t\tdefault:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tthrow std::exception();\n", offset.c_str() );
	fprintf( header, "%s\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t::globalmq::marshalling::impl::parseStateUpdateBlockEnd( parser );\n", offset.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
			
	fprintf( header, "%s}\n", offset.c_str() );
	
	fprintf( header, "%selse // replacement of the whole dictionary\n", offset.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\t::globalmq::marshalling::impl::publishableParseLeafeDictionaryBegin( parser );\n", offset.c_str() );
	fprintf( header, "\n" );
	fprintf( header, "%s\tPublishableDictionaryProcessor::parse<ParserT, %s, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "\n" );
	fprintf( header, "%s\t::globalmq::marshalling::impl::publishableParseLeafeDictionaryEnd( parser );\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );
	fprintf( header, "\n" );
}

void impl_generateApplyUpdateForFurtherProcessingInVectorNoNotifiers( FILE* header, Root& root, MessageParameter& member, bool addOffsetInAddr, std::string offset )
{
//	assert( (!addOffsetInAddr) || (addOffsetInAddr && forwardAddress) );
	const char* offsetPlusStr = addOffsetInAddr ? "offset + " : "";

	const char* libType = paramTypeToLibType( member.type.vectorElemKind );
	fprintf( header, "%sif ( addr.size() > %s1 ) // one of actions over elements of the vector\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\tsize_t pos = addr[%s1];\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s\tif ( pos > t.%s.size() )\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\tthrow std::exception();\n", offset.c_str() );

	fprintf( header, "%s\tif ( addr.size() > %s2 ) // update for a member of a particular vector element\n", offset.c_str(), offsetPlusStr );
	fprintf( header, "%s\t{\n", offset.c_str() );

	if ( member.type.vectorElemKind == MessageParameterType::KIND::STRUCT || member.type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
	{
		assert( member.type.structIdx < root.structs.size() );
		fprintf( header, "%s\t\ttypename %s::value_type& value = t.%s[pos];\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
		fprintf( header, "%s\t\t%s::parse<ParserT, typename %s::value_type>( parser, value, addr, %s2 );\n", offset.c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), offsetPlusStr );
	}
	else
		fprintf( header, "%s\t\tthrow std::exception(); // deeper address is unrelated to simple type of vector elements (IDL type of t.%s elements is %s)\n", offset.c_str(), member.name.c_str(), impl_kindToString( member.type.vectorElemKind ) );
				
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s\telse // update of one or more elelments as a whole\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	fprintf( header, "%s\t\tsize_t action;\n", offset.c_str() );
	fprintf( header, "%s\t\t::globalmq::marshalling::impl::parseActionInPublishable( parser, action );\n", offset.c_str() );
	fprintf( header, "%s\t\tswitch ( action )\n", offset.c_str() );
	fprintf( header, "%s\t\t{\n", offset.c_str() );

	fprintf( header, "%s\t\t\tcase ActionOnVector::remove_at:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tt.%s.erase( t.%s.begin() + pos );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\tcase ActionOnVector::update_at:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::value_type& value = t.%s[pos];\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableVectorProcessor::parseSingleValue<ParserT, %s, %s>( parser, value );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );

	fprintf( header, "%s\t\t\tcase ActionOnVector::insert_single_before:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t{\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\t::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\ttypename %s::value_type value;\n", offset.c_str(), impl_templateMemberTypeName( "T", member, true ).c_str() );
	fprintf( header, "%s\t\t\t\tPublishableVectorProcessor::parseSingleValue<ParserT, %s, %s>( parser, value );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str() );
				
	fprintf( header, "%s\t\t\t\tt.%s.insert( t.%s.begin() + pos, value );\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t\t\t\tbreak;\n", offset.c_str() );
	fprintf( header, "%s\t\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t\tdefault:\n", offset.c_str() );
	fprintf( header, "%s\t\t\t\tthrow std::exception();\n", offset.c_str() );
	fprintf( header, "%s\t\t}\n", offset.c_str() );
	fprintf( header, "%s\t\t::globalmq::marshalling::impl::parseStateUpdateBlockEnd( parser );\n", offset.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
			
	fprintf( header, "%s}\n", offset.c_str() );
	fprintf( header, "%selse // replacement of the whole vector\n", offset.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\t::globalmq::marshalling::impl::publishableParseLeafeVectorBegin( parser );\n", offset.c_str() );
	fprintf( header, "%s\tPublishableVectorProcessor::parse<ParserT, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
	fprintf( header, "%s\t::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );
	fprintf( header, "\n" );
}

void impl_GeneratePublishableStateMemberGetter( FILE* header, Root& root, CompositeType& s, MessageParameter& param )
{
	assert( s.type == CompositeType::Type::publishable || ( ( s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case ) && s.isStruct4Publishing ) );
	if ( param.type.isNumericType() )
		fprintf( header, "\tauto get_%s() { return t.%s; }\n", param.name.c_str(), impl_memberOrAccessFunctionName( param ).c_str() );
	else if ( param.type.kind == MessageParameterType::KIND::VECTOR )
	{
		if ( param.type.vectorElemKind == MessageParameterType::KIND::STRUCT || param.type.vectorElemKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
		{
			assert( root.structs.size() > param.type.structIdx );
			fprintf( header, "\tauto get_%s() { return globalmq::marshalling::VectorOfStructRefWrapper<%s_RefWrapper<typename %s::value_type>, %s>(t.%s); }\n", 
				param.name.c_str(), root.structs[param.type.structIdx]->name.c_str(), impl_templateMemberTypeName( "T", param, true ).c_str(), impl_templateMemberTypeName( "T", param).c_str(), impl_memberOrAccessFunctionName( param ).c_str() );
		}
		else
			fprintf( header, "\tauto get_%s() { return globalmq::marshalling::VectorOfSimpleTypeRefWrapper(t.%s); }\n", param.name.c_str(), impl_memberOrAccessFunctionName( param ).c_str() );
	}
	else if ( param.type.kind == MessageParameterType::KIND::DICTIONARY )
	{
		if ( param.type.dictionaryValueKind == MessageParameterType::KIND::STRUCT || param.type.dictionaryValueKind == MessageParameterType::KIND::DISCRIMINATED_UNION )
		{
			assert( root.structs.size() > param.type.structIdx );
			fprintf( header, "\tauto get_%s() { return globalmq::marshalling::DictionaryOfStructRefWrapper<%s_RefWrapper<typename %s::value_type>, %s>(t.%s); }\n", 
				param.name.c_str(), root.structs[param.type.structIdx]->name.c_str(), impl_templateMemberTypeName( "T", param, true ).c_str(), impl_templateMemberTypeName( "T", param).c_str(), impl_memberOrAccessFunctionName( param ).c_str() );
		}
		else
			fprintf( header, "\tauto get_%s() { return globalmq::marshalling::DictionaryOfSimpleTypeRefWrapper(t.%s); }\n", param.name.c_str(), impl_memberOrAccessFunctionName( param ).c_str() );
	}
	else
		fprintf( header, "\tconst auto& get_%s() { return t.%s; }\n", param.name.c_str(), impl_memberOrAccessFunctionName( param ).c_str() );
}

void impl_GeneratePublishableStateMemberGetter4Set( FILE* header, Root& root, const char* rootTypeNameBase, MessageParameter& param, size_t idx )
{
	string rootType;
	string addr;
	string rootObjName;
	if ( rootTypeNameBase == nullptr )
	{
		rootType = "RootT";
		addr = "address";
		rootObjName = "root";
	}
	else
	{
		rootType = fmt::format( "{}_WrapperForPublisher", rootTypeNameBase );
		addr = "GMQ_COLL vector<size_t>()";
		rootObjName = "*this";
	}
	if ( param.type.kind == MessageParameterType::KIND::STRUCT )
	{
		assert( param.type.structIdx < root.structs.size() );
//		fprintf( header, "\tauto get4set_%s() { return %s_RefWrapper4Set</*aaa*/%s, %s>(t.%s, *this, %s, %zd); }\n", 
//			param.name.c_str(), root.structs[param.type.structIdx]->name.c_str(), impl_templateMemberTypeName( "T", param ).c_str(), rootType.c_str(), impl_memberOrAccessFunctionName( param ).c_str(), addr.c_str(), idx );
		fprintf( header, "\tauto get4set_%s() { return %s_RefWrapper4Set<%s, %s>(t.%s, %s, %s, %zd); }\n", 
			param.name.c_str(), root.structs[param.type.structIdx]->name.c_str(), impl_templateMemberTypeName( "T", param ).c_str(), rootType.c_str(), impl_memberOrAccessFunctionName( param ).c_str(), rootObjName.c_str(), addr.c_str(), idx );
	}
	else if ( param.type.kind == MessageParameterType::KIND::DISCRIMINATED_UNION ) // TODO: revise DU
	{
		assert( param.type.structIdx < root.structs.size() );
		fprintf( header, "\tauto get4set_%s() { return %s_RefWrapper4Set<%s, %s>(t.%s, %s, %s, %zd); }\n", 
			param.name.c_str(), root.structs[param.type.structIdx]->name.c_str(), impl_templateMemberTypeName( "T", param ).c_str(), rootType.c_str(), impl_memberOrAccessFunctionName( param ).c_str(), rootObjName.c_str(), addr.c_str(), idx );
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
				fprintf( header, "\tauto get4set_%s() { return globalmq::marshalling::VectorRefWrapper4Set<%s, %s, %s>(t.%s, %s, %s, %zd); }\n", 
					param.name.c_str(), impl_templateMemberTypeName( "T", param ).c_str(), libType, rootType.c_str(), impl_memberOrAccessFunctionName( param ).c_str(), rootObjName.c_str(), addr.c_str(), idx );
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION: // TODO: revise DU (lib kw: VectorOfStructRefWrapper4Set and around)
				assert( param.type.structIdx < root.structs.size() );
				fprintf( header, 
					"\tauto get4set_%s() { return globalmq::marshalling::VectorOfStructRefWrapper4Set<%s, %s, %s, %s_RefWrapper4Set<typename %s::value_type, %s>>(t.%s, %s, %s, %zd); }\n", 
					param.name.c_str(), impl_templateMemberTypeName( "T", param ).c_str(),
					impl_generatePublishableStructName( *(root.structs[param.type.structIdx]) ).c_str(), 
					rootType.c_str(), 
					root.structs[param.type.structIdx]->name.c_str(), 
					impl_templateMemberTypeName( "T", param, true ).c_str(),
					rootType.c_str(), 
					impl_memberOrAccessFunctionName( param ).c_str(), 
					rootObjName.c_str(),
					addr.c_str(), idx );
				break;
			default:
				assert( false ); // unexpected or not (yet) implemented
		}
	}
	else if ( param.type.kind == MessageParameterType::KIND::DICTIONARY )
	{
		std::string libKeyType = dictionaryKeyTypeToLibTypeOrTypeProcessor( param.type, root );
		std::string libValueType = dictionaryValueTypeToLibTypeOrTypeProcessor( param.type, root );
		switch( param.type.dictionaryValueKind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION: // TODO: revise DU (lib kw: VectorOfStructRefWrapper4Set and around)
				fprintf( header, "\tauto get4set_%s() { return globalmq::marshalling::DictionaryRefWrapper4Set<%s, %s, %s, %s>(t.%s, %s, %s, %zd); }\n", 
					param.name.c_str(), impl_templateMemberTypeName( "T", param ).c_str(), libKeyType.c_str(), libValueType.c_str(), rootType.c_str(), impl_memberOrAccessFunctionName( param ).c_str(), rootObjName.c_str(), addr.c_str(), idx );
				break;
			default:
				assert( false ); // unexpected or not (yet) implemented
		}
	}
}


void impl_generateComposeFunctionForPublishableStruct_MemberIterationBlock( FILE* header, Root& root, CompositeType& obj, const char* offset )
{
	assert( obj.type != CompositeType::Type::discriminated_union );

	const char* composer = "composer";

	for ( size_t i=0; i<obj.getMembers().size(); ++i )
	{
		auto& it = obj.getMembers()[i];
		assert( it != nullptr );
		auto& member = *it;
		const char* addSepar = i + 1 < obj.getMembers().size() ? "true": "false";
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "%s::globalmq::marshalling::impl::publishableStructComposeInteger( %s, t.%s, \"%s\", %s );\n", offset, composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
				fprintf( header, "\n" );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "%s::globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( %s, t.%s, \"%s\", %s );\n", offset, composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
				fprintf( header, "\n" );
				break;
			case MessageParameterType::KIND::REAL:
				fprintf( header, "%s::globalmq::marshalling::impl::publishableStructComposeReal( %s, t.%s, \"%s\", %s );\n", offset, composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
				fprintf( header, "\n" );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "%s::globalmq::marshalling::impl::publishableStructComposeString( %s, t.%s, \"%s\", %s );\n", offset, composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
				fprintf( header, "\n" );
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION: // TODO: revise DU
			{
				fprintf( header, "%s::globalmq::marshalling::impl::composePublishableStructBegin( %s, \"%s\" );\n", offset, composer, member.name.c_str() );
				fprintf( header, "%s%s::compose( %s, t.%s );\n", offset, impl_generatePublishableStructName( member ).c_str(), composer, impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s::globalmq::marshalling::impl::composePublishableStructEnd( %s, %s );\n", offset, composer, addSepar );
				fprintf( header, "\n" );
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				switch ( member.type.vectorElemKind )
				{
					case MessageParameterType::KIND::INTEGER:
						fprintf( header, "%sPublishableVectorProcessor::compose<ComposerT, %s, ::globalmq::marshalling::impl::SignedIntegralType>( %s, t.%s, \"%s\", %s );\n", offset, impl_templateMemberTypeName( "T", member).c_str(), composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
						break;
					case MessageParameterType::KIND::UINTEGER:
						fprintf( header, "%sPublishableVectorProcessor::compose<ComposerT, %s, ::globalmq::marshalling::impl::UnsignedIntegralType>( %s, t.%s, \"%s\", %s );\n", offset, impl_templateMemberTypeName( "T", member).c_str(), composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
						break;
					case MessageParameterType::KIND::REAL:
						fprintf( header, "%sPublishableVectorProcessor::compose<ComposerT, %s, ::globalmq::marshalling::impl::RealType>( %s, t.%s, \"%s\", %s );\n", offset, impl_templateMemberTypeName( "T", member).c_str(), composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
						break;
					case MessageParameterType::KIND::CHARACTER_STRING:
						fprintf( header, "%sPublishableVectorProcessor::compose<ComposerT, %s, ::globalmq::marshalling::impl::StringType>( %s, t.%s, \"%s\", %s );\n", offset, impl_templateMemberTypeName( "T", member).c_str(), composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
						break;
					case MessageParameterType::KIND::VECTOR:
					case MessageParameterType::KIND::DICTIONARY:
						assert( false ); // unexpected
						break;
					case MessageParameterType::KIND::STRUCT:
					case MessageParameterType::KIND::DISCRIMINATED_UNION:
						assert( member.type.structIdx < root.structs.size() );
						fprintf( header, "%sPublishableVectorProcessor::compose<ComposerT, %s, %s>( %s, t.%s, \"%s\", %s );\n", offset, impl_templateMemberTypeName( "T", member).c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str(), composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
						break;
					default:
						assert( false ); // not implemented (yet)
				}
				fprintf( header, "\n" );
				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				switch ( member.type.dictionaryValueKind )
				{
					case MessageParameterType::KIND::INTEGER:
					case MessageParameterType::KIND::UINTEGER:
					case MessageParameterType::KIND::REAL:
					case MessageParameterType::KIND::CHARACTER_STRING:
						fprintf( header, "%sPublishableDictionaryProcessor::compose<ComposerT, %s, %s, %s>( %s, t.%s, \"%s\", %s );\n", offset, impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
						break;
					case MessageParameterType::KIND::VECTOR:
					case MessageParameterType::KIND::DICTIONARY:
						assert( false ); // unexpected
						break;
					case MessageParameterType::KIND::STRUCT:
					case MessageParameterType::KIND::DISCRIMINATED_UNION:
						assert( member.type.structIdx < root.structs.size() );
						fprintf( header, "%sPublishableDictionaryProcessor::compose<ComposerT, %s, %s, %s>( %s, t.%s, \"%s\", %s );\n", offset, impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_generatePublishableStructName( *(root.structs[member.type.structIdx]) ).c_str(), composer, impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str(), addSepar );
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
}

void impl_generateComposeFunctionForPublishableStruct( FILE* header, Root& root, CompositeType& obj, const string& className )
{
	if ( obj.type == CompositeType::Type::structure || obj.type == CompositeType::Type::discriminated_union )
	{
		fprintf( header, "\ttemplate<class ComposerT, class T>\n" );
		fprintf( header, "\tvoid %s::compose( ComposerT& composer, const T& t )\n", className.c_str() );
		fprintf( header, "\t{\n" );
	}
	else if ( obj.type == CompositeType::Type::publishable )
	{
		fprintf( header, "\ttemplate<class ComposerType>\n" );
		fprintf( header, "\tvoid compose( ComposerType& composer )\n" );
		fprintf( header, "\t{\n" );
		fprintf( header, "\t\t::globalmq::marshalling::impl::composeStructBegin( composer );\n" );
		fprintf( header, "\n" );
	}
	else
		assert( false );

	if ( obj.isDiscriminatedUnion() )
	{
		fprintf( header, "\t\tuint64_t caseId = t.currentVariant();\n" );
		fprintf( header, "\t\t::globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, caseId, \"caseId\", true );\n" );

		fprintf( header, "\t\tif ( caseId != T::Variants::unknown )\n" );
		fprintf( header, "\t\t{\n" );
		fprintf( header, "\t\t\t::globalmq::marshalling::impl::composePublishableStructBegin( composer, \"caseData\" );\n" );
		fprintf( header, "\t\t\tswitch ( caseId )\n" );
		fprintf( header, "\t\t\t{\n" );
		for ( auto& it: obj.getDiscriminatedUnionCases() )
		{
			fprintf( header, "\t\t\t\tcase %zd: // IDL CASE %s\n", it->numID, it->name.c_str() );
			fprintf( header, "\t\t\t\t{\n" );
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_generateComposeFunctionForPublishableStruct_MemberIterationBlock( header, root, cs, "\t\t\t\t\t" );
			fprintf( header, "\t\t\t\t\tbreak;\n" );
			fprintf( header, "\t\t\t\t}\n" );
		}
		fprintf( header, "\t\t\t\tdefault:\n" );
		fprintf( header, "\t\t\t\t\tthrow std::exception(); // unexpected\n" );
		fprintf( header, "\t\t\t}\n" );
		fprintf( header, "\t\t\t::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );\n" );

		fprintf( header, "\t\t}\n" );
	}
	else
		impl_generateComposeFunctionForPublishableStruct_MemberIterationBlock( header, root, obj, "\t\t" );

	if ( obj.type == CompositeType::Type::publishable )
	{
		fprintf( header, "\n" );
		fprintf( header, "\t\t::globalmq::marshalling::impl::composeStructEnd( composer );\n" );
	}

	fprintf( header, "\t}\n" );
}

void impl_generateContinueParsingFunctionForPublishableStruct_MemberIterationBlock( FILE* header, Root& root, CompositeType& obj, const char* offset, size_t idxBase )
{
	assert( obj.type != CompositeType::Type::discriminated_union );

	for ( size_t i=0; i<obj.getMembers().size(); ++i )
	{
		fprintf( header, "%scase %zd:\n", offset, idxBase + i );
		fprintf( header, "%s{\n", offset );
		assert( obj.getMembers()[i] != nullptr );
		auto& member = *(obj.getMembers()[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				fprintf( header, "%s\tif ( addr.size() > offset + 1 )\n", offset );
				fprintf( header, "%s\t\tthrow std::exception(); // bad format, TODO: ...\n", offset );
				impl_generateApplyUpdateForSimpleType( header, member, true, true, std::string( offset ) + '\t' );
				break;
			}
			case  MessageParameterType::KIND::STRUCT:
			case  MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				fprintf( header, "%s\tif ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child\n", offset );
				fprintf( header, "%s\t{\n", offset );
				fprintf( header, "%s\t\t::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );\n", offset );

				impl_generateApplyUpdateForStructItself( header, member, true, std::string( offset) + "\t\t" ); // TODO: revise DU: we may need something DU_spec here

				fprintf( header, "%s\t\t::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );\n", offset );
				fprintf( header, "%s\t}\n", offset );
				fprintf( header, "%s\telse // let child continue parsing\n", offset );
				fprintf( header, "%s\t{\n", offset );

				impl_generateApplyUpdateForFurtherProcessingInStruct( header, member, true, true, true, std::string( offset) + "\t\t" );

				fprintf( header, "%s\t}\n", offset );
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				fprintf( header, "%s\t{\n", offset );
				impl_generateApplyUpdateForFurtherProcessingInVector( header, root, member, true, std::string( offset ) + "\t\t" );
				fprintf( header, "%s\t}\n", offset );
				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				fprintf( header, "%s\t{\n", offset );
				impl_generateApplyUpdateForFurtherProcessingInDictionary( header, root, member, true, false, std::string( offset ) + "\t\t" );
				fprintf( header, "%s\t}\n", offset );
				break;
			}
			default:
				assert( false );
		}
		fprintf( header, "%s\tbreak;\n", offset );
		fprintf( header, "%s}\n", offset );
	}
}

void impl_generateApplyUpdateForDiscriminatedUnionVariant( FILE* header, bool addReportChanges, std::string offset )
{
	fprintf( header, "%suint64_t newVar = T::Variants::unknown;\n", offset.c_str() );
	fprintf( header, "%s::globalmq::marshalling::impl::publishableParseLeafeUnsignedInteger<ParserT, uint64_t>( parser, &newVar );\n", offset.c_str() );

	if ( addReportChanges )
		fprintf( header, "%sif constexpr( has_any_notifier_for_currentVariant || reportChanges || has_update_notifier )\n", offset.c_str() );
	else
		fprintf( header, "%sif constexpr( has_any_notifier_for_currentVariant )\n", offset.c_str() );
	fprintf( header, "%s{\n", offset.c_str() );
	fprintf( header, "%s\tauto oldVal = t.currentVariant();\n", offset.c_str() );
	fprintf( header, "%s\tt.initAs( (typename T::Variants)( newVar ) );\n", offset.c_str() );
	fprintf( header, "%s\tbool currentChanged = oldVal != t.currentVariant();\n", offset.c_str() );
	fprintf( header, "%s\tif ( currentChanged )\n", offset.c_str() );
	fprintf( header, "%s\t{\n", offset.c_str() );
	if ( addReportChanges )
	{
		fprintf( header, "%s\t\tif constexpr ( reportChanges || has_update_notifier )\n", offset.c_str() );
		fprintf( header, "%s\t\t\tchanged = true;\n", offset.c_str() );
	}
	fprintf( header, "%s\t\tif constexpr ( has_void_update_notifier_for_currentVariant )\n", offset.c_str() );
	fprintf( header, "%s\t\t\tt.notifyUpdated_currentVariant();\n", offset.c_str() );
	fprintf( header, "%s\t\tif constexpr ( has_update_notifier_for_currentVariant )\n", offset.c_str() );
	fprintf( header, "%s\t\t\tt.notifyUpdated_currentVariant( oldVal );\n", offset.c_str() );
	fprintf( header, "%s\t}\n", offset.c_str() );
	fprintf( header, "%s}\n", offset.c_str() );
	fprintf( header, "%selse\n", offset.c_str() );

	fprintf( header, "%s\tt.initAs( (typename T::Variants)( newVar ) );\n", offset.c_str() );
}

void impl_GenerateDiscriminatedUnionVariantUpdateNotifierPresenceCheckingBlock( FILE* header, const char* offset )
{
	// along lines of impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock()
	fprintf( header, "%sstatic constexpr bool has_void_update_notifier_for_currentVariant = has_void_update_notifier_call_for_currentVariant<T>;\n", offset );
	fprintf( header, "%sstatic constexpr bool has_update_notifier_for_currentVariant = has_update_notifier_call_for_currentVariant<T, typename T::Variants>;\n", offset );
	fprintf( header, "%sstatic constexpr bool has_any_notifier_for_currentVariant = has_void_update_notifier_for_currentVariant || has_update_notifier_for_currentVariant;\n", offset );
}

void impl_generateContinueParsingFunctionForPublishableStruct( FILE* header, Root& root, CompositeType& obj, const string& className )
{
	fprintf( header, "\ttemplate<class ParserT, class T, class RetT>\n" );
	fprintf( header, "\tRetT %s::parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset )\n", className.c_str() );
	fprintf( header, "\t{\n" );
//	fprintf( header, "\t\t//****  ContinueParsing  **************************************************************************************************************************************************************\n" );
	fprintf( header, "\t\tstatic_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );\n" );
	fprintf( header, "\t\tconstexpr bool reportChanges = std::is_same<RetT, bool>::value;\n" );
	fprintf( header, "\t\tbool changed = false;\n" );

	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, obj, "\t\t" );


	if ( obj.isDiscriminatedUnion() )
	{
		impl_GenerateDiscriminatedUnionVariantUpdateNotifierPresenceCheckingBlock( header, "\t\t" );
		fprintf( header, "\t\tGMQ_ASSERT( addr.size() );\n" );
		fprintf( header, "\t\tif ( addr[offset] == 0 ) // changing current variant\n" );
		fprintf( header, "\t\t{\n" );
		fprintf( header, "\t\t\tif ( addr.size() > offset + 1 )\n" );
		fprintf( header, "\t\t\t\tthrow std::exception(); // bad format, TODO: ...\n" );
		impl_generateApplyUpdateForDiscriminatedUnionVariant( header, true, "\t\t\t" );
		fprintf( header, "\t\t}\n" );

		fprintf( header, "\t\telse // updating particular members in particular cases\n" );

		fprintf( header, "\t\t{\n" );
		fprintf( header, "\t\t\tswitch ( t.currentVariant() )\n" );
		fprintf( header, "\t\t\t{\n" );
		for ( auto& it: obj.getDiscriminatedUnionCases() )
		{
			fprintf( header, "\t\t\t\tcase %zd: // IDL CASE %s\n", it->numID, it->name.c_str() );
			fprintf( header, "\t\t\t\t{\n" );
			fprintf( header, "\t\t\t\t\tswitch ( addr[offset] )\n" );
			fprintf( header, "\t\t\t\t\t{\n" );
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_generateContinueParsingFunctionForPublishableStruct_MemberIterationBlock( header, root, cs, "\t\t\t\t\t\t", 1 );
			fprintf( header, "\t\t\t\t\t\tdefault:\n" );
			fprintf( header, "\t\t\t\t\t\t\tthrow std::exception(); // unexpected\n" );
			fprintf( header, "\t\t\t\t\t}\n" );
			fprintf( header, "\t\t\t\t\tbreak;\n" );
			fprintf( header, "\t\t\t\t}\n" );
		}
		fprintf( header, "\t\t\t\tdefault:\n" );
		fprintf( header, "\t\t\t\t\tthrow std::exception(); // unexpected\n" );
		fprintf( header, "\t\t\t}\n" );
		fprintf( header, "\t\t}\n" );
//		fprintf( header, "\t\t::globalmq::marshalling::impl::parsePublishableStructEnd( parser );\n" );
	}
	else
	{
		fprintf( header, "\t\tGMQ_ASSERT( addr.size() );\n" );
		fprintf( header, "\t\tswitch ( addr[offset] )\n" );
		fprintf( header, "\t\t{\n" );
		impl_generateContinueParsingFunctionForPublishableStruct_MemberIterationBlock( header, root, obj, "\t\t\t", 0 );
		fprintf( header, "\t\t\tdefault:\n" );
		fprintf( header, "\t\t\t\tthrow std::exception(); // unexpected\n" );
		fprintf( header, "\t\t}\n" );
	}

	fprintf( header, "\t\tif constexpr ( has_update_notifier )\n" );
	fprintf( header, "\t\t{\n" );
	fprintf( header, "\t\t\tif ( changed )\n" );
	fprintf( header, "\t\t\t\tt.notifyUpdated();\n" );
	fprintf( header, "\t\t}\n" );

	fprintf( header, "\t\tif constexpr ( reportChanges )\n" );
	fprintf( header, "\t\t\treturn changed;\n" );
	fprintf( header, "\t}\n" );
}

void impl_generateParseFunctionForPublishableStruct_MemberIterationBlock( FILE* header, Root& root, CompositeType& obj, std::string offset )
{
	assert( obj.type != CompositeType::Type::discriminated_union );

	for ( size_t i=0; i<obj.getMembers().size(); ++i )
	{
		assert( obj.getMembers()[i] != nullptr );
		auto& member = *(obj.getMembers()[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				impl_generateApplyUpdateForSimpleType( header, member, true, false, offset );
				fprintf( header, "\n" );
				break;
			}
			case  MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "%s::globalmq::marshalling::impl::parsePublishableStructBegin( parser, \"%s\" );\n", offset.c_str(), member.name.c_str() );

				impl_generateApplyUpdateForFurtherProcessingInStruct( header, member, false, true, false, offset );

				fprintf( header, "%s::globalmq::marshalling::impl::parsePublishableStructEnd( parser );\n", offset.c_str() );
				break;
			}
			case  MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				
				fprintf( header, "%s::globalmq::marshalling::impl::parsePublishableStructBegin( parser, \"%s\" );\n", offset.c_str(), member.name.c_str() );

				impl_generateApplyUpdateForFurtherProcessingInStruct( header, member, false, true, false, offset ); // TODO: revise DU: we may need something DU-spec here

				fprintf( header, "%s::globalmq::marshalling::impl::parsePublishableStructEnd( parser );\n", offset.c_str() );
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				fprintf( header, "%sif constexpr( reportChanges || has_update_notifier )\n", offset.c_str() );
				fprintf( header, "%s{\n", offset.c_str() );
				fprintf( header, "%s\t%s oldVectorVal;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
				fprintf( header, "%s\t::globalmq::marshalling::impl::copyVector<%s, %s>( t.%s, oldVectorVal );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s\t::globalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", offset.c_str(), member.name.c_str() );
				fprintf( header, "%s\tPublishableVectorProcessor::parse<ParserT, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s\tbool currentChanged = !::globalmq::marshalling::impl::isSameVector<%s, %s>( oldVectorVal, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s\tchanged = changed || currentChanged;\n", offset.c_str() );
				fprintf( header, "%s}\n", offset.c_str() );
				fprintf( header, "%selse\n", offset.c_str() );
				fprintf( header, "%s{\n", offset.c_str() );
				fprintf( header, "%s\t::globalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", offset.c_str(), member.name.c_str() );
				fprintf( header, "%s\tPublishableVectorProcessor::parse<ParserT, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s}\n", offset.c_str() );
				fprintf( header, "\n" );

				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				fprintf( header, "//// TODO: revise notifier list !!!!!!!!!!!!!!!!!!!!!!!!!\n" );
				fprintf( header, "%sif constexpr( reportChanges || has_update_notifier )\n", offset.c_str() );
				fprintf( header, "%s{\n", offset.c_str() );
				fprintf( header, "%s\t%s oldDictionaryVal;\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str() );
				fprintf( header, "%s\t::globalmq::marshalling::impl::copyDictionary<%s, %s, %s>( t.%s, oldDictionaryVal );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s\t::globalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", offset.c_str(), member.name.c_str() );
				fprintf( header, "%s\tPublishableDictionaryProcessor::parse<ParserT, %s, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s\tbool currentChanged = !::globalmq::marshalling::impl::isSameDictionary<%s, %s>( oldDictionaryVal, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s\tchanged = changed || currentChanged;\n", offset.c_str() );
				fprintf( header, "%s}\n", offset.c_str() );
				fprintf( header, "%selse\n", offset.c_str() );
				fprintf( header, "%s{\n", offset.c_str() );
				fprintf( header, "%s\t::globalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", offset.c_str(), member.name.c_str() );
				fprintf( header, "%s\tPublishableDictionaryProcessor::parse<ParserT, %s, %s, %s>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s}\n", offset.c_str() );
				fprintf( header, "\n" );

				break;
			}
			default:
				assert( false );
		}
	}
}

void impl_generateParseFunctionForPublishableStruct( FILE* header, Root& root, CompositeType& obj, const string& className )
{
	assert( obj.type == CompositeType::Type::structure || obj.type == CompositeType::Type::discriminated_union );

	fprintf( header, "\ttemplate<class ParserT, class T, class RetT>\n" );
	fprintf( header, "\tRetT %s::parse( ParserT& parser, T& t )\n", className.c_str() );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\tstatic_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );\n" );
	fprintf( header, "\t\tconstexpr bool reportChanges = std::is_same<RetT, bool>::value;\n" );
	fprintf( header, "\t\tbool changed = false;\n" );

	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, obj, "\t\t" );

	if ( obj.isDiscriminatedUnion() )
	{
		fprintf( header, "\t\tuint64_t caseId;\n" );
		fprintf( header, "\t\t::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &(caseId), \"caseId\" );\n" );
		fprintf( header, "\t\tt.initAs( (typename T::Variants)(caseId) );\n" );
		fprintf( header, "\t\tif ( caseId != T::Variants::unknown )\n" );
		fprintf( header, "\t\t{\n" );

		fprintf( header, "\t\t\t::globalmq::marshalling::impl::parsePublishableStructBegin( parser, \"caseData\" );\n" );
		fprintf( header, "\t\t\tswitch ( caseId )\n" );
		fprintf( header, "\t\t\t{\n" );

		for ( auto& it: obj.getDiscriminatedUnionCases() )
		{
			fprintf( header, "\t\t\t\tcase %zd: // IDL CASE %s\n", it->numID, it->name.c_str() );
			fprintf( header, "\t\t\t\t{\n" );
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_generateParseFunctionForPublishableStruct_MemberIterationBlock( header, root, cs, "\t\t\t\t\t" );
			fprintf( header, "\t\t\t\t\tbreak;\n" );
			fprintf( header, "\t\t\t\t}\n" );
		}

		fprintf( header, "\t\t\t\tdefault:\n" );
		fprintf( header, "\t\t\t\t\tthrow std::exception(); // unexpected\n" );
		fprintf( header, "\t\t\t}\n" );
		fprintf( header, "\t\t\t::globalmq::marshalling::impl::parsePublishableStructEnd( parser );\n" );

		fprintf( header, "\t\t}\n" );
	}
	else
	{
		impl_generateParseFunctionForPublishableStruct_MemberIterationBlock( header, root, obj, "\t\t" );
	}

	fprintf( header, "\n" );

	fprintf( header, "\t\tif constexpr ( has_update_notifier )\n" );
	fprintf( header, "\t\t{\n" );
	fprintf( header, "\t\t\tif ( changed )\n" );
	fprintf( header, "\t\t\t\tt.notifyUpdated();\n" );
	fprintf( header, "\t\t}\n" );

	fprintf( header, "\t\tif constexpr ( reportChanges )\n" );
	fprintf( header, "\t\t\treturn changed;\n" );
	fprintf( header, "\t}\n" );
}

void impl_generateParseFunctionForPublishableStructStateSync_MemberIterationBlock( FILE* header, Root& root, CompositeType& obj, std::string offset )
{
	assert( obj.type != CompositeType::Type::discriminated_union );

	for ( size_t i=0; i<obj.getMembers().size(); ++i )
	{
		assert( obj.getMembers()[i] != nullptr );
		auto& member = *(obj.getMembers()[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "%s::globalmq::marshalling::impl::%s<ParserT, %s>( parser, &(t.%s), \"%s\" );\n", offset.c_str(), paramTypeToParser( member.type.kind ), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str() );
				break;
			case  MessageParameterType::KIND::STRUCT:
			case  MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf( header, "%s::globalmq::marshalling::impl::parsePublishableStructBegin( parser, \"%s\" );\n", offset.c_str(), member.name.c_str() );
				fprintf( header, "%s%s::parseForStateSyncOrMessageInDepth( parser, t.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "%s::globalmq::marshalling::impl::parsePublishableStructEnd( parser );\n", offset.c_str() );
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf( header, "%s::globalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", offset.c_str(), member.name.c_str() );
				fprintf( header, "%sPublishableVectorProcessor::parse<ParserT, %s, %s, true>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			case MessageParameterType::KIND::DICTIONARY:
				fprintf( header, "%s::globalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", offset.c_str(), member.name.c_str() );
				fprintf( header, "%sPublishableDictionaryProcessor::parse<ParserT, %s, %s, %s, true>( parser, t.%s );\n", offset.c_str(), impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			default:
				assert( false );
		}
		fprintf( header, "\n" );
	}
}

void impl_generateParseFunctionBodyForPublishableStructStateSyncOrMessageInDepth( FILE* header, Root& root, CompositeType& obj )
{
	assert( obj.type == CompositeType::Type::message || obj.type == CompositeType::Type::structure || obj.type == CompositeType::Type::discriminated_union );

	if ( obj.isDiscriminatedUnion() )
	{
		fprintf( header, "\t\tuint64_t caseId;\n" );
		fprintf( header, "\t\t::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &(caseId), \"caseId\" );\n" );
		fprintf( header, "\t\tt.initAs( (typename T::Variants)(caseId) );\n" );
		fprintf( header, "\t\tif ( caseId != T::Variants::unknown )\n" );
		fprintf( header, "\t\t{\n" );

		fprintf( header, "\t\t\t::globalmq::marshalling::impl::parsePublishableStructBegin( parser, \"caseData\" );\n" );
		fprintf( header, "\t\t\tswitch ( caseId )\n" );
		fprintf( header, "\t\t\t{\n" );

		for ( auto& it: obj.getDiscriminatedUnionCases() )
		{
			fprintf( header, "\t\t\t\tcase %zd: // IDL CASE %s\n", it->numID, it->name.c_str() );
			fprintf( header, "\t\t\t\t{\n" );

			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_generateParseFunctionForPublishableStructStateSync_MemberIterationBlock( header, root, cs, "\t\t\t\t\t" );

			fprintf( header, "\t\t\t\t\tbreak;\n" );
			fprintf( header, "\t\t\t\t}\n" );
		}

		fprintf( header, "\t\t\t\tdefault:\n" );
		fprintf( header, "\t\t\t\t\tthrow std::exception(); // unexpected\n" );
		fprintf( header, "\t\t\t}\n" );
		fprintf( header, "\t\t\t::globalmq::marshalling::impl::parsePublishableStructEnd( parser );\n" );

		fprintf( header, "\t\t}\n" );
	}
	else
		impl_generateParseFunctionForPublishableStructStateSync_MemberIterationBlock( header, root, obj, "\t\t" );
}

void impl_generateParseFunctionForPublishableStructStateSyncOrMessageInDepth( FILE* header, Root& root, CompositeType& obj, const string& className )
{
	assert( obj.type == CompositeType::Type::structure || obj.type == CompositeType::Type::discriminated_union );
	fprintf( header, "\ttemplate<class ParserT, class T>\n" );
	fprintf( header, "\tvoid %s::parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )\n", className.c_str() );
	fprintf( header, "\t{\n" );

	impl_generateParseFunctionBodyForPublishableStructStateSyncOrMessageInDepth( header, root, obj );

	fprintf( header, "\t}\n" );
}

void impl_generateParseFunctionForPublishableState( FILE* header, Root& root, CompositeType& obj, bool addFullUpdateNotifierBlock )
{
	assert( obj.type == CompositeType::Type::publishable );
	fprintf( header, "\ttemplate<class ParserT>\n" );
	fprintf( header, "\tvoid parseStateSyncMessage( ParserT& parser )\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t::globalmq::marshalling::impl::parseStructBegin( parser );\n" );
	fprintf( header, "\n" );

	for ( size_t i=0; i<obj.getMembers().size(); ++i )
	{
		assert( obj.getMembers()[i] != nullptr );
		auto& member = *(obj.getMembers()[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				fprintf( header, "\t\t::globalmq::marshalling::impl::%s<ParserT, %s>( parser, &(t.%s), \"%s\" );\n", paramTypeToParser( member.type.kind ), impl_templateMemberTypeName( "T", member).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), member.name.c_str() );
				fprintf( header, "\n" );
				break;
			}
			case  MessageParameterType::KIND::DISCRIMINATED_UNION:
			case  MessageParameterType::KIND::STRUCT:
			{
				fprintf( header, "\t\t::globalmq::marshalling::impl::parsePublishableStructBegin( parser, \"%s\" );\n", member.name.c_str() );
				fprintf( header, "\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "\t\t::globalmq::marshalling::impl::parsePublishableStructEnd( parser );\n" );
				fprintf( header, "\n" );
				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				fprintf( header, "\t\t::globalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", member.name.c_str() );
				fprintf( header, "\t\tPublishableVectorProcessor::parse<ParserT, %s, %s>( parser, t.%s );\n", impl_templateMemberTypeName( "T", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "\n" );

				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				fprintf( header, "\t\t::globalmq::marshalling::impl::parseKey( parser, \"%s\" );\n", member.name.c_str() );
				fprintf( header, "\t\tPublishableDictionaryProcessor::parse<ParserT, %s, %s, %s>( parser, t.%s );\n", impl_templateMemberTypeName( "T", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				fprintf( header, "\n" );

				break;
			}
			default:
				assert( false );
		}
	}

	fprintf( header, "\t\t::globalmq::marshalling::impl::parseStructEnd( parser );\n" );
	if ( addFullUpdateNotifierBlock )
	{
		fprintf( header, "\n" );
		fprintf( header, "\t\tif constexpr ( has_full_update_notifier )\n" );
		fprintf( header, "\t\t\tt.notifyFullyUpdated();\n" );
	}
	fprintf( header, "\t}\n" );
}

void impl_generatePublishableStructForwardDeclaration( FILE* header, Root& root, CompositeType& obj )
{
	if ( obj.isDiscriminatedUnion() )
		fprintf( header, "class %s;\n", impl_generatePublishableStructName( obj ).c_str() );
	else
		fprintf( header, "struct %s;\n", impl_generatePublishableStructName( obj ).c_str() );
}

void impl_GeneratePublishableStructCopyFn_MemberIterationBlock( FILE* header, Root& root, CompositeType& s, std::string offset )
{
	assert( s.type != CompositeType::Type::discriminated_union );

	for ( size_t i=0; i<s.getMembers().size(); ++i )
	{
		assert( s.getMembers()[i] != nullptr );
		auto& member = *(s.getMembers()[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "%sdst.%s = src.%s;\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf( header, "%s%s::copy( src.%s, dst.%s );\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				fprintf( header, "%s::globalmq::marshalling::impl::copyVector<%s, %s>( src.%s, dst.%s );\n", offset.c_str(), impl_templateMemberTypeName( "UserT", member).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				fprintf( header, "%s::globalmq::marshalling::impl::copyDictionary<%s, %s, %s>( src.%s, dst.%s );\n", offset.c_str(), impl_templateMemberTypeName( "UserT", member).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			}
			default:
				assert( false ); // TODO: revise or add cases
		}
	}
}

void impl_GeneratePublishableStructCopyFn( FILE* header, Root& root, CompositeType& s, const string& className )
{
	fprintf( header, "\ttemplate<typename UserT>\n" );
	fprintf( header, "\tvoid %s::copy(const UserT& src, UserT& dst)\n", className.c_str() );
	fprintf( header, "\t{\n" );

	if ( s.isDiscriminatedUnion() )
	{
		fprintf( header, "\t\tauto srcCaseId = src.currentVariant();\n" );
		fprintf( header, "\t\tdst.initAs( srcCaseId );\n" );

		fprintf( header, "\t\tif ( srcCaseId != UserT::Variants::unknown )\n" );
		fprintf( header, "\t\t\tswitch ( srcCaseId )\n" );
		fprintf( header, "\t\t\t{\n" );

		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			fprintf( header, "\t\t\t\tcase %zd: // IDL CASE %s\n", it->numID, it->name.c_str() );
			fprintf( header, "\t\t\t\t{\n" );

			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_GeneratePublishableStructCopyFn_MemberIterationBlock( header, root, cs, "\t\t\t\t\t" );

			fprintf( header, "\t\t\t\t\tbreak;\n" );
			fprintf( header, "\t\t\t\t}\n" );
		}

		fprintf( header, "\t\t\t\tdefault:\n" );
		fprintf( header, "\t\t\t\t\tthrow std::exception(); // unexpected\n" );
		fprintf( header, "\t\t\t}\n" );
	}
	else
		impl_GeneratePublishableStructCopyFn_MemberIterationBlock( header, root, s, "\t\t" );

	fprintf( header, "\t}\n" );
}

void impl_GeneratePublishableStructIsSameFn_MemberIterationBlock( FILE* header, Root& root, CompositeType& s, std::string offset )
{
	assert( s.type != CompositeType::Type::discriminated_union );

	for ( size_t i=0; i<s.getMembers().size(); ++i )
	{
		assert( s.getMembers()[i] != nullptr );
		auto& member = *(s.getMembers()[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "%sif ( s1.%s != s2.%s ) return false;\n", offset.c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf( header, "%sif( ! %s::isSame( s1.%s, s2.%s ) ) return false;\n", offset.c_str(), impl_generatePublishableStructName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				fprintf( header, "%sif ( !::globalmq::marshalling::impl::isSameVector<%s, %s>( s1.%s, s2.%s ) ) return false;\n", offset.c_str(), impl_templateMemberTypeName( "UserT", member ).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				fprintf( header, "%sif ( !::globalmq::marshalling::impl::isSameDictionary<%s, %s>( s1.%s, s2.%s ) ) return false;\n", offset.c_str(), impl_templateMemberTypeName( "UserT", member ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( member.type, root ).c_str(), impl_memberOrAccessFunctionName( member ).c_str(), impl_memberOrAccessFunctionName( member ).c_str() );
				break;
			}
			default:
				assert( false ); // TODO: revise or add cases
		}
	}
}

void impl_GeneratePublishableStructIsSameFn( FILE* header, Root& root, CompositeType& s, const string& className )
{
	fprintf( header, "\ttemplate<typename UserT>\n" );
	fprintf( header, "\tbool %s::isSame(const UserT& s1, const UserT& s2)\n", className.c_str() );
	fprintf( header, "\t{\n" );

	if ( s.isDiscriminatedUnion() )
	{
		fprintf( header, "\t\tif ( s1.currentVariant() != s2.currentVariant() )\n" );
		fprintf( header, "\t\t\treturn false;\n" );

		fprintf( header, "\t\tif ( s1.currentVariant() != UserT::Variants::unknown )\n" );
		fprintf( header, "\t\t\tswitch ( s1.currentVariant() )\n" );
		fprintf( header, "\t\t\t{\n" );

		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			fprintf( header, "\t\t\t\tcase %zd: // IDL CASE %s\n", it->numID, it->name.c_str() );
			fprintf( header, "\t\t\t\t{\n" );

			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_GeneratePublishableStructIsSameFn_MemberIterationBlock( header, root, cs, "\t\t\t\t\t" );

			fprintf( header, "\t\t\t\t\tbreak;\n" );
			fprintf( header, "\t\t\t\t}\n" );
		}

		fprintf( header, "\t\t\t\tdefault:\n" );
		fprintf( header, "\t\t\t\t\tthrow std::exception(); // unexpected\n" );
		fprintf( header, "\t\t\t}\n" );
	}
	else
		impl_GeneratePublishableStructIsSameFn_MemberIterationBlock( header, root, s, "\t\t" );

	fprintf( header, "\t\treturn true;\n" );
	fprintf( header, "\t}\n" );
}

void impl_generatePublishableStructDeclaration( FILE* header, Root& root, CompositeType& obj )
{
	assert( obj.type == CompositeType::Type::structure || obj.type == CompositeType::Type::discriminated_union );

	fprintf( header, "struct %s : public ::globalmq::marshalling::impl::StructType\n", impl_generatePublishableStructName( obj ).c_str() );
	fprintf( header, "{\n" );

	// impl_generateParseFunctionForPublishableStructStateSyncOrMessageInDepth( header, root, obj );
	fprintf( header, "\ttemplate<class ParserT, class T>\n" );
	fprintf( header, "\tstatic\n" );
	fprintf( header, "\tvoid parseForStateSyncOrMessageInDepth( ParserT& parser, T& t );\n" );

	if ( obj.isStruct4Publishing )
	{
		fprintf( header, "\n" );
		// impl_generateComposeFunctionForPublishableStruct( header, root, obj );
		fprintf( header, "\ttemplate<class ComposerT, class T>\n" );
		fprintf( header, "\tstatic\n" );
		fprintf( header, "\tvoid compose( ComposerT& composer, const T& t );\n" );
		fprintf( header, "\n" );

		// impl_generateParseFunctionForPublishableStruct( header, root, obj );
		fprintf( header, "\ttemplate<class ParserT, class T, class RetT = void>\n" );
		fprintf( header, "\tstatic\n" );
		fprintf( header, "\tRetT parse( ParserT& parser, T& t );\n" );
		fprintf( header, "\n" );

		// impl_generateContinueParsingFunctionForPublishableStruct( header, root, obj );
		fprintf( header, "\ttemplate<class ParserT, class T, class RetT = void>\n" );
		fprintf( header, "\tstatic\n" );
		fprintf( header, "\tRetT parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset );\n" );
		fprintf( header, "\n" );

		// impl_GeneratePublishableStructCopyFn( header, root, obj );
		fprintf( header, "\ttemplate<typename UserT>\n" );
		fprintf( header, "\tstatic void copy(const UserT& src, UserT& dst);\n" );
		fprintf( header, "\n" );

		// impl_GeneratePublishableStructIsSameFn( header, root, obj );
		fprintf( header, "\ttemplate<typename UserT>\n" );
		fprintf( header, "\tstatic bool isSame(const UserT& s1, const UserT& s2);\n" );
	}

	fprintf( header, "};\n\n" );
}

void impl_generatePublishableStruct( FILE* header, Root& root, CompositeType& obj )
{
	assert( obj.type == CompositeType::Type::structure || obj.type == CompositeType::Type::discriminated_union );

	string className = impl_generatePublishableStructName( obj );

	impl_generateParseFunctionForPublishableStructStateSyncOrMessageInDepth( header, root, obj, className );
	fprintf( header, "\n" );

	if ( obj.isStruct4Publishing )
	{
		impl_generateComposeFunctionForPublishableStruct( header, root, obj, className );
		fprintf( header, "\n" );
		impl_generateParseFunctionForPublishableStruct( header, root, obj, className );
		fprintf( header, "\n" );
		impl_generateContinueParsingFunctionForPublishableStruct( header, root, obj, className );
		fprintf( header, "\n" );
		impl_GeneratePublishableStructCopyFn( header, root, obj, className );
		fprintf( header, "\n" );
		impl_GeneratePublishableStructIsSameFn( header, root, obj, className );
	}
}

void impl_GeneratePublishableStateMemberSetter( FILE* header, Root& root, bool forRoot, MessageParameter& param, size_t idx )
{
//	assert( (forRoot && rootName != nullptr) || (forRoot == false && rootName == nullptr) );
	const char* composer = forRoot ? "composer" : "root.getComposer()";
	const char* composerType = forRoot ? "ComposerT" : "decltype(root.getComposer())";
	const char* addrVector = forRoot ? "GMQ_COLL vector<size_t>()" : "address";

	fprintf( header, "\tvoid set_%s( %s val) { \n", param.name.c_str(), impl_templateMemberTypeName( "T", param ).c_str() );
	fprintf( header, "\t\tt.%s = val; \n", impl_memberOrAccessFunctionName( param ).c_str() );

	fprintf( header, "\t\t::globalmq::marshalling::impl::composeAddressInPublishable( %s, %s, %zd );\n", composer, addrVector, idx );

	switch ( param.type.kind )
	{
		case MessageParameterType::KIND::INTEGER:
			fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeInteger( %s, t.%s );\n", composer, impl_memberOrAccessFunctionName( param ).c_str() );
			break;
		case MessageParameterType::KIND::UINTEGER:
			fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeUnsignedInteger( %s, t.%s );\n", composer, impl_memberOrAccessFunctionName( param ).c_str() );
			break;
		case MessageParameterType::KIND::REAL:
			fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeReal( %s, t.%s );\n", composer, impl_memberOrAccessFunctionName( param ).c_str() );
			break;
		case MessageParameterType::KIND::CHARACTER_STRING:
			fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeString( %s, t.%s );\n", composer, impl_memberOrAccessFunctionName( param ).c_str() );
			break;
		case MessageParameterType::KIND::VECTOR:
		{
			fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( %s );\n", composer );
			switch ( param.type.vectorElemKind )
			{
				case MessageParameterType::KIND::INTEGER:
					fprintf( header, "\t\tPublishableVectorProcessor::compose<%s, %s, ::globalmq::marshalling::impl::SignedIntegralType>( %s, t.%s );\n", composerType, impl_templateMemberTypeName( "T", param).c_str(), composer, impl_memberOrAccessFunctionName( param ).c_str() );
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf( header, "\t\tPublishableVectorProcessor::compose<%s, %s, ::globalmq::marshalling::impl::UnsignedIntegralType>( %s, t.%s );\n", composerType, impl_templateMemberTypeName( "T", param).c_str(), composer, impl_memberOrAccessFunctionName( param ).c_str() );
					break;
				case MessageParameterType::KIND::REAL:
					fprintf( header, "\t\tPublishableVectorProcessor::compose<%s, %s, ::globalmq::marshalling::impl::RealType>( %s, t.%s );\n", composerType, impl_templateMemberTypeName( "T", param).c_str(), composer, impl_memberOrAccessFunctionName( param ).c_str() );
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf( header, "\t\tPublishableVectorProcessor::compose<%s, %s, ::globalmq::marshalling::impl::StringType>( %s, t.%s );\n", composerType, impl_templateMemberTypeName( "T", param).c_str(), composer, impl_memberOrAccessFunctionName( param ).c_str() );
					break;
				case MessageParameterType::KIND::VECTOR:
				case MessageParameterType::KIND::DICTIONARY:
					assert( false ); // unexpected
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					assert( param.type.structIdx < root.structs.size() );
//					fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( %s );\n", composer );
//					fprintf( header, "\t\t%s::compose( %s, t.%s );\n", impl_generatePublishableStructName( *(root.structs[param.type.structIdx]) ).c_str(), composer, impl_memberOrAccessFunctionName( param ).c_str() );
//					fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( %s );\n", composer );
					fprintf( header, "\t\tPublishableVectorProcessor::compose<%s, %s, %s>( %s, t.%s );\n", composerType, impl_templateMemberTypeName( "T", param).c_str(), vectorElementTypeToLibTypeOrTypeProcessor( param.type, root ).c_str(), composer, impl_memberOrAccessFunctionName( param ).c_str() );
					break;
				default:
					assert( false ); // not implemented (yet)
			}
			fprintf( header, "\t\t::globalmq::marshalling::impl::composeStateUpdateBlockEnd( %s );\n", composer );
			break;
		}
		case MessageParameterType::KIND::DICTIONARY:
		{
			fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( %s );\n", composer );
			switch ( param.type.dictionaryValueKind )
			{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					fprintf( header, "\t\tPublishableDictionaryProcessor::compose<%s, %s, %s, %s>( %s, t.%s );\n", composerType, impl_templateMemberTypeName( "T", param).c_str(), dictionaryKeyTypeToLibTypeOrTypeProcessor( param.type, root ).c_str(), dictionaryValueTypeToLibTypeOrTypeProcessor( param.type, root ).c_str(), composer, impl_memberOrAccessFunctionName( param ).c_str() );
					break;
				default:
					assert( false ); // not implemented (yet)
			}
			fprintf( header, "\t\t::globalmq::marshalling::impl::composeStateUpdateBlockEnd( %s );\n", composer );
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( %s );\n", composer );
			fprintf( header, "\t\t%s::compose( %s, t.%s );\n", impl_generatePublishableStructName( param ).c_str(), composer, impl_memberOrAccessFunctionName( param ).c_str() );
			fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( %s );\n", composer );
			break;
		}
		default:
			assert( false ); // not implemented (yet)
	}
	
	fprintf( header, "\t}\n" );
}

void impl_GeneratePublishableStateMemberAccessors_MemberIterationBlock( FILE* header, Root& root, CompositeType& s, bool forRoot, const char* rootName, bool allowSeters, size_t idxBase )
{
	assert( s.type != CompositeType::Type::discriminated_union );
	for ( size_t i=0; i<s.getMembers().size(); ++i )
	{
		auto& it = s.getMembers()[i];
		assert( it != nullptr );
		impl_GeneratePublishableStateMemberGetter( header, root, s, *it );
		if ( allowSeters )
		{
			impl_GeneratePublishableStateMemberSetter( header, root, forRoot, *it, idxBase + i );
			impl_GeneratePublishableStateMemberGetter4Set( header, root, rootName, *it, idxBase + i );
		}
	}
}

void impl_GeneratePublishableStateMemberAccessors( FILE* header, Root& root, CompositeType& s, bool allowSeters )
{
	assert( s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure || s.isDiscriminatedUnion() );
	bool forRoot = s.type == CompositeType::Type::publishable;
	const char* rootName = forRoot ? s.name.c_str() : nullptr;
//	const char* rootName = forRoot ? s.name.c_str() : "RootT";
	if ( s.isDiscriminatedUnion() )
	{
		fprintf( header, "\tauto get_currentVariant() { return t.currentVariant(); }\n" );
		if ( allowSeters )
		{
			const char* composer = forRoot ? "composer" : "root.getComposer()";
			const char* composerType = forRoot ? "ComposerT" : "decltype(root.getComposer())";
			const char* addrVector = forRoot ? "GMQ_COLL vector<size_t>()" : "address";
			fprintf( header, "\tvoid set_currentVariant( typename T::Variants v ) { \n" );
			fprintf( header, "\t\tt.initAs( v ); \n" );
			fprintf( header, "\t\t::globalmq::marshalling::impl::composeAddressInPublishable( %s, %s, %d );\n", composer, addrVector, 0 );
			fprintf( header, "\t\t::globalmq::marshalling::impl::publishableComposeLeafeUnsignedInteger( %s, (uint64_t)(t.currentVariant()) );\n", composer );
			fprintf( header, "\t}\n" );
		}

		for ( auto& it: s.getDiscriminatedUnionCases() )
		{
			assert( it != nullptr );
			CompositeType& cs = *it;
			assert( cs.type == CompositeType::Type::discriminated_union_case );
			impl_GeneratePublishableStateMemberAccessors_MemberIterationBlock( header, root, cs,  forRoot, rootName,  allowSeters, 1 );
		}
	}
	else
		impl_GeneratePublishableStateMemberAccessors_MemberIterationBlock( header, root, s,  forRoot, rootName,  allowSeters, 0 );
}

void impl_GenerateApplyUpdateMessageMemberFn( FILE* header, Root& root, CompositeType& s, bool addNotifications )
{
	fprintf( header, "\ttemplate<typename ParserT>\n" );
	fprintf( header, "\tvoid applyMessageWithUpdates(ParserT& parser)\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t::globalmq::marshalling::impl::parseStateUpdateMessageBegin( parser );\n" );
	fprintf( header, "\t\tbool changed = false;\n" );
	fprintf( header, "\t\tGMQ_COLL vector<size_t> addr;\n" );
	fprintf( header, "\t\twhile( ::globalmq::marshalling::impl::parseAddressInPublishable<ParserT, GMQ_COLL vector<size_t>>( parser, addr ) )\n" );
	fprintf( header, "\t\t{\n" );
	fprintf( header, "\t\t\tGMQ_ASSERT( addr.size() );\n" );
	fprintf( header, "\t\t\tswitch ( addr[0] )\n" );
	fprintf( header, "\t\t\t{\n" );

	assert( s.type != CompositeType::Type::discriminated_union ); // not implemented

	for ( size_t i=0; i<s.getMembers().size(); ++i )
	{
		fprintf( header, "\t\t\t\tcase %zd:\n", i );
		fprintf( header, "\t\t\t\t{\n" );

		assert( s.getMembers()[i] != nullptr );
		auto& member = *(s.getMembers()[i]);
		switch ( member.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				fprintf( header, "\t\t\t\t\tif ( addr.size() > 1 )\n" );
				fprintf( header, "\t\t\t\t\t\tthrow std::exception(); // bad format, TODO: ...\n" );
				if ( addNotifications )
//					impl_generateApplyUpdateForSimpleType( header, member, false, true, "\t\t\t\t\t" );
					impl_generateApplyUpdateForSimpleType( header, member, true, true, "\t\t\t\t\t" );
				else
					impl_generateApplyUpdateForSimpleTypeNoNotifiers( header, member, true, "\t\t\t\t\t" );
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				fprintf( header, "\t\t\t\t\tif ( addr.size() == 1 ) // we have to parse and apply changes of this child\n" );
				fprintf( header, "\t\t\t\t\t{\n" );
				fprintf( header, "\t\t\t\t\t\t::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );\n" );
				fprintf( header, "\n" );

				if ( addNotifications )
//					impl_generateApplyUpdateForStructItself( header, member, false, "\t\t\t\t\t\t" );
					impl_generateApplyUpdateForStructItself( header, member, true, "\t\t\t\t\t\t" );
				else
					impl_generateApplyUpdateForStructItselfNoNotifiers( header, member, "\t\t\t\t\t\t" );

				fprintf( header, "\n" );
				fprintf( header, "\t\t\t\t\t\t::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );\n" );
				fprintf( header, "\t\t\t\t\t}\n" );
				fprintf( header, "\t\t\t\t\telse // let child continue parsing\n" );
				fprintf( header, "\t\t\t\t\t{\n" );

				if ( addNotifications )
					impl_generateApplyUpdateForFurtherProcessingInStruct( header, member, false, true, true, "\t\t\t\t\t\t" );
				else
					impl_generateApplyUpdateForFurtherProcessingInStructNoNotifiers( header, member, false, true, "\t\t\t\t\t\t" );

				fprintf( header, "\t\t\t\t\t}\n" );

				break;
			}
			case MessageParameterType::KIND::VECTOR:
			{
				if ( addNotifications )
					impl_generateApplyUpdateForFurtherProcessingInVector( header, root, member, false, "\t\t\t\t\t" );
				else
					impl_generateApplyUpdateForFurtherProcessingInVectorNoNotifiers( header, root, member, false, "\t\t\t\t\t" );

				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				if ( addNotifications )
					impl_generateApplyUpdateForFurtherProcessingInDictionary( header, root, member, false, false, "\t\t\t\t\t" );
				else
					impl_generateApplyUpdateForFurtherProcessingInDictionaryNoNotifiers( header, root, member, false, "\t\t\t\t\t" );

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

	if ( addNotifications )
	{
		fprintf( header, "\t\tif constexpr ( has_update_notifier )\n" );
		fprintf( header, "\t\t{\n" );
		fprintf( header, "\t\t\tif ( changed )\n" );
		fprintf( header, "\t\t\t\tt.notifyUpdated();\n" );
		fprintf( header, "\t\t}\n" );
	}

	fprintf( header, "\t}\n" );
}

void impl_GeneratePublishableStateWrapperForPublisher( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, "template<class T, class ComposerT>\n" );
	fprintf( header, "class %s_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>\n", s.name.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tT t;\n" );
	fprintf( header, "\tusing BufferT = typename ComposerT::BufferType;\n" );
	fprintf( header, "\tBufferT buffer;\n" );
	fprintf( header, "\tComposerT composer;\n" );

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );
//	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, s, "\t" );

	fprintf( header, "\npublic:\n" );
	fprintf( header, "\tstatic constexpr uint64_t numTypeID = %lld;\n", s.numID );
	fprintf( header, "\tstatic constexpr const char* stringTypeID = \"%s\";\n", s.name.c_str() );
	fprintf( header, "\n" );
	fprintf( header, "\ttemplate<class ... ArgsT>\n" );
	fprintf( header, "\t%s_WrapperForPublisher( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ), composer( buffer ) {}\n", s.name.c_str() );
	fprintf( header, "\tconst T& getState() { return t; }\n" );
	fprintf( header, "\tComposerT& getComposer() { return composer; }\n" );
	fprintf( header, "\tvoid startTick( BufferT&& buff ) { buffer = std::move( buff ); composer.reset(); ::globalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( composer );}\n" );
	fprintf( header, "\tBufferT&& endTick() { ::globalmq::marshalling::impl::composeStateUpdateMessageEnd( composer ); return std::move( buffer ); }\n" );
	fprintf( header, "\tconst char* name() { return stringTypeID; }\n" );
	fprintf( header, "\tconst char* publishableName() { return stringTypeID; }\n" );
	fprintf( header, "\tvirtual uint64_t stateTypeID() { return numTypeID; }\n" );

	impl_GeneratePublishableStateMemberAccessors( header, root, s, true );
	fprintf( header, "\n" );
	impl_generateComposeFunctionForPublishableStruct( header, root, s, "" );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStatePlatformWrapperForPublisher( FILE* header, Root& root, CompositeType& s, std::string platformPrefix, std::string classNotifierName )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, "template<class T, class RegistrarT>\n" );
	fprintf( header, "class %s_%sWrapperForPublisher : public %s_WrapperForPublisher<T, typename %s::ComposerT>\n", s.name.c_str(), platformPrefix.c_str(), s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tusing ComposerT = typename %s::ComposerT;\n", classNotifierName.c_str() );
	fprintf( header, "\tRegistrarT& registrar;\n" );
	fprintf( header, "public:\n" );
	fprintf( header, "\tusing BufferT = typename %s::ComposerT::BufferType;\n", classNotifierName.c_str() );
	fprintf( header, "\ttemplate<class ... ArgsT>\n" );
	fprintf( header, "\t%s_%sWrapperForPublisher( RegistrarT& registrar_, ArgsT&& ... args ) : %s_WrapperForPublisher<T, typename %s::ComposerT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )\n", s.name.c_str(), platformPrefix.c_str(), s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t{ \n" );
	fprintf( header, "\t\tregistrar.add( this );\n" );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual ~%s_%sWrapperForPublisher()\n", s.name.c_str(), platformPrefix.c_str() );
	fprintf( header, "\t{ \n" );
	fprintf( header, "\t\tregistrar.remove( this );\n" );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );

	fprintf( header, "\tvirtual void startTick( BufferT&& buff ) { %s_WrapperForPublisher<T, ComposerT>::startTick( std::move( buff ) ); }\n", s.name.c_str() );
	fprintf( header, "\tvirtual BufferT&& endTick() { return  %s_WrapperForPublisher<T, ComposerT>::endTick(); }\n", s.name.c_str() );
	fprintf( header, "\tvirtual void generateStateSyncMessage(ComposerT& composer) { %s_WrapperForPublisher<T, ComposerT>::compose(composer); }\n", s.name.c_str() );
	fprintf( header, "\tvirtual const char* name() { return %s_WrapperForPublisher<T, ComposerT>::name(); }\n", s.name.c_str() );
	fprintf( header, "\tvirtual const char* publishableName() { return %s_WrapperForPublisher<T, ComposerT>::publishableName(); }\n", s.name.c_str() );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStateWrapperForSubscriber( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, "template<class T, class BufferT>\n" );
	fprintf( header, "class %s_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>\n", s.name.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tT t;\n" );

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );
	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, s, "\t" );

	fprintf( header, "\npublic:\n" );
	fprintf( header, "\tstatic constexpr uint64_t numTypeID = %lld;\n", s.numID );
	fprintf( header, "\tstatic constexpr const char* stringTypeID = \"%s\";\n", s.name.c_str() );
	fprintf( header, "\n" );
	fprintf( header, "\tstatic constexpr bool reportChanges = false;\n" );
	fprintf( header, "\tbool changed = false;\n" );
	fprintf( header, "\n" );
	fprintf( header, "\ttemplate<class ... ArgsT>\n" );
	fprintf( header, "\t%s_WrapperForSubscriber( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ) {}\n", s.name.c_str() );
	fprintf( header, "\tconst T& getState() { return t; }\n" );
	fprintf( header, "\tvirtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }\n" );
	fprintf( header, "\tvirtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }\n" );
	fprintf( header, "\tvirtual const char* name() { return stringTypeID; }\n" );
	fprintf( header, "\tvirtual const char* publishableName() { return stringTypeID; }\n" );
	fprintf( header, "\tvirtual uint64_t stateTypeID() { return numTypeID; }\n" );
	fprintf( header, "\n" );

	impl_GenerateApplyUpdateMessageMemberFn( header, root, s, true );
	fprintf( header, "\n" );
	fprintf( header, "\n" );
	impl_generateParseFunctionForPublishableState( header, root, s, true );

	impl_GeneratePublishableStateMemberAccessors( header, root, s, false );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStatePlatformWrapperForSubscriber( FILE* header, Root& root, CompositeType& s, std::string platformPrefix, std::string classNotifierName )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, "template<class T, class RegistrarT>\n" );
	fprintf( header, "class %s_%sWrapperForSubscriber : public %s_WrapperForSubscriber<T, typename %s::BufferT>\n", s.name.c_str(), platformPrefix.c_str(), s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tRegistrarT& registrar;\n" );
	fprintf( header, "public:\n" );
	fprintf( header, "\ttemplate<class ... ArgsT>\n" );
	fprintf( header, "\t%s_%sWrapperForSubscriber( RegistrarT& registrar_, ArgsT&& ... args ) : %s_WrapperForSubscriber<T, typename %s::BufferT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )\n", s.name.c_str(), platformPrefix.c_str(), s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t{ \n" );
	fprintf( header, "\t\tregistrar.add( this );\n" );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual ~%s_%sWrapperForSubscriber()\n", s.name.c_str(), platformPrefix.c_str() );
	fprintf( header, "\t{ \n" );
	fprintf( header, "\t\tregistrar.remove( this );\n" );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename %s::BufferT>& parser ) \n", classNotifierName.c_str() );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t%s_WrapperForSubscriber<T, typename %s::BufferT>::applyMessageWithUpdates(parser);\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename %s::BufferT>& parser )\n", classNotifierName.c_str() );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t%s_WrapperForSubscriber<T, typename %s::BufferT>::applyMessageWithUpdates(parser);\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<typename %s::BufferT>& parser ) \n", classNotifierName.c_str() );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t%s_WrapperForSubscriber<T, typename %s::BufferT>::parseStateSyncMessage(parser);\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "\tvirtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<typename %s::BufferT>& parser )\n", classNotifierName.c_str() );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\t%s_WrapperForSubscriber<T, typename %s::BufferT>::parseStateSyncMessage(parser);\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\tvirtual const char* name()\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\treturn %s_WrapperForSubscriber<T, typename %s::BufferT>::name();\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\tvirtual const char* publishableName()\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\treturn %s_WrapperForSubscriber<T, typename %s::BufferT>::publishableName();\n", s.name.c_str(), classNotifierName.c_str() );
	fprintf( header, "\t}\n" );
	fprintf( header, "\tvoid subscribe(GMQ_COLL string path)\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\tregistrar.subscribe( this, path );\n" );
	fprintf( header, "\t}\n" );
	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStateWrapperForConcentrator( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable );

	fprintf( header, "template<class T, class InputBufferT, class ComposerT>\n" );
	fprintf( header, "class %s_WrapperForConcentrator : public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>\n", s.name.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tT t;\n" );
	fprintf( header, "\tusing BufferT = typename ComposerT::BufferType;\n" );

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );

	fprintf( header, "\npublic:\n" );
	fprintf( header, "\tstatic constexpr uint64_t numTypeID = %lld;\n", s.numID );
	fprintf( header, "\n" );
	fprintf( header, "\t%s_WrapperForConcentrator() {}\n", s.name.c_str() );
	fprintf( header, "\tconst char* name() {return \"%s\";}\n", s.name.c_str() );
	fprintf( header, "\tconst char* publishableName() {return \"%s\";}\n", s.name.c_str() );
	fprintf( header, "\t\n" );

	fprintf( header, "\t// Acting as publisher\n" );
	fprintf( header, "\tvirtual void generateStateSyncMessage( ComposerT& composer ) { compose(composer); }\n" );

	impl_generateComposeFunctionForPublishableStruct( header, root, s, "" );
	fprintf( header, "\n" );

	fprintf( header, "\t// Acting as subscriber\n" );
	fprintf( header, "\tvirtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }\n" );
	fprintf( header, "\tvirtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }\n" );
	fprintf( header, "\tvirtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferT>& parser ) { parseStateSyncMessage(parser); }\n" );
	fprintf( header, "\tvirtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser ) { parseStateSyncMessage(parser); }\n" );
	fprintf( header, "\n" );

	impl_GenerateApplyUpdateMessageMemberFn( header, root, s, false );
	fprintf( header, "\n" );
	impl_generateParseFunctionForPublishableState( header, root, s, false );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStructWrapperForwardDeclaration( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union );

	fprintf( header, "template<class T> class %s_RefWrapper;\n", s.name.c_str() );
}

void impl_GeneratePublishableStructWrapper( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union );

	fprintf( header, "template<class T>\n" );
	fprintf( header, "class %s_RefWrapper\n", s.name.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tT& t;\n" );

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );
//	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, s, "\t" );

	fprintf( header, "\npublic:\n" );
	fprintf( header, "\t%s_RefWrapper( T& actual ) : t( actual ) {}\n", s.name.c_str() );

	impl_GeneratePublishableStateMemberAccessors( header, root, s, false );

	fprintf( header, "};\n\n" );
}

void impl_GeneratePublishableStructWrapper4SetForwardDeclaration( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union );

	fprintf( header, "template<class T, class RootT> class %s_RefWrapper4Set;\n", s.name.c_str() );
}

void impl_GeneratePublishableStructWrapper4Set( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union );

	fprintf( header, "template<class T, class RootT>\n" );
	fprintf( header, "class %s_RefWrapper4Set\n", s.name.c_str() );
	fprintf( header, "{\n" );
	fprintf( header, "\tT& t;\n" );
	fprintf( header, "\tRootT& root;\n" );
	fprintf( header, "\tGMQ_COLL vector<size_t> address;\n" );

	impl_GeneratePublishableStateMemberPresenceCheckingBlock( header, root, s );

	fprintf( header, "\npublic:\n" );
	fprintf( header, "\t%s_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {\n", s.name.c_str() );
	fprintf( header, "\t\taddress = address_;\n" );
	fprintf( header, "\t\taddress.push_back (idx );\n" );
	fprintf( header, "\t}\n" );

	impl_GeneratePublishableStateMemberAccessors( header, root, s, true );

	fprintf( header, "};\n\n" );
}


void collectMemberNamesFromPublishableObjects( vector<unique_ptr<CompositeType>> &structs, set<string>& names )
{
	for ( auto& s : structs )
	{
		assert( s != nullptr );
		assert( s->type == CompositeType::structure || s->type == CompositeType::Type::discriminated_union || s->type == CompositeType::publishable );
		if ( s->type == CompositeType::publishable || ( s->type == CompositeType::structure && s->isStruct4Publishing ) )
			for ( auto& member : s->getMembers() )
			{
				assert( member != nullptr );
				names.insert( member->name );
			}
		else if ( s->type == CompositeType::discriminated_union && s->isStruct4Publishing )
			for ( auto& cs : s->getDiscriminatedUnionCases() )
				for ( auto& member : cs->getMembers() )
				{
					assert( member != nullptr );
					names.insert( member->name );
				}
	}
}

void collectVectorAndDictionaryMemberNamesFromPublishableObjects( vector<unique_ptr<CompositeType>> &structs, set<string>& vectorNames, set<string>& dictionaryNames )
{
	for ( auto& s : structs )
	{
		assert( s != nullptr );
		assert( s->type == CompositeType::structure || s->type == CompositeType::discriminated_union || s->type == CompositeType::publishable );
		if ( s->type == CompositeType::publishable || ( s->type == CompositeType::structure && s->isStruct4Publishing ) )
			for ( auto& member : s->getMembers() )
			{
				assert( member != nullptr );
				if ( member->type.kind == MessageParameterType::KIND::VECTOR )
					vectorNames.insert( member->name );
				else if ( member->type.kind == MessageParameterType::KIND::DICTIONARY )
					dictionaryNames.insert( member->name );
			}
		else if ( s->type == CompositeType::discriminated_union && s->isStruct4Publishing )
			for ( auto& cs : s->getDiscriminatedUnionCases() )
				for ( auto& member : cs->getMembers() )
				{
					assert( member != nullptr );
					if ( member->type.kind == MessageParameterType::KIND::VECTOR )
						vectorNames.insert( member->name );
					else if ( member->type.kind == MessageParameterType::KIND::DICTIONARY )
						dictionaryNames.insert( member->name );
				}
	}
}

void generateNotifierPresenceTesterBlock( FILE* header, Root& root )
{
	set<string> names;
	collectMemberNamesFromPublishableObjects( root.publishables, names );
	collectMemberNamesFromPublishableObjects( root.structs, names );
	names.insert( "currentVariant" ); // TODO: if there are DUs...
	set<string> namesOfVectors;
	set<string> namesOfDictionaries;
	collectVectorAndDictionaryMemberNamesFromPublishableObjects( root.publishables, namesOfVectors, namesOfDictionaries );
	collectVectorAndDictionaryMemberNamesFromPublishableObjects( root.structs, namesOfVectors, namesOfDictionaries );

	fprintf( header, "// member update notifier presence checks\n" );
	fprintf( header, "using index_type_for_array_notifiers = size_t&;\n" );
	fprintf( header, "template<typename T> concept has_full_update_notifier_call = requires(T t) { { t.notifyFullyUpdated() }; };\n" );
	fprintf( header, "template<typename T> concept has_update_notifier_call = requires(T t) { { t.notifyUpdated() }; };\n" );
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

		if ( namesOfDictionaries.find( name ) != namesOfDictionaries.end() )
		{
			fprintf( header, "\t//// TODO: revise notifier list !!!!!!!!!!!!!!!!!!!!!!!!!\n" );
			fprintf( header, 
				"template<typename T> concept has_value_updated_void_notifier_call_for_%s = requires(T t) { { t.notifyValueUpdated_%s() }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename KeyT> concept has_value_updated_notifier_call_for_%s = requires { { std::declval<StateT>().notifyValueUpdated_%s(std::declval<KeyT>()) }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename KeyT, typename ValueT> concept has_full_value_updated_notifier_call_for_%s = requires { { std::declval<StateT>().notifyValueUpdated_%s(std::declval<KeyT>(), std::declval<ValueT>()) }; };\n",
				name.c_str(), name.c_str()
			);

			fprintf( header, 
				"template<typename T> concept has_void_insert_notifier_call_for_%s = requires(T t) { { t.notifyInserted_%s() }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename KeyT> concept has_insert_notifier_call2_for_%s = requires { { std::declval<StateT>().notifyInserted_%s(std::declval<KeyT>()) }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename KeyT, typename ValueT> concept has_insert_notifier_call3_for_%s = requires { { std::declval<StateT>().notifyInserted_%s(std::declval<KeyT>(), std::declval<ValueT>()) }; };\n",
				name.c_str(), name.c_str()
			);

			fprintf( header, 
				"template<typename T> concept has_void_removed_notifier_call_for_%s = requires(T t) { { t.notifyRemoved_%s() }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename KeyT> concept has_removed_notifier_call2_for_%s = requires { { std::declval<StateT>().notifyErased_%s(std::declval<KeyT>()) }; };\n",
				name.c_str(), name.c_str()
			);
			fprintf( header, 
				"template<typename StateT, typename KeyT, typename ValueT> concept has_removed_notifier_call3_for_%s = requires { { std::declval<StateT>().notifyErased_%s(std::declval<KeyT>(), std::declval<ValueT>()) }; };\n",
				name.c_str(), name.c_str()
			);
		}
	}
	fprintf( header, "\n" );
}

void generatePublishable( FILE* header, Root& root, CompositeType& s, std::string platformPrefix, std::string classNotifierName )
{
	// bool checked = impl_checkParamNameUniqueness(s);
	// checked = impl_checkFollowingExtensionRules(s) && checked;
	// if ( !checked )
	// 	throw std::exception();

	assert( ( platformPrefix.size() == 0 && classNotifierName.size() == 0 ) || ( platformPrefix.size() != 0 && classNotifierName.size() != 0 ) );
	bool generatePlatformSpec = platformPrefix.size() != 0 && classNotifierName.size() != 0;

	impl_generatePublishableCommentBlock( header, s );

	impl_GeneratePublishableStateWrapperForPublisher( header, root, s );
	if ( generatePlatformSpec )
		impl_GeneratePublishableStatePlatformWrapperForPublisher( header, root, s, platformPrefix, classNotifierName );

	impl_GeneratePublishableStateWrapperForSubscriber( header, root, s );
	if ( generatePlatformSpec )
		impl_GeneratePublishableStatePlatformWrapperForSubscriber( header, root, s, platformPrefix, classNotifierName );

	impl_GeneratePublishableStateWrapperForConcentrator( header, root, s );
}

} // namespace cpptemplates
