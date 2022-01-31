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

		fprintf(header, "public interface I%s\n", type_name.c_str());
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
				fprintf(header, "\tI%s %s { get; set; }\n", member.type.name.c_str(), member.name.c_str());
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
						fprintf(header, "\tIList<%s> %s { get; set; }\n", elem_type_name, member.name.c_str());
						break;
					}
					case MessageParameterType::KIND::STRUCT:
					case MessageParameterType::KIND::DISCRIMINATED_UNION:
					{
						assert(member.type.structIdx < root.structs.size());
						const std::string type_name = root.structs[member.type.structIdx]->name;
						fprintf(header, "\tIList<I%s> %s { get; set; }\n", type_name.c_str(), member.name.c_str());
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

		fprintf(header, "public class %s_impl : I%s, IEquatable<%s_impl>\n", type_name.c_str(), type_name.c_str(), type_name.c_str());
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
				fprintf(header, "\tpublic I%s %s { get; set; }\n", member.type.name.c_str(), member.name.c_str());
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
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();
					fprintf(header, "\tpublic IList<I%s> %s { get; set; }\n", elem_type_name, member.name.c_str());
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

		fprintf(header, "\tpublic static void parseForStateSync(IPublishableParser parser, I%s t)\n", type_name.c_str());
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
				fprintf(header, "\t\tt.%s = new %s_impl();\n", member.name.c_str(), member.type.name.c_str());
				fprintf(header, "\t\t%s_subscriber.parseForStateSync(parser, t.%s);\n", member.type.name.c_str(), member.name.c_str());
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
				{
					const char* elem_type_name = csharpPub_getCSharpType(member.type.vectorElemKind);
					fprintf(header, "\t\tt.%s = new List<%s>();\n", member.name.c_str(), elem_type_name);
					fprintf(header, "\t\tparser.parseSimpleVector(\"%s\", Publishable.makeParser(t.%s));\n", member.name.c_str(), member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();

					fprintf(header, "\t\tt.%s = new List<I%s>();\n", member.name.c_str(), elem_type_name);
					fprintf(header, "\t\tparser.parseVector(\"%s\", (IPublishableParser parser, int index) =>\n", member.name.c_str());
					fprintf(header, "\t\t\t{\n");
					fprintf(header, "\t\t\t\tparser.parseStructBegin();\n");
					fprintf(header, "\t\t\t\tI%s val = new %s_impl();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t%s_subscriber.parseForStateSync(parser, val);\n", elem_type_name);
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

		fprintf(header, "\tpublic static bool parse(IPublishableParser parser, I%s t)\n", type_name.c_str());
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
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* type_name = csharpPub_getCSharpType(member.type.kind);
				const char* parse_method = csharpPub_getParseMethod(member.type.kind);

				fprintf(header, "\t\t\t%s newVal = parser.parse%s(\"%s\");\n", type_name, parse_method, member.name.c_str());
				fprintf(header, "\t\t\tif(newVal != t.%s)\n", member.name.c_str());
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\tt.%s = newVal;\n", member.name.c_str());
				fprintf(header, "\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t}\n");
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\tparser.parsePublishableStructBegin(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\t\tbool currentChanged = %s_subscriber.parse(parser, t.%s);\n", member.type.name.c_str(), member.name.c_str());
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

					fprintf(header, "\t\tIList<%s> newVal = new List<%s>();\n", elem_type_name, elem_type_name);
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
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();

					fprintf(header, "\t\tIList<I%s> newVal = new List<I%s>();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\tparser.parseVector(\"%s\", (IPublishableParser parser, int index) =>\n", member.name.c_str());
					fprintf(header, "\t\t\t{\n");
					fprintf(header, "\t\t\t\tparser.parseStructBegin();\n");
					fprintf(header, "\t\t\t\tI%s val = new %s_impl();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t%s_subscriber.parseForStateSync(parser, val);\n", elem_type_name);
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

		fprintf(header, "\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber, UInt64[] addr, int offset)\n", type_name.c_str());
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
				fprintf(header, "\t\t\t\tif(newVal != subscriber.data.%s)\n", member.name.c_str());
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\t%s oldVal = subscriber.data.%s;\n", type_name, member.name.c_str());
				fprintf(header, "\t\t\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
				fprintf(header, "\t\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t\t\tsubscriber.notifyUpdated_%s(oldVal);\n", member.name.c_str());
				fprintf(header, "\t\t\t\t}\n");
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\t\tbool currentChanged = false;\n");
				fprintf(header, "\t\t\t\tif(addr.Length == offset + 1) // full element replace\n");
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tparser.parsePublishableStructBegin(\"value\");\n");
				fprintf(header, "\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.%s_wrapper);\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\t\t\tparser.parsePublishableStructEnd();\n");
				fprintf(header, "\t\t\t\t}\n");
				fprintf(header, "\t\t\t\telse if(addr.Length > offset + 1) // let child continue parsing\n");
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.%s_wrapper, addr, offset + 1);\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\t\t}\n");
				fprintf(header, "\t\t\t\telse\n");
				fprintf(header, "\t\t\t\t\tthrow new Exception();\n\n");

				fprintf(header, "\t\t\t\tif(currentChanged)\n");
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
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
					fprintf(header, "\t\t\t\t\tIList<%s> newVal = new List<%s>();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t\tparser.parseSimpleVector(\"value\", Publishable.makeParser(newVal));\n");
					fprintf(header, "\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.data.%s))\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tcurrentChanged = true;\n");
					fprintf(header, "\t\t\t\t\t}\n");

					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tint index = (int)addr[offset + 1];\n");
					fprintf(header, "\t\t\t\t\tcurrentChanged = SubscriberVectorHelper.parseVectorPrimitive<%s>(\n", elem_type_name);
					fprintf(header, "\t\t\t\t\t\tparser, subscriber.data.%s, index,\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t(IPublishableParser parser) => { return parser.parse%s(\"value\"); },\n", parse_method);
					fprintf(header, "\t\t\t\t\t\tsubscriber.notifyElementUpdated_%s,\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tsubscriber.notifyInserted_%s,\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tsubscriber.notifyErased_%s\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t);\n");
					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse // simple type can't handle deeper address\n");
					fprintf(header, "\t\t\t\t\tthrow new Exception();\n\n");

					fprintf(header, "\t\t\t\tif(currentChanged)\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tchanged = true;\n");
					fprintf(header, "\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					fprintf(header, "\t\t\t\t}\n");

					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();

					fprintf(header, "\t\t\t\tbool currentChanged = false;\n");
					fprintf(header, "\t\t\t\tif(addr.Length == offset + 1) // full vector replace\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tIList<I%s> newVal = new List<I%s>();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t\tSubscriberVectorWrapper<%s_subscriber> newVal_wrapper = new SubscriberVectorWrapper<%s_subscriber>();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t\tparser.parseVector(\"value\",\n");
					fprintf(header, "\t\t\t\t\t\t(IPublishableParser parser, int index) =>\n");
					fprintf(header, "\t\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\t\tparser.parseStructBegin();\n");
					fprintf(header, "\t\t\t\t\t\t\tI%s val = new %s_impl();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t\t\t\t%s_subscriber.parseForStateSync(parser, val);\n", elem_type_name);
					fprintf(header, "\t\t\t\t\t\t\t%s_subscriber val_wrapper = subscriber.makeElementHandler_%s();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t\tval_wrapper.data = val;\n");
					fprintf(header, "\t\t\t\t\t\t\tnewVal.Add(val);\n");
					fprintf(header, "\t\t\t\t\t\t\tnewVal_wrapper.Add(val_wrapper);\n");
					fprintf(header, "\t\t\t\t\t\t\tparser.parseStructEnd();\n");
					fprintf(header, "\t\t\t\t\t\t}\n");
					fprintf(header, "\t\t\t\t\t);\n");

					fprintf(header, "\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.data.%s))\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tsubscriber.%s_wrapper = newVal_wrapper;\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tcurrentChanged = true;\n");
					fprintf(header, "\t\t\t\t\t}\n");

					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tint index = (int)addr[offset + 1];\n");
					fprintf(header, "\t\t\t\t\tPublishable.ActionOnVector action = (Publishable.ActionOnVector)parser.parseActionInPublishable();\n");

					fprintf(header, "\t\t\t\t\tswitch (action)\n");
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tcase Publishable.ActionOnVector.update_at:\n");
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\tparser.parsePublishableStructBegin(\"value\");\n");
					fprintf(header, "\t\t\t\t\t\tbool elemChanged = %s_subscriber.parse(parser, subscriber.data.%s[index]);\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tparser.parsePublishableStructEnd();\n");
					fprintf(header, "\t\t\t\t\t\tif (elemChanged)\n");
					fprintf(header, "\t\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\t\tcurrentChanged = true;\n");
					fprintf(header, "\t\t\t\t\t\t\tsubscriber.notifyElementUpdated_%s(index);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t}\n");
					fprintf(header, "\t\t\t\t\t\tbreak;\n");
					fprintf(header, "\t\t\t\t\t}\n");


					fprintf(header, "\t\t\t\t\tcase Publishable.ActionOnVector.insert_single_before:\n");
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\tparser.parsePublishableStructBegin(\"value\");\n");
					fprintf(header, "\t\t\t\t\t\tI%s newVal = new %s_impl();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t\t\t%s_subscriber.parse(parser, newVal);\n", elem_type_name);
					fprintf(header, "\t\t\t\t\t\t%s_subscriber newVal_wrapper = subscriber.makeElementHandler_%s();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tparser.parsePublishableStructEnd();\n");
					fprintf(header, "\t\t\t\t\t\tsubscriber.data.%s.Insert(index, newVal);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tsubscriber.%s_wrapper.Insert(index, newVal_wrapper);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tcurrentChanged = true;\n");
					fprintf(header, "\t\t\t\t\t\tsubscriber.notifyInserted_%s(index);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tbreak;\n");
					fprintf(header, "\t\t\t\t\t}\n");



					fprintf(header, "\t\t\t\t\tcase Publishable.ActionOnVector.remove_at:\n");
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\tsubscriber.data.%s.RemoveAt(index);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tsubscriber.%s_wrapper.RemoveAt(index);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tcurrentChanged = true;\n");
					fprintf(header, "\t\t\t\t\t\tsubscriber.notifyErased_%s(index);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tbreak;\n");
					fprintf(header, "\t\t\t\t\t}\n");
					fprintf(header, "\t\t\t\t\tdefault:\n");
					fprintf(header, "\t\t\t\t\t\tthrow new Exception();\n");
					fprintf(header, "\t\t\t\t\t}\n");


					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse if(addr.Length > offset + 2) // let child continue parsing\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tint index = (int)addr[offset + 1];\n");
					fprintf(header, "\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.%s_wrapper[index], addr, offset + 2);\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t\telse\n");
					fprintf(header, "\t\t\t\t\tthrow new Exception();\n\n");

					fprintf(header, "\t\t\t\tif(currentChanged)\n");
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tchanged = true;\n");
					fprintf(header, "\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
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

		fprintf(header, "\tpublic void applyMessageWithUpdates(IPublishableParser parser)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tparser.parseStateUpdateMessageBegin();\n");
		fprintf(header, "\t\tUInt64[] addr = null;\n");
		fprintf(header, "\t\twhile(parser.parseAddress(ref addr))\n");
		fprintf(header, "\t\t{\n");
		fprintf(header, "\t\t\t%s_subscriber.parse(parser, this, addr, 0);\n", type_name.c_str());
		fprintf(header, "\t\t\tparser.parseAddressEnd();\n");
		fprintf(header, "\t\t\taddr = null;\n");
		fprintf(header, "\t\t}\n");
		fprintf(header, "\t\tparser.parseStateUpdateMessageEnd();\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic void applyStateSyncMessage(IPublishableParser parser)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tparser.parseStructBegin();\n");
		fprintf(header, "\t\t%s_subscriber.parseForStateSync(parser, this.data);\n", type_name.c_str());
		fprintf(header, "\t\tparser.parseStructEnd();\n");
		fprintf(header, "\t\tthis.notifyFullyUpdated();\n");
		fprintf(header, "\t}\n");
	}

	void csharpPub_generateEventHandlerSubs(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "\n\t// event handlers\n");

		if (s.type == CompositeType::Type::publishable)
			fprintf(header, "\tpublic virtual void notifyFullyUpdated() { }\n");

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
			{
				const char* type_name = csharpPub_getCSharpType(member.type.kind);
				fprintf(header, "\tpublic virtual void notifyUpdated_%s(%s old) { }\n", member.name.c_str(), type_name);
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
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
					fprintf(header, "\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
					fprintf(header, "\tpublic virtual void notifyElementUpdated_%s(int index, %s old) { }\n", member.name.c_str(), elem_type_name);
					fprintf(header, "\tpublic virtual void notifyInserted_%s(int index) { }\n", member.name.c_str());
					fprintf(header, "\tpublic virtual void notifyErased_%s(int index) { }\n", member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					//assert(member.type.structIdx < root.structs.size());
					//const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();
					fprintf(header, "\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
					fprintf(header, "\tpublic virtual void notifyElementUpdated_%s(int index) { }\n", member.name.c_str());
					fprintf(header, "\tpublic virtual void notifyInserted_%s(int index) { }\n", member.name.c_str());
					fprintf(header, "\tpublic virtual void notifyErased_%s(int index) { }\n", member.name.c_str());
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
	}


	void csharpPub_generateStructSubs(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "public class %s_subscriber : I%s", type_name.c_str(), type_name.c_str());

		if(s.type == CompositeType::Type::publishable)
			fprintf(header, ", StateSubscriberBase");

		fprintf(header, "\n");

		fprintf(header, "{\n");
		fprintf(header, "\tinternal I%s data;\n", type_name.c_str());

		csharpPub_generateAddressEnum(header, s);

		fprintf(header, "\tpublic %s_subscriber() { }\n", type_name.c_str());
		fprintf(header, "\tpublic %s_subscriber(I%s data)\n", type_name.c_str(), type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tthis.data = data;\n");
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
				fprintf(header, "\t\tget { return data.%s; }\n", member.name.c_str());
				fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
				fprintf(header, "\t}\n");
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic %s_subscriber %s_wrapper;\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget\n");
				fprintf(header, "\t\t{\n");
				fprintf(header, "\t\t\tif(data.%s != null && %s_wrapper == null)\n", member.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\t%s_wrapper = makeHandler_%s();\n", member.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\t\t%s_wrapper.data = data.%s;\n", member.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\t}\n");
				fprintf(header, "\t\t\treturn %s_wrapper;\n", member.name.c_str());
				fprintf(header, "\t\t}\n");
				fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
				fprintf(header, "\t}\n");
				fprintf(header, "\tpublic virtual %s_subscriber makeHandler_%s() { return new %s_subscriber(); }\n", member.type.name.c_str(), member.name.c_str(), member.type.name.c_str());
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
					fprintf(header, "\t\tget { return new SubscriberVectorWrapper<%s>(data.%s); }\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
					fprintf(header, "\t}\n");
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();
					fprintf(header, "\tSubscriberVectorWrapper<%s_subscriber> %s_wrapper;\n", elem_type_name, member.name.c_str());
					fprintf(header, "\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t{\n");
					fprintf(header, "\t\tget\n");
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\tif(data.%s != null && %s_wrapper == null)\n", member.name.c_str(), member.name.c_str());
					fprintf(header, "\t\t\t{\n");
					fprintf(header, "\t\t\t\t%s_wrapper = new SubscriberVectorWrapper<%s_subscriber>();\n", member.name.c_str(), elem_type_name);
					fprintf(header, "\t\t\t\tfor(int i = 0; i < data.%s.Count; ++i)\n", member.name.c_str());
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t%s_subscriber val_wrapper = makeElementHandler_%s();\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\tval_wrapper.data = data.%s[i];\n", member.name.c_str());
					fprintf(header, "\t\t\t\t%s_wrapper.Add(val_wrapper);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t}\n");
					fprintf(header, "\t\t\t}\n");
					fprintf(header, "\t\t\treturn (IList<I%s>)%s_wrapper;\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t}\n");
					fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
					fprintf(header, "\t}\n");
					fprintf(header, "\tpublic virtual %s_subscriber makeElementHandler_%s() { return new %s_subscriber(); }\n", elem_type_name, member.name.c_str(), elem_type_name);
					//fprintf(header, "\t%s_subscriber makeElementHandler_%s_internal(I%s data)\n", elem_type_name, member.name.c_str(), elem_type_name);
					//fprintf(header, "\t{\n");
					//fprintf(header, "\t\t%s_subscriber wrapper = makeElementHandler_%s();\n", elem_type_name, member.name.c_str());
					//fprintf(header, "\t\twrapper.data = data;\n");
					//fprintf(header, "\t\treturn wrapper;\n");
					//fprintf(header, "\t}\n");
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

		csharpPub_generateEventHandlerSubs(header, root, s, type_name);

		fprintf(header, "} // class %s_subscriber\n\n", type_name.c_str());
	}

	void csharpPub_generateCompose(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "\tpublic static void compose(IPublishableComposer composer, I%s t)\n", type_name.c_str());
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
				fprintf(header, "\t\t%s_publisher.compose(composer, t.%s);\n", member.type.name.c_str(), member.name.c_str());
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
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();
					fprintf(header, "\t\tcomposer.composeVector(\"%s\", t.%s.Count,\n", member.name.c_str(), member.name.c_str());
					fprintf(header, "\t\t\t(IPublishableComposer composer, int ordinal) => { %s_publisher.compose(composer, t.%s[ordinal]); }, ", elem_type_name, member.name.c_str());
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
		if(mem.size() != 0)
			fprintf(header, "false);\n");

		fprintf(header, "\t}\n");
	}


	void csharpPub_generateStatePublishableBase(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);

		fprintf(header, "\tpublic int idx { get; set; } // for use in pools, etc\n");
		fprintf(header, "\tpublic String statePublisherName() { return \"%s\"; }\n", s.name.c_str());
		fprintf(header, "\tpublic UInt64 stateTypeID() { return %lld; }\n", s.numID);

		fprintf(header, "\tpublic void generateStateSyncMessage(IPublishableComposer composer)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tcomposer.composeStructBegin();\n");
		fprintf(header, "\t\t%s_publisher.compose(composer, this.t);\n", type_name.c_str());
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


		fprintf(header, "public class %s_publisher : I%s", type_name.c_str(), type_name.c_str());

		if (s.type == CompositeType::Type::publishable)
			fprintf(header, ", IStatePublisher");

		fprintf(header, "\n");


		fprintf(header, "{\n");
		fprintf(header, "\tI%s t;\n", type_name.c_str());
		fprintf(header, "\tIPublishableComposer composer;\n");
		fprintf(header, "\tUInt64[] address;\n");

		csharpPub_generateAddressEnum(header, s);

		fprintf(header, "\tpublic %s_publisher(I%s t, IPublishableComposer composer, UInt64[] address)\n", type_name.c_str(), type_name.c_str());
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
				fprintf(header, "\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return new %s_publisher(t.%s, composer, Publishable.makeAddress(address, (UInt64)Address.%s)); }\n", member.type.name.c_str(), member.name.c_str(), member.name.c_str());
				fprintf(header, "\t\tset\n");
				fprintf(header, "\t\t{\n");
				//fprintf(header, "\t\t\tif (value != t.%s)\n", member.name.c_str());
				//fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\tt.%s = value;\n", member.name.c_str());
				fprintf(header, "\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
				fprintf(header, "\t\t\tcomposer.composePublishableStructBegin(\"value\");\n");
				fprintf(header, "\t\t\t%s_publisher.compose(composer, value);\n", member.type.name.c_str());
				fprintf(header, "\t\t\tcomposer.composePublishableStructEnd(false);\n");
				fprintf(header, "\t\t\tcomposer.composeAddressEnd();\n");
				//fprintf(header, "\t\t\t}\n");
				fprintf(header, "\t\t}\n");
				fprintf(header, "\t}\n");

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
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();

					fprintf(header, "\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t{\n");
					fprintf(header, "\t\tget\n");
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\treturn new PublisherVectorWrapper<I%s>(t.%s, composer,\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\tPublishable.makeAddress(address, (UInt64)Address.%s),\n", member.name.c_str());
					fprintf(header, "\t\t\t\t(IPublishableComposer composer, I%s val) =>\n", elem_type_name);
					fprintf(header, "\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\tcomposer.composePublishableStructBegin(\"value\");\n");
					fprintf(header, "\t\t\t\t\t%s_publisher.compose(composer, val);\n", elem_type_name);
					fprintf(header, "\t\t\t\t\tcomposer.composePublishableStructEnd(false);\n");
					fprintf(header, "\t\t\t\t},\n");
					fprintf(header, "\t\t\t\t(I%s val, IPublishableComposer composer, UInt64[] addr) =>\n", elem_type_name);
					fprintf(header, "\t\t\t\t{ return new %s_publisher(val, composer, addr); }\n", elem_type_name);
					fprintf(header, "\t\t\t);\n");
					fprintf(header, "\t\t}\n");
					fprintf(header, "\t\tset\n");
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\tt.%s = value;\n", member.name.c_str());
					fprintf(header, "\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
					fprintf(header, "\t\t\tcomposer.composeVector(\"value\", value.Count,\n");
					fprintf(header, "\t\t\t\t(IPublishableComposer composer, int ordinal) => { %s_publisher.compose(composer, value[ordinal]); },\n", elem_type_name);
					fprintf(header, "\t\t\t\tfalse);\n");
					fprintf(header, "\t\t\tcomposer.composeAddressEnd();\n");
					fprintf(header, "\t\t}\n");
					fprintf(header, "\t}\n");
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

		fprintf(header, "} // class %s_publisher\n\n", type_name.c_str());
	}

	void csharpPub_generateStateConcentratorBase(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);

		//fprintf(header, "\tpublic String stateSubscriberName() { return \"%s\"; }\n", s.name.c_str());
		//fprintf(header, "\tpublic UInt64 stateTypeID() { return %lld; }\n", s.numID);

		//fprintf(header, "\tpublic void applyMessageWithUpdates(IPublishableParser parser)\n");
		//fprintf(header, "\t{\n");
		//fprintf(header, "\t\tparser.parseStateUpdateMessageBegin();\n");
		//fprintf(header, "\t\tUInt64[] addr = null;\n");
		//fprintf(header, "\t\twhile(parser.parseAddress(ref addr))\n");
		//fprintf(header, "\t\t{\n");
		//fprintf(header, "\t\t\t%s_subscriber.parse(parser, this, addr, 0);\n", type_name.c_str());
		//fprintf(header, "\t\t\tparser.parseAddressEnd();\n");
		//fprintf(header, "\t\t\taddr = null;\n");
		//fprintf(header, "\t\t}\n");
		//fprintf(header, "\t\tparser.parseStateUpdateMessageEnd();\n");
		//fprintf(header, "\t}\n");

		//fprintf(header, "\tpublic void applyStateSyncMessage(IPublishableParser parser)\n");
		//fprintf(header, "\t{\n");
		//fprintf(header, "\t\tparser.parseStructBegin();\n");
		//fprintf(header, "\t\t%s_subscriber.parseForStateSync(parser, this.data);\n", type_name.c_str());
		//fprintf(header, "\t\tparser.parseStructEnd();\n");
		//fprintf(header, "\t}\n");

		fprintf(header, "\tpublic void generateStateSyncMessage(IPublishableComposer composer)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tcomposer.composeStructBegin();\n");
		fprintf(header, "\t\t%s_publisher.compose(composer, this.data);\n", type_name.c_str());
		fprintf(header, "\t\tcomposer.composeStructEnd();\n");
		fprintf(header, "\t}\n");

	}


	void csharpPub_generateStructConcentrator(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);


		fprintf(header, "public class %s_concentrator : %s_subscriber, IStateConcentrator\n", type_name.c_str(), type_name.c_str());

		////if (s.type == CompositeType::Type::publishable)
		////	fprintf(header, " : IStateConcentrator");

		//fprintf(header, "\n");


		fprintf(header, "{\n");
		//fprintf(header, "\tI%s data = new %s_impl();\n", type_name.c_str(), type_name.c_str());

		//csharpPub_generateAddressEnum(header, s);

		//fprintf(header, "\tpublic %s_concentrator(I%s t)\n", type_name.c_str(), type_name.c_str());
		//fprintf(header, "\t{\n");
		//fprintf(header, "\t\tthis.t = t;\n");
		//fprintf(header, "\t}\n");


		csharpPub_generateCompose(header, root, s, type_name);
		//csharpPub_generateParseStateSync(header, root, s, type_name);
		//csharpPub_generateParse1(header, root, s, type_name);
		//csharpPub_generateParse2(header, root, s, type_name);


		//if (s.type == CompositeType::Type::publishable)
		csharpPub_generateStateConcentratorBase(header, s, type_name);

		fprintf(header, "} // class %s_concentrator\n\n", type_name.c_str());
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

		// wrapper for concentrator
		if (s.type == CompositeType::Type::publishable)
			csharpPub_generateStructConcentrator(header, root, s, name);


		fprintf(header, "\n");

	}
}




void generateCsharpPublishables( const char* fileName, uint32_t fileChecksum, FILE* header, const std::string& metascope, const std::string& platformPrefix, const std::string& classNotifierName, Root& s )
{
	vector<CompositeType*> structsOrderedByDependency;
	std::unordered_set<size_t> collElementTypes;
	orderStructsByDependency( s.structs, structsOrderedByDependency, collElementTypes );
	

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

	if (!s.publishables.empty())
	{
		fprintf(header, "public class StateConcentratorFactory : IStateConcentratorFactory\n");
		fprintf(header, "{\n");
		fprintf(header, "\tpublic IStateConcentrator createConcentrator(UInt64 typeID)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tswitch(typeID)\n");
		fprintf(header, "\t\t{\n");
		for (auto& it : s.publishables)
		{
			auto& obj_1 = it;
			assert(it != nullptr);
			assert(typeid(*(it)) == typeid(CompositeType));
			assert(it->type == CompositeType::Type::publishable);
			std::string name = csharpPub_getTypeName(*it);
			fprintf(header, "\t\tcase %lld:\n", it->numID);
			fprintf(header, "\t\t\treturn new %s_concentrator();\n", name.c_str());
		}
		fprintf(header, "\t\tdefault:\n");
		fprintf(header, "\t\t\treturn null;\n");
		fprintf(header, "\t\t}\n");

		fprintf(header, "\t}\n");
		fprintf(header, "} // class StateConcentratorFactory\n\n");
	}

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

