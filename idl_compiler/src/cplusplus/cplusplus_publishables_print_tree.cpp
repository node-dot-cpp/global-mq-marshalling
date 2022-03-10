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

#include "cplusplus_idl_tree_serializer.h"

namespace cplusplus
{

void printRoot( Root& s )
{
	if ( s.messages.size() == 0 && s.publishables.size() == 0 && s.structs.size() == 0 )
	{
		printf( "Root (no messages or publishable structs) {\n" );
		printf( "}\n" );
		return;
	}
	printf( "Root (" );
	if ( s.messages.size() )
		printf( "%zd messages ", s.messages.size() );
	if ( s.publishables.size() )
		printf( "%zd publishables ", s.publishables.size() );
	if ( s.structs.size() )
		printf( "%zd structs ", s.structs.size() );
	printf( ") {\n" );
	for ( auto& it : s.scopes )
	{
		assert( it != nullptr && typeid( *(it) ) == typeid( Scope ) );
		printScope( *(dynamic_cast<Scope*>(&(*(it)))), 4 );
	}
	for ( auto& it : s.messages )
	{
		assert( it != nullptr && typeid( *(it) ) == typeid( CompositeType ) );
		printMessage( *(dynamic_cast<CompositeType*>(&(*(it)))), 4 );
	}
	for ( auto& it : s.publishables )
	{
		assert( it != nullptr && typeid( *(it) ) == typeid( CompositeType ) );
		printPublishable( *(dynamic_cast<CompositeType*>(&(*(it)))), 4 );
	}
	for ( auto& it : s.structs )
	{
		assert( it != nullptr && typeid( *(it) ) == typeid( CompositeType ) );
		printStruct( *(dynamic_cast<CompositeType*>(&(*(it)))), 4 );
	}
	printf( "}\n" );
}

void printScope( Scope& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	printf( "%sScope: name = \"%s\" Protocol: ", offsetch, s.name.c_str() );
	switch ( s.proto )
	{
		case Proto::gmq: printf( "GMQ " ); break;
		case Proto::json: printf( "JSON " ); break;
		default: assert( false );
	}
	printf( "(%zd messages) {\n", s.objectList.size() );
	for ( auto msg : s.objectList )
		printf( "%s  %s\n", offsetch, msg->name.c_str() );
	printf( "%s}\n", offsetch );
}

void printMessage( CompositeType& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	printf( "%sMessage: name = \"%s:%s=%zd\" %sTargets: ", offsetch, s.scopeName.c_str(), s.name.c_str(), s.numID, s.isNonExtendable ? "NONEXTENDABLE " : "" );
	for ( auto t:s.protoList )
		switch ( t )
		{
			case Proto::gmq: printf( "GMQ " ); break;
			case Proto::json: printf( "JSON " ); break;
			default: assert( false );
		}
	printf( "(%zd parameters) {\n", s.getMembers().size() );
	printMessageMembers( s, offset + 4 );
	printf( "%s}\n", offsetch );
}


void printPublishable( CompositeType& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	printf( "%sPublishabe: name = \"%s\"(%lld)", offsetch, s.name.c_str(), s.numID );
	printf( "(%zd members) {\n", s.getMembers().size() );
	printPublishableMembers( s, offset + 4 );
	printf( "%s}\n", offsetch );
}


void printStruct( CompositeType& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	if ( s.isDiscriminatedUnion() )
	{
		printf( "%sDiscriminatedUnion: name = \"%s\"", offsetch, s.name.c_str() );
		printf( "(%zd cases) {\n", s.getDiscriminatedUnionCases().size() );
		printDiscriminatedUnionCases( s, offset + 4 );
		printf( "%s}\n", offsetch );
	}
	else
	{
		printf( "%sStruct: name = \"%s\"", offsetch, s.name.c_str() );
		printf( "(%zd members) {\n", s.getMembers().size() );
		printStructMembers( s, offset + 4 );
		printf( "%s}\n", offsetch );
	}
}


void printDiscriminatedUnionCase( CompositeType& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	printf( "%sDU_Case: name = \"%s\"(%lld)", offsetch, s.name.c_str(), s.numID );
	printf( "(%zd members) {\n", s.getMembers().size() );
	printStructMembers( s, offset + 4 );
	printf( "%s}\n", offsetch );
}


void printMessageMembers( CompositeType& s, size_t offset )
{
	for ( auto& it : s.getMembers() )
	{
		auto& obj_1 = it;
		print__unique_ptr_MessageParameter( obj_1, offset + 4 );
	}
}


void printPublishableMembers( CompositeType& s, size_t offset )
{
	for ( auto& it : s.getMembers() )
	{
		auto& obj_1 = it;
		print__unique_ptr_MessageParameter( obj_1, offset + 4 );
	}
}


void printStructMembers( CompositeType& s, size_t offset )
{
	for ( auto& it : s.getMembers() )
	{
		auto& obj_1 = it;
		print__unique_ptr_MessageParameter( obj_1, offset + 4 );
	}
}


void print__unique_ptr_MessageParameter( unique_ptr<MessageParameter>& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;
	if ( s == nullptr ) 
	{
		printf( "%sParameter <null> {}\n", offsetch );
		return;
	}
	printMessageParameter( *(dynamic_cast<MessageParameter*>(&(*(s)))), offset + 4 );
}

void printMessageParameter( MessageParameter& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	if ( s.type.kind == MessageParameterType::KIND::EXTENSION )
	{
		printf( "%sEXTENSION:\n", offsetch );
		return;
	}

	printf( "%sname: \"%s\" type: \"", offsetch, s.name.c_str() );
	printDataType( s.type );

	if ( s.extendTo )
	{
		printf( "\" extendTo: %s ", s.extendTo ? "yes" : "no" );
		assert( s.defaultValue.kind != Variant::KIND::NONE );
		printf( ", defaultValue: " );
		printVariant( s.defaultValue );
	}
	else
		assert( s.defaultValue.kind == Variant::KIND::NONE );

	/*if ( s.whenDiscriminant.size() )
		printf( "\" whenDiscriminant: %zd\n", s.whenDiscriminant.size() );
	else*/
		printf( "\"\n" );
}


void printDiscriminatedUnionCases( CompositeType& s, size_t offset )
{
	for ( auto& it : s.getDiscriminatedUnionCases() )
	{
		auto& obj_1 = it;
		print__unique_ptr_DiscriminatedUnionCase( obj_1, offset + 4 );
	}
}


void print__unique_ptr_DiscriminatedUnionCase( unique_ptr<CompositeType>& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;
	if ( s == nullptr ) 
	{
		printf( "%sCase <null> {}\n", offsetch );
		return;
	}
	printDiscriminatedUnionCase( *(dynamic_cast<CompositeType*>(&(*(s)))), offset + 4 );
}

void printDataType( MessageParameterType& s )
{
	if ( s.kind == MessageParameterType::KIND::VECTOR )
	{
		if ( s.vectorElemKind == MessageParameterType::KIND::STRUCT )
			printf( "%s<%s%s %s>", impl_kindToString( s.kind ), s.isNonExtendable ? "NONEXTENDABLE " : "", impl_kindToString( s.vectorElemKind ), s.name.c_str() );
		else
			printf( "%s<%s>", impl_kindToString( s.kind ), impl_kindToString( s.vectorElemKind ) );
	}
	else if ( s.kind == MessageParameterType::KIND::DICTIONARY )
	{
		if ( s.dictionaryValueKind == MessageParameterType::KIND::STRUCT )
			printf( "%s<%s, %s%s %s>", impl_kindToString( s.kind ), impl_kindToString( s.dictionaryKeyKind ), s.isNonExtendable ? "NONEXTENDABLE " : "", impl_kindToString( s.dictionaryValueKind ), s.name.c_str() );
		else
			printf( "%s<%s, %s>", impl_kindToString( s.kind ), impl_kindToString( s.dictionaryKeyKind ), impl_kindToString( s.dictionaryValueKind ) );
	}
	else if ( s.kind == MessageParameterType::KIND::STRUCT || s.kind == MessageParameterType::KIND::DISCRIMINATED_UNION )
	{
		printf( "%s %s %s", impl_kindToString( s.kind ), s.isNonExtendable ? "NONEXTENDABLE " : "", s.name.c_str() );
	}
	else if ( s.kind == MessageParameterType::KIND::ENUM )
	{
		printf( "%s %s", impl_kindToString( s.kind ), s.name.c_str() );
	}
	else
	{
		assert( s.name.empty() );
		printf( "%s", impl_kindToString( s.kind ) );
	}

//	if ( s.name.size() )
//		printf( "name: %s, ", s.name.c_str() );
	if ( s.kind == MessageParameterType::KIND::INTEGER || s.kind == MessageParameterType::KIND::UINTEGER )
	{
		if ( s.hasLowLimit )
		{
			printf( "lowLimit: " );
			printLimit( s.lowLimit );
		}
		else
		{
			printf( "<no low limit> " );
		}
		if ( s.hasHighLimit )
		{
			printf( " highLimit: " );
			printLimit( s.highLimit );
		}
		else
		{
			printf( "<no high limit> " );
		}
		if ( s.hasDefault )
		{
			printf( "DEFAULT: %f ", s.numericalDefault );
		}
	}
	else if ( s.kind == MessageParameterType::KIND::BYTE_ARRAY )
	{
		printf( "size: %d ", s.arrayFixedaxSize );
	}
	else if ( s.kind == MessageParameterType::KIND::CHARACTER_STRING )
	{
		if ( s.hasMaxLength )
			printf( "max_length: %d ", s.stringMaxLength );
		if ( s.hasDefault )
			printf( "default: \"%s\" ", s.stringDefault.c_str() );
	}

	if ( s.enumValues.size() )
	{
		printf( ", enumValues (%zd): { ", s.enumValues.size() );
		for ( auto item:s.enumValues )
			printf( "%s = %d", item.first.c_str(), item.second  );
		printf( " }" );
	}
}

void printLimit( Limit& s ) {
	printf( "{value: %f, inclusive: %s}", s.value, s.inclusive ? "yes" : "no" );
}

void printVariant( Variant& s ) {
	switch ( s.kind )
	{
		case Variant::KIND::NONE:
			printf( "???" ); break;
		case Variant::KIND::NUMBER:
			printf( "%f", s.numberValue ); break;
		case Variant::KIND::STRING:
			printf( "%s", s.stringValue.c_str() ); break;
	}
}

}
