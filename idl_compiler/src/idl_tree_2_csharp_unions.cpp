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

	void csharpDu_generateUnionParseJson3(CsharpWritter f, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string type_name = s.name;

		f.write("\tpublic static void parse(JsonParser parser, I%s val)\n", type_name.c_str());

		f.write("\t{\n");
		//f.write("\t\t%s val = new %s();\n\n", type_name.c_str(), type_name.c_str());

		f.write("\t\tparser.skipDelimiter('{');\n");
		f.write("\t\tstring key;\n");
		f.write("\t\tparser.readKeyFromJson( out key );\n");
		f.write("\t\tif (key != \"caseid\")\n");
		f.write("\t\t\tthrow new FormatException();\n");
		f.write("\t\tInt64 caseID;\n");
		f.write("\t\tparser.parseSignedInteger(out caseID);\n");
		f.write("\t\tparser.skipSpacesEtc();\n");
		f.write("\t\tparser.skipDelimiter(',');\n");
		f.write("\t\tparser.readKeyFromJson(out key);\n");
		f.write("\t\tif (key != \"caseData\")\n");
		f.write("\t\t\tthrow new FormatException();\n\n");


		f.write("\t\tval.setCurrentVariant((I%s.Variants)caseID);\n", type_name.c_str());
		f.write("\t\tswitch (val.currentVariant())\n");
		f.write("\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCaseTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name.c_str(), cs.name.c_str());
			f.write("\t\t\t{\n");
			f.write("\t\t\t\t%s_message.parse(parser, val);\n", case_name.c_str());
			f.write("\t\t\t\tbreak;\n");
			f.write("\t\t\t}\n");
		}

		f.write("\t\t\tdefault:\n");
		f.write("\t\t\t\t// TODO: actually skip any data\n");
		f.write("\t\t\t\tbreak;\n");
		f.write("\t\t}\n\n");

		f.write("\t\tparser.skipDelimiter('}');\n\n");

		//f.write("\t\treturn val;\n");
		f.write("\t}\n");
	}
	void csharpDu_generateUnionParseGmq3(CsharpWritter f, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string type_name = s.name;

		f.write("\tpublic static void parse(GmqParser parser, I%s val)\n", type_name.c_str());

		f.write("\t{\n");
		//f.write("\t\t%s val = new %s();\n\n", type_name.c_str(), type_name.c_str());


		f.write("\t\tInt64 caseID;\n");
		f.write("\t\tparser.parseSignedInteger(out caseID);\n\n");

		f.write("\t\t\t\tval.setCurrentVariant((I%s.Variants)caseID);\n", type_name.c_str());
		f.write("\t\tswitch (val.currentVariant())\n", type_name.c_str());
		f.write("\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCaseTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name.c_str(), cs.name.c_str());
			f.write("\t\t\t{\n");
			f.write("\t\t\t\t%s_message.parse(parser, val);\n", case_name.c_str());
			f.write("\t\t\t\tbreak;\n");
			f.write("\t\t\t}\n");
		}

		f.write("\t\t\tdefault:\n");
		f.write("\t\t\t\t// TODO: actually skip any data\n");
		f.write("\t\t\t\tbreak;\n");
		f.write("\t\t}\n\n");

		//f.write("\t\treturn val;\n");
		f.write("\t}\n");
	}

	void csharpDu_generateUnionComposeJson2(CsharpWritter f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		f.write("\tpublic static void compose(JsonComposer composer, I%s val)\n", type_name);

		f.write("\t{\n");


		f.write("\t\tcomposer.append( \"{\\n  \");\n");
		f.write("\t\tcomposer.addNamePart(\"caseid\");\n");
		f.write("\t\tcomposer.composeSignedInteger((Int64)val.currentVariant());\n");
		f.write("\t\tcomposer.append( \",\\n  \" );\n");

		f.write("\t\tswitch (val.currentVariant())\n");
		f.write("\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCaseTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name, cs.name.c_str());
			f.write("\t\t\t{\n");
			f.write("\t\t\t\tcomposer.addNamePart(\"caseData\");\n");

			string caller = generateCsharpCallerParams(cs, true);
			f.write("\t\t\t\t%s_message.compose(composer, %s);\n", case_name.c_str(), caller.c_str());
		
			f.write("\t\t\t\tbreak;\n");
			f.write("\t\t\t}\n");
		}

		f.write("\t\t\tdefault:\n");
		f.write("\t\t\t\tbreak;\n");
		f.write("\t\t}\n\n");

		f.write("\t\tcomposer.append( \"\\n}\" );\n");

		f.write("\t}\n");
	}

	void csharpDu_generateUnionComposeGmq2(CsharpWritter f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		f.write("\tpublic static void compose(GmqComposer composer, I%s val)\n", type_name);

		f.write("\t{\n");
		f.write("\t\tcomposer.composeSignedInteger((Int64)val.currentVariant());\n");

		f.write("\t\tswitch (val.currentVariant())\n");
		f.write("\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCaseTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name, cs.name.c_str());
			f.write("\t\t\t{\n");
			string caller = generateCsharpCallerParams(cs, true);
			f.write("\t\t\t\t%s_message.compose(composer, %s);\n", case_name.c_str(), caller.c_str());
			f.write("\t\t\t\tbreak;\n");
			f.write("\t\t\t}\n");
		}

		f.write("\t\t\tdefault:\n");
		f.write("\t\t\t\tbreak;\n");
		f.write("\t\t}\n\n");

		f.write("\t}\n");
	}

	void csharpDu_generateUnionCaseProperties(CsharpWritter f, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union_case);

		std::string case_name = getCaseTypeName(s);

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
				f.write("\tpublic %s %s\n", type_name, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());
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
					const char* type_name = getCSharpPrimitiveType(member.type.vectorElemKind);
					f.write("\tpublic IList<%s> %s\n", type_name, member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					f.write("\tpublic IList<I%s> %s\n", member.type.name.c_str(), member.name.c_str());
					break;
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}

			f.write("\t{\n");
			f.write("\t\tget { return ((%s)this.mem).%s; }\n", case_name.c_str(), member.name.c_str());
			f.write("\t\tset { ((%s)this.mem).%s = value; }\n", case_name.c_str(), member.name.c_str());
			f.write("\t}\n");
		}
	}



	void csharpDu_generateUnionEqualsMethod(CsharpWritter f, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		f.write("\tpublic bool Equals(%s other)\n", type_name.c_str());
		f.write("\t{\n");
		f.write("\t\tif (ReferenceEquals(this, other))\n");
		f.write("\t\t\treturn true;\n");
		f.write("\t\telse if (ReferenceEquals(null, other))\n");
		f.write("\t\t\treturn false;\n");
		f.write("\t\telse\n");
		f.write("\t\t\treturn this.mem.Equals(other.mem);\n");
		f.write("\t}\n");
	}

	void csharpDu_generateUnionEquivalentMethod(CsharpWritter f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		f.write("\tpublic bool isEquivalent(I%s other)\n", type_name);
		f.write("\t{\n");
		f.write("\t\tif (ReferenceEquals(other, null))\n");
		f.write("\t\t\treturn false;\n");
		f.write("\t\telse if (ReferenceEquals(this, other))\n");
		f.write("\t\t\treturn true;\n");
		f.write("\t\telse if (currentVariant() != other.currentVariant())\n");
		f.write("\t\t\treturn false;\n");
		f.write("\t\telse\n");
		f.write("\t\t{\n");

		f.write("\t\t\tswitch(currentVariant())\n");
		f.write("\t\t\t{\n");

		for (auto& it : s.getDiscriminatedUnionCases())
		{
			assert(it != nullptr);
			CompositeType& cs = *it;
			assert(cs.type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCaseTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name, cs.name.c_str());
			generateCsharpStructEquivalentExpression(f.indent(1), cs);
		}
		f.write("\t\t\tdefault: return true; // both unknown\n");
		f.write("\t\t\t}\n");
		f.write("\t\t}\n");
		f.write("\t}\n");
	}

	void csharpDu_generateUnionResetHandlers(CsharpWritter f, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		f.write("\tvoid _reset_handlers()\n");
		f.write("\t{\n");


		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);
			f.write("\n");
			f.write("\t\t// IDL CASE %s:\n", duit->name.c_str());

			for (auto& it : duit->getMembers())
			{
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
				default:
					assert(false); // not implemented (yet)
				}
			}
		}

		f.write("\t}\n");
	}

	void csharpDu_generateUnionAddressEnum(CsharpWritter f, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		f.write("\tenum Address\n");
		f.write("\t{\n");
		f.write("\t\tCurrentVariant = 0,\n");

		size_t dusz = s.getDiscriminatedUnionCases().size();
		for(size_t j = 0; j != dusz; ++j)
		{
			auto& duit = s.getDiscriminatedUnionCases()[j];
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			f.write("\n");
			f.write("\t\t// IDL CASE %s:\n", duit->name.c_str());

			size_t sz = duit->getMembers().size();
			for (size_t i = 0; i != sz; ++i)
			{
				auto& it = duit->getMembers()[i];
				assert(it != nullptr);

				// use i + 1, sice 0 is already used by CurrentVariant
				string number = std::to_string(i + 1);

				if(i == sz - 1 && j == dusz - 1)
					f.write("\t\t%s = %s\n", it->name.c_str(), number.c_str());
				else
					f.write("\t\t%s = %s,\n", it->name.c_str(), number.c_str());
			}
		}
		f.write("\t};\n");

	}

	void csharpDu_generateCompose(CsharpWritter f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);


		f.write("\tpublic static void compose(IPublishableComposer composer, String name, I%s val, bool separator)\n", type_name);
		f.write("\t{\n");
		f.write("\t\tcomposer.composeKey(name);\n");
		f.write("\t\tcompose(composer, val);\n");
		f.write("\t\tcomposer.addSeparator(separator);\n");


		f.write("\t}\n");



		f.write("\tpublic static void compose(IPublishableComposer composer, I%s t)\n", type_name);
		f.write("\t{\n");

		f.write("\t\tcomposer.composeStructBegin();\n");

		f.write("\t\tI%s.Variants c = t.currentVariant();\n", type_name);
		//f.write("\t\tif( c == I%s.Variants.unknown)\n", type_name);

		//f.write("\t\t\tcomposer.composeUnsigned(\"caseId\", (UInt64)c, false);\n");
		//f.write("\t\telse\n");
		//f.write("\t\t{\n");
		f.write("\t\t\tcomposer.composeUnsigned(\"caseId\", (UInt64)c, true);\n");
		f.write("\t\t\tswitch(c)\n");
		f.write("\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCaseTypeName(*duit);

			f.write("\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			f.write("\t\t\t\t\t%s_publisher.compose(composer, \"caseData\", t, false);\n", case_type_name.c_str());
			f.write("\t\t\t\t\tbreak;\n");

		}

		f.write("\t\t\t\tdefault:\n");
		f.write("\t\t\t\t\tbreak;\n");
		f.write("\t\t\t}\n");

		//f.write("\t\t}\n");
		f.write("\t\tcomposer.composeStructEnd();\n");
		f.write("\t}\n");
	}

	void csharpDu_generateParseStateSync(CsharpWritter f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		f.write("\tpublic static void parseForStateSync(IPublishableParser parser, String expectedName, I%s data)\n", type_name);
		f.write("\t{\n");
		f.write("\t\tparser.parseKey(expectedName);\n");
		f.write("\t\tparseForStateSync(parser, data);\n");
		f.write("\t}\n");

		f.write("\tpublic static %s parseForStateSync(IPublishableParser parser)\n", type_name);
		f.write("\t{\n");
		f.write("\t\t%s data = new %s();\n", type_name, type_name);
		f.write("\t\tparseForStateSync(parser, data);\n");
		f.write("\t\treturn data;\n");
		f.write("\t}\n");

		f.write("\tstatic void parseForStateSync(IPublishableParser parser, I%s data)\n", type_name);
		f.write("\t{\n");

		f.write("\t\tparser.parseStructBegin();\n");

		f.write("\t\tI%s.Variants newVal = (I%s.Variants)parser.parseUnsigned(\"caseId\");\n", type_name, type_name);
		f.write("\t\tdata.setCurrentVariant(newVal);\n");

		//f.write("\t\tif( newVal != I%s.Variants.unknown)\n", type_name);
		//f.write("\t\t{\n");

		f.write("\t\t\t// mb: get it back from data, in case is an unknown value\n");
		f.write("\t\t\tswitch(data.currentVariant())\n");
		f.write("\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCaseTypeName(*duit);

			f.write("\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			f.write("\t\t\t\t\t%s_subscriber.parseForStateSync(parser, \"caseData\", data);\n", case_type_name.c_str());
			f.write("\t\t\t\t\tbreak;\n");

		}
		f.write("\t\t\t\tdefault:\n");
		f.write("\t\t\t\t\tbreak; //TODO: actually skip any caseData\n");
		f.write("\t\t\t}\n");

		//f.write("\t\t}\n");

		f.write("\t\tparser.parseStructEnd();\n");

		f.write("\t}\n");
	}
	void csharpDu_generateParse1(CsharpWritter f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		f.write("\tpublic static bool parse(IPublishableParser parser, String expectedName, %s_subscriber subscriber)\n", type_name);
		f.write("\t{\n");

		f.write("\t\tparser.parseKey(expectedName);\n");
		f.write("\t\tparser.parseStructBegin();\n");

		f.write("\t\tbool changed = subscriber.update_CurrentVariant(parser, \"caseId\");\n");
		//f.write("\t\tI%s.Variants c = subscriber.currentVariant();\n", type_name);

		//f.write("\t\tif( c != I%s.Variants.unknown)\n", type_name);
		//f.write("\t\t{\n");
		//f.write("\t\t\tparser.parsePublishableStructBegin(\"caseData\");\n");

		f.write("\t\t\tswitch(subscriber.currentVariant())\n");
		f.write("\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCaseTypeName(*duit);

			f.write("\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			f.write("\t\t\t\t\tchanged = %s_subscriber.parse(parser, \"caseData\", subscriber) | changed;\n", case_type_name.c_str());
			f.write("\t\t\t\t\tbreak;\n");

		}
		f.write("\t\t\t\tdefault:\n");
		f.write("\t\t\t\t\tbreak; //TODO: actually skip any caseData\n");
		f.write("\t\t\t}\n");

		//f.write("\t\t}\n");

		f.write("\t\tparser.parseStructEnd();\n");

		f.write("\t\treturn changed;\n");
		f.write("\t}\n");
	}

	void csharpDu_generateParse2(CsharpWritter f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		f.write("\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber, UInt64[] addr, int offset)\n", type_name);
		f.write("\t{\n");

		f.write("\t\tbool changed = false;\n");

		f.write("\t\tif( (Address)addr[offset] == Address.CurrentVariant)\n");
		f.write("\t\t{\n");
		f.write("\t\t\tif(addr.Length != offset + 1)\n");
		f.write("\t\t\t\tthrow new Exception();\n");
		f.write("\t\t\tchanged = subscriber.update_CurrentVariant(parser, \"value\") | changed;\n");
		f.write("\t\t}\n");

		f.write("\t\telse\n");
		f.write("\t\t{\n");

		f.write("\t\t\tswitch(subscriber.currentVariant())\n");
		f.write("\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCaseTypeName(*duit);

			f.write("\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			f.write("\t\t\t\t\tchanged = %s_subscriber.parse(parser, subscriber, addr, offset) | changed;\n", case_type_name.c_str());
			f.write("\t\t\t\t\tbreak;\n");
		}

		f.write("\t\t\t\tdefault:\n");
		f.write("\t\t\t\t\tbreak; //TODO: actually skip any deeper data\n");
		f.write("\t\t\t}\n");

		f.write("\t\t}\n");

		f.write("\t\treturn changed;\n");
		f.write("\t}\n");
	}

}



void generateCsharpUnionInterface(CsharpWritter f, CompositeType& s)
{
	assert(s.type == CompositeType::Type::discriminated_union);

	bool checked = impl_checkParamNameUniqueness(s);
	checked = impl_checkFollowingExtensionRules(s) && checked;
	if (!checked)
		throw std::exception();


	impl_generateMessageCommentBlock(f.getFile(), s);


	
	std::string type_name = s.name;


	f.write("public interface I%s : IEquivalenceComparable<I%s>\n", type_name.c_str(), type_name.c_str());
	f.write("{\n");

	f.write("\tpublic enum Variants\n");
	f.write("\t{\n");
	//f.write("\t\tunknown = 0,\n");

	size_t sz = s.getDiscriminatedUnionCases().size();
	for (size_t i = 0; i != sz; ++i)
	{
		auto& it = s.getDiscriminatedUnionCases()[i];
		assert(it != nullptr);

		assert(it->type == CompositeType::Type::discriminated_union_case);

		string number = std::to_string(it->numID);

		//if(i != sz - 1)
			f.write("\t\t%s = %s,\n", it->name.c_str(), number.c_str());
		//else
		//	f.write("\t\t%s = %s\n", it->name.c_str(), number.c_str());
	}
	f.write("\t\tunknown\n");

	f.write("\t}\n");

	f.write("\tVariants currentVariant();\n");

	f.write("\tvoid setCurrentVariant(Variants v);\n");

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpInterfaceMember(f, *it);
		}
	}
	f.write("} // interface I%s\n\n", s.name.c_str());
}

void generateCsharpUnionImpl(CsharpWritter f, CompositeType& s)
{
	assert(s.type == CompositeType::Type::discriminated_union);



	std::string type_name = s.name;

	f.write("public class %s : I%s, IEquatable<%s>\n", type_name.c_str(), type_name.c_str(), type_name.c_str());

	f.write("{\n");

	f.write("\tObject mem;\n");


	f.write("\n");


	generateCsharpStandardMethods(f, type_name.c_str());
	csharpDu_generateUnionEqualsMethod(f, s, type_name);
	csharpDu_generateUnionEquivalentMethod(f, s, type_name.c_str());

	// currentVariant()

	f.write("\tpublic I%s.Variants currentVariant()\n", type_name.c_str());
	f.write("\t{\n");
	f.write("\t\tif(this.mem == null)\n");
	f.write("\t\t\treturn I%s.Variants.unknown;\n", type_name.c_str());

	for (auto& it : s.getDiscriminatedUnionCases())
	{
		assert(it != nullptr);
		CompositeType& cs = *it;
		assert(cs.type == CompositeType::Type::discriminated_union_case);

		std::string case_type_name = getCaseTypeName(cs);
		f.write("\t\telse if(this.mem is %s)\n", case_type_name.c_str());
		f.write("\t\t\treturn I%s.Variants.%s;\n", type_name.c_str(), cs.name.c_str());
	}

	f.write("\t\telse\n");
	f.write("\t\t\treturn I%s.Variants.unknown;\n", type_name.c_str());
	f.write("\t}\n");

	f.write("\tpublic void setCurrentVariant(I%s.Variants v)\n", type_name.c_str());
	f.write("\t{\n");

	f.write("\t\tswitch(v)\n");
	f.write("\t\t{\n");
	for (auto& it : s.getDiscriminatedUnionCases())
	{
		assert(it != nullptr);
		CompositeType& cs = *it;
		assert(cs.type == CompositeType::Type::discriminated_union_case);

		std::string case_type_name = getCaseTypeName(cs);

		f.write("\t\tcase I%s.Variants.%s:\n", type_name.c_str(), cs.name.c_str());
		f.write("\t\t\tthis.mem = new %s();\n", case_type_name.c_str());
		f.write("\t\t\tbreak;\n");
	}
	f.write("\t\tdefault:\n");
	f.write("\t\t\tthis.mem = null;\n");
	f.write("\t\t\tbreak;\n");
	f.write("\t\t}\n");
	f.write("\t}\n");

	// each case attributes
	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", cs.name.c_str());

		csharpDu_generateUnionCaseProperties(f, cs);
	}

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", cs.name.c_str());

		std::string case_type_name = getCaseTypeName(cs);

		generateCsharpStructImpl(f.indent(), cs, case_type_name.c_str(), nullptr);
		//csharpDu_generateStructMessage(f, root, cs, case_type_name.c_str(), interface_name.c_str());
	}

	f.write("} // class %s\n\n", s.name.c_str());
}

void generateCsharpUnionReadOnly(CsharpWritter f, CompositeType& s)
{
	assert(s.type == CompositeType::Type::discriminated_union);

	const char* type_name = s.name.c_str();

	f.write("public class %s_readonly : I%s\n", type_name, type_name);
	f.write("{\n");

	f.write("\tprotected I%s _data;\n", type_name);
	f.write("\tpublic %s_readonly(I%s data) { this._data = data; }\n", type_name, type_name);

	f.write("\tpublic I%s.Variants currentVariant() { return this._data.currentVariant(); }\n", type_name);
	f.write("\tpublic void setCurrentVariant(I%s.Variants v) { throw new InvalidOperationException(); }\n", type_name);

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);

			generateCsharpReadOnlyMember(f, *it);
		}
	}

	generateCsharpSimpleEquivalentMethod(f, type_name, "_data");

	f.write("} // class %s_readonly\n\n", type_name);
}


void generateCsharpUnionMessage(CsharpWritter f, CompositeType& s)
{
	assert(s.type == CompositeType::Type::discriminated_union);



	std::string type_name = s.name;

	f.write("public class %s_message\n", type_name.c_str());
	f.write("{\n");

	csharpDu_generateUnionParseJson3(f, s);
	csharpDu_generateUnionParseGmq3(f, s);

	csharpDu_generateUnionComposeJson2(f, s, type_name.c_str());
	csharpDu_generateUnionComposeGmq2(f, s, type_name.c_str());


	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", cs.name.c_str());

		std::string case_type_name = getCaseTypeName(cs);
		std::string interface_name = "I" + type_name;

		generateCsharpStructMessage(f.indent(), cs, case_type_name.c_str(), interface_name.c_str());
	}


	f.write("} // class %s_message\n\n", s.name.c_str());
}


void generateCsharpUnionSubscriber(CsharpWritter f, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::discriminated_union);



	f.write("public class %s_subscriber : I%s", type_name, type_name);

	//if (s.type == CompositeType::Type::publishable)
	//	f.write(", StateSubscriberBase");

	f.write("\n");
	f.write("{\n");

	f.write("\n");
	f.write("\t/////////////////////////////////  begin user override section /////////////////////////////////\n\n");

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpSubscriberFactoryMethod(f, *it);
		}
	}

	f.write("\tpublic virtual void notifyUpdated_CurrentVariant(I%s.Variants old) { }\n", type_name);

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpSubscriberEventHandler(f, *it);
		}
	}
	f.write("\n");
	f.write("\t/////////////////////////////////   end user override section  /////////////////////////////////\n\n\n");


	f.write("\tprotected I%s _data;\n", type_name);

	csharpDu_generateUnionAddressEnum(f, s);

	f.write("\tpublic %s_subscriber(I%s data) { this._data = data; }\n", type_name, type_name);

	f.write("\tpublic I%s.Variants currentVariant() { return this._data.currentVariant(); }\n", type_name);
	f.write("\tpublic void setCurrentVariant(I%s.Variants v) { throw new InvalidOperationException(); }\n", type_name);

	f.write("\tbool update_CurrentVariant(IPublishableParser parser, String name)\n");
	f.write("\t{\n");
	f.write("\t\tI%s.Variants newVal = (I%s.Variants)parser.parseUnsigned(name);\n", type_name, type_name);
						
	f.write("\t\tif (newVal != this._data.currentVariant())\n");
	f.write("\t\t{\n");
	f.write("\t\t\tI%s.Variants oldVal = this._data.currentVariant();\n", type_name);
	f.write("\t\t\tthis._data.setCurrentVariant(newVal);\n");
	f.write("\t\t\tthis._reset_handlers();\n");
	f.write("\t\t\tnotifyUpdated_CurrentVariant(oldVal);\n");
	f.write("\t\t\treturn true;\n");
	f.write("\t\t}\n");
	f.write("\t\telse\n");
	f.write("\t\t\treturn false;\n");
	f.write("\t}\n");


	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpSubscriberMember(f, *it);
		}
	}

	generateCsharpSimpleEquivalentMethod(f, type_name, "_data");

	csharpDu_generateParseStateSync(f, s, type_name);
	csharpDu_generateParse1(f, s, type_name);
	csharpDu_generateParse2(f, s, type_name);

	csharpDu_generateUnionResetHandlers(f, s);

	f.write("\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
	f.write("\tpublic void debugOnlySetData(I%s data)\n", type_name);
	f.write("\t{\n");
	f.write("\t\tthis._data = data;\n");
	f.write("\t\t_reset_handlers();\n");
	f.write("\t}\n");

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", duit->name.c_str());
		std::string case_type_name = getCaseTypeName(*duit);
		generateCsharpCaseSubscriber(f.indent(), *duit, case_type_name.c_str(), type_name);
	}


	f.write("} // class %s_subscriber\n\n", type_name);
}

void generateCsharpUnionPublisher(CsharpWritter f, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::discriminated_union);



	f.write("public class %s_publisher : I%s", type_name, type_name);

	f.write("\n");


	f.write("{\n");
	f.write("\tI%s t;\n", type_name);
	f.write("\tIPublishableComposer composer;\n");
	f.write("\tUInt64[] address;\n");

	csharpDu_generateUnionAddressEnum(f, s);

	f.write("\tpublic %s_publisher(I%s t, IPublishableComposer composer, UInt64[] address)\n", type_name, type_name);
	f.write("\t{\n");
	f.write("\t\tthis.t = t;\n");
	f.write("\t\tthis.composer = composer;\n");
	f.write("\t\tthis.address = address;\n");
	f.write("\t}\n");

	f.write("\tpublic I%s.Variants currentVariant() { return t.currentVariant(); }\n", type_name);

	f.write("\tpublic void setCurrentVariant(I%s.Variants v)\n", type_name);
	f.write("\t{\n");
	f.write("\t\tt.setCurrentVariant(v);\n");
	f.write("\t\tcomposer.composeAddress(address, (UInt64)Address.CurrentVariant);\n");
	f.write("\t\tcomposer.composeUnsigned(\"value\", (UInt64)v, false);\n");
	f.write("\t\tcomposer.composeAddressEnd();\n");
	f.write("\t}\n");


	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpPublisherMember(f, *it);
		}
	}

	generateCsharpSimpleEquivalentMethod(f, type_name, "t");

	csharpDu_generateCompose(f, s, type_name);


	f.write("\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
	f.write("\tpublic void debugOnlySetData(I%s data) { this.t = data; }\n", type_name);



	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n");
		f.write("\t// IDL CASE %s:\n", duit->name.c_str());
		std::string case_type_name = getCaseTypeName(*duit);
		generateCsharpCasePublisher(f.indent(), *duit, case_type_name.c_str(), type_name);

	}

	f.write("} // class %s_publisher\n\n", type_name);
}

