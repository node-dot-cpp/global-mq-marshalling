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

	void csharpDu_generateUnionParseJson3(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

		std::string type_name = getCSharpTypeName(s);

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

		f.write("\t\tswitch ((I%s.Variants)caseID)\n", type_name.c_str());
		f.write("\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCSharpTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name.c_str(), cs.name.c_str());
			f.write("\t\t\t{\n");
			f.write("\t\t\t\tval.setCurrentVariant(I%s.Variants.%s);\n", type_name.c_str(), cs.name.c_str());
			f.write("\t\t\t\t%s_message.parse(parser, val);\n", case_name.c_str());
			f.write("\t\t\t\tbreak;\n");
			f.write("\t\t\t}\n");
		}

		f.write("\t\t\tdefault: throw new System.Exception();\n");
		f.write("\t\t}\n\n");

		f.write("\t\tparser.skipDelimiter('}');\n\n");

		//f.write("\t\treturn val;\n");
		f.write("\t}\n");
	}
	void csharpDu_generateUnionParseGmq3(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

		std::string type_name = getCSharpTypeName(s);

		f.write("\tpublic static void parse(GmqParser parser, I%s val)\n", type_name.c_str());

		f.write("\t{\n");
		//f.write("\t\t%s val = new %s();\n\n", type_name.c_str(), type_name.c_str());


		f.write("\t\tInt64 caseID;\n");
		f.write("\t\tparser.parseSignedInteger(out caseID);\n\n");

		f.write("\t\tswitch ((I%s.Variants)caseID)\n", type_name.c_str());
		f.write("\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCSharpTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name.c_str(), cs.name.c_str());
			f.write("\t\t\t{\n");
			f.write("\t\t\t\tval.setCurrentVariant(I%s.Variants.%s);\n", type_name.c_str(), cs.name.c_str());
			f.write("\t\t\t\t%s_message.parse(parser, val);\n", case_name.c_str());
			f.write("\t\t\t\tbreak;\n");
			f.write("\t\t\t}\n");
		}

		f.write("\t\t\tdefault: throw new System.Exception();\n");
		f.write("\t\t}\n\n");

		//f.write("\t\treturn val;\n");
		f.write("\t}\n");
	}

	void csharpDu_generateUnionComposeJson2(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic static void compose(JsonComposer composer, I%s val)\n", type_name);

		f.write("\t{\n");


		f.write("\t\tcomposer.append( \"{\\n  \");\n");
		f.write("\t\tcomposer.addNamePart(\"caseid\");\n");
		f.write("\t\tcomposer.composeSignedInteger((Int64)val.currentVariant());\n");
		f.write("\t\tcomposer.append( \",\\n  \" );\n");
		f.write("\t\tcomposer.addNamePart(\"caseData\");\n");

		f.write("\t\tswitch (val.currentVariant())\n");
		f.write("\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCSharpTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name, cs.name.c_str());
			f.write("\t\t\t{\n");

			string caller = generateCsharpCallerParams(cs, true);
			f.write("\t\t\t\t%s_message.compose(composer, %s);\n", case_name.c_str(), caller.c_str());
		
			f.write("\t\t\t\tbreak;\n");
			f.write("\t\t\t}\n");
		}

		f.write("\t\t\tdefault: throw new System.Exception();\n");
		f.write("\t\t}\n\n");

		f.write("\t\tcomposer.append( \"\\n}\" );\n");

		f.write("\t}\n");
	}

	void csharpDu_generateUnionComposeGmq2(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

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

			std::string case_name = getCSharpTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name, cs.name.c_str());
			f.write("\t\t\t{\n");
			string caller = generateCsharpCallerParams(cs, true);
			f.write("\t\t\t\t%s_message.compose(composer, %s);\n", case_name.c_str(), caller.c_str());
			f.write("\t\t\t\tbreak;\n");
			f.write("\t\t\t}\n");
		}

		f.write("\t\t\tdefault: throw new System.Exception();\n");
		f.write("\t\t}\n\n");

		f.write("\t}\n");
	}

	void csharpDu_generateUnionCaseProperties(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union_case);

		CsharpFileWritter f(header, 0);

		std::string case_name = getCSharpTypeName(s);

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
				f.write("public %s %s\n", type_name, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("public I%s %s\n", member.type.name.c_str(), member.name.c_str());
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
					f.write("public IList<%s> %s\n", type_name, member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					f.write("public IList<I%s> %s\n", member.type.name.c_str(), member.name.c_str());
					break;
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}

			f.write("\n");
			f.write("\t{\n");
			f.write("\t\tget { return ((%s)this.mem).%s; }\n", case_name.c_str(), member.name.c_str());
			f.write("\t\tset { ((%s)this.mem).%s = value; }\n", case_name.c_str(), member.name.c_str());
			f.write("\t}\n");
		}
	}



	void csharpDu_generateUnionEqualsMethod(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

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

	void csharpDu_generateUnionEquivalentMethod(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

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

			std::string case_type_name = getCSharpTypeName(cs);

			f.write("\t\t\tcase I%s.Variants.%s:\n", type_name, cs.name.c_str());
			f.increment();
			generateCsharpStructEquivalentExpression(f, cs);
			f.decrement();
		}
		f.write("\t\t\tdefault: return false;\n");
		f.write("\t\t\t}\n");
		f.write("\t\t}\n");
		f.write("\t}\n");
	}

	void csharpDu_generateUnionResetHandlers(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

		f.write("\tvoid reset_handlers()\n");
		f.write("\t{\n");


		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);
			f.write("\n\t// IDL CASE %s:\n", duit->name.c_str());

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

	void csharpDu_generateUnionAddressEnum(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

		f.write("\tenum Address\n");
		f.write("\t{\n");
		f.write("\t\tCurrentVariant = 0,\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			size_t sz = duit->getMembers().size();
			for (size_t i = 0; i != sz; ++i)
			{
				auto& it = duit->getMembers()[i];
				assert(it != nullptr);

				// use i + 1, sice 0 is already used by CurrentVariant
				string number = std::to_string(i + 1);

				if(i != sz - 1)
					f.write("\t\t%s = %s,\n", it->name.c_str(), number.c_str());
				else
					f.write("\t\t%s = %s\n", it->name.c_str(), number.c_str());
			}
		}
		f.write("\t};\n");

	}

	void csharpDu_generateCompose(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic static void compose(IPublishableComposer composer, I%s t)\n", type_name);
		f.write("\t{\n");

		f.write("\t\tI%s.Variants c = t.currentVariant();\n", type_name);
		f.write("\t\tif( c == I%s.Variants.unknown)\n", type_name);
		f.write("\t\t{\n");

		f.write("\t\tcomposer.composeUnsigned(\"caseId\", (UInt64)c, false);\n");
		f.write("\t\t}\n");
		f.write("\t\telse\n");
		f.write("\t\t{\n");
		f.write("\t\t\tcomposer.composeUnsigned(\"caseId\", (UInt64)c, true);\n");
		f.write("\t\t\tcomposer.composePublishableStructBegin(\"caseData\");\n");
		f.write("\t\t\tswitch(c)\n");
		f.write("\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(*duit);

			f.write("\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			f.write("\t\t\t\t{\n");

			generateCsharpPublisherCompose(header, *duit);

			f.write("\t\t\t\t}\n");
			f.write("\t\t\t\tbreak;\n");

		}

		f.write("\t\t\t\tdefault:\n");
		f.write("\t\t\t\t\tthrow new Exception();\n");
		f.write("\t\t\t}\n");

		f.write("\t\t\tcomposer.composePublishableStructEnd(false);\n");
		f.write("\t\t}\n");
		f.write("\t}\n");
	}

	void csharpDu_generateParseStateSync(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic static void parseForStateSync(IPublishableParser parser, %s_subscriber subscriber)\n", type_name);
		f.write("\t{\n");
		f.write("\t\tsubscriber.parse_CurrentVariant(parser, \"caseId\", true);\n");
		f.write("\t\tI%s.Variants c = subscriber.currentVariant();\n", type_name);
		f.write("\t\tif(  c != I%s.Variants.unknown)\n", type_name);
		f.write("\t\t{\n");
		f.write("\t\t\tparser.parsePublishableStructBegin(\"caseData\");\n");

		f.write("\t\t\tswitch(c)\n");
		f.write("\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(*duit);

			f.write("\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			f.write("\t\t\t\t{\n");

			generateCsharpSubscriberParseForStateSync(header, *duit);

			f.write("\t\t\t\t}\n");
			f.write("\t\t\t\tbreak;\n");

		}
		f.write("\t\t\t\tdefault:\n");
		f.write("\t\t\t\t\tthrow new Exception();\n");
		f.write("\t\t\t}\n");

		f.write("\t\t\tparser.parsePublishableStructEnd();\n");
		f.write("\t\t}\n");
		f.write("\t}\n");
	}
	void csharpDu_generateParse1(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber)\n", type_name);
		f.write("\t{\n");

		f.write("\t\tbool changed = subscriber.parse_CurrentVariant(parser, \"caseId\", false);\n");
		f.write("\t\tI%s.Variants c = subscriber.currentVariant();\n", type_name);

		f.write("\t\tif( c != I%s.Variants.unknown)\n", type_name);
		f.write("\t\t{\n");
		f.write("\t\t\tparser.parsePublishableStructBegin(\"caseData\");\n");

		f.write("\t\t\tswitch(c)\n");
		f.write("\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(*duit);

			f.write("\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			f.write("\t\t\t\t{\n");

			generateCsharpSubscriberParse1(header, *duit);

			f.write("\t\t\t\t}\n");
			f.write("\t\t\t\tbreak;\n");

		}
		f.write("\t\t\t\tdefault:\n");
		f.write("\t\t\t\t\tthrow new Exception();\n");
		f.write("\t\t\t}\n");

		f.write("\t\t\tparser.parsePublishableStructEnd();\n");
		f.write("\t\t}\n");

		f.write("\t\treturn changed;\n");
		f.write("\t}\n");
	}

	void csharpDu_generateParse2(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber, UInt64[] addr, int offset)\n", type_name);
		f.write("\t{\n");

		f.write("\t\tbool changed = false;\n");

		f.write("\t\tif( (Address)addr[offset] == Address.CurrentVariant)\n");
		f.write("\t\t{\n");
		f.write("\t\t\tif(addr.Length != offset + 1)\n");
		f.write("\t\t\t\tthrow new Exception();\n");
		f.write("\t\t\tchanged = subscriber.parse_CurrentVariant(parser, \"value\", false) | changed;\n");
		f.write("\t\t}\n");

		f.write("\t\telse\n");
		f.write("\t\t{\n");

		f.write("\t\t\tswitch(subscriber.currentVariant())\n");
		f.write("\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(*duit);

			f.write("\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			f.write("\t\t\t\t{\n");

			generateCsharpSubscriberParse2(header, *duit);

			f.write("\t\t\t\t}\n");
			f.write("\t\t\t\tbreak;\n");

		}
		f.write("\t\t\t\tdefault:\n");
		f.write("\t\t\t\t\tthrow new Exception();\n");
		f.write("\t\t\t}\n");

		f.write("\t\t}\n");

		f.write("\t\treturn changed;\n");
		f.write("\t}\n");
	}

}



void generateCsharpUnionInterface(FILE* header, CompositeType& s)
{
	assert(s.isDiscriminatedUnion());

	bool checked = impl_checkParamNameUniqueness(s);
	checked = impl_checkFollowingExtensionRules(s) && checked;
	if (!checked)
		throw std::exception();


	impl_generateMessageCommentBlock(header, s);

	CsharpFileWritter f(header, 0);
	
	std::string type_name = getCSharpTypeName(s);


	f.write("public interface I%s : IEquivalenceComparable<I%s>\n", type_name.c_str(), type_name.c_str());
	f.write("{\n");

	f.write("\tpublic enum Variants\n");
	f.write("\t{\n");
	f.write("\t\tunknown = 0,\n");

	size_t sz = s.getDiscriminatedUnionCases().size();
	for (size_t i = 0; i != sz; ++i)
	{
		auto& it = s.getDiscriminatedUnionCases()[i];
		assert(it != nullptr);

		assert(it->type == CompositeType::Type::discriminated_union_case);

		string number = std::to_string(it->numID);

		if(i != sz - 1)
			f.write("\t\t%s = %s,\n", it->name.c_str(), number.c_str());
		else
			f.write("\t\t%s = %s\n", it->name.c_str(), number.c_str());
	}

	f.write("\t}\n");

	f.write("\tVariants currentVariant();\n");

	f.write("\tvoid setCurrentVariant(Variants v);\n");

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpInterfaceMember(header, *it);
		}
	}
	f.write("} // interface I%s\n\n", s.name.c_str());
}

void generateCsharpUnionImpl(FILE* header, CompositeType& s)
{
	assert(s.isDiscriminatedUnion());

	CsharpFileWritter f(header, 0);

	std::string type_name = getCSharpTypeName(s);

	f.write("public class %s : I%s, IEquatable<%s>\n", type_name.c_str(), type_name.c_str(), type_name.c_str());

	f.write("{\n");

	f.write("\tObject mem;\n");


	f.write("\n");


	generateCsharpStandardMethods(header, type_name.c_str());
	csharpDu_generateUnionEqualsMethod(header, s, type_name);
	csharpDu_generateUnionEquivalentMethod(header, s, type_name.c_str());

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

		std::string case_type_name = getCSharpTypeName(cs);
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

		std::string case_type_name = getCSharpTypeName(cs);

		f.write("\t\tcase I%s.Variants.%s: this.mem = new %s(); break;\n", type_name.c_str(), cs.name.c_str(), case_type_name.c_str());
	}
	f.write("\t\tdefault: throw new Exception();\n");
	f.write("\t\t}\n");
	f.write("\t}\n");

	// each case attributes
	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		f.write("\n\t// IDL CASE %s:\n", cs.name.c_str());

		csharpDu_generateUnionCaseProperties(header, cs);
	}

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		f.write("\n\t// IDL CASE %s:\n", cs.name.c_str());

		std::string case_type_name = getCSharpTypeName(cs);

		generateCsharpStructImpl(header, cs, case_type_name.c_str(), nullptr);
		//csharpDu_generateStructMessage(header, root, cs, case_type_name.c_str(), interface_name.c_str());
	}

	f.write("} // class %s\n\n", s.name.c_str());
}

void generateCsharpUnionMessage(FILE* header, CompositeType& s)
{
	assert(s.isDiscriminatedUnion());

	CsharpFileWritter f(header, 0);

	std::string type_name = getCSharpTypeName(s);

	f.write("public class %s_message\n", type_name.c_str());
	f.write("{\n");

	csharpDu_generateUnionParseJson3(header, s);
	csharpDu_generateUnionParseGmq3(header, s);

	csharpDu_generateUnionComposeJson2(header, s, type_name.c_str());
	csharpDu_generateUnionComposeGmq2(header, s, type_name.c_str());


	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		f.write("\n\t// IDL CASE %s:\n", cs.name.c_str());

		std::string case_type_name = getCSharpTypeName(cs);
		std::string interface_name = "I" + type_name;

		generateCsharpStructMessage(header, cs, case_type_name.c_str(), interface_name.c_str());
	}


	f.write("} // class %s_message\n\n", s.name.c_str());
}


void generateCsharpUnionSubscriber(FILE* header, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::discriminated_union);

	CsharpFileWritter f(header, 0);

	f.write("public class %s_subscriber : I%s", type_name, type_name);

	//if (s.type == CompositeType::Type::publishable)
	//	f.write(", StateSubscriberBase");

	f.write("\n{\n");

	f.write("\n\t/////////////////////////////////  begin user override section /////////////////////////////////\n\n");

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpSubscriberFactoryMethod(header, *it);
		}
	}

	f.write("\tpublic virtual void notifyUpdated_CurrentVariant(I%s.Variants old) { }\n", type_name);

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		f.write("\n\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpSubscriberEventHandler(header, *it);
		}
	}
	f.write("\n\t/////////////////////////////////   end user override section  /////////////////////////////////\n\n\n");


	f.write("\tprotected I%s data;\n", type_name);

	csharpDu_generateUnionAddressEnum(header, s);

	f.write("\tpublic %s_subscriber(I%s data) { this.data = data; }\n", type_name, type_name);

	f.write("\tpublic I%s.Variants currentVariant() { return data.currentVariant(); }\n", type_name);
	f.write("\tpublic void setCurrentVariant(I%s.Variants v) { throw new InvalidOperationException(); }\n", type_name);

	f.write("\tbool parse_CurrentVariant(IPublishableParser parser, String name, bool forceUpdate)\n");
	f.write("\t{\n");
	f.write("\t\tI%s.Variants newVal = (I%s.Variants)parser.parseUnsigned(name);\n", type_name, type_name);
						
	f.write("\t\tif (forceUpdate)\n");
	f.write("\t\t{\n");
	f.write("\t\t\tdata.setCurrentVariant(newVal);\n");
	f.write("\t\t\treset_handlers();\n");
	f.write("\t\t\treturn true;\n");
	f.write("\t\t}\n");
	f.write("\t\telse if (newVal != data.currentVariant())\n");
	f.write("\t\t{\n");
	f.write("\t\t\tI%s.Variants oldVal = data.currentVariant();\n", type_name);
	f.write("\t\t\tdata.setCurrentVariant(newVal);\n");
	f.write("\t\t\treset_handlers();\n");
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
		f.write("\n\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpSubscriberMember(header, *it);
		}
	}

	generateCsharpSimpleEquivalentMethod(header, type_name, "data");

	csharpDu_generateParseStateSync(header, s, type_name);
	csharpDu_generateParse1(header, s, type_name);
	csharpDu_generateParse2(header, s, type_name);

	csharpDu_generateUnionResetHandlers(header, s);

	//for (auto& duit : s.getDiscriminatedUnionCases())
	//{
	//	assert(duit != nullptr);
	//	auto& cs = *duit;
	//	assert(cs.type == CompositeType::Type::discriminated_union_case);
	//	f.write("\n\t// IDL CASE %s:\n", cs.name.c_str());

	//	std::string case_type_name = getCSharpTypeName(cs);

	//	generateCsharpStructSubscriberForUnionCase(header, cs, case_type_name.c_str());
	//}



	//csharpPub_generateDebugMethodSubs type_name);
	f.write("\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
	f.write("\tpublic void debugOnlySetData(I%s data)\n", type_name);
	f.write("\t{\n");
	f.write("\t\tthis.data = data;\n");
	f.write("\t\treset_handlers();\n");
	f.write("\t}\n");


	f.write("} // class %s_subscriber\n\n", type_name);
}

void generateCsharpUnionPublisher(FILE* header, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::discriminated_union);

	CsharpFileWritter f(header, 0);

	f.write("public class %s_publisher : I%s", type_name, type_name);

	f.write("\n");


	f.write("{\n");
	f.write("\tI%s t;\n", type_name);
	f.write("\tIPublishableComposer composer;\n");
	f.write("\tUInt64[] address;\n");

	csharpDu_generateUnionAddressEnum(header, s);

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
		f.write("\n\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpPublisherMember(header, *it);
		}
	}

	generateCsharpSimpleEquivalentMethod(header, type_name, "t");

	csharpDu_generateCompose(header, s, type_name);


	f.write("\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
	f.write("\tpublic void debugOnlySetData(I%s data) { this.t = data; }\n", type_name);

	f.write("} // class %s_publisher\n\n", type_name);
}

