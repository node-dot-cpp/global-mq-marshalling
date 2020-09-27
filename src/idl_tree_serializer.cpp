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
		case MessageParameterType::KIND::CHARACTER_STRING: return "CHARACTER_STRING";
		case MessageParameterType::KIND::BYTE_ARRAY: return "BYTE_ARRAY";
		case MessageParameterType::KIND::BLOB: return "BLOB";
		case MessageParameterType::KIND::VECTOR: return "VECTOR";
		case MessageParameterType::KIND::MESSAGE: return "MESSAGE";
		default: assert( false );
	}
}

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

	printf( "%sMessage: name = \"%s\" %sTargets: ", offsetch, s.name.c_str(), s.isNonExtendable ? "NONEXTENDABLE " : "" );
	for ( auto t:s.protoList )
		switch ( t )
		{
			case Message::Proto::gmq: printf( "GMQ " ); break;
			case Message::Proto::json: printf( "JSON " ); break;
			default: assert( false );
		}
	printf( "(%zd parameters) {\n", s.members.size() );
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
	if ( s.kind == MessageParameterType::KIND::VECTOR )
	{
		if ( s.vectorElemKind == MessageParameterType::KIND::MESSAGE )
			printf( "%s<%s%s %s>", impl_kindToString( s.kind ), s.isNonExtendable ? "NONEXTENDABLE " : "", impl_kindToString( s.vectorElemKind ), s.messageName.c_str() );
		else
			printf( "%s<%s>", impl_kindToString( s.kind ), impl_kindToString( s.vectorElemKind ) );
	}
	else
		printf( "%s", impl_kindToString( s.kind ) );

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

void impl_CollectParamNamesFromMessage( std::set<string>& params, Message& s )
{
	for ( auto& it : s.members )
	{
		auto& obj_1 = it;
		impl_CollectParamNamesFrom__unique_ptr_MessageParameter( params, obj_1 );
	}
}

void impl_CollectParamNamesFrom__unique_ptr_Message( std::set<string>& params, unique_ptr<Message>& s ) {

	if ( s != nullptr && typeid( *(s) ) == typeid( Message ) )
	{
		impl_CollectParamNamesFromMessage( params, *(dynamic_cast<Message*>(&(*(s)))) );
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

void impl_generateSrcFile( const char* hFileName, FILE* src, const std::set<string>& params )
{
	fprintf( src, 
			"#include \"%s.h\"\n"
			"\n"
			"namespace m {\n"
			"\n", hFileName );

	// objects
	for ( auto name : params )
	{
		fprintf( src, "static const %s_Type::TypeConverter %s;\n", name.c_str(), name.c_str() );
	}
	
	fprintf( src, "\n} // namespace m\n" );
}

bool impl_checkMessageNameUniqueness(Root& s)
{
	bool ok = true;
	std::map<string, Location> names;
	for ( auto& it : s.messages )
	{
		auto ins = names.insert( std::make_pair( it->name, it->location ) );
		if ( !ins.second )
		{
			fprintf( stderr, "Message name \"%s\" has already been used, see %s : %d\n", it->name.c_str(), ins.first->second.fileName.c_str(), ins.first->second.lineNumber );
			ok = false;
		}
	}
	return ok;
}

bool impl_processMessageNamesInVectorTypes(Root& s)
{
	bool ok = true;
	for ( auto& msg : s.messages )
		for ( auto& param : msg->members )
			if ( param->type.kind == MessageParameterType::VECTOR && param->type.vectorElemKind == MessageParameterType::MESSAGE )
			{
				param->type.messageIdx = (size_t)(-1);
				for ( size_t i=0; i<s.messages.size(); ++i )
					if ( param->type.messageName == s.messages[i]->name )
					{
						param->type.messageIdx = i;
						if ( param->type.isNonExtendable && !s.messages[i]->isNonExtendable )
						{
							fprintf( stderr, "%s, line %d: message \"%s\" is not declared as NONEXTENDABLE (see message declaration at %s, line %d)\n", param->location.fileName.c_str(), param->location.lineNumber, param->type.messageName.c_str(), s.messages[i]->location.fileName.c_str(), s.messages[i]->location.lineNumber );
							ok = false;
						}
						for ( auto proto : msg->protoList )
							if ( s.messages[i]->protoList.find( proto ) == s.messages[i]->protoList.end() )
							{
								fprintf( stderr, "%s, line %d: message \"%s\" does not support all protocols of current message\n", param->location.fileName.c_str(), param->location.lineNumber, param->type.messageName.c_str() );
								fprintf( stderr, "             see declaration of current message at %s, line %d\n", msg->location.fileName.c_str(), msg->location.lineNumber );
								fprintf( stderr, "             see declaration of message \"%s\" at %s, line %d\n", param->type.messageName.c_str(), s.messages[i]->location.fileName.c_str(), s.messages[i]->location.lineNumber );
								ok = false;
							}
						break;
					}
				if ( param->type.messageIdx == (size_t)(-1) )
				{
					fprintf( stderr, "%s, line %d: message name \"%s\" not found\n", param->location.fileName.c_str(), param->location.lineNumber, param->type.messageName.c_str() );
					ok = false;
				}
			}
	return ok;
}

bool impl_checkMessageParamNameUniqueness(Message& s)
{
	bool ok = true;
	std::map<string, Location> names;
	for ( auto& it : s.members )
	{
		auto ins = names.insert( std::make_pair( it->name, it->location ) );
		if ( !ins.second )
		{
			fprintf( stderr, "Message parameter \"%s\" has already been used within this message, see %s : %d\n", it->name.c_str(), ins.first->second.fileName.c_str(), ins.first->second.lineNumber );
			ok = false;
		}
	}
	return ok;
}

void generateRoot( const char* fileName, FILE* header, FILE* src, Root& s )
{
	bool ok = impl_checkMessageNameUniqueness(s);
	ok = impl_processMessageNamesInVectorTypes(s) && ok;
	if ( !ok )
		throw std::exception();

	std::set<string> params;
	impl_CollectParamNamesFromRoot( params, s );

	impl_generateSrcFile( fileName, src, params );

	fprintf( header, "#ifndef %s_H\n"
		"#define %s_H\n"
		"\n"
		"#include <named_params_core.h>\n"
		"\n"
		"namespace m {\n\n",
		fileName, fileName );

	generateParamNameBlock( header, params );

//	fprintf( header, "Root (%zd messages) {\n", s.messages.size() );
	for ( auto& it : s.messages )
	{
		auto& obj_1 = it;
		generate__unique_ptr_Message( header, src, obj_1 );
	}

	fprintf( header, "\n"
		"} // namespace m\n"
		"\n"
		"#endif // %s_H\n",
		fileName );
}

void generate__unique_ptr_Message( FILE* header, FILE* src, unique_ptr<Message>& s ) {

	if ( s == nullptr )
	{
		fprintf( header, "// Message = <null>\n" );
	}
	else if ( typeid( *(s) ) == typeid( Message ) )
	{
		generateMessage( header, src, *(dynamic_cast<Message*>(&(*(s)))) );
	}
}

string impl_MessageNameToDefaultsNamespaceName( string name )
{
	return fmt::format( "Message_{}_defaults", name );
}

void impl_GenerateMessageDefaults( FILE* header, Message& s )
{
	int count = 0; // let's see whether we need this block at all
	for ( auto& it : s.members )
	{
		if ( it != nullptr )
		{
			MessageParameter& param = *it;
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
						break;
					case MessageParameterType::KIND::BYTE_ARRAY:
					case MessageParameterType::KIND::BLOB:
					case MessageParameterType::KIND::ENUM:
					case MessageParameterType::KIND::VECTOR:
					{
						break; // unsupported (yet)
					}
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

		++count;

		MessageParameter& param = *it;
		if ( param.type.hasDefault )
			switch ( param.type.kind )
			{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
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

void impl_generateParamTypeLIst( FILE* header, Message& s )
{
	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		++count;

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::SignedIntegralType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::UnsignedIntegralType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
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
						if ( param.type.isNonExtendable )
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						else
							fprintf( header, "\tusing arg_%d_type = NamedParameterWithType<impl::VectorOfMessageType, %s::Name>;\n", count, paramNameToNameTagType( param.name ).c_str() );
						break;
				}
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

void impl_generateParamCallBlockForComposing( FILE* header, Message& s )
{
	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		++count;

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "\timpl::composeParam<arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(arg_%d_type::nameAndTypeID, b, args...);\n", count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "\timpl::composeParam<arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(arg_%d_type::nameAndTypeID, b, args...);\n", count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), count );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "\timpl::composeParam<arg_%d_type, %s, nodecpp::string, const impl::StringLiteralForComposing*, &%s::default_%d>(arg_%d_type::nameAndTypeID, b, args...);\n", count, param.type.hasDefault ? "false" : "true", impl_MessageNameToDefaultsNamespaceName(s.name).c_str(), count, count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				break;
			default:
			{
				assert( false ); // unexpected
				break;
			}
		}
	}
}

void impl_generateParamCallBlockForParsing( FILE* header, Message& s )
{
	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		++count;

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "\timpl::parseParam<arg_%d_type, %s>(arg_%d_type::nameAndTypeID, p, args...);\n", count, param.type.hasDefault ? "false" : "true", count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "\timpl::parseParam<arg_%d_type, %s>(arg_%d_type::nameAndTypeID, p, args...);\n", count, param.type.hasDefault ? "false" : "true", count );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "\timpl::parseParam<arg_%d_type, %s>(arg_%d_type::nameAndTypeID, p, args...);\n", count, param.type.hasDefault ? "false" : "true", count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
			case MessageParameterType::KIND::VECTOR:
				break;
			default:
			{
				assert( false ); // unexpected
				break;
			}
		}
	}
}

void impl_generateMatchCountBlock( FILE* header, Message& s )
{
	fprintf( header, "\tconstexpr size_t matchCount = " );

	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );
		MessageParameter& param = *it;
			
		if ( count )
			fprintf( header, " + \n\t\t" );

		++count;

		fprintf( header, "isMatched(arg_%d_type::nameAndTypeID, Args::nameAndTypeID...)", count );
	}

	fprintf( header, ";\n" );
}

void impl_addParamStatsCheckBlock( FILE* header, Message& s )
{
	impl_generateMatchCountBlock( header, s );
	fprintf( header, 
		"\tconstexpr size_t argCount = sizeof ... (Args);\n"
		"\tif constexpr ( argCount != 0 )\n"
		"\t\tensureUniqueness(args.nameAndTypeID...);\n"
		"\tstatic_assert( argCount == matchCount, \"unexpected arguments found\" );\n\n" );
}

void impl_generateMessageCommentBlock( FILE* header, Message& s )
{
	fprintf( header, "//**********************************************************************\n" );
	fprintf( header, "// Message \"%s\" %sTargets: ", s.name.c_str(), s.isNonExtendable ? "NONEXTENDABLE " : "" );
	for ( auto t:s.protoList )
		switch ( t )
		{
			case Message::Proto::gmq: fprintf( header, "GMQ " ); break;
			case Message::Proto::json: fprintf( header, "JSON " ); break;
			default: assert( false );
		}
	fprintf( header, "(%zd parameters)\n", s.members.size() );

	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );
		++count;
		MessageParameter& param = *it;
			
		if ( param.type.kind == MessageParameterType::KIND::VECTOR )
		{
			if ( param.type.vectorElemKind == MessageParameterType::KIND::MESSAGE )
				fprintf( header, "// %d. %s<%s%s %s>", count, impl_kindToString( param.type.kind ), param.type.isNonExtendable ? "NONEXTENDABLE " : " ", impl_kindToString( param.type.vectorElemKind ), param.type.messageName.c_str() );
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

void impl_generateComposeFunction( FILE* header, Message& s )
{
	fprintf( header, "template<typename ... Args>\n"
	"void %s_compose(Buffer& b, Args&& ... args)\n"
	"{\n", s.name.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForComposing( header, s );


	fprintf( header, "}\n\n" );
}

void impl_generateParseFunction( FILE* header, Message& s )
{
	fprintf( header, "template<typename ... Args>\n"
	"void %s_parse(impl::Parser& p, Args&& ... args)\n"
	"{\n", s.name.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForParsing( header, s );

	fprintf( header, "}\n\n" );
}

void impl_generateParamCallBlockForComposingJson( FILE* header, Message& s )
{
	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		++count;

		switch ( param.type.kind )
		{
			case MessageParameterType::KIND::INTEGER:
				fprintf( header, "\timpl::json::composeParam<arg_%d_type, %s, int64_t, int64_t, (int64_t)(%lld)>(\"%s\", arg_%d_type::nameAndTypeID, b, args...);\n", count, param.type.hasDefault ? "false" : "true", (int64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf( header, "\timpl::json::composeParam<arg_%d_type, %s, uint64_t, uint64_t, (uint64_t)(%llu)>(\"%s\", arg_%d_type::nameAndTypeID, b, args...);\n", count, param.type.hasDefault ? "false" : "true", (uint64_t)(param.type.numericalDefault), param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf( header, "\timpl::json::composeParam<arg_%d_type, %s, nodecpp::string, const impl::StringLiteralForComposing*, &%s::default_%d>(\"%s\", arg_%d_type::nameAndTypeID, b, args...);\n", count, param.type.hasDefault ? "false" : "true", impl_MessageNameToDefaultsNamespaceName(s.name).c_str(), count, param.name.c_str(), count );
				break;
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
			case MessageParameterType::KIND::VECTOR:
				break;
			default:
			{
				assert( false ); // unexpected
				break;
			}
		}
	
		if ( count != s.members.size() )
			fprintf( header, "\tb.append( \",\\n  \", 4 );\n" );
	}

	fprintf( header, "\tb.appendUint8( \'\\n\' );\n" );
	fprintf( header, "\tb.appendUint8( 0 );\n" );
}

void impl_generateParamCallBlockForParsingJson( FILE* header, Message& s )
{
	fprintf( header, "\tfor ( ;; )\n\t{\n" );
	fprintf( header, "\t\tnodecpp::string key;\n" );
	fprintf( header, "\t\tp.readKey( &key );\n" );

	int count = 0;
	for ( auto& it : s.members )
	{
		assert( it != nullptr );

		MessageParameter& param = *it;
		++count;

		fprintf( header, "\t\t%s( key == \"%s\" )\n", count == 1 ? "if " : "else if ", param.name.c_str() );
		fprintf( header, "\t\t\timpl::json::parseJsonParam<arg_%d_type, false>(arg_%d_type::nameAndTypeID, p, args...);\n", count, count );
	}

	fprintf( header, 
		"\t\tp.skipSpacesEtc();\n"
		"\t\tif ( p.isComma() )\n"
		"\t\t{\n"
		"\t\t\tp.skipComma();\n"
		"\t\t\tcontinue;\n"
		"\t\t}\n"
		"\t\tif ( !p.isData() )\n"
		"\t\t\tbreak;\n"
		"\t\tthrow std::exception(); // bad format\n" );

	fprintf( header, "\t}\n" );
}

void impl_generateComposeFunctionJson( FILE* header, Message& s )
{
	fprintf( header, "template<typename ... Args>\n"
	"void %s_composeJson(Buffer& b, Args&& ... args)\n"
	"{\n", s.name.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForComposingJson( header, s );


	fprintf( header, "}\n\n" );
}

void impl_generateParseFunctionJson( FILE* header, Message& s )
{
	fprintf( header, "template<typename ... Args>\n"
	"void %s_parseJson(impl::Parser& p, Args&& ... args)\n"
	"{\n", s.name.c_str() );

	impl_generateParamTypeLIst( header, s );
	impl_addParamStatsCheckBlock( header, s );
	impl_generateParamCallBlockForParsingJson( header, s );

	fprintf( header, "}\n\n" );
}

void generateMessage( FILE* header, FILE* src, Message& s )
{
	if ( !impl_checkMessageParamNameUniqueness(s) )
		throw std::exception();

	impl_generateMessageCommentBlock( header, s );
	impl_GenerateMessageDefaults( header, s );

	if ( s.protoList.find( Message::Proto::gmq ) != s.protoList.end() )
	{
		impl_generateComposeFunction( header, s );
		impl_generateParseFunction( header, s );
	}

	if ( s.protoList.find( Message::Proto::json ) != s.protoList.end() )
	{
		impl_generateComposeFunctionJson( header, s );
		impl_generateParseFunctionJson( header, s );
	}
}


