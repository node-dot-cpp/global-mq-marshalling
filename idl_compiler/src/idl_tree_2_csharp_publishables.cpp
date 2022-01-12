/* -------------------------------------------------------------------------------
* Copyright (c) 2022, OLogN Technologies AG
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

#include "idl_tree_2_csharp.h"
#include "idl_tree_serializer.h"

#include <set>



namespace {

	std::string csharpPub_getTypeName(CompositeType& s)
	{
		if (s.type == CompositeType::Type::discriminated_union_case)
			return fmt::format("CASE_{}", s.name);
		else
			return s.name;
	}

	const char* csharpPub_getCSharpType(MessageParameterType::KIND kind)
	{
		switch (kind)
		{
		case MessageParameterType::KIND::INTEGER:
			return "Int64";
		case MessageParameterType::KIND::UINTEGER:
			return "UInt64";
		case MessageParameterType::KIND::REAL:
			return "Double";
		case MessageParameterType::KIND::CHARACTER_STRING:
			return "String";
		default:
			assert(false);
		}
		return nullptr;
	}

	void csharpPub_generateStructInterface(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "public interface %s\n", type_name.c_str());
		fprintf(header, "{\n");


		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;
			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t%s %s { get; set; }\n", csharpPub_getCSharpType(member.type.kind), member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t%s %s { get; set; }\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t%s make_%s();\n", member.type.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				assert(false); // not implemented (yet)
				//TODO
				//switch (member.type.vectorElemKind)
				//{
				//case MessageParameterType::KIND::INTEGER:
				//	fprintf(header, "\tpublic List<Int64> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::UINTEGER:
				//	fprintf(header, "\tpublic List<UInt64> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::REAL:
				//	fprintf(header, "\tpublic List<Double> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::CHARACTER_STRING:
				//	fprintf(header, "\tpublic List<String> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::VECTOR:
				//	assert(false); // unexpected
				//	break;
				//case MessageParameterType::KIND::STRUCT:
				//case MessageParameterType::KIND::DISCRIMINATED_UNION:
				//	assert(member.type.messageIdx < root.structs.size());
				//	fprintf(header, "\tpublic List<%s> %s;\n", root.structs[member.type.messageIdx]->name.c_str(), member.name.c_str());
				//	break;
				//default:
				//	assert(false); // not implemented (yet)
				//}
				//break;
			}
			default:
				assert(false); // not implemented (yet)
			}
		}

		fprintf(header, "} // interface %s\n\n", type_name.c_str());
	}

	void csharpPub_generateStructImpl(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);


		fprintf(header, "public class %s_impl : %s\n", type_name.c_str(), type_name.c_str());
		fprintf(header, "{\n");


		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;
			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\tpublic %s %s { get; set; }\n", csharpPub_getCSharpType(member.type.kind), member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic %s %s { get; set; }\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\tpublic %s make_%s() { return new %s_impl(); }\n", member.type.name.c_str(), member.name.c_str(), member.type.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				assert(false); // not implemented (yet)
				//TODO
				//switch (member.type.vectorElemKind)
				//{
				//case MessageParameterType::KIND::INTEGER:
				//	fprintf(header, "\tpublic List<Int64> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::UINTEGER:
				//	fprintf(header, "\tpublic List<UInt64> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::REAL:
				//	fprintf(header, "\tpublic List<Double> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::CHARACTER_STRING:
				//	fprintf(header, "\tpublic List<String> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::VECTOR:
				//	assert(false); // unexpected
				//	break;
				//case MessageParameterType::KIND::STRUCT:
				//case MessageParameterType::KIND::DISCRIMINATED_UNION:
				//	assert(member.type.messageIdx < root.structs.size());
				//	fprintf(header, "\tpublic List<%s> %s;\n", root.structs[member.type.messageIdx]->name.c_str(), member.name.c_str());
				//	break;
				//default:
				//	assert(false); // not implemented (yet)
				//}
				//break;
			}
			default:
				assert(false); // not implemented (yet)
			}
		}

		fprintf(header, "} // class %s_impl\n\n", type_name.c_str());
	}

	void csharpPub_generateStructSubs(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "public class %s_subs : %s\n", type_name.c_str(), type_name.c_str());
		fprintf(header, "{\n");
		fprintf(header, "\t%s t;\n", type_name.c_str());

		fprintf(header, "\tpublic %s_subs(%s t)\n", type_name.c_str(), type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tthis.t = t;\n");
		fprintf(header, "\t}\n");

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;
			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\tpublic %s %s\n", csharpPub_getCSharpType(member.type.kind), member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return t.%s; }\n", member.name.c_str());
				fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
				fprintf(header, "\t}\n");
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic %s %s\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return new %s_subs(t.%s); }\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
				fprintf(header, "\t}\n");
				fprintf(header, "\tpublic %s make_%s() { throw new InvalidOperationException(); }\n", member.type.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				assert(false); // not implemented (yet)
				//TODO
				//switch (member.type.vectorElemKind)
				//{
				//case MessageParameterType::KIND::INTEGER:
				//	fprintf(header, "\tpublic List<Int64> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::UINTEGER:
				//	fprintf(header, "\tpublic List<UInt64> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::REAL:
				//	fprintf(header, "\tpublic List<Double> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::CHARACTER_STRING:
				//	fprintf(header, "\tpublic List<String> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::VECTOR:
				//	assert(false); // unexpected
				//	break;
				//case MessageParameterType::KIND::STRUCT:
				//case MessageParameterType::KIND::DISCRIMINATED_UNION:
				//	assert(member.type.messageIdx < root.structs.size());
				//	fprintf(header, "\tpublic List<%s> %s;\n", root.structs[member.type.messageIdx]->name.c_str(), member.name.c_str());
				//	break;
				//default:
				//	assert(false); // not implemented (yet)
				//}
				//break;
			}
			default:
				assert(false); // not implemented (yet)
			}
		}

		fprintf(header, "} // class %s_subs\n\n", type_name.c_str());
	}

	void csharpPub_generateStructPubl(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);


		fprintf(header, "public class %s_publ : %s\n", type_name.c_str(), type_name.c_str());
		fprintf(header, "{\n");
		fprintf(header, "\t%s t;\n", type_name.c_str());
		fprintf(header, "\tComposerBase composer;\n");
		fprintf(header, "\tUInt64[] address;\n");

		{
			fprintf(header, "\tenum Address { ");
			auto& mem = s.getMembers();
			for (size_t i = 0; i < mem.size(); ++i)
			{
				if (i != 0)
					fprintf(header, ", ");

				auto& it = mem[i];
				assert(it != nullptr);
				auto& member = *it;

				fprintf(header, "%s = %lu", member.name.c_str(), (unsigned long)i);
			}
			fprintf(header, " };\n");
		}

		fprintf(header, "\tpublic %s_publ(%s t, ComposerBase composer, UInt64[] baseAddr, UInt64 id)\n", type_name.c_str(), type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tthis.t = t;\n");
		fprintf(header, "\t\tthis.composer = composer;\n");
		fprintf(header, "\t\tthis.address = Base.makeAddress(baseAddr, id);\n");
		fprintf(header, "\t}\n");

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\tpublic %s %s\n", csharpPub_getCSharpType(member.type.kind), member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return t.%s; }\n", member.name.c_str());
				fprintf(header, "\t\tset\n");
				fprintf(header, "\t\t{\n");
				fprintf(header, "\t\t\tif (value != t.%s)\n", member.name.c_str());
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\tt.%s = value;\n", member.name.c_str());
				fprintf(header, "\t\t\t\tBase.composeAddressInPublishable(composer, address, (UInt64)Address.%s);\n", member.name.c_str());
				fprintf(header, "\t\t\t\tBase.publishableComposeLeafe(composer, value);\n");
				fprintf(header, "\t\t\t}\n");
				fprintf(header, "\t\t}\n");
				fprintf(header, "\t}\n");
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic %s %s\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return new %s_subs(t.%s); }\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\tset\n");
				fprintf(header, "\t\t{\n");
				fprintf(header, "\t\t\tif (value != t.%s)\n", member.name.c_str());
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\tt.%s = value;\n", member.name.c_str());
				fprintf(header, "\t\t\t\tBase.composeAddressInPublishable(composer, address, (UInt64)Address.%s);\n", member.name.c_str());
				fprintf(header, "\t\t\t\tBase.publishableComposeLeafeStructBegin(composer);\n");
				//TODO 
				fprintf(header, "\t\t\t\t%s_publ.compose(composer, value);\n", member.type.name.c_str());
				fprintf(header, "\t\t\t\tBase.publishableComposeLeafeStructEnd(composer);\n");
				fprintf(header, "\t\t\t}\n");
				fprintf(header, "\t\t}\n");
				fprintf(header, "\t}\n");

				fprintf(header, "\tpublic %s make_%s() { return t.make_%s(); }\n", member.type.name.c_str(), member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				assert(false); // not implemented (yet)
				//TODO
				//switch (member.type.vectorElemKind)
				//{
				//case MessageParameterType::KIND::INTEGER:
				//	fprintf(header, "\tpublic List<Int64> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::UINTEGER:
				//	fprintf(header, "\tpublic List<UInt64> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::REAL:
				//	fprintf(header, "\tpublic List<Double> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::CHARACTER_STRING:
				//	fprintf(header, "\tpublic List<String> %s;\n", member.name.c_str());
				//	break;
				//case MessageParameterType::KIND::VECTOR:
				//	assert(false); // unexpected
				//	break;
				//case MessageParameterType::KIND::STRUCT:
				//case MessageParameterType::KIND::DISCRIMINATED_UNION:
				//	assert(member.type.messageIdx < root.structs.size());
				//	fprintf(header, "\tpublic List<%s> %s;\n", root.structs[member.type.messageIdx]->name.c_str(), member.name.c_str());
				//	break;
				//default:
				//	assert(false); // not implemented (yet)
				//}
				//break;
			}
			default:
				assert(false); // not implemented (yet)
			}
		}

		fprintf(header, "\tpublic static void compose(ComposerBase composer, %s val)\n", type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\t// TODO\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic static bool parse(ParserBase parser, %s val)\n", type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\t// TODO\n");
		fprintf(header, "\t\treturn false;\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic static bool parse(ParserBase parser, %s val, UInt64[] addr, int offset)\n", type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\t// TODO\n");
		fprintf(header, "\t\treturn false;\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic static void parseForStateSync(ParserBase parser, %s val)\n", type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\t// TODO\n");
		fprintf(header, "\t}\n");

		fprintf(header, "} // class %s_publ\n\n", type_name.c_str());
	}

	void csharpPub_generateStruct(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		bool checked = impl_checkParamNameUniqueness(s);
		checked = impl_checkFollowingExtensionRules(s) && checked;
		if (!checked)
			throw std::exception();

		std::string name = csharpPub_getTypeName(s);

		if(s.type == CompositeType::Type::publishable)
			impl_generatePublishableCommentBlock(header, s);

		// base interface
		csharpPub_generateStructInterface(header, root, s, name);

		// default implementation
		csharpPub_generateStructImpl(header, root, s, name);

		// wrapper for subscriber
		csharpPub_generateStructSubs(header, root, s, name);

		// wrapper for publisher
		csharpPub_generateStructPubl(header, root, s, name);


		fprintf(header, "\n");

	}
}




void generateCsharpPublishables( const char* fileName, uint32_t fileChecksum, FILE* header, const std::string& metascope, const std::string& platformPrefix, const std::string& classNotifierName, Root& s )
{
	vector<CompositeType*> structsOrderedByDependency;
	orderStructsByDependency( s.structs, structsOrderedByDependency );
	

	fprintf(header, "//////////////////////////////////////////////////////////////\n");
	fprintf(header, "//\n");
	fprintf(header, "//                 Publishables\n");
	fprintf(header, "//\n");
	fprintf(header, "//////////////////////////////////////////////////////////////\n\n");

	fprintf(header,
		"namespace publishable\n"
		"{\n"
		"\n");

	//for ( auto& it : s.publishables )
	//{
	//	auto& obj_1 = it;
	//	assert( obj_1 != nullptr );
	//	assert( typeid( *(obj_1) ) == typeid( CompositeType ) );
	//	assert( obj_1->type == CompositeType::Type::publishable );
	//	generatePublishable( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))), platformPrefix, classNotifierName );
	//}

	//fprintf( header, "//===============================================================================\n" );
	//fprintf( header, "// Publishable c-structures\n" );
	//fprintf( header, "// Use them as-is or copy and edit member types as necessary\n\n" );
	for ( auto it : structsOrderedByDependency )
	{
		assert( it != nullptr );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union);
		if ( it->isStruct4Publishing )
			csharpPub_generateStruct( header, s, *it);
	}

	//for ( auto& it : s.publishables )
	//{
	//	auto& obj_1 = it;
	//	assert( obj_1 != nullptr );
	//	assert( typeid( *(obj_1) ) == typeid( CompositeType ) );
	//	assert( obj_1->type == CompositeType::Type::publishable );
	//	generatePublishableAsCStruct( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	//}
	//fprintf( header, "\n//===============================================================================\n\n" );

	//if ( !s.publishables.empty() )
	//{
	//	generateStateConcentratorFactory( header, s );
	//	fprintf( header, "\n//===============================================================================\n\n" );
	//}

	//for ( auto& it : structsOrderedByDependency )
	//{
	//	assert( it != nullptr );
	//	assert( typeid( *(it) ) == typeid( CompositeType ) );
	//	assert( it->type == CompositeType::Type::structure );
	//	if ( it->isStruct4Publishing )
	//	{
	//		impl_GeneratePublishableStructWrapper( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	//		impl_GeneratePublishableStructWrapper4Set( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	//	}
	//}

	fprintf(header, "\n} // namespace publishable\n\n");
}

