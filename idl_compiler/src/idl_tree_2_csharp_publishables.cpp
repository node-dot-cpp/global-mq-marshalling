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

	const char* csharpPub_getComposeMethod(MessageParameterType::KIND kind)
	{
		switch (kind)
		{
		case MessageParameterType::KIND::INTEGER:
			return "composeInteger";
		case MessageParameterType::KIND::UINTEGER:
			return "composeUnsigned";
		case MessageParameterType::KIND::REAL:
			return "composeReal";
		case MessageParameterType::KIND::CHARACTER_STRING:
			return "composeString";
		default:
			assert(false);
		}
		return nullptr;
	}

	const char* csharpPub_getParseMethod(MessageParameterType::KIND kind)
	{
		switch (kind)
		{
		case MessageParameterType::KIND::INTEGER:
			return "Integer";
		case MessageParameterType::KIND::UINTEGER:
			return "Unsigned";
		case MessageParameterType::KIND::REAL:
			return "Real";
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
				switch (member.type.vectorElemKind)
				{
					case MessageParameterType::KIND::INTEGER:
					case MessageParameterType::KIND::UINTEGER:
					case MessageParameterType::KIND::REAL:
					case MessageParameterType::KIND::CHARACTER_STRING:
						fprintf(header, "\tIList<%s> %s { get; set; }\n", csharpPub_getCSharpType(member.type.vectorElemKind), member.name.c_str());
						fprintf(header, "\tIList<%s> make_%s();\n", csharpPub_getCSharpType(member.type.vectorElemKind), member.name.c_str());
						break;
					case MessageParameterType::KIND::STRUCT:
					case MessageParameterType::KIND::DISCRIMINATED_UNION:
					{
						assert(member.type.messageIdx < root.structs.size());
						const std::string type_name = root.structs[member.type.messageIdx]->name;
						fprintf(header, "\tIList<%s> %s { get; set; }\n", type_name.c_str(), member.name.c_str());
						fprintf(header, "\tIList<%s> make_%s();\n", type_name.c_str(), member.name.c_str());
						fprintf(header, "\t%s make_%s_element();\n", type_name.c_str(), member.name.c_str());
						break;
					}
				default:
					assert(false); // not implemented (yet)
				}
				break;
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

		fprintf(header, "public class %s_impl : %s, IEquatable<%s_impl>\n", type_name.c_str(), type_name.c_str(), type_name.c_str());
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
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = csharpPub_getCSharpType(member.type.vectorElemKind);
					fprintf(header, "\tpublic IList<%s> %s { get; set; }\n", elem_type_name, member.name.c_str());
					fprintf(header, "\tpublic IList<%s> make_%s() { return new List<%s>(); }\n", elem_type_name, member.name.c_str(), elem_type_name);
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.messageIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.messageIdx]->name.c_str();
					fprintf(header, "\tpublic IList<%s> %s { get; set; }\n", elem_type_name, member.name.c_str());
					fprintf(header, "\tpublic IList<%s> make_%s() { return new List<%s>(); }\n", elem_type_name, member.name.c_str(), elem_type_name);
					fprintf(header, "\tpublic %s make_%s_element() { return new %s_impl(); }\n", elem_type_name, member.name.c_str(), elem_type_name);
					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}
		}

		generateCsharpStructEqualsMethod(header, s, type_name + "_impl");

		fprintf(header, "} // class %s_impl\n\n", type_name.c_str());
	}

	void csharpPub_generateAddressEnum(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);
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

	void csharpPub_generateParseStateSync(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "\tpublic static void parseForStateSync(IPublishableParser parser, %s t)\n", type_name.c_str());
		fprintf(header, "\t{\n");

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;


			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\t\tt.%s = parser.parseInteger(\"%s\");\n", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\t\tt.%s = parser.parseUnsigned(\"%s\");\n", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "\t\tt.%s = parser.parseReal(\"%s\");\n", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\tt.%s = parser.parseString(\"%s\");\n", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\tparser.parsePublishableStructBegin(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\tt.%s = t.make_%s();\n", member.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t%s_subs.parseForStateSync(parser, t.%s);\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\tparser.parsePublishableStructEnd();\n");
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\tt.%s = t.make_%s();\n", member.name.c_str(), member.name.c_str());
					fprintf(header, "\t\tparser.parseSimpleVector(\"%s\", Publishable.makeParser(t.%s));\n", member.name.c_str(), member.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.messageIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.messageIdx]->name.c_str();

					fprintf(header, "\t\tt.%s = t.make_%s();\n", member.name.c_str(), member.name.c_str());
					fprintf(header, "\t\tparser.parseVector(\"%s\", (IPublishableParser parser, int index) =>\n", member.name.c_str());
					fprintf(header, "\t\t\t{\n");
					fprintf(header, "\t\t\t\tparser.parseStructBegin();\n");
					fprintf(header, "\t\t\t\t%s val = t.make_%s_element();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t%s_subs.parseForStateSync(parser, val);\n", elem_type_name);
					fprintf(header, "\t\t\t\tt.%s.Add(val);\n", member.name.c_str());
					fprintf(header, "\t\t\t\tparser.parseStructEnd();\n");
					fprintf(header, "\t\t\t}\n");
					fprintf(header, "\t\t);\n");
					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}
		}
		fprintf(header, "\t}\n");
	}

	void csharpPub_generateParse1(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "\tpublic static bool parse(IPublishableParser parser, %s t)\n", type_name.c_str());
		fprintf(header, "\t{\n");

		fprintf(header, "\t\tbool changed = false;\n");

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;

			fprintf(header, "\t\t{\n");

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\t\t\tInt64 newVal = parser.parseInteger(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\t\tif(newVal != t.%s)\n", member.name.c_str());
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\tt.%s = newVal;\n", member.name.c_str());
				fprintf(header, "\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t}\n");
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\t\t\tUInt64 newVal = parser.parseUnsigned(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\t\tif(newVal != t.%s)\n", member.name.c_str());
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\tt.%s = newVal;\n", member.name.c_str());
				fprintf(header, "\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t}\n");
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "\t\t\tDouble newVal = parser.parseReal(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\t\tif(newVal != t.%s)\n", member.name.c_str());
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\tt.%s = newVal;\n", member.name.c_str());
				fprintf(header, "\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t}\n");
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\t\tString newVal = parser.parseString(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\t\tif(newVal != t.%s)\n", member.name.c_str());
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\tt.%s = newVal;\n", member.name.c_str());
				fprintf(header, "\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t}\n");
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\tparser.parsePublishableStructBegin(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\t\tbool currentChanged = %s_subs.parse(parser, t.%s);\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\tparser.parsePublishableStructEnd();\n");
				fprintf(header, "\t\t\tif(currentChanged)\n");
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t\t\t// TODO\n");
				fprintf(header, "\t\t\t}\n");
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = csharpPub_getCSharpType(member.type.vectorElemKind);

					fprintf(header, "\t\tIList<%s> newVal = t.make_%s();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\tparser.parseSimpleVector(\"%s\", Publishable.makeParser(newVal));\n", member.name.c_str());
					fprintf(header, "\t\tif(!Enumerable.SequenceEqual(newVal, t.%s))\n", member.name.c_str());
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\tt.%s = newVal;\n", member.name.c_str());
					fprintf(header, "\t\t\tchanged = true;\n");
					fprintf(header, "\t\t}\n");

					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.messageIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.messageIdx]->name.c_str();

					fprintf(header, "\t\tIList<%s> newVal = t.make_%s();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\tparser.parseVector(\"%s\", (IPublishableParser parser, int index) =>\n", member.name.c_str());
					fprintf(header, "\t\t\t{\n");
					fprintf(header, "\t\t\t\tparser.parseStructBegin();\n");
					fprintf(header, "\t\t\t\t%s val = t.make_%s_element();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t%s_subs.parseForStateSync(parser, val);\n", elem_type_name);
					fprintf(header, "\t\t\t\tt.%s.Add(val);\n", member.name.c_str());
					fprintf(header, "\t\t\t\tparser.parseStructEnd();\n");
					fprintf(header, "\t\t\t}\n");
					fprintf(header, "\t\t);\n");
					fprintf(header, "\t\tif(!Enumerable.SequenceEqual(newVal, t.%s))\n", member.name.c_str());
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\tt.%s = newVal;\n", member.name.c_str());
					fprintf(header, "\t\t\tchanged = true;\n");
					fprintf(header, "\t\t}\n");
					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}
			fprintf(header, "\t\t}\n");
		}

		fprintf(header, "\t\treturn changed;\n");
		fprintf(header, "\t}\n");
	}

	void csharpPub_generateParse2(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "\tpublic static bool parse(IPublishableParser parser, %s t, UInt64[] addr, int offset)\n", type_name.c_str());
		fprintf(header, "\t{\n");

		fprintf(header, "\t\tbool changed = false;\n");
		fprintf(header, "\t\tswitch ((Address)addr[offset])\n");
		fprintf(header, "\t\t{\n");

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;


			fprintf(header, "\t\t\tcase Address.%s:\n", member.name.c_str());
			fprintf(header, "\t\t\t{\n");

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* type_name = csharpPub_getCSharpType(member.type.kind);
				const char* parse_method = csharpPub_getParseMethod(member.type.kind);

				fprintf(header, "\t\t\t\tif(addr.Length != offset + 1)\n");
				fprintf(header, "\t\t\t\t\tthrow new Exception();\n");
				fprintf(header, "\t\t\t\t%s newVal = parser.parse%s(\"value\");\n", type_name, parse_method);
				fprintf(header, "\t\t\t\tif(newVal != t.%s)\n", member.name.c_str());
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tt.%s = newVal;\n", member.name.c_str());
				fprintf(header, "\t\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t\t}\n");
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\t\tbool currentChanged = false;\n");
				fprintf(header, "\t\t\t\tif(addr.Length == offset + 1) // full element replace\n");
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tparser.parsePublishableStructBegin(\"value\");\n");
				fprintf(header, "\t\t\t\t\tcurrentChanged = %s_subs.parse(parser, t.%s);\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\t\t\tparser.parsePublishableStructEnd();\n");
				fprintf(header, "\t\t\t\t}\n");
				fprintf(header, "\t\t\t\telse if(addr.Length > offset + 1) // let child continue parsing\n");
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tcurrentChanged = %s_subs.parse(parser, t.%s, addr, offset + 1);\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\t\t}\n");
				fprintf(header, "\t\t\t\telse\n");
				fprintf(header, "\t\t\t\t\tthrow new Exception();\n\n");

				fprintf(header, "\t\t\t\tif(currentChanged)\n");
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t\t\t// TODO\n");
				fprintf(header, "\t\t\t\t}\n");

				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = csharpPub_getCSharpType(member.type.vectorElemKind);
					const char* parse_method = csharpPub_getParseMethod(member.type.vectorElemKind);

					fprintf(header, "\t\t\t\tbool currentChanged = false;\n");
					fprintf(header, "\t\t\t\tif(addr.Length == offset + 1) // full vector replace\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tIList<%s> newVal = t.make_%s();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\tparser.parseSimpleVector(\"value\", Publishable.makeParser(newVal));\n");
					fprintf(header, "\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, t.%s))\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\tt.%s = newVal;\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tcurrentChanged = true;\n");
					fprintf(header, "\t\t\t\t\t}\n");

					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tint index = (int)addr[offset + 1];\n");
					fprintf(header, "\t\t\t\t\tcurrentChanged = PublisherVectorWrapper<%s>.parseVectorActionSimple(parser, t.%s, index,\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t(IPublishableParser parser) => { return parser.parse%s(\"value\"); });\n", parse_method);
					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse // simple type can't handle deeper address\n");
					fprintf(header, "\t\t\t\t\tthrow new Exception();\n\n");

					fprintf(header, "\t\t\t\tif(currentChanged)\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tchanged = true;\n");
					fprintf(header, "\t\t\t\t\t// TODO\n");
					fprintf(header, "\t\t\t\t}\n");

					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.messageIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.messageIdx]->name.c_str();

					fprintf(header, "\t\t\t\tbool currentChanged = false;\n");
					fprintf(header, "\t\t\t\tif(addr.Length == offset + 1) // full vector replace\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tIList<%s> newVal = t.make_%s();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\tparser.parseVector(\"%s\",\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t(IPublishableParser parser, int index) =>\n");
					fprintf(header, "\t\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\t\tparser.parseStructBegin();\n");
					fprintf(header, "\t\t\t\t\t\t\t%s val = t.make_%s_element();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t\t%s_subs.parseForStateSync(parser, val);\n", elem_type_name);
					fprintf(header, "\t\t\t\t\t\t\tt.%s.Add(val);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t\tparser.parseStructEnd();\n");
					fprintf(header, "\t\t\t\t\t\t}\n");
					fprintf(header, "\t\t\t\t\t);\n");

					fprintf(header, "\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, t.%s))\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\tt.%s = newVal;\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tcurrentChanged = true;\n");
					fprintf(header, "\t\t\t\t\t}\n");

					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tint index = (int)addr[offset + 1];\n");
					fprintf(header, "\t\t\t\t\tcurrentChanged = PublisherVectorWrapper<%s>.parseVectorActionSimple(parser, t.%s, index,\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t(IPublishableParser parser) =>\n");
					fprintf(header, "\t\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\t\t%s newVal = t.make_%s_element();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t\tparser.parsePublishableStructBegin(\"value\");\n");
					fprintf(header, "\t\t\t\t\t\t\t%s_subs.parseForStateSync(parser, newVal);\n", member.type.name.c_str());
					fprintf(header, "\t\t\t\t\t\t\tparser.parsePublishableStructEnd();\n");
					fprintf(header, "\t\t\t\t\t\t\treturn newVal;\n");
					fprintf(header, "\t\t\t\t\t\t}\n");
					fprintf(header, "\t\t\t\t\t);\n");
					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse if(addr.Length > offset + 2) // let child continue parsing\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tint index = (int)addr[offset + 1];\n");
					fprintf(header, "\t\t\t\t\tcurrentChanged = %s_subs.parse(parser, t.%s[index], addr, offset + 2);\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse\n");
					fprintf(header, "\t\t\t\t\tthrow new Exception();\n\n");

					fprintf(header, "\t\t\t\tif(currentChanged)\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tchanged = true;\n");
					fprintf(header, "\t\t\t\t\t// TODO\n");
					fprintf(header, "\t\t\t\t}\n");


					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}
			fprintf(header, "\t\t\t}\n");
			fprintf(header, "\t\t\tbreak;\n");

		}

		fprintf(header, "\t\t\tdefault:\n");
		fprintf(header, "\t\t\t\tthrow new Exception();\n");

		fprintf(header, "\t\t}\n");
		fprintf(header, "\t\treturn changed;\n");
		fprintf(header, "\t}\n");
	}

	void csharpPub_generateStateSubscriberBase(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);

		fprintf(header, "\tpublic String stateSubscriberName() { return \"%s\"; }\n", s.name.c_str());
		fprintf(header, "\tpublic UInt64 stateTypeID() { return %lld; }\n", s.numID);

		fprintf(header, "\tpublic void applyGmqMessageWithUpdates(IPublishableParser parser) { applyMessageWithUpdates(parser); }\n");
		fprintf(header, "\tpublic void applyJsonMessageWithUpdates(IPublishableParser parser) { applyMessageWithUpdates(parser); }\n");
		fprintf(header, "\tpublic void applyGmqStateSyncMessage(IPublishableParser parser) { applyStateSyncMessage(parser); }\n");
		fprintf(header, "\tpublic void applyJsonStateSyncMessage(IPublishableParser parser) { applyStateSyncMessage(parser); }\n");

		fprintf(header, "\tpublic void applyMessageWithUpdates(IPublishableParser parser)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tparser.parseStateUpdateMessageBegin();\n");
		fprintf(header, "\t\tUInt64[] addr = null;\n");
		fprintf(header, "\t\twhile(parser.parseAddress(ref addr))\n");
		fprintf(header, "\t\t{\n");
		fprintf(header, "\t\t\t%s_subs.parse(parser, this.t, addr, 0);\n", type_name.c_str());
		fprintf(header, "\t\t\tparser.parseAddressEnd();\n");
		fprintf(header, "\t\t\taddr = null;\n");
		fprintf(header, "\t\t}\n");
		fprintf(header, "\t\tparser.parseStateUpdateMessageEnd();\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic void applyStateSyncMessage(IPublishableParser parser)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tparser.parseStructBegin();\n");
		fprintf(header, "\t\t%s_subs.parseForStateSync(parser, this.t);\n", type_name.c_str());
		fprintf(header, "\t\tparser.parseStructEnd();\n");
		fprintf(header, "\t}\n");
	}

	void csharpPub_generateStructSubs(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "public class %s_subs : %s", type_name.c_str(), type_name.c_str());

		if(s.type == CompositeType::Type::publishable)
			fprintf(header, ", StateSubscriberBase");

		fprintf(header, "\n");

		fprintf(header, "{\n");
		fprintf(header, "\t%s t;\n", type_name.c_str());

		csharpPub_generateAddressEnum(header, s);

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
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = csharpPub_getCSharpType(member.type.vectorElemKind);
					fprintf(header, "\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t{\n");
					fprintf(header, "\t\tget { return new SubscriberVectorWrapper<%s>(t.%s); }\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
					fprintf(header, "\t}\n");
					fprintf(header, "\tpublic IList<%s> make_%s() { throw new InvalidOperationException(); }\n", elem_type_name, member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.messageIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.messageIdx]->name.c_str();
					fprintf(header, "\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t{\n");
					fprintf(header, "\t\tget { return new SubscriberVectorWrapper<%s>(t.%s); }\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
					fprintf(header, "\t}\n");
					fprintf(header, "\tpublic IList<%s> make_%s() { throw new InvalidOperationException(); }\n", elem_type_name, member.name.c_str());
					fprintf(header, "\tpublic %s make_%s_element() { throw new InvalidOperationException(); }\n", elem_type_name, member.name.c_str());
					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}
		}

		csharpPub_generateParseStateSync(header, root, s, type_name);
		csharpPub_generateParse1(header, root, s, type_name);
		csharpPub_generateParse2(header, root, s, type_name);

		if (s.type == CompositeType::Type::publishable)
			csharpPub_generateStateSubscriberBase(header, s, type_name);

		fprintf(header, "} // class %s_subs\n\n", type_name.c_str());
	}

	void csharpPub_generateCompose(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "\tpublic static void compose(IPublishableComposer composer, %s t)\n", type_name.c_str());
		fprintf(header, "\t{\n");

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;

			if(i!= 0)
				fprintf(header, "true);\n");


			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\tcomposer.%s(\"%s\", t.%s, ", csharpPub_getComposeMethod(member.type.kind), member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\tcomposer.composePublishableStructBegin(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\t%s_publ.compose(composer, t.%s);\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\tcomposer.composePublishableStructEnd(");
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\tcomposer.composeSimpleVector(\"%s\", Publishable.makeComposer(t.%s), ", member.name.c_str(), member.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.messageIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.messageIdx]->name.c_str();
					fprintf(header, "\t\tcomposer.composeVector(\"%s\", t.%s.Count,\n", member.name.c_str(), member.name.c_str());
					fprintf(header, "\t\t\t(IPublishableComposer composer, int ordinal) => { %s_publ.compose(composer, t.%s[ordinal]); }, ", elem_type_name, member.name.c_str());
					break;
				}
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
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}
		}
		if(mem.size() != 0)
			fprintf(header, "false);\n");

		fprintf(header, "\t}\n");
	}


	void csharpPub_generateStatePublishableBase(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);

		//fprintf(header, "\tpublic UInt64 idx { get; set; }\n");
		fprintf(header, "\tpublic String statePublisherName() { return \"%s\"; }\n", s.name.c_str());
		fprintf(header, "\tpublic UInt64 stateTypeID() { return %lld; }\n", s.numID);

		fprintf(header, "\tpublic void generateStateSyncMessage(IPublishableComposer composer)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tcomposer.composeStructBegin();\n");
		fprintf(header, "\t\t%s_publ.compose(composer, this);\n", type_name.c_str());
		fprintf(header, "\t\tcomposer.composeStructEnd();\n");
		fprintf(header, "\t}\n");
		
		fprintf(header, "\tpublic void startTick(BufferT buff)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tcomposer.startTick(buff);\n");
		fprintf(header, "\t\tcomposer.composeStateUpdateMessageBegin();\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic BufferT endTick()\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tcomposer.composeStateUpdateMessageEnd();\n");
		fprintf(header, "\t\treturn composer.endTick();\n");
		fprintf(header, "\t}\n");
	}


	void csharpPub_generateStructPubl(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);


		fprintf(header, "public class %s_publ : %s", type_name.c_str(), type_name.c_str());

		if (s.type == CompositeType::Type::publishable)
			fprintf(header, ", StatePublisherBase");

		fprintf(header, "\n");


		fprintf(header, "{\n");
		fprintf(header, "\t%s t;\n", type_name.c_str());
		fprintf(header, "\tIPublishableComposer composer;\n");
		fprintf(header, "\tUInt64[] address;\n");

		csharpPub_generateAddressEnum(header, s);

		fprintf(header, "\tpublic %s_publ(%s t, IPublishableComposer composer, UInt64[] address)\n", type_name.c_str(), type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tthis.t = t;\n");
		fprintf(header, "\t\tthis.composer = composer;\n");
		fprintf(header, "\t\tthis.address = address;\n");
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
				//fprintf(header, "\t\t\tif (value != t.%s)\n", member.name.c_str());
				//fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\tt.%s = value;\n", member.name.c_str());
				fprintf(header, "\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
				fprintf(header, "\t\t\tcomposer.%s(\"value\", value, false);\n", csharpPub_getComposeMethod(member.type.kind));
				fprintf(header, "\t\t\tcomposer.composeAddressEnd();\n");
				//fprintf(header, "\t\t\t}\n");
				fprintf(header, "\t\t}\n");
				fprintf(header, "\t}\n");
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic %s %s\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return new %s_publ(t.%s, composer, Publishable.makeAddress(address, (UInt64)Address.%s)); }\n", member.type.name.c_str(), member.name.c_str(), member.name.c_str());
				fprintf(header, "\t\tset\n");
				fprintf(header, "\t\t{\n");
				//fprintf(header, "\t\t\tif (value != t.%s)\n", member.name.c_str());
				//fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\tt.%s = value;\n", member.name.c_str());
				fprintf(header, "\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
				fprintf(header, "\t\t\tcomposer.composePublishableStructBegin(\"value\");\n");
				fprintf(header, "\t\t\t%s_publ.compose(composer, value);\n", member.type.name.c_str());
				fprintf(header, "\t\t\tcomposer.composePublishableStructEnd(false);\n");
				fprintf(header, "\t\t\tcomposer.composeAddressEnd();\n");
				//fprintf(header, "\t\t\t}\n");
				fprintf(header, "\t\t}\n");
				fprintf(header, "\t}\n");

				fprintf(header, "\tpublic %s make_%s() { return t.make_%s(); }\n", member.type.name.c_str(), member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = csharpPub_getCSharpType(member.type.vectorElemKind);
					const char* compose_method = csharpPub_getComposeMethod(member.type.vectorElemKind);


					fprintf(header, "\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t{\n");
					fprintf(header, "\t\tget\n");
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\treturn new PublisherVectorWrapper<%s>(t.%s, composer,\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\tPublishable.makeAddress(address, (UInt64)Address.%s),\n", member.name.c_str());
					fprintf(header, "\t\t\t\t(IPublishableComposer composer, %s val) => { composer.%s(\"value\", val, false); },\n", elem_type_name, compose_method);
					fprintf(header, "\t\t\t\tnull\n");
					fprintf(header, "\t\t\t);\n");
					fprintf(header, "\t\t}\n");
					fprintf(header, "\t\tset\n");
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\tt.%s = value;\n", member.name.c_str());
					fprintf(header, "\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
					fprintf(header, "\t\t\tcomposer.composeSimpleVector(\"value\", Publishable.makeComposer(value), false);\n");
					fprintf(header, "\t\t\tcomposer.composeAddressEnd();\n");
					fprintf(header, "\t\t}\n");
					fprintf(header, "\t}\n");
					fprintf(header, "\tpublic IList<%s> make_%s() { return t.make_%s(); }\n", elem_type_name, member.name.c_str(), member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.messageIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.messageIdx]->name.c_str();

					fprintf(header, "\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t{\n");
					fprintf(header, "\t\tget\n");
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\treturn new PublisherVectorWrapper<%s>(t.%s, composer,\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\tPublishable.makeAddress(address, (UInt64)Address.%s),\n", member.name.c_str());
					fprintf(header, "\t\t\t\t(IPublishableComposer composer, %s val) =>\n", elem_type_name);
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tcomposer.composePublishableStructBegin(\"value\");\n");
					fprintf(header, "\t\t\t\t\t%s_publ.compose(composer, val);\n", elem_type_name);
					fprintf(header, "\t\t\t\t\tcomposer.composePublishableStructEnd(false);\n");
					fprintf(header, "\t\t\t\t},\n");
					fprintf(header, "\t\t\t\t(%s val, IPublishableComposer composer, UInt64[] addr) =>\n", elem_type_name);
					fprintf(header, "\t\t\t\t{ return new %s_publ(val, composer, addr); }\n", elem_type_name);
					fprintf(header, "\t\t\t);\n");
					fprintf(header, "\t\t}\n");
					fprintf(header, "\t\tset\n");
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\tt.%s = value;\n", member.name.c_str());
					fprintf(header, "\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
					fprintf(header, "\t\t\tcomposer.composeVector(\"value\", value.Count,\n");
					fprintf(header, "\t\t\t\t(IPublishableComposer composer, int ordinal) => { %s_publ.compose(composer, value[ordinal]); },\n", elem_type_name);
					fprintf(header, "\t\t\t\tfalse);\n");
					fprintf(header, "\t\t\tcomposer.composeAddressEnd();\n");
					fprintf(header, "\t\t}\n");
					fprintf(header, "\t}\n");

					fprintf(header, "\tpublic IList<%s> make_%s() { return t.make_%s(); }\n", elem_type_name, member.name.c_str(), member.name.c_str());
					fprintf(header, "\tpublic %s make_%s_element() { return t.make_%s_element(); }\n", elem_type_name, member.name.c_str(), member.name.c_str());
					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}
		}

		csharpPub_generateCompose(header, root, s, type_name);

		if (s.type == CompositeType::Type::publishable)
			csharpPub_generateStatePublishableBase(header, s, type_name);

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

	for ( auto& it : s.publishables )
	{
		auto& obj_1 = it;
		assert(it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert(it->type == CompositeType::Type::publishable );
		csharpPub_generateStruct( header, s, *it );
	}
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

