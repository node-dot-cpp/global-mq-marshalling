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

		fprintf(header, "\tpublic static void parseForStateSync(IPublishableParser parser, %s_subscriber subscriber)\n", type_name.c_str());
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
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* parse_method = csharpPub_getParseMethod(member.type.kind);
				fprintf(header, "\t\tsubscriber.data.%s = parser.parse%s(\"%s\");\n", member.name.c_str(), parse_method, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\tparser.parsePublishableStructBegin(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\t%s_subscriber.parseForStateSync(parser, subscriber.lazy_%s_handler());\n", member.type.name.c_str(), member.name.c_str());
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
					fprintf(header, "\t\tparser.parseSimpleVector(\"%s\", Publishable.makeParser(subscriber.data.%s));\n", member.name.c_str(), member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();

					fprintf(header, "\t\tparser.parseVector(\"%s\", (IPublishableParser parser, int index) =>\n", member.name.c_str());
					fprintf(header, "\t\t\t{\n");
					fprintf(header, "\t\t\t\tparser.parseStructBegin();\n");
					fprintf(header, "\t\t\t\tI%s val = new %s();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t%s_subscriber handler = subscriber.makeElementHandler_%s(val);\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t%s_subscriber.parseForStateSync(parser, handler);\n", elem_type_name);
					fprintf(header, "\t\t\t\tsubscriber.data.%s.Add(val);\n", member.name.c_str());
					fprintf(header, "\t\t\t\tsubscriber.lazy_%s_handlers().Add(handler);\n", member.name.c_str());
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

		fprintf(header, "\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber)\n", type_name.c_str());
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
				fprintf(header, "\t\t\tchanged = subscriber.update_%s(newVal) || changed;\n", member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\tparser.parsePublishableStructBegin(\"%s\");\n", member.name.c_str());
				fprintf(header, "\t\t\tbool currentChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handler());\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\tparser.parsePublishableStructEnd();\n");
				fprintf(header, "\t\t\tif(currentChanged)\n");
				fprintf(header, "\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tchanged = true;\n");
				fprintf(header, "\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
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
					fprintf(header, "\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.data.%s))\n", member.name.c_str());
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
					fprintf(header, "\t\t\tchanged = true;\n");
					fprintf(header, "\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					fprintf(header, "\t\t}\n");

					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();

					fprintf(header, "\t\tList<I%s> newVal = new List<I%s>();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\tList<%s_subscriber> newHandlers = new List<%s_subscriber>();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\tparser.parseVector(\"%s\", (IPublishableParser parser, int index) =>\n", member.name.c_str());
					fprintf(header, "\t\t\t{\n");
					fprintf(header, "\t\t\t\tparser.parseStructBegin();\n");
					fprintf(header, "\t\t\t\tI%s val = new %s();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t%s_subscriber handler = subscriber.makeElementHandler_%s(val);\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t%s_subscriber.parseForStateSync(parser, handler);\n", elem_type_name);
					fprintf(header, "\t\t\t\tnewVal.Add(val);\n");
					fprintf(header, "\t\t\t\tnewHandlers.Add(handler);\n");
					fprintf(header, "\t\t\t\tparser.parseStructEnd();\n");
					fprintf(header, "\t\t\t}\n");
					fprintf(header, "\t\t);\n");
					fprintf(header, "\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.data.%s))\n", member.name.c_str());
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
					fprintf(header, "\t\t\tsubscriber.%s_handlers = newHandlers;\n", member.name.c_str());
					fprintf(header, "\t\t\tchanged = true;\n");
					fprintf(header, "\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
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
				fprintf(header, "\t\t\t\tchanged = subscriber.update_%s(newVal) || changed;\n", member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\t\tbool currentChanged = false;\n");
				fprintf(header, "\t\t\t\tif(addr.Length == offset + 1) // full element replace\n");
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tparser.parsePublishableStructBegin(\"value\");\n");
				fprintf(header, "\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handler());\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t\t\t\t\tparser.parsePublishableStructEnd();\n");
				fprintf(header, "\t\t\t\t}\n");
				fprintf(header, "\t\t\t\telse if(addr.Length > offset + 1) // let child continue parsing\n");
				fprintf(header, "\t\t\t\t{\n");
				fprintf(header, "\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handler(), addr, offset + 1);\n", member.type.name.c_str(), member.name.c_str());
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
					fprintf(header, "\t\t\t\t\tList<I%s> newVal = new List<I%s>();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t\tList<%s_subscriber> newHandlers = new List<%s_subscriber>();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t\tparser.parseVector(\"value\",\n");
					fprintf(header, "\t\t\t\t\t\t(IPublishableParser parser, int index) =>\n");
					fprintf(header, "\t\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\t\tparser.parseStructBegin();\n");
					fprintf(header, "\t\t\t\t\t\t\tI%s val = new %s();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t\t\t\t%s_subscriber handler = subscriber.makeElementHandler_%s(val);\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t\t%s_subscriber.parseForStateSync(parser, handler);\n", elem_type_name);
					fprintf(header, "\t\t\t\t\t\t\tnewVal.Add(val);\n");
					fprintf(header, "\t\t\t\t\t\t\tnewHandlers.Add(handler);\n");
					fprintf(header, "\t\t\t\t\t\t\tparser.parseStructEnd();\n");
					fprintf(header, "\t\t\t\t\t\t}\n");
					fprintf(header, "\t\t\t\t\t);\n");

					fprintf(header, "\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.data.%s))\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tsubscriber.%s_handlers = newHandlers;\n", member.name.c_str());
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
					fprintf(header, "\t\t\t\t\t\tbool elemChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handlers()[index]);\n", elem_type_name, member.name.c_str());
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
					fprintf(header, "\t\t\t\t\t\tI%s newVal = new %s();\n", elem_type_name, elem_type_name);
					fprintf(header, "\t\t\t\t\t\t%s_subscriber handler = subscriber.makeElementHandler_%s(newVal);\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t\t\t\t\t\t%s_subscriber.parse(parser, handler);\n", elem_type_name);
					fprintf(header, "\t\t\t\t\t\tsubscriber.data.%s.Insert(index, newVal);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tsubscriber.lazy_%s_handlers().Insert(index, handler);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tparser.parsePublishableStructEnd();\n");
					fprintf(header, "\t\t\t\t\t\tcurrentChanged = true;\n");
					fprintf(header, "\t\t\t\t\t\tsubscriber.notifyInserted_%s(index);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tbreak;\n");
					fprintf(header, "\t\t\t\t\t}\n");



					fprintf(header, "\t\t\t\t\tcase Publishable.ActionOnVector.remove_at:\n");
					fprintf(header, "\t\t\t\t\t{\n");
					fprintf(header, "\t\t\t\t\t\t// mb: reversed in case of lazy initialization\n");
					fprintf(header, "\t\t\t\t\t\tsubscriber.lazy_%s_handlers().RemoveAt(index);\n", member.name.c_str());
					fprintf(header, "\t\t\t\t\t\tsubscriber.data.%s.RemoveAt(index);\n", member.name.c_str());
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
					fprintf(header, "\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handlers()[index], addr, offset + 2);\n", elem_type_name, member.name.c_str());
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
		fprintf(header, "\t\t%s_subscriber.parseForStateSync(parser, this);\n", type_name.c_str());
		fprintf(header, "\t\tparser.parseStructEnd();\n");
		fprintf(header, "\t\tthis.notifyFullyUpdated();\n");
		fprintf(header, "\t}\n");
	}

	void csharpPub_generateEventHandlerSubs(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

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

	void csharpPub_generateFactoryMethodSubs(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

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
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic virtual %s_subscriber makeHandler_%s(I%s data) { return new %s_subscriber(data); }\n", member.type.name.c_str(), member.name.c_str(), member.type.name.c_str(), member.type.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();

					fprintf(header, "\tpublic virtual %s_subscriber makeElementHandler_%s(I%s data) { return new %s_subscriber(data); }\n", elem_type_name, member.name.c_str(), elem_type_name, elem_type_name);
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

	void csharpPub_generateDebugMethodSubs(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
		fprintf(header, "\tpublic I%s debugOnlyGetData() { return this.data; }\n", type_name.c_str());

		fprintf(header, "\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
		fprintf(header, "\tpublic void debugOnlySetData(I%s data)\n", type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tthis.data = data;\n");

		for (auto& each : s.getMembers())
		{
			auto& member = *each;
			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\tthis.%s_handler = null;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					fprintf(header, "\t\tthis.%s_handlers = null;\n", member.name.c_str());
					break;
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

	void csharpPub_generateStructSubs(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		fprintf(header, "public class %s_subscriber : I%s", type_name.c_str(), type_name.c_str());

		if(s.type == CompositeType::Type::publishable)
			fprintf(header, ", StateSubscriberBase");

		fprintf(header, "\n{\n");

		fprintf(header, "\n\t/////////////////////////////////  begin user override section /////////////////////////////////\n\n");

		csharpPub_generateFactoryMethodSubs(header, root, s, type_name);
		csharpPub_generateEventHandlerSubs(header, root, s, type_name);

		fprintf(header, "\n\t/////////////////////////////////   end user override section  /////////////////////////////////\n\n\n");


		fprintf(header, "\tprotected I%s data;\n", type_name.c_str());

		csharpPub_generateAddressEnum(header, s);

		if (s.type == CompositeType::Type::publishable)
			fprintf(header, "\tpublic %s_subscriber() { this.data = new %s(); }\n", type_name.c_str(), type_name.c_str());
		else
			fprintf(header, "\tpublic %s_subscriber(I%s data) { this.data = data; }\n", type_name.c_str(), type_name.c_str());


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
				fprintf(header, "\tpublic %s %s\n", type_name, member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return data.%s; }\n", member.name.c_str());
				fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
				fprintf(header, "\t}\n");
				fprintf(header, "\tbool update_%s(%s newVal)\n", member.name.c_str(), csharpPub_getCSharpType(member.type.kind));
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tif (newVal != data.%s)\n", member.name.c_str());
				fprintf(header, "\t\t{\n");
				fprintf(header, "\t\t\t%s oldVal = data.%s;\n", type_name, member.name.c_str());
				fprintf(header, "\t\t\tdata.%s = newVal;\n", member.name.c_str());
				fprintf(header, "\t\t\tnotifyUpdated_%s(oldVal);\n", member.name.c_str());
				fprintf(header, "\t\t\treturn true;\n");
				fprintf(header, "\t\t}\n");
				fprintf(header, "\t\telse\n");
				fprintf(header, "\t\t\treturn false;\n");
				fprintf(header, "\t}\n");
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t%s_subscriber %s_handler;\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t%s_subscriber lazy_%s_handler()\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t{ // mb: MUST use lazy initialization\n");
				fprintf(header, "\t\tif (%s_handler == null)\n", member.name.c_str());
				fprintf(header, "\t\t\t%s_handler = makeHandler_%s(data.%s);\n", member.name.c_str(), member.name.c_str(), member.name.c_str());
				fprintf(header, "\t\treturn %s_handler;\n", member.name.c_str());
				fprintf(header, "\t}\n");


				fprintf(header, "\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return lazy_%s_handler(); }\n", member.name.c_str());
				fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
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
					fprintf(header, "\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t{\n");
					fprintf(header, "\t\tget { return new SubscriberVectorWrapper<%s, %s>(data.%s); }\n", elem_type_name, elem_type_name, member.name.c_str());
					fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
					fprintf(header, "\t}\n");
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();
					fprintf(header, "\tList<%s_subscriber> %s_handlers;\n", elem_type_name, member.name.c_str());
					fprintf(header, "\tList<%s_subscriber> lazy_%s_handlers()\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t{ // mb: MUST use lazy initialization\n");
					fprintf(header, "\t\tif (%s_handlers == null)\n", member.name.c_str());
					fprintf(header, "\t\t{\n");
					fprintf(header, "\t\t\t%s_handlers = new List<%s_subscriber>();\n", member.name.c_str(), elem_type_name);
					fprintf(header, "\t\t\tfor (int i = 0; i < data.%s.Count; ++i)\n", member.name.c_str());
					fprintf(header, "\t\t\t{\n");
					fprintf(header, "\t\t\t\t%s_subscriber handler = makeElementHandler_%s(data.%s[i]);\n", elem_type_name, member.name.c_str(), member.name.c_str());
					fprintf(header, "\t\t\t\t%s_handlers.Add(handler);\n", member.name.c_str());
					fprintf(header, "\t\t\t}\n");
					fprintf(header, "\t\t}\n");
					fprintf(header, "\t\treturn %s_handlers;\n", member.name.c_str());
					fprintf(header, "\t}\n");
					fprintf(header, "\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
					fprintf(header, "\t{\n");
					fprintf(header, "\t\tget { return new SubscriberVectorWrapper<I%s, %s_subscriber>(lazy_%s_handlers()); }\n", elem_type_name, elem_type_name, member.name.c_str());
					fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
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

		csharpPub_generateParseStateSync(header, root, s, type_name);
		csharpPub_generateParse1(header, root, s, type_name);
		csharpPub_generateParse2(header, root, s, type_name);

		if (s.type == CompositeType::Type::publishable)
			csharpPub_generateStateSubscriberBase(header, s, type_name);

		csharpPub_generateDebugMethodSubs(header, root, s, type_name);

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
		
		fprintf(header, "\tpublic void startTick(IPublishableComposer composer)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tthis.composer = composer;\n");
		fprintf(header, "\t\tcomposer.composeStateUpdateMessageBegin();\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic IPublishableComposer endTick()\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tcomposer.composeStateUpdateMessageEnd();\n");
		fprintf(header, "\t\tIPublishableComposer tmp = composer;\n");
		fprintf(header, "\t\tthis.composer = null;\n");
		fprintf(header, "\t\treturn tmp;\n");
		fprintf(header, "\t}\n");
	}


	void csharpPub_generateStructPubl(FILE* header, Root& root, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);


		fprintf(header, "public class %s_publisher : I%s", type_name.c_str(), type_name.c_str());

		if (s.type == CompositeType::Type::publishable)
			fprintf(header, ", StatePublisherBase");

		fprintf(header, "\n");


		fprintf(header, "{\n");
		fprintf(header, "\tI%s t;\n", type_name.c_str());
		fprintf(header, "\tIPublishableComposer composer;\n");
		fprintf(header, "\tUInt64[] address;\n");

		csharpPub_generateAddressEnum(header, s);

		if (s.type == CompositeType::Type::publishable)
		{
			fprintf(header, "\tpublic %s_publisher()\n", type_name.c_str());
			fprintf(header, "\t{\n");
			fprintf(header, "\t\tthis.t = new %s();\n", type_name.c_str());
			fprintf(header, "\t\tthis.composer = null;\n");
			fprintf(header, "\t\tthis.address = new UInt64[] { };\n");
			fprintf(header, "\t}\n");
		}
		else
		{
			fprintf(header, "\tpublic %s_publisher(I%s t, IPublishableComposer composer, UInt64[] address)\n", type_name.c_str(), type_name.c_str());
			fprintf(header, "\t{\n");
			fprintf(header, "\t\tthis.t = t;\n");
			fprintf(header, "\t\tthis.composer = composer;\n");
			fprintf(header, "\t\tthis.address = address;\n");
			fprintf(header, "\t}\n");
		}
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


		fprintf(header, "\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
		fprintf(header, "\tpublic I%s debugOnlyGetData() { return this.t; }\n", type_name.c_str());

		fprintf(header, "\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
		fprintf(header, "\tpublic void debugOnlySetData(I%s data) { this.t = data; }\n", type_name.c_str());


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


		fprintf(header, "public class %s_concentrator : %s_subscriber, StateConcentratorBase\n", type_name.c_str(), type_name.c_str());

		////if (s.type == CompositeType::Type::publishable)
		////	fprintf(header, " : StateConcentratorBase");

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

	void csharpPub_generateConcentratorFactory(FILE* header, Root& root)
	{
		assert(!root.publishables.empty());

		fprintf(header, "public class StateConcentratorFactory : IStateConcentratorFactory\n");
		fprintf(header, "{\n");
		fprintf(header, "\tpublic StateConcentratorBase createConcentrator(UInt64 typeID)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tswitch(typeID)\n");
		fprintf(header, "\t\t{\n");
		for (auto& it : root.publishables)
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
}




void generateCsharpPublishables( FILE* header, Root& root, const std::string& metascope )
{
	vector<CompositeType*> structsOrderedByDependency;
	std::unordered_set<size_t> collElementTypes;
	orderStructsByDependency( root.structs, structsOrderedByDependency, collElementTypes );
	
	for ( auto it : structsOrderedByDependency )
	{
		assert( it != nullptr );
		assert( it->type == CompositeType::Type::structure || it->type == CompositeType::Type::discriminated_union);
		if ( it->isStruct4Publishing )
		{
			std::string name = csharpPub_getTypeName(*it);

			csharpPub_generateStructSubs(header, root, *it, name);
			csharpPub_generateStructPubl(header, root, *it, name);
		}
	}

	for ( auto& it : root.publishables )
	{
		auto& obj_1 = it;
		assert(it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert(it->type == CompositeType::Type::publishable );

		checkCsharpStruct(*it);

		std::string type_name = getCSharpTypeName(*it);
		std::string interface_name = "I" + type_name;

		generateCsharpStructInterface(header, root, *it, type_name.c_str());
		generateCsharpStructImpl(header, root, *it, type_name.c_str(), interface_name.c_str());

		impl_generatePublishableCommentBlock(header, *it);

		csharpPub_generateStructSubs(header, root, *it, type_name);
		csharpPub_generateStructPubl(header, root, *it, type_name);
		csharpPub_generateStructConcentrator(header, root, *it, type_name);
	}
	//fprintf( header, "\n//===============================================================================\n\n" );

	if (!root.publishables.empty())
		csharpPub_generateConcentratorFactory(header, root);

}

