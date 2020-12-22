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


struct FloatingParts
{
	int64_t fraction;
	int64_t exponent;
	FloatingParts( double d ) { fromFloating( d ); }
	void fromFloating( double d ) { 
		uint64_t fraction_ = *(uint64_t*)(&d) & 0x800fffffffffffffULL; 
		fraction = *(int64_t*)(&fraction_);
		uint64_t exponent_ = ( *(uint64_t*)(&d) << 1 ) >> 53;
		exponent = *(uint64_t*)(&exponent_) - 1023;
	}
	double value() { 
		int64_t exp_ = exponent + 1023;
		uint64_t res = (*(uint64_t*)(&exp_) << 52) | *(uint64_t*)(&fraction);
		assert( ( *(uint64_t*)(&fraction) & 0x7ff0000000000000 ) == 0 );
		assert( ( exp_ & ~0x7ffLLU ) == 0 );
		return *(double*)(&res);
	}
};

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

std::string impl_generateComposeFunctionName( CompositeType& s )
{
	return fmt::format( "{}_{}_compose", s.type2string(), s.name );
}

std::string impl_generateParseFunctionName( CompositeType& s )
{
	return fmt::format( "{}_{}_parse", s.type2string(), s.name );
}

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
	printf( "(%zd parameters) {\n", s.members.size() );
	printMessageMembers( s, offset + 4 );
	printf( "%s}\n", offsetch );
}


void printPublishable( CompositeType& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	printf( "%sPublishabe: name = \"%s\"", offsetch, s.name.c_str() );
	printf( "(%zd members) {\n", s.members.size() );
	printPublishableMembers( s, offset + 4 );
	printf( "%s}\n", offsetch );
}


void printStruct( CompositeType& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	printf( "%sStruct: name = \"%s\"", offsetch, s.name.c_str() );
	printf( "(%zd members) {\n", s.members.size() );
	printStructMembers( s, offset + 4 );
	printf( "%s}\n", offsetch );
}


void printMessageMembers( CompositeType& s, size_t offset )
{
	for ( auto& it : s.members )
	{
		auto& obj_1 = it;
		print__unique_ptr_MessageParameter( obj_1, offset + 4 );
	}
}


void printPublishableMembers( CompositeType& s, size_t offset )
{
	for ( auto& it : s.members )
	{
		auto& obj_1 = it;
		print__unique_ptr_MessageParameter( obj_1, offset + 4 );
	}
}


void printStructMembers( CompositeType& s, size_t offset )
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

	if ( s.whenDiscriminant.size() )
		printf( "\" whenDiscriminant: %zd\n", s.whenDiscriminant.size() );
	else
		printf( "\"\n" );
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
	else if ( s.kind == MessageParameterType::KIND::STRUCT )
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

string paramNameToNameTagStruct( string name )
{
	return fmt::format( "{}_Struct", name );
}

string paramNameToNameTagType( string name )
{
	return fmt::format( "{}_Type", name );
}

string paramNameToNameObject( string name )
{
	return name;
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

void impl_generateScopeEnum( FILE* header, Scope& scope )
{
	for ( auto msg : scope.objectList )
	{
		fprintf( header, "using %s = impl::MessageName<%lld>;\n", msg->name.c_str(), msg->numID );
	}
	fprintf( header, "\n" );
}

void impl_generateScopeHandler( FILE* header, Scope& scope )
{
	fprintf( header, 
		"template<class BufferT, class ... HandlersT >\n"
		"void handleMessage( BufferT& buffer, HandlersT ... handlers )\n"
		"{\n"
		"\tuint64_t msgID;\n\n"
	);
	switch ( scope.proto )
	{
		case Proto::gmq:
			fprintf( header, 
				"\tGmqParser parser( buffer );\n"
				"\tparser.parseUnsignedInteger( &msgID );\n"
			);
			break;
		case Proto::json:
			fprintf( header, 
				"\tJsonParser parser( buffer );\n"
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
		fprintf( header, "\t\tcase %s::id: impl::implHandleMessage<%s>( parser, handlers... ); break;\n", msg->name.c_str(), msg->name.c_str() );
	fprintf( header, "\t}\n\n" );
	switch ( scope.proto )
	{
		case Proto::gmq: break;
		case Proto::json:
			fprintf( header, 
				"\tp.skipMessageFromJson();\n"
				"\tparser = p;\n\n"
				"\tif (!parser.isDelimiter(\'}\'))\n"
				"\t\tthrow std::exception(); // bad format\n"
				"\tparser.skipDelimiter(\'}\');\n"
			);
			break;
		default:
			assert( false );
	}
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
		"\tstatic_assert( std::is_base_of<impl::MessageNameBase, msgID>::value );\n" 
	);
	switch ( scope.proto )
	{
		case Proto::gmq: 
			fprintf( header, 
				"\tm::GmqComposer composer( buffer );\n"
				"\timpl::composeUnsignedInteger( composer, msgID::id );\n"
			);
			break;
		case Proto::json: 
			fprintf( header, 
				"\tm::JsonComposer composer( buffer );\n"
				"\tcomposer.buff.append( \"{\\n  \", sizeof(\"{\\n  \") - 1 );\n"
				"\timpl::json::composeNamedSignedInteger( composer, \"msgid\", msgID::id);\n"
				"\tcomposer.buff.append( \",\\n  \", sizeof(\",\\n  \") - 1 );\n"
				"\timpl::json::addNamePart( composer, \"msgbody\" );\n"
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
		"\t\tstatic_assert( std::is_same<impl::MessageNameBase, msgID>::value, \"unexpected value of msgID\" ); // note: should be just static_assert(false,\"...\"); but it seems that in this case clang asserts yet before looking at constexpr conditions\n" );
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


string impl_MessageNameToDefaultsNamespaceName( string name )
{
	return fmt::format( "Message_{}_defaults", name );
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
					fprintf( header, "static constexpr impl::StringLiteralForComposing default_%d = { \"%s\", sizeof( \"%s\" ) - 1};\n", count, param.type.stringDefault.c_str(), param.type.stringDefault.c_str() );
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( FILE* header, Root& root, CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable || ( s.type == CompositeType::Type::structure && s.isStruct4Publishing ) );
	for ( auto& it : s.members )
	{
		assert( it != nullptr );
		fprintf( header, "\tstatic constexpr bool has_prenotifier_for_%s = has_prenotifier_call_for_%s<T>;\n", it->name.c_str(), it->name.c_str() );
		fprintf( header, "\tstatic constexpr bool has_postnotifier_for_%s = has_postnotifier_call_for_%s<T>;\n", it->name.c_str(), it->name.c_str() );
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
		fprintf( header, "\tauto get4set_%s() { return %s_RefWrapper<decltype(T::%s)>(t.%s); }\n", param.name.c_str(), param.name.c_str(), param.name.c_str(), param.name.c_str() );
	else if ( param.type.kind == MessageParameterType::KIND::VECTOR )
	{
		/*if ( param.type.vectorElemKind == MessageParameterType::KIND::STRUCT )
		{
			assert( root.structs.size() > param.type.messageIdx );
			fprintf( header, "\tauto get4set_%s() { return m::VectorOfStructRefWrapper4Set<%s_RefWrapper4Set<typename decltype(T::%s)::value_type, %s_Wrapper>, decltype(T::%s), %s_Wrapper>(t.%s, *this, GMQ_COLL vector<size_t>(), %zd); }\n", 
				param.name.c_str(), root.structs[param.type.messageIdx]->name.c_str(), param.name.c_str(), rootName, param.name.c_str(), rootName, param.name.c_str(), idx );
		}
		else*/
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
		"\ttemplate<class ParserT, class T>\n"
		"\tstatic\n"
		"\tvoid parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset )\n"
		"\t{\n"
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
				fprintf( header, "\t\t\t\tassert( addr.size() == offset + 1 );\n" );
				fprintf( header, 
					"\t\t\t\tm::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s) );\n",
					paramTypeToLeafeParser( member.type.kind ), member.name.c_str(), member.name.c_str()
				);
				break;
			case  MessageParameterType::KIND::STRUCT:
				fprintf( header, "\t\t\t\tif ( addr.size() > offset + 1 )\n" );
				fprintf( header, "\t\t\t\t{\n" );
				fprintf( header, "\t\t\t\t\tm::impl::publishableParseLeafeStructBegin( parser );\n" );
				fprintf( header, "\t\t\t\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
				fprintf( header, "\t\t\t\t\tm::impl::publishableParseLeafeStructEnd( parser );\n" );
				fprintf( header, "\t\t\t\t}\n" );
				fprintf( header, "\t\t\t\telse\n" );
				fprintf( header, "\t\t\t\t\t%s::parse( parser, t.%s, addr, offset + 1 );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
				break;
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
	fprintf( header, "\t\t}\n" );
	fprintf( header, "\t}\n" );
}

void impl_generateParseFunctionForPublishableStruct( FILE* header, Root& root, CompositeType& obj )
{
	fprintf( header, 
		"\ttemplate<class ParserT, class T>\n"
		"\tstatic\n"
		"\tvoid parse( ParserT& parser, T& t )\n"
		"\t{\n"
	);

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
				fprintf( header, 
					"\t\tm::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s), \"%s\" );\n",
					paramTypeToParser( member.type.kind ), member.name.c_str(), member.name.c_str(), member.name.c_str()
				);
				break;
			case  MessageParameterType::KIND::STRUCT:
				/*fprintf( header, "\t\tif ( addr.size() == offset + 1 )\n" );
				fprintf( header, "\t\t{\n" );
				fprintf( header, "\t\t\t\tm::impl::publishableParseLeafeStructBegin( parser );\n" );
				fprintf( header, "\t\t\t\tm::impl::parsePublishableStructBegin( parser, \"%s\" );\n", member.name.c_str() );
				fprintf( header, "\t\t\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
				fprintf( header, "\t\t\t\tm::impl::parsePublishableStructEnd( parser );\n" );
				fprintf( header, "\t\t\t\tm::impl::publishableParseLeafeStructEnd( parser );\n" );
				fprintf( header, "\t\t}\n" );
				fprintf( header, "\t\telse\n" );
				fprintf( header, "\t\t{\n" );
				fprintf( header, "\t\t\t\tm::impl::parsePublishableStructBegin( parser, \"%s\" );\n", member.name.c_str() );
				fprintf( header, "\t\t\t\t%s::parse( parser, t.%s, addr, offset + 1 );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
				fprintf( header, "\t\t\t\tm::impl::parsePublishableStructEnd( parser );\n" );
				fprintf( header, "\t\t}\n" );*/

				fprintf( header, "\t\tm::impl::parsePublishableStructBegin( parser, \"%s\" );\n", member.name.c_str() );
				fprintf( header, "\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
				fprintf( header, "\t\tm::impl::parsePublishableStructEnd( parser );\n" );
				break;
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

	fprintf( header, "\t}\n" );
}
void impl_generatePublishableStructForwardDeclaration( FILE* header, Root& root, CompositeType& obj )
{
	fprintf( header, 
		"struct %s;\n",
		impl_generatePublishableStructName( obj ).c_str()
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
	
	if ( forRoot )
	{
		/*fprintf( header, 
			"\t\t// NOTE: fake code below\n"
			"\t\t// TODO: form respective message or register change otherwise\n"
			"\t\tfmt::print( \"updating T::%s with value {};\\n\", val );\n",
			param.name.c_str()
		);*/
	}
	else
	{
		/*fprintf( header, 
			"\t\t// NOTE: fake code below\n"
			"\t\t// TODO: form respective message or register change otherwise\n"
			"\t\tfmt::print( \"updating T::%s with value {}; path = [ \", val );\n"
			"\t\tfor ( size_t i=0; i<address.size(); ++i )\n"
			"\t\t\tfmt::print( \"{} \", address[i] );\n"
			"\t\tfmt::print( \"] \\n\" );\n",
			param.name.c_str()
		);*/
	}
	fprintf( header, "\t}\n" );
}

void impl_GeneratePublishableStateMemberAccessors( FILE* header, Root& root, CompositeType& s, bool allowSeters )
{
	assert( s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure );
	bool forRoot = s.type == CompositeType::Type::publishable;
	const char* rootName = forRoot ? s.name.c_str() : nullptr;
	for ( size_t i=0; i<s.members.size(); ++i )
	{
		auto& it = s.members[i];
		assert( it != nullptr );
		impl_GeneratePublishableStateMemberGetter( header, root, s, *it );
		if ( allowSeters )
			impl_GeneratePublishableStateMemberSetter( header, root, forRoot, rootName, *it, i );
		impl_GeneratePublishableStateMemberGetter4Set( header, root, rootName, *it, i );
	}
}

void impl_GenerateApplyUpdateMessageMemberFn( FILE* header, Root& root, CompositeType& s )
{
	fprintf( header, 
		"\ttemplate<typename ParserT>\n"
		"\tvoid applyMessageWithUpdates(ParserT& parser) {\n"
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
					"\t\t\t\t\tif constexpr( has_prenotifier_for_%s || has_postnotifier_for_%s )\n"
					"\t\t\t\t\t{\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tdecltype(T::%s) newVal;\n"
					"\t\t\t\t\t\tm::impl::publishableParseLeafeInteger<ParserT, decltype(T::%s)>( parser, &newVal );\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tif ( newVal != t.%s )\n"
					"\t\t\t\t\t\t{\n"
					"\t\t\t\t\t\t\tif constexpr ( has_prenotifier_for_%s )\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\t\tt.notifyBefore_%s();\n"
					"\t\t\t\t\t\t\tt.%s = newVal;\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\t\tif constexpr ( has_postnotifier_for_%s )\n"
					"\t\t\t\t\t\t\t\tt.notifyAfter_%s();\n"
					"\t\t\t\t\t\t}\n"
					"\t\t\t\t\t}\n"
					"\t\t\t\t\telse\n",
					member.name.c_str(), member.name.c_str()
				);
				fprintf( header, 
					"\t\t\t\t\t\tm::impl::%s<ParserT, decltype(T::%s)>( parser, &(t.%s) );\n",
					paramTypeToLeafeParser( member.type.kind ), member.name.c_str(), member.name.c_str()
				);
				break;
				break;
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "\t\t\t\t\tif ( addr.size() == 1 )\n" );
				fprintf( header, "\t\t\t\t\t{\n" );
				fprintf( header, "\t\t\t\t\t\t\tm::impl::publishableParseLeafeStructBegin( parser );\n" );
				fprintf( header, "\t\t\t\t\t\t\t%s::parse( parser, t.%s );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
				fprintf( header, "\t\t\t\t\t\t\tm::impl::publishableParseLeafeStructEnd( parser );\n" );
				fprintf( header, "\t\t\t\t\t}\n" );
				fprintf( header, "\t\t\t\t\telse\n" );
				fprintf( header, "\t\t\t\t\t{\n" );
				fprintf( header, "\t\t\t\t\t\t\tm::impl::parsePublishableStructBegin( parser, \"%s\" );\n", member.name.c_str() );
				fprintf( header, "\t\t\t\t\t\t\t%s::parse( parser, t.%s, addr, 1 );\n", impl_generatePublishableStructName( member ).c_str(), member.name.c_str() );
				fprintf( header, "\t\t\t\t\t\t\tm::impl::parsePublishableStructEnd( parser );\n" );
				fprintf( header, "\t\t\t\t\t}\n" );
				break;
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
	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, s );

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
	impl_GeneratePublishableMemberUpdateNotifierPresenceCheckingBlock( header, root, s );

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
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::SignedIntegralType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::UnsignedIntegralType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::REAL:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::RealType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::StringType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::ByteArrayType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::BLOB:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::BlobType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::ENUM:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::EnumType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::VECTOR:
				switch ( param.type.vectorElemKind )
				{
					case MessageParameterType::KIND::INTEGER:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::SignedIntegralType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::UINTEGER:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::UnsignedIntegralType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::REAL:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::RealType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::CHARACTER_STRING:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::StringType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::BYTE_ARRAY:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::ByteArrayType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::BLOB:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::BlobType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::ENUM:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::EnumType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::STRUCT:
						if ( param.type.isNonExtendable )
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						else
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfMessageType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					default:
						assert( false ); // unexpected
						break;
				}
				break;
			case MessageParameterType::KIND::STRUCT:
				if ( param.type.isNonExtendable )
					fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::NonextMessageType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				else
					fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::MessageType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
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
				fprintf( header, "%simpl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "%simpl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), count );
				break;
			case MessageParameterType::KIND::REAL:
			{
				FloatingParts parts(param.type.numericalDefault);
				fprintf( header, "%simpl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, FloatingDefault<%lldll,%lldll>, int, 0>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", parts.fraction, parts.exponent, count );
				break;
			}
			case MessageParameterType::KIND::CHARACTER_STRING:
				if ( param.type.hasDefault )
					fprintf( header, "%simpl::gmq::composeParamToGmq<ComposerT, arg_%d_type, false, nodecpp::string, const impl::StringLiteralForComposing*, &%s::default_%d>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, impl_MessageNameToDefaultsNamespaceName(s.name).c_str(), count, count );
				else
					fprintf( header, "%simpl::gmq::composeParamToGmq<ComposerT, arg_%d_type, true, uint64_t, uint64_t, (uint64_t)0>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf( header, "%simpl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), count );
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: treatment
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "%simpl::gmq::composeParamToGmq<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", count );
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
				fprintf( header, "%simpl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "%simpl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::REAL:
				fprintf( header, "%simpl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "%simpl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf( header, "%simpl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: treatment
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "%simpl::gmq::parseGmqParam<ParserT, arg_%d_type, false>(p, arg_%d_type::nameAndTypeID, args...);\n", offset, count, count );
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
				fprintf( header, "%simpl::json::composeParamToJson<ComposerT, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "%simpl::json::composeParamToJson<ComposerT, arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::REAL:
			{
				FloatingParts parts(param.type.numericalDefault);
//				fprintf( header, "%simpl::json::composeParamToJson<ComposerT, arg_%d_type, %s, double, double, %f>(\"%s\", arg_%d_type::nameAndTypeID, composer, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", param.type.numericalDefault, param.name.c_str(), count );
				fprintf( header, "%simpl::json::composeParamToJson<ComposerT, arg_%d_type, %s, FloatingDefault<%lldll,%lldll>, int, 0>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", parts.fraction, parts.exponent, param.name.c_str(), count );
				break;
			}
			case MessageParameterType::KIND::CHARACTER_STRING:
				if ( param.type.hasDefault )
					fprintf( header, "%simpl::json::composeParamToJson<ComposerT, arg_%d_type, false, nodecpp::string, const impl::StringLiteralForComposing*, &%s::default_%d>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, impl_MessageNameToDefaultsNamespaceName(s.name).c_str(), count, param.name.c_str(), count );
				else
					fprintf( header, "%simpl::json::composeParamToJson<ComposerT, arg_%d_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf( header, "%simpl::json::composeParamToJson<ComposerT, arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: ...
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "%simpl::json::composeParamToJson<ComposerT, arg_%d_type, %s, int64_t, int64_t, (int64_t)(0)>(composer, \"%s\", arg_%d_type::nameAndTypeID, args...);\n", offset, count, param.type.hasDefault ? "false" : "true", param.name.c_str(), count );
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
		fprintf( header, "%s\t\timpl::json::parseJsonParam<ParserT, arg_%d_type, false>(arg_%d_type::nameAndTypeID, p, args...);\n", offset, count, count );
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

void generatePublishable( FILE* header, Root& root, CompositeType& s )
{
	bool checked = impl_checkParamNameUniqueness(s);
	checked = impl_checkFollowingExtensionRules(s) && checked;
	if ( !checked )
		throw std::exception();

	impl_generatePublishableCommentBlock( header, s );
	impl_GeneratePublishableStateWrapper( header, root, s );
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

void generateNotifierPresenceTesterBlock( FILE* header, Root& root )
{
	set<string> names;
	collectMemberNamesFromPublishableObjects( root.publishables, names );
	collectMemberNamesFromPublishableObjects( root.structs, names );

	fprintf( header, "// member update notifier presence checks\n" );
	for ( auto& name : names )
	{
		fprintf( header, 
			"template<typename T> concept has_prenotifier_call_for_%s = requires(T t) { { t.notifyBefore_%s() }; };\n",
//			"template<typename StateT, typename NodeT> concept has_prenotifier_call_for_%s = requires { { std::declval<StateT>().notifyBefore_%s(std::declval<NodeT>()) }; };\n",
			name.c_str(), name.c_str()
		);
		fprintf( header, 
			"template<typename T> concept has_postnotifier_call_for_%s = requires(T t) { { t.notifyAfter_%s() }; };\n",
//			"template<typename StateT, typename NodeT> concept has_postnotifier_call_for_%s = requires { { std::declval<StateT>().notifyAfter_%s(std::declval<NodeT>()) }; };\n",
			name.c_str(), name.c_str()
		);
	}
	fprintf( header, "\n" );
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

