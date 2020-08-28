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


void printRoot( Root& s )
{
	printf( "Root (%zd messages) {\n", s.messages.size() );
	for ( auto& it : s.messages )
	{
		auto& obj_1 = it;
		print__unique_ptr_Message( obj_1, 4 );
	}
	printf( "}\n" );
}

void print__unique_ptr_Message( unique_ptr<Message>& s, size_t offset ) {

	if ( s == nullptr )
	{
		char offsetch[1024];
		memset( offsetch, ' ', offset );
		offsetch[ offset ] = 0;
		printf( "%sMessage <null> {}\n", offsetch );
	}
	else if ( typeid( *(s) ) == typeid( Message ) )
	{
		printMessage( *(dynamic_cast<Message*>(&(*(s)))), offset );
	}
}

void printMessage( Message& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	printf( "%sMessage: name = \"%s\" (%zd parameters) {\n", offsetch, s.name.c_str(), s.members.size() );
	printMessageMembers( s, offset + 4 );
	printf( "%s}\n", offsetch );
}


void printMessageMembers( Message& s, size_t offset )
{
	for ( auto& it : s.members )
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

	if ( s.whenDiscriminant.size() )
		printf( "\" whenDiscriminant: %zd\n", s.whenDiscriminant.size() );
	else
		printf( "\"\n" );
}

void printDataType( MessageParameterType& s )
{
	const char* kind = "";
	switch ( s.kind )
	{
		case MessageParameterType::KIND::ENUM: kind = "ENUM"; break;
		case MessageParameterType::KIND::UNDEFINED: kind = "UNDEFINED"; break;
		case MessageParameterType::KIND::INTEGER: kind = "INTEGER"; break;
		case MessageParameterType::KIND::UINTEGER: kind = "UINTEGER"; break;
		case MessageParameterType::KIND::CHARACTER_STRING: kind = "CHARACTER_STRING"; break;
		case MessageParameterType::KIND::BYTE_ARRAY: kind = "BYTE_ARRAY"; break;
	}

	printf( "{kind: %d (%s), ", (int)(s.kind), kind );
	if ( s.name.size() )
		printf( "name: %s, ", s.name.c_str() );
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

	if ( s.enumValues.size() )
	{
		printf( ", enumValues (%zd): { ", s.enumValues.size() );
		for ( auto item:s.enumValues )
			printf( "%s = %d", item.first.c_str(), item.second  );
		printf( " }" );
	}

	printf( "}" );
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

