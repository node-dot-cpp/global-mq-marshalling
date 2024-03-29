/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
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

#include "csharp2.h"
#include "idl_tree_common.h"

#include <set>



namespace csharp2
{

	void csharpPub_generateAddressEnum(FileWritter f, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);



		f.write("\tenum Address\n");
		f.write("\t{\n");

		auto& mem = s.getMembers();
		size_t sz = mem.size();
		for (size_t i = 0; i != sz; ++i)
		{
			auto& member = mem[i];
			assert(member != nullptr);

			string number = std::to_string(i);
			if(i != sz - 1)
				f.write("\t\t%s = %s,\n", member->name.c_str(), number.c_str());
			else
				f.write("\t\t%s = %s\n", member->name.c_str(), number.c_str());
		}
		f.write("\t}\n");
	}

	void csharpPub_generateParseStateSync(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union_case);


		f.write("\tpublic static void parseForStateSync(IPublishableParser parser, String expectedName, I%s data)\n", type_name.c_str());
		f.write("\t{\n");
		f.write("\t\tparser.parseKey(expectedName);\n");
		f.write("\t\tparseForStateSync(parser, data);\n");
		f.write("\t}\n");

		f.write("\tpublic static %s parseForStateSync(IPublishableParser parser)\n", type_name.c_str());
		f.write("\t{\n");
		f.write("\t\t%s data = new %s();\n", type_name.c_str(), type_name.c_str());
		f.write("\t\tparseForStateSync(parser, data);\n");
		f.write("\t\treturn data;\n");
		f.write("\t}\n");

		f.write("\tstatic void parseForStateSync(IPublishableParser parser, I%s data)\n", type_name.c_str());
		f.write("\t{\n");

		f.write("\t\tparser.parseStructBegin();\n");

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
				const char* idl = getIdlPrimitiveType(member.type.kind);
				f.write("\t\tdata.%s = parser.parse%s(\"%s\");\n", member.name.c_str(), idl, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				//f.write("\t\tparser.parsePublishableStructBegin(\"%s\");\n", member.name.c_str());
				f.write("\t\t%s_subscriber.parseForStateSync(parser, \"%s\", data.%s);\n", member.type.name.c_str(), member.name.c_str(), member.name.c_str());
				//f.write("\t\tparser.parsePublishableStructEnd();\n");
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
					const char* elem_type_name = getCSharpPrimitiveType(member.type.vectorElemKind);
					f.write("\t\tparser.parseVector2(\"%s\", data.%s, %s_subscriber.parseForStateSync);\n", member.name.c_str(), member.name.c_str(), elem_type_name);
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* elem_type_name = member.type.name.c_str();
					f.write("\t\tparser.parseVector2(\"%s\", data.%s, %s_subscriber.parseForStateSync);\n", member.name.c_str(), member.name.c_str(), elem_type_name);
					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);

				switch (member.type.dictionaryValueKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
					f.write("\t\tparser.parseDictionary(\"%s\", data.%s,\n", member.name.c_str(), member.name.c_str());
					f.write("\t\t\t%s_subscriber.parseForStateSync,\n", key);
					f.write("\t\t\t%s_subscriber.parseForStateSync\n", value);
					f.write("\t\t);\n");
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* value = member.type.name.c_str();

					f.write("\t\tparser.parseDictionary(\"%s\", data.%s,\n", member.name.c_str(), member.name.c_str());
					f.write("\t\t\t%s_subscriber.parseForStateSync,\n", key);
					f.write("\t\t\t%s_subscriber.parseForStateSync\n", value);
					f.write("\t\t);\n");
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
		f.write("\t\tparser.parseStructEnd();\n");

		f.write("\t}\n");
	}

	void csharpPub_generateParse1(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union_case);


		f.write("\tpublic static bool parse(IPublishableParser parser, String expectedName, %s_subscriber subscriber)\n", type_name.c_str());
		f.write("\t{\n");

		f.write("\t\tparser.parseKey(expectedName);\n");
		f.write("\t\treturn parse(parser, subscriber);\n");
		f.write("\t}\n");

		f.write("\tpublic static bool parse_notify(IPublishableParser parser, %s_subscriber subscriber)\n", type_name.c_str());
		f.write("\t{\n");
		f.write("\t\treturn parse(parser, subscriber);\n");
		f.write("\t}\n");


		f.write("\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber)\n", type_name.c_str());
		f.write("\t{\n");

		f.write("\t\tparser.parseStructBegin();\n");


		f.write("\t\tbool changed = false;\n");


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
				const char* type_name = getCSharpPrimitiveType(member.type.kind);
				const char* parse_method = getIdlPrimitiveType(member.type.kind);

				f.write("\t\tchanged = subscriber.update_%s(parser, \"%s\") | changed;\n", member.name.c_str(), member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\t\t{\n");
				f.write("\t\t\tbool currentChanged = %s_subscriber.parse(parser, \"%s\", subscriber.lazy_%s_handler());\n", member.type.name.c_str(), member.name.c_str(), member.name.c_str());

				f.write("\t\t\tif(currentChanged)\n");
				f.write("\t\t\t{\n");
				f.write("\t\t\t\t\tchanged = true;\n");
				f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
				f.write("\t\t\t}\n");
				f.write("\t\t}\n");
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
					const char* elem_type_name = getCSharpPrimitiveType(member.type.vectorElemKind);
					const char* idl = getIdlPrimitiveType(member.type.vectorElemKind);

					f.write("\t\t{\n");
					f.write("\t\t\tIList<%s> newVal = new List<%s>();\n", elem_type_name, elem_type_name);
					f.write("\t\t\tparser.parseVector2(\"%s\", newVal, %s_subscriber.parseForStateSync);\n", member.name.c_str(), elem_type_name);

					f.write("\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber._data.%s))\n", member.name.c_str());
					f.write("\t\t\t{\n");
					f.write("\t\t\t\tsubscriber._data.%s = newVal;\n", member.name.c_str());
					f.write("\t\t\t\tchanged = true;\n");
					f.write("\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					f.write("\t\t\t}\n");
					f.write("\t\t}\n");

					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* elem_type_name = member.type.name.c_str();

					f.write("\t\t{\n");
					f.write("\t\t\tList<I%s> newVal = new List<I%s>();\n", elem_type_name, elem_type_name);
					f.write("\t\t\tparser.parseVectorComplex(\"%s\", newVal, %s_subscriber.parse_notify, subscriber.makeElementHandler_%s);\n", member.name.c_str(), elem_type_name, member.name.c_str());

					f.write("\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.lazy_%s_handlers()))\n", member.name.c_str());
					f.write("\t\t\t{\n");
					// f.write("\t\t\t\tsubscriber._data.%s = newVal;\n", member.name.c_str());
					f.write("\t\t\t\tsubscriber.%s_handlers = newVal;\n", member.name.c_str());
					f.write("\t\t\t\tchanged = true;\n");
					f.write("\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					f.write("\t\t\t}\n");
					f.write("\t\t}\n");
					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
				const char* idlKey = getIdlPrimitiveType(member.type.dictionaryKeyKind);

				switch (member.type.dictionaryValueKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
					const char* idlValue = getIdlPrimitiveType(member.type.dictionaryValueKind);

					f.write("\t\t{\n");
					f.write("\t\t\tDictionary<%s, %s> newVal = new Dictionary<%s, %s>();\n", key, value, key, value);
			
					f.write("\t\tparser.parseDictionary(\"%s\", newVal,\n", member.name.c_str());
					f.write("\t\t\t%s_subscriber.parseForStateSync,\n", key);
					f.write("\t\t\t%s_subscriber.parseForStateSync\n", value);
					f.write("\t\t);\n");

					f.write("\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber._data.%s))\n", member.name.c_str());
					f.write("\t\t\t{\n");
					f.write("\t\t\t\tsubscriber._data.%s = newVal;\n", member.name.c_str());
					f.write("\t\t\t\tchanged = true;\n");
					f.write("\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					f.write("\t\t\t}\n");
					f.write("\t\t}\n");

					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* value = member.type.name.c_str();

					f.write("\t\t{\n");
					f.write("\t\t\tDictionary<%s, I%s> newVal = new Dictionary<%s, I%s>();\n", key, value, key, value);
					f.write("\t\t\tparser.parseDictionaryComplex(\"%s\", newVal,\n", member.name.c_str());
					f.write("\t\t\t\t%s_subscriber.parseForStateSync,\n", key);
					f.write("\t\t\t\t%s_subscriber.parse_notify,\n", value);
					f.write("\t\t\t\tsubscriber.makeValueHandler_%s\n", member.name.c_str());
					f.write("\t\t\t);\n");

					f.write("\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.lazy_%s_handlers()))\n", member.name.c_str());
					f.write("\t\t\t{\n");
					// f.write("\t\t\t\tsubscriber._data.%s = newVal;\n", member.name.c_str());
					f.write("\t\t\t\tsubscriber.%s_handlers = newVal;\n", member.name.c_str());
					f.write("\t\t\t\tchanged = true;\n");
					f.write("\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					f.write("\t\t\t}\n");
					f.write("\t\t}\n");
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
		f.write("\t\tparser.parseStructEnd();\n");

		f.write("\t\tif(changed)\n");
		f.write("\t\t\tsubscriber.notifyUpdated();\n");

		f.write("\t\treturn changed;\n");
		f.write("\t}\n");
	}

	void csharpPub_generateParse2(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union_case);



		f.write("\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber, UInt64[] addr, int offset)\n", type_name.c_str());
		f.write("\t{\n");

		f.write("\t\tbool changed = false;\n");

		f.write("\t\tswitch ((Address)addr[offset])\n");
		f.write("\t\t{\n");

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;


			f.write("\t\t\tcase Address.%s:\n", member.name.c_str());
			f.write("\t\t\t{\n");

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* type_name = getCSharpPrimitiveType(member.type.kind);
				const char* parse_method = getIdlPrimitiveType(member.type.kind);

				f.write("\t\t\t\tif(addr.Length != offset + 1)\n");
				f.write("\t\t\t\t\tthrow new Exception();\n");
				f.write("\t\t\t\tchanged = subscriber.update_%s(parser, \"value\") | changed;\n", member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\t\t\t\tbool currentChanged = false;\n");
				f.write("\t\t\t\tif(addr.Length == offset + 1) // full element replace\n");
				f.write("\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, \"value\", subscriber.lazy_%s_handler());\n", member.type.name.c_str(), member.name.c_str());
				f.write("\t\t\t\telse if(addr.Length > offset + 1) // let child continue parsing\n");
				f.write("\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handler(), addr, offset + 1);\n", member.type.name.c_str(), member.name.c_str());
				f.write("\t\t\t\telse\n");
				f.write("\t\t\t\t\tthrow new Exception();\n\n");

				f.write("\t\t\t\tif(currentChanged)\n");
				f.write("\t\t\t\t{\n");
				f.write("\t\t\t\t\tchanged = true;\n");
				f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
				f.write("\t\t\t\t}\n");

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
					const char* elem_type_name = getCSharpPrimitiveType(member.type.vectorElemKind);
					const char* idl = getIdlPrimitiveType(member.type.vectorElemKind);

					f.write("\t\t\t\tbool currentChanged = false;\n");
					f.write("\t\t\t\tif(addr.Length == offset + 1) // full vector replace\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tIList<%s> newVal = new List<%s>();\n", elem_type_name, elem_type_name);
					f.write("\t\t\t\t\tparser.parseVector2(\"value\", newVal, %s_subscriber.parseForStateSync);\n", elem_type_name);

					f.write("\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber._data.%s))\n", member.name.c_str());
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\tsubscriber._data.%s = newVal;\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tint index = (int)addr[offset + 1];\n");

					f.write("\t\t\t\t\tPublishable.ActionOnVector action = (Publishable.ActionOnVector)parser.parseActionInPublishable();\n");

					f.write("\t\t\t\t\tswitch (action)\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\tcase Publishable.ActionOnVector.update_at:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t%s newVal = parser.parse%s(\"value\");\n", elem_type_name, idl);
					f.write("\t\t\t\t\t\tif (!subscriber._data.%s[index].Equals(newVal))\n", member.name.c_str());
					f.write("\t\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t\t%s oldVal = subscriber._data.%s[index];\n", elem_type_name, member.name.c_str());
					f.write("\t\t\t\t\t\t\tsubscriber._data.%s[index] = newVal;\n", member.name.c_str());
					f.write("\t\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\t\tsubscriber.notifyElementUpdated_%s(index, oldVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\t}\n");
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");


					f.write("\t\t\t\t\tcase Publishable.ActionOnVector.insert_single_before:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t%s newVal = parser.parse%s(\"value\");\n", elem_type_name, idl);
					f.write("\t\t\t\t\t\tsubscriber._data.%s.Insert(index, newVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\tsubscriber.notifyInserted_%s(index);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t\tcase Publishable.ActionOnVector.remove_at:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t%s oldVal = subscriber._data.%s[index];\n", elem_type_name, member.name.c_str());
					f.write("\t\t\t\t\t\tsubscriber._data.%s.RemoveAt(index);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\tsubscriber.notifyErased_%s(index, oldVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");
					f.write("\t\t\t\t\tdefault:\n");
					f.write("\t\t\t\t\t\tthrow new Exception();\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse // simple type can't handle deeper address\n");
					f.write("\t\t\t\t\tthrow new Exception();\n\n");

					f.write("\t\t\t\tif(currentChanged)\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tchanged = true;\n");
					f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					f.write("\t\t\t\t}\n");

					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* elem_type_name = member.type.name.c_str();

					f.write("\t\t\t\tbool currentChanged = false;\n");
					f.write("\t\t\t\tif(addr.Length == offset + 1) // full vector replace\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tList<I%s> newVal = new List<I%s>();\n", elem_type_name, elem_type_name);

					f.write("\t\t\t\t\tparser.parseVectorComplex(\"value\", newVal, %s_subscriber.parse_notify, subscriber.makeElementHandler_%s);\n", elem_type_name, member.name.c_str());
					f.write("\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.lazy_%s_handlers()))\n", member.name.c_str());
					f.write("\t\t\t\t\t{\n");
					// f.write("\t\t\t\t\t\tsubscriber._data.%s = newVal;\n", member.name.c_str());
					f.write("\t\t\t\t\t\tsubscriber.%s_handlers = newVal;\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tint index = (int)addr[offset + 1];\n");
					f.write("\t\t\t\t\tPublishable.ActionOnVector action = (Publishable.ActionOnVector)parser.parseActionInPublishable();\n");

					f.write("\t\t\t\t\tswitch (action)\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\tcase Publishable.ActionOnVector.update_at:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\tbool elemChanged = %s_subscriber.parse(parser, \"value\", (%s_subscriber)(subscriber.lazy_%s_handlers()[index]));\n", elem_type_name, elem_type_name, member.name.c_str());
					f.write("\t\t\t\t\t\tif (elemChanged)\n");
					f.write("\t\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\t\tsubscriber.notifyElementUpdated_%s(index);\n", member.name.c_str());
					f.write("\t\t\t\t\t\t}\n");
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");


					f.write("\t\t\t\t\tcase Publishable.ActionOnVector.insert_single_before:\n");
					f.write("\t\t\t\t\t{\n");
					// f.write("\t\t\t\t\t\tI%s newVal = new %s();\n", elem_type_name, elem_type_name);
					f.write("\t\t\t\t\t\t%s_subscriber handler = subscriber.makeElementHandler_%s(null);\n", elem_type_name, member.name.c_str());
					f.write("\t\t\t\t\t\t%s_subscriber.parse(parser, \"value\", handler);\n", elem_type_name);
					// f.write("\t\t\t\t\t\t// mb: lazy initialization always first\n");
					f.write("\t\t\t\t\t\tsubscriber.lazy_%s_handlers().Insert(index, handler);\n", member.name.c_str());
					// f.write("\t\t\t\t\t\tsubscriber._data.%s.Insert(index, newVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\tsubscriber.notifyInserted_%s(index);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t\tcase Publishable.ActionOnVector.remove_at:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\tI%s oldVal = subscriber.lazy_%s_handlers()[index];\n", elem_type_name, member.name.c_str());
					// f.write("\t\t\t\t\t\t// mb: lazy initialization always first\n");
					f.write("\t\t\t\t\t\tsubscriber.lazy_%s_handlers().RemoveAt(index);\n", member.name.c_str());
					// f.write("\t\t\t\t\t\tsubscriber._data.%s.RemoveAt(index);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\tsubscriber.notifyErased_%s(index, oldVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");
					f.write("\t\t\t\t\tdefault:\n");
					f.write("\t\t\t\t\t\tthrow new Exception();\n");
					f.write("\t\t\t\t\t}\n");


					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse if(addr.Length > offset + 2) // let child continue parsing\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tint index = (int)addr[offset + 1];\n");
					f.write("\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, (%s_subscriber)(subscriber.lazy_%s_handlers()[index]), addr, offset + 2);\n", elem_type_name, elem_type_name, member.name.c_str());
					f.write("\t\t\t\t\tif (currentChanged)\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\tsubscriber.notifyElementUpdated_%s(index);\n", member.name.c_str());
					f.write("\t\t\t\t\t}\n");
					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse\n");
					f.write("\t\t\t\t\tthrow new Exception();\n\n");

					f.write("\t\t\t\tif(currentChanged)\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tchanged = true;\n");
					f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					f.write("\t\t\t\t}\n");


					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
				const char* idlKey = getIdlPrimitiveType(member.type.dictionaryKeyKind);

				switch (member.type.dictionaryValueKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
					const char* idlValue = getIdlPrimitiveType(member.type.dictionaryValueKind);

					f.write("\t\t\t\tbool currentChanged = false;\n");
					f.write("\t\t\t\tif(addr.Length == offset + 1) // full dictionary replace\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tDictionary<%s, %s> newVal = new Dictionary<%s, %s>();\n", key, value, key, value);
					//f.write("\t\t\t\t\tparser.parseSimpleVector(\"value\", newVal);\n");
					//f.write("\t\tparser.parseSimpleVector(\"%s\", data.%s);\n", member.name.c_str(), member.name.c_str());
					f.write("\t\t\t\t\tparser.parseDictionary(\"value\", newVal,\n");
					f.write("\t\t\t\t\t\t%s_subscriber.parseForStateSync,\n", key);
					f.write("\t\t\t\t\t\t%s_subscriber.parseForStateSync\n", value);
					f.write("\t\t\t\t\t);\n");

					f.write("\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber._data.%s))\n", member.name.c_str());
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\tsubscriber._data.%s = newVal;\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tPublishable.ActionOnDictionary action = (Publishable.ActionOnDictionary)addr[offset + 1];\n");
					f.write("\t\t\t\t\tswitch (action)\n");
					f.write("\t\t\t\t\t{\n");

					f.write("\t\t\t\t\tcase Publishable.ActionOnDictionary.update_value:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t%s key = parser.parse%s(\"key\");\n", key, idlKey);
					f.write("\t\t\t\t\t\t%s newVal = parser.parse%s(\"value\");\n", value, idlValue);
					f.write("\t\t\t\t\t\t%s oldVal = subscriber._data.%s[key];\n", value, member.name.c_str());
					f.write("\t\t\t\t\t\tsubscriber._data.%s[key] = newVal;\n", member.name.c_str());
					f.write("\t\t\t\t\t\tif (newVal != oldVal)\n");
					f.write("\t\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\t\tsubscriber.notifyValueUpdated_%s(key, oldVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\t}\n");
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t\tcase Publishable.ActionOnDictionary.insert:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t%s key = parser.parse%s(\"key\");\n", key, idlKey);
					f.write("\t\t\t\t\t\t%s newVal = parser.parse%s(\"value\");\n", value, idlValue);
					f.write("\t\t\t\t\t\tsubscriber._data.%s.Add(key, newVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\tsubscriber.notifyInserted_%s(key, newVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t\tcase Publishable.ActionOnDictionary.remove:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t%s key = parser.parse%s(\"key\");\n", key, idlKey);
					f.write("\t\t\t\t\t\t%s oldVal = subscriber._data.%s[key];\n", value, member.name.c_str());
					f.write("\t\t\t\t\t\tsubscriber._data.%s.Remove(key);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\tsubscriber.notifyRemoved_%s(key, oldVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");
					f.write("\t\t\t\t\tdefault:\n");
					f.write("\t\t\t\t\t\tthrow new Exception();\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse // simple type can't handle deeper address\n");
					f.write("\t\t\t\t\tthrow new Exception();\n\n");

					f.write("\t\t\t\tif(currentChanged)\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tchanged = true;\n");
					f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					f.write("\t\t\t\t}\n");

					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* value = member.type.name.c_str();

					f.write("\t\t\t\tbool currentChanged = false;\n");
					f.write("\t\t\t\tif(addr.Length == offset + 1) // full vector replace\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tDictionary<%s, I%s> newVal = new Dictionary<%s, I%s>();\n", key, value, key, value);
					f.write("\t\t\t\t\tparser.parseDictionaryComplex(\"value\", newVal,\n");
					f.write("\t\t\t\t\t\t%s_subscriber.parseForStateSync,\n", key);
					f.write("\t\t\t\t\t\t%s_subscriber.parse_notify,\n", value);
					f.write("\t\t\t\t\t\tsubscriber.makeValueHandler_%s\n", member.name.c_str());
					f.write("\t\t\t\t\t);\n");
					f.write("\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.lazy_%s_handlers()))\n", member.name.c_str());
					f.write("\t\t\t\t\t{\n");
					// f.write("\t\t\t\t\t\tsubscriber._data.%s = newVal;\n", member.name.c_str());
					f.write("\t\t\t\t\t\tsubscriber.%s_handlers = newVal;\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t}\n");

					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tPublishable.ActionOnDictionary action = (Publishable.ActionOnDictionary)addr[offset + 1];\n");
					f.write("\t\t\t\t\tswitch (action)\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\tcase Publishable.ActionOnDictionary.update_value:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t%s key = parser.parse%s(\"key\");\n", key, idlKey);
					// f.write("\t\t\t\t\t\tI%s v = new %s();\n", value, value);
					f.write("\t\t\t\t\t\t%s_subscriber newVal = subscriber.makeValueHandler_%s(null);\n", value, member.name.c_str());
					f.write("\t\t\t\t\t\t%s_subscriber.parse(parser, \"value\", newVal);\n", value);
					f.write("\t\t\t\t\t\tI%s oldValue = subscriber.lazy_%s_handlers()[key];\n", value, member.name.c_str());
					f.write("\t\t\t\t\t\tsubscriber.lazy_%s_handlers()[key] = newVal;\n", member.name.c_str());
					// f.write("\t\t\t\t\t\tsubscriber._data.%s[key] = v;\n", member.name.c_str());
					f.write("\t\t\t\t\t\tif (!newVal.isEquivalent(oldValue))\n");
					f.write("\t\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\t\tsubscriber.notifyValueUpdated_%s(key, oldValue);\n", member.name.c_str());
					f.write("\t\t\t\t\t\t}\n");
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");


					f.write("\t\t\t\t\tcase Publishable.ActionOnDictionary.insert:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t%s key = parser.parse%s(\"key\");\n", key, idlKey);
					// f.write("\t\t\t\t\t\tI%s v = new %s();\n", value, value);
					f.write("\t\t\t\t\t\t%s_subscriber newVal = subscriber.makeValueHandler_%s(null);\n", value, member.name.c_str());
					f.write("\t\t\t\t\t\t%s_subscriber.parse(parser, \"value\", newVal);\n", value);
					f.write("\t\t\t\t\t\tsubscriber.lazy_%s_handlers().Add(key, newVal);\n", member.name.c_str());
					// f.write("\t\t\t\t\t\tsubscriber._data.%s.Add(key, v);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\tsubscriber.notifyInserted_%s(key);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");



					f.write("\t\t\t\t\tcase Publishable.ActionOnDictionary.remove:\n");
					f.write("\t\t\t\t\t{\n");
					f.write("\t\t\t\t\t\t%s key = parser.parse%s(\"key\");\n", key, idlKey);
					f.write("\t\t\t\t\t\tI%s oldVal = subscriber.lazy_%s_handlers()[key];\n", value, member.name.c_str());
					f.write("\t\t\t\t\t\tsubscriber.lazy_%s_handlers().Remove(key);\n", member.name.c_str());
					// f.write("\t\t\t\t\t\tsubscriber._data.%s.Remove(key);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
					f.write("\t\t\t\t\t\tsubscriber.notifyRemoved_%s(key, oldVal);\n", member.name.c_str());
					f.write("\t\t\t\t\t\tbreak;\n");
					f.write("\t\t\t\t\t}\n");
					f.write("\t\t\t\t\tdefault:\n");
					f.write("\t\t\t\t\t\tthrow new Exception();\n");
					f.write("\t\t\t\t\t}\n");


					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse if(addr.Length > offset + 2) // let child continue parsing\n");
					f.write("\t\t\t\t{\n");

					if (member.type.dictionaryKeyKind == MessageParameterType::KIND::UINTEGER)
					{
						f.write("\t\t\t\t\t%s key = addr[offset + 1];\n", key);
						f.write("\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, (%s_subscriber)(subscriber.lazy_%s_handlers()[key]), addr, offset + 2);\n", value, value, member.name.c_str());
					}
					else if (member.type.dictionaryKeyKind == MessageParameterType::KIND::INTEGER)
					{
						f.write("\t\t\t\t\t%s key = ZigZag.decode(addr[offset + 1]);\n", key, key);
						f.write("\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, (%s_subscriber)(subscriber.lazy_%s_handlers()[key]), addr, offset + 2);\n", value, value, member.name.c_str());
					}
					else if (member.type.dictionaryKeyKind == MessageParameterType::KIND::CHARACTER_STRING)
					{
						f.write("\t\t\t\t\tint newOffset = offset + 1;\n", key, key);
						f.write("\t\t\t\t\t%s key = AsciiAddress.decode(addr, ref newOffset);\n", key, key);
						f.write("\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, (%s_subscriber)(subscriber.lazy_%s_handlers()[key]), addr, newOffset);\n", value, value, member.name.c_str());
					}
					else
					{
						f.write("\t\t\t\t\tthrow new Exception(); // key type not supported for deeper address\n");
					}
					// f.write("\t\t\t\t\t\tif (currentChanged)\n");
					// f.write("\t\t\t\t\t\t{\n");
					// f.write("\t\t\t\t\t\t\tsubscriber.notifyValueUpdated_%s(key, oldVal);\n", member.name.c_str());
					// f.write("\t\t\t\t\t\t}\n");

					f.write("\t\t\t\t}\n");
					f.write("\t\t\t\telse\n");
					f.write("\t\t\t\t\tthrow new Exception();\n\n");

					f.write("\t\t\t\tif(currentChanged)\n");
					f.write("\t\t\t\t{\n");
					f.write("\t\t\t\t\tchanged = true;\n");
					f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
					f.write("\t\t\t\t}\n");


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
			f.write("\t\t\t\tbreak;\n");
			f.write("\t\t\t}\n");

		}

		f.write("\t\t\tdefault:\n");
		f.write("\t\t\t\tthrow new Exception();\n");

		f.write("\t\t}\n");

		f.write("\t\tif(changed)\n");
		f.write("\t\t\tsubscriber.notifyUpdated();\n");

		f.write("\t\treturn changed;\n");
		f.write("\t}\n");
	}

	void csharpPub_generateStateSubscriberBase(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);



		f.write("\tpublic String stateSubscriberName() { return \"%s\"; }\n", s.name.c_str());

		string number = std::to_string(s.numID);
		f.write("\tpublic UInt64 stateTypeID() { return %s; }\n", number.c_str());

		f.write("\tpublic void applyMessageWithUpdates(IPublishableParser parser)\n");
		f.write("\t{\n");
		f.write("\t\tparser.parseStateUpdateMessageBegin();\n");
		f.write("\t\tUInt64[] addr = null;\n");
		f.write("\t\tbool changed = false;\n");
		f.write("\t\twhile(parser.parseAddress(ref addr))\n");
		f.write("\t\t{\n");
		f.write("\t\t\tchanged = %s_subscriber.parse(parser, this, addr, 0) | changed;\n", type_name.c_str());
		f.write("\t\t\tparser.parseAddressEnd();\n");
		f.write("\t\t\taddr = null;\n");
		f.write("\t\t}\n");
		f.write("\t\tparser.parseStateUpdateMessageEnd();\n");
		// f.write("\t\tif(changed)\n");
		// f.write("\t\t\tthis.notifyUpdated();\n");
		f.write("\t}\n");

		f.write("\tpublic void applyStateSyncMessage(IPublishableParser parser)\n");
		f.write("\t{\n");
		//f.write("\t\tparser.parseStructBegin();\n");
		//f.write("\t\t new %s();\n", type_name.c_str(), type_name.c_str());
		f.write("\t\t%s val = %s_subscriber.parseForStateSync(parser);\n", type_name.c_str(), type_name.c_str());
		//f.write("\t\tparser.parseStructEnd();\n");
		f.write("\t\tthis._data = val;\n");
		f.write("\t\tthis._reset_handlers();\n");
		f.write("\t\tthis.notifyFullyUpdated();\n");
		f.write("\t}\n");
	}

	void csharpPub_generateEventHandlerSubs(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);



		if (s.type == CompositeType::Type::publishable)
			f.write("\tpublic virtual void notifyFullyUpdated() { }\n");

		f.write("\tpublic virtual void notifyUpdated() { }\n");
		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			generateCsharpSubscriberEventHandler(f, *it);
		}
	}



	void csharpPub_generateFactoryMethodSubs(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			generateCsharpSubscriberFactoryMethod(f, *it);
		}
	}

	void csharpPub_generateSubscriberResetHandlers(FileWritter f, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);



		f.write("\tpublic void _reset_handlers()\n");
		f.write("\t{\n");

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
				f.write("\t\tthis.%s_handler = null;\n", member.name.c_str());
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
					f.write("\t\tthis.%s_handlers = null;\n", member.name.c_str());
					break;
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				switch (member.type.dictionaryValueKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					f.write("\t\tthis.%s_handlers = null;\n", member.name.c_str());
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
		f.write("\t}\n");
	}


	void csharpPub_generateCompose(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union_case);


		f.write("\tpublic static void compose(IPublishableComposer composer, String name, I%s val, bool separator)\n", type_name.c_str());
		f.write("\t{\n");
		f.write("\t\tcomposer.composeKey(name);\n");
		f.write("\t\tcompose(composer, val);\n");
		f.write("\t\tcomposer.addSeparator(separator);\n");


		f.write("\t}\n");



		f.write("\tpublic static void compose(IPublishableComposer composer, I%s t)\n", type_name.c_str());
		f.write("\t{\n");
		f.write("\t\tcomposer.composeStructBegin();\n");

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;

			const char* last = (i == mem.size() - 1) ? "false" : "true";

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				f.write("\t\tcomposer.compose%s(\"%s\", t.%s, %s);\n", getIdlPrimitiveType(member.type.kind), member.name.c_str(), member.name.c_str(), last);
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				//f.write("\t\tcomposer.composePublishableStructBegin(\"%s\");\n");
				f.write("\t\t%s_publisher.compose(composer, \"%s\", t.%s, %s);\n", member.type.name.c_str(), member.name.c_str(), member.name.c_str(), last);
				//f.write("\t\tcomposer.composePublishableStructEnd(%s);\n", last);
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
					const char* elem = getCSharpPrimitiveType(member.type.vectorElemKind);
					const char* idl = getIdlPrimitiveType(member.type.vectorElemKind);
					f.write("\t\tcomposer.composeVector2(\"%s\", t.%s,\n", member.name.c_str(), member.name.c_str());
					f.write("\t\t\t(IPublishableComposer c, %s v) => { c.compose%s(null, v, false); },\n", elem, idl);
					f.write("\t\t\t%s);\n", last);
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* elem_type_name = member.type.name.c_str();
					f.write("\t\tcomposer.composeVector2(\"%s\", t.%s,\n", member.name.c_str(), member.name.c_str());
					f.write("\t\t\t%s_publisher.compose,\n", elem_type_name);

					//f.write("\t\t\t(IPublishableComposer composer, I%s val) =>\n", elem_type_name);
					//f.write("\t\t\t{\n");
					////f.write("\t\t\t\tcomposer.composePublishableStructBegin(null);\n");
					//f.write("\t\t\t\t%s_publisher.compose(composer, val);\n", elem_type_name);
					////f.write("\t\t\t\tcomposer.composePublishableStructEnd(false);\n");
					//f.write("\t\t\t},\n");

					f.write("\t\t\t%s);\n", last);
					break;
				}
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			case MessageParameterType::KIND::DICTIONARY:
			{
				const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
				const char* idlKey = getIdlPrimitiveType(member.type.dictionaryKeyKind);
				switch (member.type.dictionaryValueKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
					const char* idlValue = getIdlPrimitiveType(member.type.dictionaryValueKind);
					f.write("\t\tcomposer.composeDictionary(\"%s\", t.%s,\n", member.name.c_str(), member.name.c_str());
					f.write("\t\t\t(IPublishableComposer c, %s v) => { c.compose%s(null, v, false); },\n", key, idlKey);
					f.write("\t\t\t(IPublishableComposer c, %s v) => { c.compose%s(null, v, false); },\n", value, idlValue);
					f.write("\t\t\t%s);\n", last);
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* value = member.type.name.c_str();
					f.write("\t\tcomposer.composeDictionary(\"%s\", t.%s,\n", member.name.c_str(), member.name.c_str());
					f.write("\t\t\t(IPublishableComposer c, %s v) => { c.compose%s(null, v, false); },\n", key, idlKey);
					f.write("\t\t\t%s_publisher.compose,\n", value);
					f.write("\t\t\t%s);\n", last);
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

		f.write("\t\tcomposer.composeStructEnd();\n");

		f.write("\t}\n");
	}


	void csharpPub_generateStatePublishableBase(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);



		f.write("\tpublic int idx { get; set; } // for use in pools, etc\n");
		f.write("\tpublic String statePublisherName() { return \"%s\"; }\n", s.name.c_str());
		string number = std::to_string(s.numID);
		f.write("\tpublic UInt64 stateTypeID() { return %s; }\n", number.c_str());

		f.write("\tpublic void generateStateSyncMessage(IPublishableComposer composer)\n");
		f.write("\t{\n");
		//f.write("\t\tcomposer.composeStructBegin();\n");
		f.write("\t\t%s_publisher.compose(composer, this.data_);\n", type_name.c_str());
		//f.write("\t\tcomposer.composeStructEnd();\n");
		f.write("\t}\n");
		
		f.write("\tpublic void startTick(IPublishableComposer composer)\n");
		f.write("\t{\n");
		f.write("\t\tthis.composer_ = composer;\n");
		f.write("\t\tthis.composer_.composeStateUpdateMessageBegin();\n");
		f.write("\t}\n");

		f.write("\tpublic IPublishableComposer endTick()\n");
		f.write("\t{\n");
		f.write("\t\tthis.composer_.composeStateUpdateMessageEnd();\n");
		f.write("\t\tIPublishableComposer tmp = this.composer_;\n");
		f.write("\t\tthis.composer_ = null;\n");
		f.write("\t\treturn tmp;\n");
		f.write("\t}\n");
	}



	void csharpPub_generateStateConcentratorBase(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);



		f.write("\tpublic void generateStateSyncMessage(IPublishableComposer composer)\n");
		f.write("\t{\n");
		//f.write("\t\tcomposer.composeStructBegin();\n");
		f.write("\t\t%s_publisher.compose(composer, this._data);\n", type_name.c_str());
		//f.write("\t\tcomposer.composeStructEnd();\n");
		f.write("\t}\n");

	}


	void csharpPub_generateStructConcentrator(FileWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);



		f.write("public class %s_concentrator : %s_subscriber, StateConcentratorBase\n", type_name.c_str(), type_name.c_str());

		f.write("{\n");
		csharpPub_generateCompose(f, s, type_name);
		csharpPub_generateStateConcentratorBase(f, s, type_name);

		f.write("} // class %s_concentrator\n\n", type_name.c_str());
	}




void generateCsharpSubscriberFactoryMethod(FileWritter f, MessageParameter& member)
{


	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
		break;
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		f.write("\tpublic virtual %s_subscriber makeHandler_%s(I%s data) { return new %s_subscriber(data); }\n", member.type.name.c_str(), member.name.c_str(), member.type.name.c_str(), member.type.name.c_str());
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
			const char* elem_type_name = member.type.name.c_str();
			f.write("\tpublic virtual %s_subscriber makeElementHandler_%s(I%s data) { return new %s_subscriber(data); }\n", elem_type_name, member.name.c_str(), elem_type_name, elem_type_name);
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}
		break;
	}
	case MessageParameterType::KIND::DICTIONARY:
	{
		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* elem_type_name = member.type.name.c_str();
			f.write("\tpublic virtual %s_subscriber makeValueHandler_%s(I%s data) { return new %s_subscriber(data); }\n", elem_type_name, member.name.c_str(), elem_type_name, elem_type_name);
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

void generateCsharpSubscriberEventHandler(FileWritter f, MessageParameter& member)
{


	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
	{
		const char* type_name = getCSharpPrimitiveType(member.type.kind);
		f.write("\tpublic virtual void notifyUpdated_%s(%s old) { }\n", member.name.c_str(), type_name);
		break;
	}
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		f.write("\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
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
			const char* element = getCSharpPrimitiveType(member.type.vectorElemKind);
			f.write("\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyElementUpdated_%s(int index, %s old) { }\n", member.name.c_str(), element);
			f.write("\tpublic virtual void notifyInserted_%s(int index) { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyErased_%s(int index, %s oldVal) { }\n", member.name.c_str(), element);
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* element = member.type.name.c_str();
			f.write("\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyElementUpdated_%s(int index) { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyInserted_%s(int index) { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyErased_%s(int index, I%s oldVal) { }\n", member.name.c_str(), element);
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}
		break;
	}
	case MessageParameterType::KIND::DICTIONARY:
	{
		const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
			f.write("\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyValueUpdated_%s(%s key, %s oldValue) { }\n", member.name.c_str(), key, value);
			f.write("\tpublic virtual void notifyInserted_%s(%s key, %s newValue) { }\n", member.name.c_str(), key, value);
			f.write("\tpublic virtual void notifyRemoved_%s(%s key, %s oldValue) { }\n", member.name.c_str(), key, value);
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyValueUpdated_%s(%s key, I%s oldValue) { }\n", member.name.c_str(), key, value);
			f.write("\tpublic virtual void notifyInserted_%s(%s key) { }\n", member.name.c_str(), key);
			f.write("\tpublic virtual void notifyRemoved_%s(%s key, I%s oldValue) { }\n", member.name.c_str(), key, value);
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

void generateCsharpSubscriberMember(FileWritter f, MessageParameter& member)
{


	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
	{
		const char* type_name = getCSharpPrimitiveType(member.type.kind);
		const char* idl_name = getIdlPrimitiveType(member.type.kind);
		f.write("\tpublic %s %s\n", type_name, member.name.c_str());
		f.write("\t{\n");
		f.write("\t\tget { return this._data.%s; }\n", member.name.c_str());
		f.write("\t\tset { throw new InvalidOperationException(); }\n");
		f.write("\t}\n");
		f.write("\tbool update_%s(IPublishableParser parser, String name)\n", member.name.c_str());
		f.write("\t{\n");
		f.write("\t\t%s newVal = parser.parse%s(name);\n", type_name, idl_name);
		f.write("\t\tif (newVal != this._data.%s)\n", member.name.c_str());
		f.write("\t\t{\n");
		f.write("\t\t\t%s oldVal = this._data.%s;\n", type_name, member.name.c_str());
		f.write("\t\t\tthis._data.%s = newVal;\n", member.name.c_str());
		f.write("\t\t\tnotifyUpdated_%s(oldVal);\n", member.name.c_str());
		f.write("\t\t\treturn true;\n");
		f.write("\t\t}\n");
		f.write("\t\telse\n");
		f.write("\t\t\treturn false;\n");
		f.write("\t}\n");
		break;
	}
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		f.write("\t%s_subscriber %s_handler;\n", member.type.name.c_str(), member.name.c_str());
		f.write("\t%s_subscriber lazy_%s_handler()\n", member.type.name.c_str(), member.name.c_str());
		f.write("\t{ // mb: lazy because can't call virtual 'makeHandler' in ctor\n");
		f.write("\t\tif (%s_handler == null)\n", member.name.c_str());
		f.write("\t\t\t%s_handler = makeHandler_%s(this._data.%s);\n", member.name.c_str(), member.name.c_str(), member.name.c_str());
		f.write("\t\treturn %s_handler;\n", member.name.c_str());
		f.write("\t}\n");


		f.write("\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());
		f.write("\t{\n");
		f.write("\t\tget { return lazy_%s_handler(); }\n", member.name.c_str());
		f.write("\t\tset { throw new InvalidOperationException(); }\n");
		f.write("\t}\n");
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
			const char* elem_type_name = getCSharpPrimitiveType(member.type.vectorElemKind);
			f.write("\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget { return new ReadOnlyCollection<%s>(this._data.%s); }\n", elem_type_name, member.name.c_str());
			f.write("\t\tset { throw new InvalidOperationException(); }\n");
			f.write("\t}\n");
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* elem_type_name = member.type.name.c_str();
			f.write("\tList<I%s> %s_handlers;\n", elem_type_name, member.name.c_str());
			f.write("\tList<I%s> lazy_%s_handlers()\n", elem_type_name, member.name.c_str());
			f.write("\t{ // mb: lazy because can't call virtual 'makeElementHandler' in ctor\n");
			f.write("\t\tif (%s_handlers == null)\n", member.name.c_str());
			f.write("\t\t{\n");
			f.write("\t\t\t%s_handlers = new List<I%s>();\n", member.name.c_str(), elem_type_name);
			f.write("\t\t\tfor (int i = 0; i < this._data.%s.Count; ++i)\n", member.name.c_str());
			f.write("\t\t\t{\n");
			f.write("\t\t\t\t%s_subscriber handler = makeElementHandler_%s(this._data.%s[i]);\n", elem_type_name, member.name.c_str(), member.name.c_str());
			f.write("\t\t\t\t%s_handlers.Add(handler);\n", member.name.c_str());
			f.write("\t\t\t}\n");
			f.write("\t\t\tthis._data.%s.Clear();\n", member.name.c_str());
			f.write("\t\t}\n");
			f.write("\t\treturn %s_handlers;\n", member.name.c_str());
			f.write("\t}\n");
			f.write("\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget { return new ReadOnlyCollection<I%s>(lazy_%s_handlers()); }\n", elem_type_name, member.name.c_str());
			f.write("\t\tset { throw new InvalidOperationException(); }\n");
			f.write("\t}\n");
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}
		break;
	}
	case MessageParameterType::KIND::DICTIONARY:
	{
		const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
		string value = getCSharpElementInterfaceType(member.type.dictionaryValueKind, member.type.name);

		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			f.write("\tpublic IDictionary<%s, %s> %s\n", key, value.c_str(), member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget { return new ReadOnlyDictionary<%s, %s>(this._data.%s); }\n", key, value.c_str(), member.name.c_str());
			f.write("\t\tset { throw new InvalidOperationException(); }\n");
			f.write("\t}\n");
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* elem_type_name = member.type.name.c_str();
			f.write("\tDictionary<%s, %s> %s_handlers;\n", key, value.c_str(), member.name.c_str());
			f.write("\tDictionary<%s, %s> lazy_%s_handlers()\n", key, value.c_str(), member.name.c_str());
			f.write("\t{ // mb: lazy because can't call virtual 'makeValueHandler' in ctor\n");
			f.write("\t\tif (%s_handlers == null)\n", member.name.c_str());
			f.write("\t\t{\n");
			f.write("\t\t\t%s_handlers = new Dictionary<%s, %s>();\n", member.name.c_str(), key, value.c_str());
			f.write("\t\t\tforeach(KeyValuePair<%s, %s> each in this._data.%s)\n", key, value.c_str(), member.name.c_str());
			f.write("\t\t\t{\n");
			f.write("\t\t\t\t%s_subscriber handler = makeValueHandler_%s(each.Value);\n", elem_type_name, member.name.c_str(), member.name.c_str());
			f.write("\t\t\t\t%s_handlers.Add(each.Key, handler);\n", member.name.c_str());
			f.write("\t\t\t}\n");
			f.write("\t\t\tthis._data.%s.Clear();\n", member.name.c_str());
			f.write("\t\t}\n");
			f.write("\t\treturn %s_handlers;\n", member.name.c_str());
			f.write("\t}\n");
			f.write("\tpublic IDictionary<%s, %s> %s\n", key, value.c_str(), member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget { return new ReadOnlyDictionary<%s, %s>(lazy_%s_handlers()); }\n", key, value.c_str(), member.name.c_str());
			f.write("\t\tset { throw new InvalidOperationException(); }\n");
			f.write("\t}\n");
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

void generateCsharpPublisherMember(FileWritter f, MessageParameter& member)
{


	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
		f.write("\tpublic %s %s\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
		f.write("\t{\n");
		f.write("\t\tget { return this.data_.%s; }\n", member.name.c_str());
		f.write("\t\tset\n");
		f.write("\t\t{\n");
		//f.write("\t\t\tif (value != t.%s)\n", member.name.c_str());
		//f.write("\t\t\t{\n");
		f.write("\t\t\tthis.data_.%s = value;\n", member.name.c_str());
		f.write("\t\t\tthis.composer_.composeAddress(this.address_, (UInt64)Address.%s);\n", member.name.c_str());
		f.write("\t\t\tthis.composer_.compose%s(\"value\", value, false);\n", getIdlPrimitiveType(member.type.kind));
		f.write("\t\t\tthis.composer_.composeAddressEnd();\n");
		//f.write("\t\t\t}\n");
		f.write("\t\t}\n");
		f.write("\t}\n");
		break;
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		f.write("\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());
		f.write("\t{\n");
		f.write("\t\tget { return new %s_publisher(this.data_.%s, this.composer_, Publishable.makeAddress(this.address_, (UInt64)Address.%s)); }\n", member.type.name.c_str(), member.name.c_str(), member.name.c_str());
		f.write("\t\tset\n");
		f.write("\t\t{\n");
		//f.write("\t\t\tif (value != t.%s)\n", member.name.c_str());
		//f.write("\t\t\t{\n");
		f.write("\t\t\tthis.data_.%s = value;\n", member.name.c_str());
		f.write("\t\t\tthis.composer_.composeAddress(this.address_, (UInt64)Address.%s);\n", member.name.c_str());
		//f.write("\t\t\tthis.composer_.composePublishableStructBegin(\"value\");\n");
		f.write("\t\t\t%s_publisher.compose(this.composer_, \"value\", value, false);\n", member.type.name.c_str());
		//f.write("\t\t\tthis.composer_.composePublishableStructEnd(false);\n");
		f.write("\t\t\tthis.composer_.composeAddressEnd();\n");
		//f.write("\t\t\t}\n");
		f.write("\t\t}\n");
		f.write("\t}\n");

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
			const char* elem_type_name = getCSharpPrimitiveType(member.type.vectorElemKind);
			const char* compose_method = getIdlPrimitiveType(member.type.vectorElemKind);


			f.write("\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget\n");
			f.write("\t\t{\n");
			f.write("\t\t\treturn new PublisherVectorWrapper<%s>(this.data_.%s, this.composer_,\n", elem_type_name, member.name.c_str());
			f.write("\t\t\t\tPublishable.makeAddress(this.address_, (UInt64)Address.%s),\n", member.name.c_str());
			f.write("\t\t\t\t(IPublishableComposer composer, %s v) => { composer.compose%s(\"value\", v, false); },\n", elem_type_name, compose_method);
			f.write("\t\t\t\tnull\n");
			f.write("\t\t\t);\n");
			f.write("\t\t}\n");
			f.write("\t\tset\n");
			f.write("\t\t{\n");
			f.write("\t\t\tthis.data_.%s = value;\n", member.name.c_str());
			f.write("\t\t\tthis.composer_.composeAddress(this.address_, (UInt64)Address.%s);\n", member.name.c_str());

			f.write("\t\t\tthis.composer_.composeVector2(\"value\", value,\n");
			f.write("\t\t\t\t(IPublishableComposer composer, %s v) => { composer.compose%s(null, v, false); }, false);\n", elem_type_name, compose_method);

			f.write("\t\t\tthis.composer_.composeAddressEnd();\n");
			f.write("\t\t}\n");
			f.write("\t}\n");
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* elem_type_name = member.type.name.c_str();

			f.write("\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget\n");
			f.write("\t\t{\n");
			f.write("\t\t\treturn new PublisherVectorWrapper<I%s>(this.data_.%s, this.composer_,\n", elem_type_name, member.name.c_str());
			f.write("\t\t\t\tPublishable.makeAddress(this.address_, (UInt64)Address.%s),\n", member.name.c_str());
			f.write("\t\t\t\t(IPublishableComposer composer, I%s val) =>\n", elem_type_name);
		/*	f.write("\t\t\t\t{\n");
		*/	//f.write("\t\t\t\t\tcomposer.composePublishableStructBegin(\"value\");\n");
			f.write("\t\t\t\t\t{ %s_publisher.compose(composer, \"value\", val, false); },\n", elem_type_name);
			//f.write("\t\t\t\t\tcomposer.composePublishableStructEnd(false);\n");
			//f.write("\t\t\t\t},\n");
			f.write("\t\t\t\t(I%s val, IPublishableComposer composer, UInt64[] addr) =>\n", elem_type_name);
			f.write("\t\t\t\t\t{ return new %s_publisher(val, composer, addr); }\n", elem_type_name);
			f.write("\t\t\t);\n");
			f.write("\t\t}\n");
			f.write("\t\tset\n");
			f.write("\t\t{\n");
			f.write("\t\t\tthis.data_.%s = value;\n", member.name.c_str());
			f.write("\t\t\tthis.composer_.composeAddress(this.address_, (UInt64)Address.%s);\n", member.name.c_str());

			f.write("\t\t\tthis.composer_.composeVector2(\"value\", value,\n");
			f.write("\t\t\t\t%s_publisher.compose,\n", elem_type_name);

			//f.write("\t\t\t\t(IPublishableComposer composer, I%s val) =>\n", elem_type_name);
			//f.write("\t\t\t\t{\n");
			//f.write("\t\t\t\t\tcomposer.composePublishableStructBegin(null);\n");
			//f.write("\t\t\t\t\t%s_publisher.compose(composer, val);\n", elem_type_name);
			//f.write("\t\t\t\t\tcomposer.composePublishableStructEnd(false);\n");
			//f.write("\t\t\t\t},\n");

			f.write("\t\t\t\tfalse);\n");

			f.write("\t\t\tthis.composer_.composeAddressEnd();\n");
			f.write("\t\t}\n");
			f.write("\t}\n");
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}
		break;
	}
	case MessageParameterType::KIND::DICTIONARY:
	{
		string type = getCSharpInterfaceType(member.type);
		const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
		const char* idlKey = getIdlPrimitiveType(member.type.dictionaryKeyKind);

		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
			const char* idlValue = getIdlPrimitiveType(member.type.dictionaryValueKind);


			f.write("\tpublic %s %s\n", type.c_str(), member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget\n");
			f.write("\t\t{\n");
			f.write("\t\t\treturn new PublisherDictionaryWrapper<%s,%s>(this.data_.%s, this.composer_,\n", key, value, member.name.c_str());
			f.write("\t\t\t\tPublishable.makeAddress(this.address_, (UInt64)Address.%s),\n", member.name.c_str());
			f.write("\t\t\t\t(IPublishableComposer composer, %s k, bool s) => { composer.compose%s(\"key\", k, s); },\n", key, idlKey);
			f.write("\t\t\t\t(IPublishableComposer composer, %s v) => { composer.compose%s(\"value\", v, false); },\n", value, idlValue);
			f.write("\t\t\t\tnull\n");
			f.write("\t\t\t);\n");
			f.write("\t\t}\n");
			f.write("\t\tset\n");
			f.write("\t\t{\n");
			f.write("\t\t\tthis.data_.%s = value;\n", member.name.c_str());
			f.write("\t\t\tthis.composer_.composeAddress(this.address_, (UInt64)Address.%s);\n", member.name.c_str());

			f.write("\t\t\tthis.composer_.composeDictionary(\"value\", value,\n");
			f.write("\t\t\t\t(IPublishableComposer composer, %s k) => { composer.compose%s(null, k, false); },\n", key, idlKey);
			f.write("\t\t\t\t(IPublishableComposer composer, %s v) => { composer.compose%s(null, v, false); },\n", value, idlValue);
			f.write("\t\t\t\tfalse);\n");
			f.write("\t\t\tthis.composer_.composeAddressEnd();\n");
			f.write("\t\t}\n");
			f.write("\t}\n");
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();

			f.write("\tpublic %s %s\n", type.c_str(), member.name.c_str());
			f.write("\t{\n");

			f.write("\t\tget\n");
			f.write("\t\t{\n");
			f.write("\t\t\treturn new PublisherDictionaryWrapper<%s,I%s>(this.data_.%s, this.composer_,\n", key, value, member.name.c_str());
			f.write("\t\t\t\tPublishable.makeAddress(this.address_, (UInt64)Address.%s),\n", member.name.c_str());
			f.write("\t\t\t\t(IPublishableComposer composer, %s k, bool s) => { composer.compose%s(\"key\", k, s); },\n", key, idlKey);
			f.write("\t\t\t\t(IPublishableComposer composer, I%s v) => { %s_publisher.compose(composer, \"value\", v, false); },\n", value, value);

			if (member.type.dictionaryKeyKind == MessageParameterType::KIND::UINTEGER)
			{
				f.write("\t\t\t\t(%s k, I%s v, IPublishableComposer composer, UInt64[] addr) =>\n", key, value);
				f.write("\t\t\t\t\t{ return new %s_publisher(v, composer, Publishable.makeAddress(addr, k)); }\n", value);
			}
			else if (member.type.dictionaryKeyKind == MessageParameterType::KIND::INTEGER)
			{
				f.write("\t\t\t\t(%s k, I%s v, IPublishableComposer composer, UInt64[] addr) =>\n", key, value);
				f.write("\t\t\t\t\t{ return new %s_publisher(v, composer, Publishable.makeAddress(addr, ZigZag.encode(k))); }\n", value);
			}
			else if (member.type.dictionaryKeyKind == MessageParameterType::KIND::CHARACTER_STRING)
			{
				f.write("\t\t\t\t(%s k, I%s v, IPublishableComposer composer, UInt64[] addr) =>\n", key, value);
				f.write("\t\t\t\t\t{ return new %s_publisher(v, composer, AsciiAddress.encode(addr, k)); }\n", value);
			}
			else
				f.write("\t\t\t\t(%s k, I%s v, IPublishableComposer composer, UInt64[] addr) => { return new %s_readonly(v); }\n", key, value, value);

			f.write("\t\t\t);\n");
			f.write("\t\t}\n");

			f.write("\t\tset\n");
			f.write("\t\t{\n");
			f.write("\t\t\tthis.data_.%s = value;\n", member.name.c_str());
			f.write("\t\t\tthis.composer_.composeAddress(this.address_, (UInt64)Address.%s);\n", member.name.c_str());

			f.write("\t\t\tthis.composer_.composeDictionary(\"value\", value,\n");
			f.write("\t\t\t\t(IPublishableComposer composer, %s k) => { composer.compose%s(null, k, false); },\n", key, idlKey);
			f.write("\t\t\t\t(IPublishableComposer composer, I%s v) => { %s_publisher.compose(composer, null, v, false); },\n", value, value);
			f.write("\t\t\t\tfalse);\n");

			f.write("\t\t\tthis.composer_.composeAddressEnd();\n");
			f.write("\t\t}\n");

			f.write("\t}\n");
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

void generateCsharpCaseSubscriber(FileWritter f, CompositeType& s, const char* type_name, const char* du_name)
{
	assert(s.type == CompositeType::Type::discriminated_union_case);

	// for DU CASE, only the static methods

	f.write("public class %s_subscriber\n", type_name);
	f.write("{\n");

	csharpPub_generateParseStateSync(f, s, du_name);
	csharpPub_generateParse1(f, s, du_name);
	csharpPub_generateParse2(f, s, du_name);

	f.write("} // class %s_subscriber\n\n", type_name);
}

void generateCsharpCasePublisher(FileWritter f, CompositeType& s, const char* type_name, const char* du_name)
{
	assert(s.type == CompositeType::Type::discriminated_union_case);

	// for DU CASE, only the static methods

	f.write("public class %s_publisher\n", type_name);
	f.write("{\n");

	csharpPub_generateCompose(f, s, du_name);

	f.write("} // class %s_publisher\n\n", type_name);
}

void generateCsharpStructSubscriber(FileWritter f, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);



	if (s.type == CompositeType::Type::publishable)
		f.write("public class %s_subscriber : I%s, StateSubscriberBase\n", type_name, type_name);
	else
		f.write("public class %s_subscriber : I%s\n", type_name, type_name);


	f.write("{\n");

	f.write("\n");
	f.write("\t/////////////////////////////////  begin user override section /////////////////////////////////\n");
	f.write("\n");

	csharpPub_generateFactoryMethodSubs(f, s, type_name);
	csharpPub_generateEventHandlerSubs(f, s, type_name);

	f.write("\n");
	f.write("\t/////////////////////////////////   end user override section  /////////////////////////////////\n");
	f.write("\n");
	f.write("\n");


	f.write("\tprotected I%s _data;\n", type_name);

	csharpPub_generateAddressEnum(f, s);

	if (s.type == CompositeType::Type::publishable)
		f.write("\tpublic %s_subscriber() { this._data = new %s(); }\n", type_name, type_name);
	else
		f.write("\tpublic %s_subscriber(I%s data) { this._data = data ?? new %s(); }\n", type_name, type_name, type_name);


	auto& mem = s.getMembers();
	for (size_t i = 0; i < mem.size(); ++i)
	{
		auto& it = mem[i];
		assert(it != nullptr);
		generateCsharpSubscriberMember(f, *it);
	}

	generateCsharpSimpleEquivalentMethod(f, type_name, "this._data");

	csharpPub_generateParseStateSync(f, s, type_name);
	csharpPub_generateParse1(f, s, type_name);
	csharpPub_generateParse2(f, s, type_name);

	if (s.type == CompositeType::Type::publishable)
		csharpPub_generateStateSubscriberBase(f, s, type_name);

	csharpPub_generateSubscriberResetHandlers(f, s);

	f.write("} // class %s_subscriber\n\n", type_name);

}

void generateCsharpStructPublisher(FileWritter f, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);



	if (s.type == CompositeType::Type::publishable)
		f.write("public class %s_publisher : I%s, StatePublisherBase\n", type_name, type_name);
	else
		f.write("public class %s_publisher : I%s\n", type_name, type_name);


	f.write("\n");


	f.write("{\n");
	f.write("\tI%s data_;\n", type_name);
	f.write("\tIPublishableComposer composer_;\n");
	f.write("\tUInt64[] address_;\n");

	csharpPub_generateAddressEnum(f, s);

	if (s.type == CompositeType::Type::publishable)
	{
		f.write("\tpublic %s_publisher()\n", type_name);
		f.write("\t{\n");
		f.write("\t\tthis.data_ = new %s();\n", type_name);
		f.write("\t\tthis.composer_ = null;\n");
		f.write("\t\tthis.address_ = new UInt64[] { };\n");
		f.write("\t}\n");

		f.write("\tpublic static %s_publisher make(IPlatformSupport platform)\n", type_name);
		f.write("\t{\n");
		f.write("\t\tif(platform == null)\n");
		f.write("\t\t\tthrow new ArgumentNullException();\n");
		f.write("\t\treturn new %s_publisher();\n", type_name);
		f.write("\t}\n");
	}
	else
	{
		f.write("\tpublic %s_publisher(I%s data, IPublishableComposer composer, UInt64[] address)\n", type_name, type_name);
		f.write("\t{\n");
		f.write("\t\tthis.data_ = data;\n");
		f.write("\t\tthis.composer_ = composer;\n");
		f.write("\t\tthis.address_ = address;\n");
		f.write("\t}\n");
	}
	auto& mem = s.getMembers();
	for (size_t i = 0; i < mem.size(); ++i)
	{
		auto& it = mem[i];
		assert(it != nullptr);
		generateCsharpPublisherMember(f, *it);
	}

	generateCsharpSimpleEquivalentMethod(f, type_name, "this.data_");

	csharpPub_generateCompose(f, s, type_name);

	if (s.type == CompositeType::Type::publishable)
	{
		csharpPub_generateStatePublishableBase(f, s, type_name);
		f.write("\tpublic void applyStateSyncMessage(IPublishableParser parser)\n");
		f.write("\t{\n");
		f.write("\t\tthis.data_ = %s_subscriber.parseForStateSync(parser);\n", type_name);
		f.write("\t}\n");
	}


	f.write("\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
	f.write("\tpublic void debugOnlySetData(I%s data) { this.data_ = data; }\n", type_name);


	f.write("} // class %s_publisher\n\n", type_name);
}

void generateCsharpStructConcentrator(FileWritter f, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);



	f.write("public class %s_concentrator : %s_subscriber, StateConcentratorBase\n", type_name, type_name);

	f.write("{\n");
	csharpPub_generateCompose(f, s, type_name);
	csharpPub_generateStateConcentratorBase(f, s, type_name);

	f.write("} // class %s_concentrator\n\n", type_name);
}

void generateCsharpConcentratorFactory(FileWritter f, Root& root)
{
	assert(!root.publishables.empty());



	f.write("public class StateConcentratorFactory : IStateConcentratorFactory\n");
	f.write("{\n");
	f.write("\tpublic StateConcentratorBase createConcentrator(UInt64 typeID)\n");
	f.write("\t{\n");
	f.write("\t\tswitch(typeID)\n");
	f.write("\t\t{\n");
	for (auto& it : root.publishables)
	{
		auto& obj_1 = it;
		assert(it != nullptr);
		assert(typeid(*(it)) == typeid(CompositeType));
		assert(it->type == CompositeType::Type::publishable);
		string number = std::to_string(it->numID);
		f.write("\t\tcase %s:\n", number.c_str());
		f.write("\t\t\treturn new %s_concentrator();\n", it->name.c_str());
	}
	f.write("\t\tdefault:\n");
	f.write("\t\t\treturn null;\n");
	f.write("\t\t}\n");

	f.write("\t}\n");
	f.write("} // class StateConcentratorFactory\n\n");
}

}
