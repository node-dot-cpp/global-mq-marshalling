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
		case MessageParameterType::KIND::MESSAGE: return "MESSAGE";
		case MessageParameterType::KIND::PUBLISHABLE: return "PUBLISHABLE";
		case MessageParameterType::KIND::STRUCT: return "STRUCT";
		case MessageParameterType::KIND::EXTENSION: return "EXTENSION";
		default: assert( false ); return "";
	}
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
	for ( auto& it : s.messages )
	{
		auto& obj_1 = it;
		print__unique_ptr_Message( obj_1, 4 );
	}
	for ( auto& it : s.publishables )
	{
		auto& obj_1 = it;
		print__unique_ptr_Publishable( obj_1, 4 );
	}
	for ( auto& it : s.structs )
	{
		auto& obj_1 = it;
		print__unique_ptr_Struct( obj_1, 4 );
	}
	printf( "}\n" );
}

void print__unique_ptr_Message( unique_ptr<CompositeType>& s, size_t offset )
{
	if ( s == nullptr )
	{
		char offsetch[1024];
		memset( offsetch, ' ', offset );
		offsetch[ offset ] = 0;
		printf( "%sMessage <null> {}\n", offsetch );
	}
	else if ( typeid( *(s) ) == typeid( CompositeType ) )
	{
		printMessage( *(dynamic_cast<CompositeType*>(&(*(s)))), offset );
	}
}

void print__unique_ptr_Publishable( unique_ptr<CompositeType>& s, size_t offset )
{
	if ( s == nullptr )
	{
		char offsetch[1024];
		memset( offsetch, ' ', offset );
		offsetch[ offset ] = 0;
		printf( "%sPublishable <null> {}\n", offsetch );
	}
	else if ( typeid( *(s) ) == typeid( CompositeType ) )
	{
		printPublishable( *(dynamic_cast<CompositeType*>(&(*(s)))), offset );
	}
}

void print__unique_ptr_Struct( unique_ptr<CompositeType>& s, size_t offset )
{
	if ( s == nullptr )
	{
		char offsetch[1024];
		memset( offsetch, ' ', offset );
		offsetch[ offset ] = 0;
		printf( "%sStruct <null> {}\n", offsetch );
	}
	else if ( typeid( *(s) ) == typeid( CompositeType ) )
	{
		printStruct( *(dynamic_cast<CompositeType*>(&(*(s)))), offset );
	}
}

void printMessage( CompositeType& s, size_t offset )
{
	char offsetch[1024];
	memset( offsetch, ' ', offset );
	offsetch[ offset ] = 0;

	printf( "%sMessage: name = \"%s\" %sTargets: ", offsetch, s.name.c_str(), s.isNonExtendable ? "NONEXTENDABLE " : "" );
	for ( auto t:s.protoList )
		switch ( t )
		{
			case CompositeType::Proto::gmq: printf( "GMQ " ); break;
			case CompositeType::Proto::json: printf( "JSON " ); break;
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
		bool isComposite = s.vectorElemKind == MessageParameterType::KIND::MESSAGE || s.vectorElemKind == MessageParameterType::KIND::PUBLISHABLE ||s.vectorElemKind == MessageParameterType::KIND::STRUCT;
		if ( isComposite )
			printf( "%s<%s%s %s>", impl_kindToString( s.kind ), s.isNonExtendable ? "NONEXTENDABLE " : "", impl_kindToString( s.vectorElemKind ), s.name.c_str() );
		else
			printf( "%s<%s>", impl_kindToString( s.kind ), impl_kindToString( s.vectorElemKind ) );
	}
	else if ( s.kind == MessageParameterType::KIND::MESSAGE || s.kind == MessageParameterType::KIND::PUBLISHABLE || s.kind == MessageParameterType::KIND::STRUCT )
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

void impl_CollectParamNamesFrom__unique_ptr_MessageParameter( std::set<string>& params, unique_ptr<MessageParameter>& s )
{
	if ( s != nullptr ) 
		impl_CollectParamNamesFromeMessageParameter( params, *(dynamic_cast<MessageParameter*>(&(*(s)))) );
}

void impl_CollectParamNamesFromMessage( std::set<string>& params, CompositeType& s )
{
	for ( auto& it : s.members )
	{
		auto& obj_1 = it;
		if ( obj_1->type.kind == MessageParameterType::KIND::EXTENSION )
			continue;
		impl_CollectParamNamesFrom__unique_ptr_MessageParameter( params, obj_1 );
	}
}

void impl_CollectParamNamesFrom__unique_ptr_Message( std::set<string>& params, unique_ptr<CompositeType>& s ) {

	if ( s != nullptr && typeid( *(s) ) == typeid( CompositeType ) )
	{
		impl_CollectParamNamesFromMessage( params, *(dynamic_cast<CompositeType*>(&(*(s)))) );
	}
}

void impl_CollectParamNamesFromRoot( std::set<string>& params, Root& s )
{
	for ( auto& it : s.messages )
	{
		auto& obj_1 = it;
		impl_CollectParamNamesFrom__unique_ptr_Message( params, obj_1 );
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

void generateParamNameBlock( FILE* header, const std::set<string>& params )
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
	fprintf( header, "\n" );
}

bool impl_checkCompositeTypeNameUniqueness(vector<unique_ptr<CompositeType>>& coll, const char* typeName)
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
	bool ok = impl_checkCompositeTypeNameUniqueness(s.messages, "MESSAGE");
	ok = impl_checkCompositeTypeNameUniqueness(s.publishables, "PUBLISHABLE") && ok;
	ok = impl_checkCompositeTypeNameUniqueness(s.structs, "STRUCT") && ok;
	return ok;
}

bool impl_processCompositeTypeNamesInMessagesAndPublishables(Root& s, CompositeType& ct)
{
	bool ok = true;
		for ( auto& param : ct.members )
		{
			if ( param->type.kind == MessageParameterType::KIND::MESSAGE || param->type.kind == MessageParameterType::KIND::PUBLISHABLE )
			{
				fprintf( stderr, "File \"%s\", line %d: error: parameter type cannot be %s (but can be a %s)\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( param->type.kind ), impl_kindToString( MessageParameterType::KIND::STRUCT ) );
				ok = false;
			}
			else if ( param->type.kind == MessageParameterType::VECTOR )
			{
				if (  param->type.kind == MessageParameterType::KIND::VECTOR && ( param->type.vectorElemKind ==  MessageParameterType::KIND::MESSAGE || param->type.vectorElemKind == MessageParameterType::KIND::PUBLISHABLE ))
				{
					fprintf( stderr, "File \"%s\", line %d: error: vector element type cannot be %s (but can be a %s)\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( param->type.kind ), impl_kindToString( MessageParameterType::KIND::STRUCT ) );
					ok = false;
				}
				else if ( param->type.vectorElemKind == MessageParameterType::KIND::STRUCT ) // existance and extentability
				{
					param->type.messageIdx = (size_t)(-1);
					for ( size_t i=0; i<s.structs.size(); ++i )
						if ( param->type.name == s.structs[i]->name )
						{
							param->type.messageIdx = i;
							if ( param->type.isNonExtendable && !s.structs[i]->isNonExtendable )
							{
								fprintf( stderr, "%s, line %d: %s \"%s\" is not declared as NONEXTENDABLE (see CompositeType declaration at %s, line %d)\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( param->type.kind ), param->type.name.c_str(), s.messages[i]->location.fileName.c_str(), s.messages[i]->location.lineNumber );
								ok = false;
							}
							/*for ( auto proto : ct.protoList )
								if ( s.structs[i]->protoList.find( proto ) == s.structs[i]->protoList.end() )
								{
									fprintf( stderr, "%s, line %d: %s \"%s\" does not support all protocols of current MESSAGE\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( param->type.kind ), param->type.name.c_str() );
									fprintf( stderr, "             see declaration of current CompositeType at %s, line %d\n", ct.location.fileName.c_str(), ct.location.lineNumber );
									fprintf( stderr, "             see declaration of CompositeType \"%s\" at %s, line %d\n", param->type.name.c_str(), s.structs[i]->location.fileName.c_str(), s.structs[i]->location.lineNumber );
									ok = false;
								}*/
							s.structs[i]->protoList.insert( ct.protoList.begin(), ct.protoList.end() );
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
							fprintf( stderr, "%s, line %d: %s \"%s\" is not declared as NONEXTENDABLE (see CompositeType declaration at %s, line %d)\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( param->type.kind ), param->type.name.c_str(), s.structs[i]->location.fileName.c_str(), s.structs[i]->location.lineNumber );
							ok = false;
						}
						break;
					}
				if ( param->type.messageIdx == (size_t)(-1) )
				{
					fprintf( stderr, "%s, line %d: %s name \"%s\" not found\n", param->location.fileName.c_str(), param->location.lineNumber, impl_kindToString( param->type.kind ), param->type.name.c_str() );
					ok = false;
				}
			}
		}
	return ok;
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
			fprintf( stderr, "%s parameter \"%s\" has already been used within this CompositeType, see %s : %d\n", s.type2string(), it->name.c_str(), ins.first->second.fileName.c_str(), ins.first->second.lineNumber );
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

void generateRoot( const char* fileName, FILE* header, Root& s )
{
	bool ok = impl_checkCompositeTypeNameUniqueness(s);
	ok = impl_processCompositeTypeNamesInMessagesAndPublishables(s) && ok;
	if ( !ok )
		throw std::exception();

	std::set<string> params;
	impl_CollectParamNamesFromRoot( params, s );

	fprintf( header, "#ifndef %s_guard\n"
		"#define %s_guard\n"
		"\n"
		"#include <marshalling.h>\n"
		"\n"
		"namespace m {\n\n",
		fileName, fileName );

	generateParamNameBlock( header, params );

	for ( auto& it : s.messages )
	{
		auto& obj_1 = it;
		if ( obj_1 == nullptr )
			fprintf( header, "// Message = <null>\n" );
		else 
		{
			assert( typeid( *(obj_1) ) == typeid( CompositeType ) );
			generateMessage( header, *(dynamic_cast<CompositeType*>(&(*(obj_1)))) );
		}
	}

	for ( auto& it : s.structs )
	{
		auto& obj_1 = it;
		if ( obj_1 == nullptr )
			fprintf( header, "// Struct = <null>\n" );
		else 
		{
			assert( typeid( *(obj_1) ) == typeid( CompositeType ) );
			generateMessage( header, *(dynamic_cast<CompositeType*>(&(*(obj_1)))) );
		}
	}

	fprintf( header, "\n"
		"} // namespace m\n"
		"\n"
		"#endif // %s_guard\n",
		fileName );
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
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::RealType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
					case MessageParameterType::KIND::REAL:
						fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::UnsignedIntegralType>, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
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
					case MessageParameterType::KIND::MESSAGE:
					case MessageParameterType::KIND::PUBLISHABLE:
					case MessageParameterType::KIND::STRUCT:
						if ( param.type.isNonExtendable )
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						else
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfMessageType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
				}
				break;
			case MessageParameterType::KIND::MESSAGE:
			case MessageParameterType::KIND::PUBLISHABLE:
			case MessageParameterType::KIND::STRUCT:
				fprintf( header, "//       MessageParameterType::KIND::MESSAGE !!!!!!!!!!!!!\n" );
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
			case MessageParameterType::KIND::MESSAGE:
			case MessageParameterType::KIND::PUBLISHABLE:
			case MessageParameterType::KIND::STRUCT:
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
			case MessageParameterType::KIND::MESSAGE:
			case MessageParameterType::KIND::PUBLISHABLE:
			case MessageParameterType::KIND::STRUCT:
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
	fprintf( header, "//**********************************************************************\n" );
	fprintf( header, "// CompositeType \"%s\" %sTargets: ", s.name.c_str(), s.isNonExtendable ? "NONEXTENDABLE " : "" );
	for ( auto t:s.protoList )
		switch ( t )
		{
			case CompositeType::Proto::gmq: fprintf( header, "GMQ " ); break;
			case CompositeType::Proto::json: fprintf( header, "JSON " ); break;
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
			bool isComposite = param.type.vectorElemKind == MessageParameterType::KIND::MESSAGE || param.type.vectorElemKind == MessageParameterType::KIND::PUBLISHABLE || param.type.vectorElemKind == MessageParameterType::KIND::STRUCT;
			if ( isComposite )
				fprintf( header, "// %d. %s<%s%s %s>", count, impl_kindToString( param.type.kind ), param.type.isNonExtendable ? "NONEXTENDABLE " : " ", impl_kindToString( param.type.vectorElemKind ), param.type.name.c_str() );
			else
				fprintf( header, "// %d. %s<%s>", count, impl_kindToString( param.type.kind ), impl_kindToString( param.type.vectorElemKind ) );
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

void impl_generateComposeFunctionGmq( FILE* header, CompositeType& s )
{
	fprintf( header, "template<typename ... Args>\n"
	"void %s_composeGmq(Composer& composer, Args&& ... args)\n"
	"{\n", s.name.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForComposingGmq( header, s, "\t" );


	fprintf( header, "}\n\n" );
}

void impl_generateParseFunctionGmq( FILE* header, CompositeType& s )
{
	fprintf( header, "template<typename ... Args>\n"
	"void %s_parseGmq(Parser& p, Args&& ... args)\n"
	"{\n", s.name.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForParsingGmq( header, s, "\t" );

	fprintf( header, "}\n\n" );
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
			case MessageParameterType::KIND::MESSAGE:
			case MessageParameterType::KIND::PUBLISHABLE:
			case MessageParameterType::KIND::STRUCT:
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

	fprintf( header, "%scomposer.buff.append( \"\\n}\", 2 );", offset );
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

void impl_generateComposeFunctionJson( FILE* header, CompositeType& s )
{
	fprintf( header, "template<typename ... Args>\n"
	"void %s_composeJson(Composer& composer, Args&& ... args)\n"
	"{\n", s.name.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForComposingJson( header, s, "\t" );


	fprintf( header, "}\n\n" );
}

void impl_generateParseFunctionJson( FILE* header, CompositeType& s )
{
	fprintf( header, "template<typename ... Args>\n"
	"void %s_parseJson(Parser& p, Args&& ... args)\n"
	"{\n", s.name.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForParsingJson( header, s, "\t" );

	fprintf( header, "}\n\n" );
}

void impl_generateParamCallBlockForComposing( FILE* header, CompositeType& s )
{
	size_t protoCount = s.protoList.size();
	if ( protoCount == 1 )
	{
		switch ( *(s.protoList.begin()) )
		{
			case CompositeType::Proto::gmq:
			{
				fprintf( header, "\tstatic_assert( ComposerT::proto == Proto::GMQ, \"this CompositeType assumes only GMQ protocol\" );\n" );
				impl_generateParamCallBlockForComposingGmq( header, s, "\t" );
				break;
			}
			case CompositeType::Proto::json:
			{
				fprintf( header, "\tstatic_assert( ComposerT::proto == Proto::JSON, \"this CompositeType assumes only JSON protocol\" );\n" );
				impl_generateParamCallBlockForComposingJson( header, s, "" );
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
		if ( s.protoList.find( CompositeType::Proto::gmq ) != s.protoList.end() )
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
				case CompositeType::Proto::gmq:
					break; // already done
				case CompositeType::Proto::json:
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
					fprintf( header, "\t}\n" );
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
			case CompositeType::Proto::gmq:
			{
				fprintf( header, "\tstatic_assert( ParserT::proto == Proto::GMQ, \"this CompositeType assumes only GMQ protocol\" );\n" );
				impl_generateParamCallBlockForParsingGmq( header, s, "\t" );
				break;
			}
			case CompositeType::Proto::json:
			{
				fprintf( header, "\tstatic_assert( ParserT::proto == Proto::JSON, \"this CompositeType assumes only JSON protocol\" );\n" );
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
		if ( s.protoList.find( CompositeType::Proto::gmq ) != s.protoList.end() )
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
				case CompositeType::Proto::gmq:
					break; // already done
				case CompositeType::Proto::json:
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
	fprintf( header, "template<class ComposerT, typename ... Args>\n"
	"void %s_compose(ComposerT& composer, Args&& ... args)\n"
	"{\n", s.name.c_str() );
	fprintf( header, "\tstatic_assert( std::is_base_of<ComposerBase, ComposerT>::value, \"Composer must be one of GmqComposer<> or JsonComposer<>\" );\n\n" );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForComposing( header, s );


	fprintf( header, "}\n\n" );
}

void impl_generateParseFunction( FILE* header, CompositeType& s )
{
	fprintf( header, "template<class ParserT, typename ... Args>\n"
	"void %s_parse(ParserT& p, Args&& ... args)\n"
	"{\n", s.name.c_str() );
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


