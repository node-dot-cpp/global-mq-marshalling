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
#include "idl_tree_common.h"
#include "idl_generators.h"

namespace cplusplus
{

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

void impl_generateScopeEnum( FILE* header, Scope& scope )
{
	for ( auto msg : scope.objectList )
	{
		fprintf( header, "using %s = ::globalmq::marshalling::impl::MessageName<%lld>;\n", msg->name.c_str(), msg->numID );
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


void addLibAliasingBlock( FILE* header )
{
	fprintf( header, "// Useful aliases:\n" );
	fprintf( header, "//     (note: since clang apparently too often requires providing template arguments for aliased type ctors we use wrappers instead of type aliasing)\n" );
	fprintf( header, "using Buffer = globalmq::marshalling::Buffer;\n" );
	fprintf( header, "using FileReadBuffer = globalmq::marshalling::FileReadBuffer;\n" );
	fprintf( header, "using GmqComposer = globalmq::marshalling2::GmqComposer2<Buffer>;\n" );
	fprintf( header, "using GmqParser = globalmq::marshalling2::GmqParser2<Buffer>;\n" );
	fprintf( header, "using JsonComposer = globalmq::marshalling2::JsonComposer2<Buffer>;\n" );
	fprintf( header, "using JsonParser = globalmq::marshalling2::JsonParser2<Buffer>;\n" );
	fprintf( header, "using IComposer2 = globalmq::marshalling2::IComposer2<Buffer>;\n" );
	fprintf( header, "using IParser2 = globalmq::marshalling2::IParser2<Buffer>;\n" );
	fprintf( header, "template<typename msgID_, class LambdaHandler>\n" );
	fprintf( header, "MessageHandler<msgID_, LambdaHandler> makeMessageHandler( LambdaHandler &&lhandler ) { return globalmq::marshalling::makeMessageHandler<msgID_, LambdaHandler>(std::forward<LambdaHandler>(lhandler)); }\n" );
	fprintf( header, "template<class LambdaHandler>\n" );
	fprintf( header, "DefaultMessageHandler<LambdaHandler> makeDefaultMessageHandler( LambdaHandler &&lhandler ) { return globalmq::marshalling::makeDefaultMessageHandler<LambdaHandler>(std::forward<LambdaHandler>(lhandler)); }\n" );
	fprintf( header, "\n" );
}

void generateStateConcentratorFactory( FILE* header, Root& root )
{
	fprintf( header, "template<class InputBufferT, class ComposerT>\n" );
	fprintf( header, "class StateConcentratorFactory : public ::globalmq::marshalling::StateConcentratorFactoryBase<InputBufferT, ComposerT>\n" );
	fprintf( header, "{\n" );
	fprintf( header, "public:\n" );
	fprintf( header, "\tvirtual StateConcentratorBase<InputBufferT, ComposerT>* createConcentrator( uint64_t typeID )\n" );
	fprintf( header, "\t{\n" );
	fprintf( header, "\t\tswitch( typeID )\n" );
	fprintf( header, "\t\t{\n" );

	for ( auto& it : root.publishables )
	{
		auto& obj_1 = it;
		assert( obj_1 != nullptr );
		assert( typeid( *(obj_1) ) == typeid( CompositeType ) );
		assert( obj_1->type == CompositeType::Type::publishable );
		string idStr = std::to_string(obj_1->numID);
		fprintf( header, "\t\t\tcase %s:\n", idStr.c_str() );
		fprintf( header, "\t\t\t\treturn new %s_concentrator<InputBufferT, ComposerT>();\n", obj_1->name.c_str() );
	}
	fprintf( header, "\t\t\tdefault:\n" );
	fprintf( header, "\t\t\t\treturn nullptr;\n" );
	fprintf( header, "\t\t}\n" );
	fprintf( header, "\t}\n" );
	fprintf( header, "};\n" );
}

std::string impl_generateDiscriminatedUnionCaseStructName( CompositeType& s )
{
	assert( s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::message || s.type == CompositeType::Type::discriminated_union_case || s.type == CompositeType::Type::structure );

	if ( s.type == CompositeType::Type::message )
		return fmt::format( "MESSAGE_{}", s.name );
	else
		return fmt::format( "{}", s.name );
}

std::string impl_generateStandardCppTypeName( MessageParameterType& s )
{
	// TODO: depending on limits this could be tuned on
	switch ( s.kind )
	{
		// simple types
		case MessageParameterType::KIND::INTEGER: return "int64_t";
		case MessageParameterType::KIND::UINTEGER: return "uint64_t";
		case MessageParameterType::KIND::REAL: return "double";
		case MessageParameterType::KIND::CHARACTER_STRING: return "GMQ_COLL string";
		// named types
		case MessageParameterType::KIND::ENUM:
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			return s.name;
		// collections
		case MessageParameterType::KIND::VECTOR: 
		{
			switch ( s.vectorElemKind )
			{
				case MessageParameterType::KIND::INTEGER: return "GMQ_COLL vector<int64_t>";
				case MessageParameterType::KIND::UINTEGER: return "GMQ_COLL vector<uint64_t>";
				case MessageParameterType::KIND::REAL: return "GMQ_COLL vector<double>";
				case MessageParameterType::KIND::CHARACTER_STRING: return "GMQ_COLL vector<GMQ_COLL string>";
				case MessageParameterType::KIND::ENUM:
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					return fmt::format( "GMQ_COLL vector<{}>", s.name );
				default: assert( false ); return ""; // unexpected or not implemented
			}
		}
		case MessageParameterType::KIND::DICTIONARY: 
		{
			std::string ret;
			switch ( s.dictionaryKeyKind )
			{
				case MessageParameterType::KIND::INTEGER:
					ret = "GMQ_COLL unordered_map<int64_t,";
					break;
				case MessageParameterType::KIND::UINTEGER:
					ret = "GMQ_COLL unordered_map<uint64_t,";
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					ret = "GMQ_COLL unordered_map<GMQ_COLL string,";
					break;
				default: assert( false ); return ""; // unexpected or not implemented
					break;
			}
			switch( s.dictionaryValueKind )
			{
				case MessageParameterType::KIND::INTEGER: return ret + "int64_t>";
				case MessageParameterType::KIND::UINTEGER: return ret + "uint64_t>";
				case MessageParameterType::KIND::REAL: return ret + "double>";
				case MessageParameterType::KIND::CHARACTER_STRING: return ret + "GMQ_COLL string>";
				case MessageParameterType::KIND::ENUM:
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					return ret + fmt::format( "{}>", s.name );
				default: assert( false ); return ""; // unexpected or not implemented
			}
		}
		// unsupported (yet)
		case MessageParameterType::KIND::BYTE_ARRAY: assert( false ); return "";
		case MessageParameterType::KIND::BLOB: assert( false ); return "";
		// unexpected
		case MessageParameterType::KIND::EXTENSION: assert( false ); return "";
		case MessageParameterType::KIND::UNDEFINED: assert( false ); return "";
		default: assert( false ); return "";
	}
}

void generateStructOrDiscriminatedUnionCaseStruct( FILE* header, CompositeType& ducs, const char* offset )
{
	assert( ducs.type == CompositeType::Type::publishable || ducs.type == CompositeType::Type::message || ducs.type == CompositeType::Type::structure || ducs.type == CompositeType::Type::discriminated_union_case );

	if ( ducs.type == CompositeType::Type::message )
	{
		if ( ducs.isAlias )
		{
			fprintf( header, "namespace %s {\n", ducs.scopeName.c_str() );
			fprintf( header, "%susing %s%s = %s;\n", offset, ducs.type == CompositeType::Type::discriminated_union_case ? "Case_" : "", impl_generateDiscriminatedUnionCaseStructName( ducs ).c_str(), ducs.aliasOf.c_str() );
			fprintf( header, "} // namespace %s\n", ducs.scopeName.c_str() );
			fprintf( header, "\n" );
			return;
		}
		else
			fprintf( header, "namespace %s {\n", ducs.scopeName.c_str() );
	}

	string typeName = ducs.type == CompositeType::Type::discriminated_union_case ? "Case_" : "";
	typeName += impl_generateDiscriminatedUnionCaseStructName( ducs );
	fprintf( header, "%sstruct %s\n", offset, typeName.c_str() );
	fprintf( header, "%s{\n", offset );
	for ( auto& mbit: ducs.getMembers() )
	{
		assert( mbit != nullptr );
		auto& m = *mbit;
		assert( typeid( m ) == typeid( MessageParameter ) );
		assert( m.type.kind != MessageParameterType::KIND::UNDEFINED );
		if ( m.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;


		if ( m.type.kind == MessageParameterType::KIND::INTEGER || m.type.kind == MessageParameterType::KIND::UINTEGER ||
			m.type.kind == MessageParameterType::KIND::REAL || m.type.kind == MessageParameterType::KIND::ENUM)
		{
			fprintf( header, "%s\t%s %s = 0;\n", offset, impl_generateStandardCppTypeName( m.type ).c_str(), m.name.c_str() );
		}
		else
		{
			fprintf( header, "%s\t%s %s;\n", offset, impl_generateStandardCppTypeName( m.type ).c_str(), m.name.c_str() );
		}
	}

	fprintf( header, "%s\tbool operator==(const %s& other) const\n", offset, typeName.c_str() );
	fprintf( header, "%s\t{\n", offset );
	fprintf( header, "%s\t\treturn\n", offset );
	
	auto& members = ducs.getMembers();
	for ( size_t i = 0; i != members.size(); ++i )
	{
		auto& mem = *members[i];
		if ( mem.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;

		if(i != 0)
			fprintf( header, " &&\n" );

		fprintf( header, "%s\t\t\tthis->%s == other.%s", offset, mem.name.c_str(), mem.name.c_str() );
	}
	fprintf( header, ";\n" );
	fprintf( header, "%s\t}\n", offset );

	fprintf( header, "%s\tbool operator!=(const %s& other) const\n", offset, typeName.c_str() );
	fprintf( header, "%s\t{\n", offset );
	fprintf( header, "%s\t\treturn !this->operator==(other);\n", offset );
	fprintf( header, "%s\t}\n", offset );

	fprintf( header, "%s};\n", offset );

	if ( ducs.type == CompositeType::Type::message )
		fprintf( header, "} // namespace %s\n", ducs.scopeName.c_str() );

	fprintf( header, "\n" );
}

void generateDUnionCaseForSubscriber( FILE* header, CompositeType& ducs, const char* offset )
{
	assert( ducs.type == CompositeType::Type::discriminated_union_case );

	string typeName = ducs.type == CompositeType::Type::discriminated_union_case ? "Case_" : "";
	typeName += impl_generateDiscriminatedUnionCaseStructName( ducs );
	fprintf( header, "%sstruct %s\n", offset, typeName.c_str() );
	fprintf( header, "%s{\n", offset );
	for ( auto& mbit: ducs.getMembers() )
	{
		auto& m = *mbit;
		if ( m.type.kind == MessageParameterType::KIND::EXTENSION )
			continue;

		if ( m.type.kind == MessageParameterType::KIND::INTEGER || m.type.kind == MessageParameterType::KIND::UINTEGER ||
			m.type.kind == MessageParameterType::KIND::REAL || m.type.kind == MessageParameterType::KIND::ENUM)
			fprintf( header, "%s\t%s %s = 0;\n", offset, getSubscriberCppType( m.type ).c_str(), m.name.c_str() );
		else
			fprintf( header, "%s\t%s %s;\n", offset, getSubscriberCppType( m.type ).c_str(), m.name.c_str() );
	}

	fprintf( header, "\t\t%s() {}\n", typeName.c_str() );

	fprintf( header, "\t\t%s( const %s &other ) = delete;\n", typeName.c_str(), typeName.c_str() );
	fprintf( header, "\t\t%s& operator = ( const %s &other) = delete;\n", typeName.c_str(), typeName.c_str() );
	fprintf( header, "\t\t%s( %s&& other) noexcept = default;\n", typeName.c_str(), typeName.c_str() );
	fprintf( header, "\t\t%s& operator = ( %s&& other) noexcept = default;\n", typeName.c_str(), typeName.c_str() );

	fprintf( header, "%s};\n", offset );

	fprintf( header, "\n" );
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

void generateDiscriminatedUnionObject( FILE* header, CompositeType& du, bool isForSubscriber )
{
	assert( du.type == CompositeType::Type::discriminated_union );
	for ( auto& duit: du.getDiscriminatedUnionCases() )
	{
		assert( duit != nullptr );
		auto& cs = *duit;
		assert( typeid( cs ) == typeid( CompositeType ) );
		assert( cs.type == CompositeType::Type::discriminated_union_case );
		for ( auto& mbit: cs.getMembers() )
			mbit->duCaseParam = true;
	}

	string typeName = du.name;
	if(isForSubscriber)
		typeName += "_subscriber_base";

	fprintf( header, "class %s : public ::globalmq::marshalling::impl::%s\n", typeName.c_str(), du.isNonExtendable ? "NonextDiscriminatedUnionType" : "DiscriminatedUnionType" );

	fprintf( header, "{\n" );
	fprintf( header, "public:\n" );

	// list of cases
	fprintf( header, "\tenum Variants { " );
	std::string concatenatedNames;
	for ( auto& duit: du.getDiscriminatedUnionCases() )
	{
		assert( duit != nullptr );
		auto& cs = *duit;
		assert( typeid( cs ) == typeid( CompositeType ) );
		assert( cs.type == CompositeType::Type::discriminated_union_case );
		fprintf( header, "%s=%lld, ", cs.name.c_str(), cs.numID );
		concatenatedNames += cs.name;
		concatenatedNames += "_";
	}
	fprintf( header, "unknown };\n" );
	fprintf( header, "private:\n" );
	fprintf( header, "\tVariants v = Variants::unknown;\n" );

	// list of structures
	for ( auto& duit: du.getDiscriminatedUnionCases() )
	{
		if(isForSubscriber)
			generateDUnionCaseForSubscriber( header, *duit, "\t" );
		else
			generateStructOrDiscriminatedUnionCaseStruct( header, *duit, "\t" );
	}

	// member types and name checking block
	fprintf( header, "public:\n" );
	for ( size_t i=0; i<du.getDiscriminatedUnionCases().size(); ++i )
	{
		auto& cs = du.getDiscriminatedUnionCases()[i];
		const char* csname = cs->name.c_str();
		for ( auto& m: cs->getMembers() )
			fprintf( header, "\tusing %s = decltype( Case_%s::%s );\n", impl_discriminatedUnionCaseMemberType( *m ).c_str(), csname, m->name.c_str() );
	}
	fprintf( header, "\n" );

	// memory and its size
	std::string memName;
	fprintf( header, "private:\n" );
	if ( du.getDiscriminatedUnionCases().size() )
	{
		if ( du.getDiscriminatedUnionCases().size() == 1 )
		{
			fprintf( header, "\tstatic constexpr size_t %smemsz = sizeof( %s );\n", concatenatedNames.c_str(), concatenatedNames.c_str() );
		}
		else
		{
			std::string val = fmt::format( "sizeof( Case_{} )", du.getDiscriminatedUnionCases()[0]->name );
			for ( size_t i=1; i<du.getDiscriminatedUnionCases().size(); ++i )
				val = fmt::format( "sizeof( Case_{} ) > ( {} ) ? sizeof( Case_{} ) : ( {} )", du.getDiscriminatedUnionCases()[i]->name, val, du.getDiscriminatedUnionCases()[i]->name, val );
			fprintf( header, "\tstatic constexpr size_t %smemsz = %s;\n", concatenatedNames.c_str(), val.c_str() );
		}
		memName = fmt::format( "{}mem", concatenatedNames );
		fprintf( header, "\tuint8_t %s[%ssz];\n", memName.c_str(), memName.c_str() );
	}

	// deinitializer
	fprintf( header, "\tvoid implDeinit() {\n" );
	fprintf( header, "\t\tif ( v != Variants::unknown ) // then destruct existing value\n" );
	fprintf( header, "\t\t{\n" );
	fprintf( header, "\t\t\tswitch ( v )\n" );
	fprintf( header, "\t\t\t{\n" );
	for ( size_t i=0; i<du.getDiscriminatedUnionCases().size(); ++i )
	{
		const char* name = du.getDiscriminatedUnionCases()[i]->name.c_str();
		fprintf( header, "\t\t\t\tcase Variants::%s: reinterpret_cast<Case_%s*>( %s ) -> ~Case_%s(); break;\n", name, name, memName.c_str(), name );
	}
	fprintf( header, "\t\t\t\tdefault: break;\n" );
	fprintf( header, "\t\t\t}\n" );
	fprintf( header, "\t\t\tv = Variants::unknown;\n" );
	fprintf( header, "\t\t}\n" );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );

	// copiers
	if(!isForSubscriber)
	{
		fprintf( header, "\tvoid implCopyFrom( const %s& other ) {\n", typeName.c_str() );
		fprintf( header, "\t\tif ( v != other.v )\n" );
		fprintf( header, "\t\t\timplDeinit();\n" );
		fprintf( header, "\t\tswitch ( other.v )\n" );
		fprintf( header, "\t\t{\n" );
		for ( size_t i=0; i<du.getDiscriminatedUnionCases().size(); ++i )
		{
			const char* name = du.getDiscriminatedUnionCases()[i]->name.c_str();
			fprintf( header, "\t\t\tcase Variants::%s:\n", name );
			fprintf( header, "\t\t\t\tnew ( %s ) Case_%s( *reinterpret_cast<const Case_%s*>( other.%s ) );\n", memName.c_str(), name, name, memName.c_str() );
			fprintf( header, "\t\t\t\tbreak;\n" );
		}
		fprintf( header, "\t\t\tcase Variants::unknown: break;\n" );
		fprintf( header, "\t\t}\n" );
		fprintf( header, "\t\tv = other.v;\n" );
		fprintf( header, "\t}\n" );
		fprintf( header, "\n" );
	}

	fprintf( header, "\tvoid implMoveFrom( %s&& other ) {\n", typeName.c_str() );
	fprintf( header, "\t\tif ( v != other.v )\n" );
	fprintf( header, "\t\t\timplDeinit();\n" );
	fprintf( header, "\t\tswitch ( other.v )\n" );
	fprintf( header, "\t\t{\n" );
	for ( size_t i=0; i<du.getDiscriminatedUnionCases().size(); ++i )
	{
		const char* name = du.getDiscriminatedUnionCases()[i]->name.c_str();
		fprintf( header, "\t\t\tcase Variants::%s:\n", name );
		fprintf( header, "\t\t\t\tnew ( %s ) Case_%s( std::move( *reinterpret_cast<Case_%s*>( other.%s ) ) );\n", memName.c_str(), name, name, memName.c_str() );
		fprintf( header, "\t\t\t\tbreak;\n" );
	}
	fprintf( header, "\t\t\tcase Variants::unknown: break;\n" );
	fprintf( header, "\t\t}\n" );
	fprintf( header, "\t\tv = other.v;\n" );
	fprintf( header, "\t\tother.v = Variants::unknown;\n" );
	fprintf( header, "\t}\n" );
	fprintf( header, "\n" );
	fprintf( header, "public:\n" );

	// ctors, drtors...
	fprintf( header, "\t%s() {}\n", typeName.c_str() );

	if(isForSubscriber)
	{
		fprintf( header, "\t%s( const %s &other ) = delete;\n", typeName.c_str(), typeName.c_str() );
		fprintf( header, "\t%s& operator = ( const %s &other) = delete;\n", typeName.c_str(), typeName.c_str() );
	}
	else
	{
		fprintf( header, "\t%s( const %s &other ) { implCopyFrom( other ); }\n", typeName.c_str(), typeName.c_str() );
		fprintf( header, "\t%s& operator = ( const %s &other) { implCopyFrom( other ); return *this; }\n", typeName.c_str(), typeName.c_str() );
	}

	fprintf( header, "\t%s( %s&& other) noexcept { implMoveFrom( std::move( other ) ); }\n", typeName.c_str(), typeName.c_str() );
	fprintf( header, "\t%s& operator = ( %s&& other) noexcept { implMoveFrom( std::move( other ) ); return *this; }\n", typeName.c_str(), typeName.c_str() );

	fprintf( header, "\tvirtual ~%s() { implDeinit(); }\n", typeName.c_str() );

	fprintf( header, "\tVariants currentVariant() const { return v; }\n" );

	if ( du.getDiscriminatedUnionCases().empty() )
	{
		fprintf( header, "};\n" );
		return;
	}

	// initAS()
	fprintf( header, "\tvoid initAs( uint64_t v_ ) {\n" );
	fprintf( header, "\t\timplDeinit();\n" );
	fprintf( header, "\t\tswitch ( v_ ) // init for a new type\n" );
	fprintf( header, "\t\t{\n" );
	for ( size_t i=0; i<du.getDiscriminatedUnionCases().size(); ++i )
	{
		const char* name = du.getDiscriminatedUnionCases()[i]->name.c_str();
		fprintf( header, "\t\t\tcase Variants::%s: new ( %s ) Case_%s; v = static_cast<Variants>(v_); break;\n", name, memName.c_str(), name );
	}
	fprintf( header, "\t\t\tdefault: v = Variants::unknown; break;\n" );
	fprintf( header, "\t\t}\n" );
	fprintf( header, "\t}\n" );

	for ( auto& duit: du.getDiscriminatedUnionCases() )
	{
		fprintf( header, "\n" );
		assert( duit != nullptr );
		auto& cs = *duit;
		fprintf( header, "\t// IDL CASE %s:\n", cs.name.c_str() );

		for ( auto& mbit: cs.getMembers() )
		{
			assert( mbit != nullptr );
			auto& m = *mbit;
			assert( typeid( m ) == typeid( MessageParameter ) );

			fprintf( header, "\t%s& %s() {\n", impl_discriminatedUnionCaseMemberType( m ).c_str(), m.name.c_str() );
			fprintf( header, "\t\tif ( v != Variants::%s )\n", cs.name.c_str() );
			fprintf( header, "\t\t\tthrow std::exception();\n" );
			fprintf( header, "\t\treturn reinterpret_cast<Case_%s*>( %s )->%s;\n", cs.name.c_str(), memName.c_str(), m.name.c_str() );
			fprintf( header, "\t}\n" );

			fprintf( header, "\tconst %s& %s() const {\n", impl_discriminatedUnionCaseMemberType( m ).c_str(), m.name.c_str() );
			fprintf( header, "\t\tif ( v != Variants::%s )\n", cs.name.c_str() );
			fprintf( header, "\t\t\tthrow std::exception();\n" );
			fprintf( header, "\t\treturn reinterpret_cast<const Case_%s*>( %s )->%s;\n", cs.name.c_str(), memName.c_str(), m.name.c_str() );
			fprintf( header, "\t}\n" );

			
			fprintf( header, "\t\n" );
		}
	}

	if(!isForSubscriber)
	{
		fprintf( header, "\tbool operator==(const %s& other) const\n", typeName.c_str() );
		fprintf( header, "\t{\n");
		fprintf( header, "\t\tif(this->currentVariant() != other.currentVariant())\n" );
		fprintf( header, "\t\t\treturn false;\n" );
		
		fprintf( header, "\t\tswitch ( this->currentVariant() )\n" );
		fprintf( header, "\t\t{\n" );
		for ( size_t i=0; i<du.getDiscriminatedUnionCases().size(); ++i )
		{
			const char* name = du.getDiscriminatedUnionCases()[i]->name.c_str();
			fprintf( header, "\t\t\tcase Variants::%s:\n", name );
			fprintf( header, "\t\t\t\treturn reinterpret_cast<const Case_%s*>( %s )->operator==(*reinterpret_cast<const Case_%s*>( other.%s ));\n", name, memName.c_str(), name, memName.c_str() );
		}
		fprintf( header, "\t\t\tcase Variants::unknown:\n" );
		fprintf( header, "\t\t\t\treturn true;\n" );
		fprintf( header, "\t\t\tdefault:\n" );
		fprintf( header, "\t\t\t\treturn false;\n" );
		fprintf( header, "\t\t}\n" );
		fprintf( header, "\t\treturn false;\n" );

		fprintf( header, "\t}\n" );

		fprintf( header, "\tbool operator!=(const %s& other) const\n", typeName.c_str() );
		fprintf( header, "\t{\n" );
		fprintf( header, "\t\treturn !this->operator==(other);\n" );
		fprintf( header, "\t}\n" );
	}


	fprintf( header, "};\n\n" );
}


void generateRoot( FILE* header, Root& s, const GenerationConfig& config)
{
	std::set<string> msgParams;
	std::set<string> msgCaseParams;
	impl_CollectMessageParamNamesFromRoot( msgParams, msgCaseParams, s );

	std::set<string> publishableMembers;
	std::set<string> publishableCaseMembers;
	impl_CollectPublishableMemberNamesFromRoot( publishableMembers, publishableCaseMembers, s );

	fprintf( header, "#ifndef %s_%08x_guard\n"
		"#define %s_%08x_guard\n"
		"\n"
		"#include <marshalling.h>\n"
		"#include <publishable_impl.h>\n"
		"#include <globalmq/marshalling2/marshalling2.h>\n"
		"using namespace globalmq::marshalling;\n"
		"namespace %s {\n\n"
		"#ifdef METASCOPE_%s_ALREADY_DEFINED\n"
		"#error metascope must reside in a single idl file\n"
		"#endif\n"
		"#define METASCOPE_%s_ALREADY_DEFINED\n\n",
		config.fileName.c_str(), config.fileChecksum, config.fileName.c_str(), config.fileChecksum,
		config.metascope.c_str(), config.metascope.c_str(), config.metascope.c_str());

	addLibAliasingBlock( header );

	impl_insertScopeList( header, s );

	generateMessageParamNameBlock( header, msgParams );

	vector<CompositeType*> structsOrderedByDependency;
	std::unordered_set<size_t> collElementTypes;
	orderStructsByDependency( s.structs, structsOrderedByDependency, collElementTypes );

	structsOrderedByDependency = orderStructsByDependency2(s.structs);

	for ( auto it : structsOrderedByDependency )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
	}

	for ( auto& it : s.messages )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::message );
	}

	for ( auto& it : s.publishables )
	{
		assert( it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert( it->type == CompositeType::Type::publishable );
	}

	for ( auto& scope : s.scopes )
	{
		for ( auto it : scope->objectList )
		{
			assert( it != nullptr );
			assert( typeid( *(it) ) == typeid( CompositeType ) );
			assert( it->type == CompositeType::Type::message );
		}
	}

	fprintf( header, "//===============================================================================\n" );
	fprintf( header, "// Dependency ordering\n" );
	for ( auto it : structsOrderedByDependency )
		fprintf( header, "// %s\n", it->name.c_str() );


	//// data structures ////

	fprintf( header, "//===============================================================================\n" );
	fprintf( header, "\n" );
	fprintf( header, "namespace structures {\n" );
	fprintf( header, "\n" );

	for ( auto it: structsOrderedByDependency )
	{
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->type == CompositeType::Type::discriminated_union )
			fprintf( header, "class %s;\n", it->name.c_str() );
		else
			fprintf( header, "struct %s;\n", it->name.c_str() );
	}

	fprintf( header, "\n" );
	for ( auto it : structsOrderedByDependency )
	{
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		if ( it->type == CompositeType::Type::structure )
			generateStructOrDiscriminatedUnionCaseStruct( header, *it, "" );
		else
			generateDiscriminatedUnionObject( header, *it );
	}

	for ( auto& it : s.messages )
	{
		generateStructOrDiscriminatedUnionCaseStruct( header, *it, "" );
	}

	for ( auto& it : s.publishables )
	{
		generateStructOrDiscriminatedUnionCaseStruct( header, *it, "" );
	}

	fprintf( header, "\n" );
	fprintf( header, "} // namespace structures\n" );
	fprintf( header, "\n//===============================================================================\n\n" );

	//// publishables forward decls ////

	for ( auto& it : structsOrderedByDependency )
	{
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union );
		impl_generateHelperForwardDeclaration( header, s, *it );
		if ( it->isStruct4Publishing )
		{
			impl_GeneratePublishableStructWrapperForwardDeclaration( header, s, *it );
			impl_GeneratePublishableStructWrapper4SetForwardDeclaration( header, s, *it );
			fprintf( header, "\n" );
		}
	}

	fprintf( header, "\n" );

	//// publishables declarations ////

	for ( auto it : structsOrderedByDependency )
	{
		impl_generateHelperDeclaration( header, s, *it, config );
		if ( it->isStruct4Publishing )
			generatePublishableDeclaration2( header, s, *it, config );
	}

	for ( auto& it : s.publishables )
	{
		impl_generateHelperDeclaration( header, s, *it, config );
		generatePublishableDeclaration2( header, s, *it, config );
	}

	//// publishables definitions ////

	for ( auto it : structsOrderedByDependency )
	{
		impl_generateHelperDefinition( header, s, *it, config );
		if ( it->isStruct4Publishing )
		{
			generatePublishableDefinition2(header, s, *it, config);
			impl_GeneratePublishableStructWrapper( header, s, *it );
			impl_GeneratePublishableStructWrapper4Set( header, s, *it );
		}
	}


	for ( auto& it : s.publishables )
	{
		impl_generateHelperDefinition( header, s, *it, config );
		generatePublishableDefinition2(header, s, *it, config);
	}

	fprintf( header, "\n//===============================================================================\n\n" );

	//// messages ////

	for ( auto& scope : s.scopes )
	{
		fprintf( header, "namespace %s {\n\n", scope->name.c_str() );

		impl_generateScopeEnum( header, *scope );

		for ( auto it : scope->objectList )
		{
			assert( it->type == CompositeType::Type::message );
			if ( !it->isAlias )
			{
				impl_generateHelperDeclaration( header, s, *it, config );
				impl_generateHelperDefinition( header, s, *it, config );
			}
		}

		for(auto& each : config.parserNames)
			impl_generateScopeHandler( header, *scope, each);

		for (auto& each : config.composerNames)
			impl_generateScopeComposerForwardDeclaration( header, *scope, each);

		for ( auto it : scope->objectList )
		{
			assert( it->type == CompositeType::Type::message );
			if ( !it->isAlias )
				generateMessage( header, s, *it, it->name, config );
			else
				generateMessageAlias( header, s, *it, config );
		}

		for (auto& each : config.composerNames)
			impl_generateScopeComposer( header, *scope, each);

		fprintf( header, "} // namespace %s \n\n", scope->name.c_str() );
	}

	for ( auto& it : s.publishables )
	{
		assert( it->type == CompositeType::Type::publishable );
		generatePublishable( header, s, *it, config );
	}

	if ( !s.publishables.empty() )
	{
		generateStateConcentratorFactory( header, s );
		fprintf( header, "\n//===============================================================================\n\n" );
	}

	fprintf( header, "\n"
		"} // namespace %s\n"
		"\n"
		"#endif // %s_%08x_guard\n",
		config.metascope.c_str(),
		config.fileName.c_str(), config.fileChecksum );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace cplusplus

void generateCplusplus( FILE* header, Root& s, GenerationConfig config)
{
	if(config.composerNames.empty())
	{
		config.composerNames.push_back("GmqComposer");
		config.composerNames.push_back("JsonComposer");
	}

	if(config.parserNames.empty())
	{
		config.parserNames.push_back("GmqParser");
		config.parserNames.push_back("JsonParser");
	}	

	cplusplus::generateRoot(header, s, config);
}
