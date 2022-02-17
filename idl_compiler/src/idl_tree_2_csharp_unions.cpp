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

		std::string type_name = getCSharpTypeName(s);

		fprintf(header, "\tpublic static void parse(JsonParser parser, I%s val)\n", type_name.c_str());

		fprintf(header, "\t{\n");
		//fprintf(header, "\t\t%s val = new %s();\n\n", type_name.c_str(), type_name.c_str());

		fprintf(header, "\t\tparser.skipDelimiter('{');\n");
		fprintf(header, "\t\tstring key;\n");
		fprintf(header, "\t\tparser.readKeyFromJson( out key );\n");
		fprintf(header, "\t\tif (key != \"caseid\")\n");
		fprintf(header, "\t\t\tthrow new FormatException();\n");
		fprintf(header, "\t\tInt64 caseID;\n");
		fprintf(header, "\t\tparser.parseSignedInteger(out caseID);\n");
		fprintf(header, "\t\tparser.skipSpacesEtc();\n");
		fprintf(header, "\t\tparser.skipDelimiter(',');\n");
		fprintf(header, "\t\tparser.readKeyFromJson(out key);\n");
		fprintf(header, "\t\tif (key != \"caseData\")\n");
		fprintf(header, "\t\t\tthrow new FormatException();\n\n");

		fprintf(header, "\t\tswitch ((I%s.Variants)caseID)\n", type_name.c_str());
		fprintf(header, "\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCSharpTypeName(cs);

			fprintf(header, "\t\t\tcase I%s.Variants.%s:\n", type_name.c_str(), cs.name.c_str());
			fprintf(header, "\t\t\t{\n");
			fprintf(header, "\t\t\t\tval.setCurrentVariant(I%s.Variants.%s);\n", type_name.c_str(), cs.name.c_str());
			fprintf(header, "\t\t\t\t%s_message.parse(parser, val);\n", case_name.c_str());
			fprintf(header, "\t\t\t\tbreak;\n");
			fprintf(header, "\t\t\t}\n");
		}

		fprintf(header, "\t\t\tdefault: throw new System.Exception();\n");
		fprintf(header, "\t\t}\n\n");

		fprintf(header, "\t\tparser.skipDelimiter('}');\n\n");

		//fprintf(header, "\t\treturn val;\n");
		fprintf(header, "\t}\n");
	}
	void csharpDu_generateUnionParseGmq3(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string type_name = getCSharpTypeName(s);

		fprintf(header, "\tpublic static void parse(GmqParser parser, I%s val)\n", type_name.c_str());

		fprintf(header, "\t{\n");
		//fprintf(header, "\t\t%s val = new %s();\n\n", type_name.c_str(), type_name.c_str());


		fprintf(header, "\t\tInt64 caseID;\n");
		fprintf(header, "\t\tparser.parseSignedInteger(out caseID);\n\n");

		fprintf(header, "\t\tswitch ((I%s.Variants)caseID)\n", type_name.c_str());
		fprintf(header, "\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCSharpTypeName(cs);

			fprintf(header, "\t\t\tcase I%s.Variants.%s:\n", type_name.c_str(), cs.name.c_str());
			fprintf(header, "\t\t\t{\n");
			fprintf(header, "\t\t\t\tval.setCurrentVariant(I%s.Variants.%s);\n", type_name.c_str(), cs.name.c_str());
			fprintf(header, "\t\t\t\t%s_message.parse(parser, val);\n", case_name.c_str());
			fprintf(header, "\t\t\t\tbreak;\n");
			fprintf(header, "\t\t\t}\n");
		}

		fprintf(header, "\t\t\tdefault: throw new System.Exception();\n");
		fprintf(header, "\t\t}\n\n");

		//fprintf(header, "\t\treturn val;\n");
		fprintf(header, "\t}\n");
	}

	void csharpDu_generateUnionComposeJson2(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		fprintf(header, "\tpublic static void compose(JsonComposer composer, I%s val)\n", type_name);

		fprintf(header, "\t{\n");


		fprintf(header, "\t\tcomposer.append( \"{\\n  \");\n");
		fprintf(header, "\t\tcomposer.addNamePart(\"caseid\");\n");
		fprintf(header, "\t\tcomposer.composeSignedInteger((Int64)val.currentVariant());\n");
		fprintf(header, "\t\tcomposer.append( \",\\n  \" );\n");
		fprintf(header, "\t\tcomposer.addNamePart(\"caseData\");\n");

		fprintf(header, "\t\tswitch (val.currentVariant())\n");
		fprintf(header, "\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCSharpTypeName(cs);

			fprintf(header, "\t\t\tcase I%s.Variants.%s:\n", type_name, cs.name.c_str());
			fprintf(header, "\t\t\t{\n");
			fprintf(header, "\t\t\t\t%s_message.compose(composer, ", case_name.c_str());
			generateCsharpCallerParams(header, cs, true);
			fprintf(header, ");\n");
			fprintf(header, "\t\t\t\tbreak;\n");
			fprintf(header, "\t\t\t}\n");
		}

		fprintf(header, "\t\t\tdefault: throw new System.Exception();\n");
		fprintf(header, "\t\t}\n\n");

		fprintf(header, "\t\tcomposer.append( \"\\n}\" );\n");

		fprintf(header, "\t}\n");
	}

	void csharpDu_generateUnionComposeGmq2(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		fprintf(header, "\tpublic static void compose(GmqComposer composer, I%s val)\n", type_name);

		fprintf(header, "\t{\n");
		fprintf(header, "\t\tcomposer.composeSignedInteger((Int64)val.currentVariant());\n");

		fprintf(header, "\t\tswitch (val.currentVariant())\n");
		fprintf(header, "\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = getCSharpTypeName(cs);

			fprintf(header, "\t\t\tcase I%s.Variants.%s:\n", type_name, cs.name.c_str());
			fprintf(header, "\t\t\t{\n");
			fprintf(header, "\t\t\t\t%s_message.compose(composer, ", case_name.c_str());
			generateCsharpCallerParams(header, cs, true);
			fprintf(header, ");\n");
			fprintf(header, "\t\t\t\tbreak;\n");
			fprintf(header, "\t\t\t}\n");
		}

		fprintf(header, "\t\t\tdefault: throw new System.Exception();\n");
		fprintf(header, "\t\t}\n\n");

		fprintf(header, "\t}\n");
	}

	void csharpDu_generateUnionCaseProperties(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union_case);

		std::string case_name = getCSharpTypeName(s);

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;

			fprintf(header, "\tpublic ");

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* type_name = getCSharpPrimitiveType(member.type.kind);
				fprintf(header, "%s %s", type_name, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "I%s %s", member.type.name.c_str(), member.name.c_str());
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
					fprintf(header, "IList<%s> %s", type_name, member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					fprintf(header, "IList<I%s> %s", member.type.name.c_str(), member.name.c_str());
					break;
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}

			fprintf(header,
				"\n"
				"\t{\n"
				"\t\tget { return ((%s)this.mem).%s; }\n", case_name.c_str(), member.name.c_str());
			fprintf(header,
				"\t\tset { ((%s)this.mem).%s = value; }\n", case_name.c_str(), member.name.c_str());
			fprintf(header,
				"\t}\n");



		}
	}



	void csharpDu_generateUnionEqualsMethod(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);


		fprintf(header, "\tpublic bool Equals(%s other)\n", type_name.c_str());
		fprintf(header,
			"\t{\n"
			"\t\tif (ReferenceEquals(this, other))\n"
			"\t\t\treturn true;\n"
			"\t\telse if (ReferenceEquals(null, other))\n"
			"\t\t\treturn false;\n"
			"\t\telse\n"
			"\t\t\treturn this.mem.Equals(other.mem);\n"
			"\t}\n"
		);
	}

	void csharpDu_generateUnionEquivalentMethod(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);
		fprintf(header, "\tpublic bool isEquivalent(I%s other)\n", type_name);
		fprintf(header,
			"\t{\n"
			"\t\tif (ReferenceEquals(other, null))\n"
			"\t\t\treturn false;\n"
			"\t\telse if (ReferenceEquals(this, other))\n"
			"\t\t\treturn true;\n"
			"\t\telse if (currentVariant() != other.currentVariant())\n"
			"\t\t\treturn false;\n"
			"\t\telse\n"
			"\t\t{\n");

		fprintf(header, "\t\t\tswitch(currentVariant())\n");
		fprintf(header, "\t\t\t{\n");

		for (auto& it : s.getDiscriminatedUnionCases())
		{
			assert(it != nullptr);
			CompositeType& cs = *it;
			assert(cs.type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(cs);

			fprintf(header, "\t\t\tcase I%s.Variants.%s: return\n", type_name, cs.name.c_str());
			generateCsharpStructEquivalentExpression(header, cs);
			fprintf(header, ";\n");

		}
		fprintf(header, "\t\t\tdefault: return false;\n");
		fprintf(header, "\t\t\t}\n");
		fprintf(header, "\t\t}\n");
		fprintf(header, "\t}\n");
	}

	void csharpDu_generateUnionResetHandlers(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		fprintf(header, "\tvoid reset_handlers()\n");
		fprintf(header, "\t{\n");


		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);
			fprintf(header, "\n\t// IDL CASE %s:\n", duit->name.c_str());

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
		}

		fprintf(header, "\t}\n");
	}

	void csharpDu_generateUnionAddressEnum(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);
		fprintf(header, "\tenum Address { CurrentVariant = 0");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			unsigned long j = 1;
			for (auto& it : duit->getMembers())
			{
				fprintf(header, ", ");

				assert(it != nullptr);
				fprintf(header, "%s = %lu", it->name.c_str(), (unsigned long)j);
				++j;
			}
		}
		fprintf(header, " };\n");

	}

	void csharpDu_generateCompose(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		fprintf(header, "\tpublic static void compose(IPublishableComposer composer, I%s t)\n", type_name);
		fprintf(header, "\t{\n");

		fprintf(header, "\t\tI%s.Variants c = t.currentVariant();\n", type_name);
		fprintf(header, "\t\tif( c == I%s.Variants.unknown)\n", type_name);
		fprintf(header, "\t\t{\n");

		fprintf(header, "\t\tcomposer.composeUnsigned(\"caseId\", (UInt64)c, false);\n");
		fprintf(header, "\t\t}\n");
		fprintf(header, "\t\telse\n");
		fprintf(header, "\t\t{\n");
		fprintf(header, "\t\t\tcomposer.composeUnsigned(\"caseId\", (UInt64)c, true);\n");
		fprintf(header, "\t\t\tcomposer.composePublishableStructBegin(\"caseData\");\n");
		fprintf(header, "\t\t\tswitch(c)\n");
		fprintf(header, "\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(*duit);

			fprintf(header, "\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			fprintf(header, "\t\t\t\t{\n");

			generateCsharpPublisherCompose(header, *duit);

			fprintf(header, "\t\t\t\t}\n");
			fprintf(header, "\t\t\t\tbreak;\n");

		}

		fprintf(header, "\t\t\t\tdefault:\n");
		fprintf(header, "\t\t\t\t\tthrow new Exception();\n");
		fprintf(header, "\t\t\t}\n");

		fprintf(header, "\t\t\tcomposer.composePublishableStructEnd(false);\n");
		fprintf(header, "\t\t}\n");
		fprintf(header, "\t}\n");
	}

	void csharpDu_generateParseStateSync(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		fprintf(header, "\tpublic static void parseForStateSync(IPublishableParser parser, %s_subscriber subscriber)\n", type_name);
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tsubscriber.parse_CurrentVariant(parser, \"caseId\", true);\n");
		fprintf(header, "\t\tI%s.Variants c = subscriber.currentVariant();\n", type_name);
		fprintf(header, "\t\tif(  c != I%s.Variants.unknown)\n", type_name);
		fprintf(header, "\t\t{\n");
		fprintf(header, "\t\t\tparser.parsePublishableStructBegin(\"caseData\");\n");

		fprintf(header, "\t\t\tswitch(c)\n");
		fprintf(header, "\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(*duit);

			fprintf(header, "\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			fprintf(header, "\t\t\t\t{\n");

			generateCsharpSubscriberParseForStateSync(header, *duit);

			fprintf(header, "\t\t\t\t}\n");
			fprintf(header, "\t\t\t\tbreak;\n");

		}
		fprintf(header, "\t\t\t\tdefault:\n");
		fprintf(header, "\t\t\t\t\tthrow new Exception();\n");
		fprintf(header, "\t\t\t}\n");

		fprintf(header, "\t\t\tparser.parsePublishableStructEnd();\n");
		fprintf(header, "\t\t}\n");
		fprintf(header, "\t}\n");
	}
	void csharpDu_generateParse1(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		fprintf(header, "\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber)\n", type_name);
		fprintf(header, "\t{\n");

		fprintf(header, "\t\tbool changed = subscriber.parse_CurrentVariant(parser, \"caseId\", false);\n");
		fprintf(header, "\t\tI%s.Variants c = subscriber.currentVariant();\n", type_name);

		fprintf(header, "\t\tif( c != I%s.Variants.unknown)\n", type_name);
		fprintf(header, "\t\t{\n");
		fprintf(header, "\t\t\tparser.parsePublishableStructBegin(\"caseData\");\n");

		fprintf(header, "\t\t\tswitch(c)\n");
		fprintf(header, "\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(*duit);

			fprintf(header, "\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			fprintf(header, "\t\t\t\t{\n");

			generateCsharpSubscriberParse1(header, *duit);

			fprintf(header, "\t\t\t\t}\n");
			fprintf(header, "\t\t\t\tbreak;\n");

		}
		fprintf(header, "\t\t\t\tdefault:\n");
		fprintf(header, "\t\t\t\t\tthrow new Exception();\n");
		fprintf(header, "\t\t\t}\n");

		fprintf(header, "\t\t\tparser.parsePublishableStructEnd();\n");
		fprintf(header, "\t\t}\n");

		fprintf(header, "\t\treturn changed;\n");
		fprintf(header, "\t}\n");
	}

	void csharpDu_generateParse2(FILE* header, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		fprintf(header, "\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber, UInt64[] addr, int offset)\n", type_name);
		fprintf(header, "\t{\n");

		fprintf(header, "\t\tbool changed = false;\n");

		fprintf(header, "\t\tif( (Address)addr[offset] == Address.CurrentVariant)\n");
		fprintf(header, "\t\t{\n");
		fprintf(header, "\t\t\tif(addr.Length != offset + 1)\n");
		fprintf(header, "\t\t\t\tthrow new Exception();\n");
		fprintf(header, "\t\t\tchanged = subscriber.parse_CurrentVariant(parser, \"value\", false) | changed;\n");
		fprintf(header, "\t\t}\n");

		fprintf(header, "\t\telse\n");
		fprintf(header, "\t\t{\n");

		fprintf(header, "\t\t\tswitch(subscriber.currentVariant())\n");
		fprintf(header, "\t\t\t{\n");

		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			assert(duit->type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(*duit);

			fprintf(header, "\t\t\t\tcase I%s.Variants.%s:\n", type_name, duit->name.c_str());
			fprintf(header, "\t\t\t\t{\n");

			generateCsharpSubscriberParse2(header, *duit);

			fprintf(header, "\t\t\t\t}\n");
			fprintf(header, "\t\t\t\tbreak;\n");

		}
		fprintf(header, "\t\t\t\tdefault:\n");
		fprintf(header, "\t\t\t\t\tthrow new Exception();\n");
		fprintf(header, "\t\t\t}\n");

		fprintf(header, "\t\t}\n");

		fprintf(header, "\t\treturn changed;\n");
		fprintf(header, "\t}\n");
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

	std::string type_name = getCSharpTypeName(s);


	fprintf(header, "public interface I%s : IEquivalenceComparable<I%s>\n", type_name.c_str(), type_name.c_str());
	fprintf(header, "{\n");

	fprintf(header, "\tpublic enum Variants { unknown = 0");

	for (auto& it : s.getDiscriminatedUnionCases())
	{
		assert(it != nullptr);
		CompositeType& cs = *it;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		fprintf(header, ", %s = %lld", cs.name.c_str(), cs.numID);
	}

	fprintf(header, " };\n");

	fprintf(header, "\tVariants currentVariant();\n");

	fprintf(header, "\tvoid setCurrentVariant(Variants v);\n");

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpInterfaceMember(header, *it);
		}
	}
	fprintf(header, "} // interface I%s\n\n", s.name.c_str());
}

void generateCsharpUnionImpl(FILE* header, CompositeType& s)
{
	assert(s.isDiscriminatedUnion());

	std::string type_name = getCSharpTypeName(s);

	fprintf(header, "public class %s : I%s, IEquatable<%s>\n"
		"{\n", type_name.c_str(), type_name.c_str(), type_name.c_str());


	fprintf(header, "\tObject mem;\n");


	fprintf(header, "\n");


	generateCsharpStandardMethods(header, type_name.c_str());
	csharpDu_generateUnionEqualsMethod(header, s, type_name);
	csharpDu_generateUnionEquivalentMethod(header, s, type_name.c_str());

	// currentVariant()

	fprintf(header, "\tpublic I%s.Variants currentVariant()\n", type_name.c_str());
	fprintf(header, "\t{\n"
		"\t\tif(this.mem == null)\n"
		"\t\t\treturn I%s.Variants.unknown;\n", type_name.c_str());

	for (auto& it : s.getDiscriminatedUnionCases())
	{
		assert(it != nullptr);
		CompositeType& cs = *it;
		assert(cs.type == CompositeType::Type::discriminated_union_case);

		std::string case_type_name = getCSharpTypeName(cs);
		fprintf(header,
			"\t\telse if(this.mem is %s)\n"
			"\t\t\treturn I%s.Variants.%s;\n", case_type_name.c_str(), type_name.c_str(), cs.name.c_str());
	}

	fprintf(header,
		"\t\telse\n"
		"\t\t\treturn I%s.Variants.unknown;\n"
		"\t}\n", type_name.c_str());


	fprintf(header, "\tpublic void setCurrentVariant(I%s.Variants v)\n", type_name.c_str());
	fprintf(header, "\t{\n");

	fprintf(header, "\t\tswitch(v)\n");
	fprintf(header, "\t\t{\n");
	for (auto& it : s.getDiscriminatedUnionCases())
	{
		assert(it != nullptr);
		CompositeType& cs = *it;
		assert(cs.type == CompositeType::Type::discriminated_union_case);

		std::string case_type_name = getCSharpTypeName(cs);

		fprintf(header, "\t\tcase I%s.Variants.%s: this.mem = new %s(); break;\n", type_name.c_str(), cs.name.c_str(), case_type_name.c_str());
	}
	fprintf(header, "\t\tdefault: throw new Exception();\n");
	fprintf(header, "\t\t}\n");
	fprintf(header, "\t}\n");

	// each case attributes
	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

		csharpDu_generateUnionCaseProperties(header, cs);
	}

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

		std::string case_type_name = getCSharpTypeName(cs);

		generateCsharpStructImpl(header, cs, case_type_name.c_str(), nullptr);
		//csharpDu_generateStructMessage(header, root, cs, case_type_name.c_str(), interface_name.c_str());
	}

	fprintf(header, "} // class %s\n\n", s.name.c_str());
}

void generateCsharpUnionMessage(FILE* header, CompositeType& s)
{
	assert(s.isDiscriminatedUnion());

	std::string type_name = getCSharpTypeName(s);


	fprintf(header, "public class %s_message\n", type_name.c_str());
	fprintf(header, "{\n");

	csharpDu_generateUnionParseJson3(header, s);
	csharpDu_generateUnionParseGmq3(header, s);

	csharpDu_generateUnionComposeJson2(header, s, type_name.c_str());
	csharpDu_generateUnionComposeGmq2(header, s, type_name.c_str());


	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

		std::string case_type_name = getCSharpTypeName(cs);
		std::string interface_name = "I" + type_name;

		generateCsharpStructMessage(header, cs, case_type_name.c_str(), interface_name.c_str());
	}


	fprintf(header, "} // class %s_message\n\n", s.name.c_str());
}


void generateCsharpUnionSubscriber(FILE* header, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::discriminated_union);

	fprintf(header, "public class %s_subscriber : I%s", type_name, type_name);

	//if (s.type == CompositeType::Type::publishable)
	//	fprintf(header, ", StateSubscriberBase");

	fprintf(header, "\n{\n");

	fprintf(header, "\n\t/////////////////////////////////  begin user override section /////////////////////////////////\n\n");

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpSubscriberFactoryMethod(header, *it);
		}
	}

	fprintf(header, "\tpublic virtual void notifyUpdated_CurrentVariant(I%s.Variants old) { }\n", type_name);

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpSubscriberEventHandler(header, *it);
		}
	}
	fprintf(header, "\n\t/////////////////////////////////   end user override section  /////////////////////////////////\n\n\n");


	fprintf(header, "\tprotected I%s data;\n", type_name);

	csharpDu_generateUnionAddressEnum(header, s);

	fprintf(header, "\tpublic %s_subscriber(I%s data) { this.data = data; }\n", type_name, type_name);

	fprintf(header, "\tpublic I%s.Variants currentVariant() { return data.currentVariant(); }\n", type_name);
	fprintf(header, "\tpublic void setCurrentVariant(I%s.Variants v) { throw new InvalidOperationException(); }\n", type_name);

	fprintf(header, "\tbool parse_CurrentVariant(IPublishableParser parser, String name, bool forceChange)\n");
	fprintf(header, "\t{\n");
	fprintf(header, "\t\tI%s.Variants newVal = (I%s.Variants)parser.parseUnsigned(name);\n", type_name, type_name);
						
	fprintf(header, "\t\tif (forceChange)\n");
	fprintf(header, "\t\t{\n");
	fprintf(header, "\t\t\tdata.setCurrentVariant(newVal);\n");
	fprintf(header, "\t\t\treset_handlers();\n");
	fprintf(header, "\t\t\treturn true;\n");
	fprintf(header, "\t\t}\n");
	fprintf(header, "\t\telse if (newVal != data.currentVariant())\n");
	fprintf(header, "\t\t{\n");
	fprintf(header, "\t\t\tI%s.Variants oldVal = data.currentVariant();\n", type_name);
	fprintf(header, "\t\t\tdata.setCurrentVariant(newVal);\n");
	fprintf(header, "\t\t\treset_handlers();\n");
	fprintf(header, "\t\t\tnotifyUpdated_CurrentVariant(oldVal);\n");
	fprintf(header, "\t\t\treturn true;\n");
	fprintf(header, "\t\t}\n");
	fprintf(header, "\t\telse\n");
	fprintf(header, "\t\t\treturn false;\n");
	fprintf(header, "\t}\n");


	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", duit->name.c_str());

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
	//	fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

	//	std::string case_type_name = getCSharpTypeName(cs);

	//	generateCsharpStructSubscriberForUnionCase(header, cs, case_type_name.c_str());
	//}



	//csharpPub_generateDebugMethodSubs type_name);
	fprintf(header, "\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
	fprintf(header, "\tpublic void debugOnlySetData(I%s data)\n", type_name);
	fprintf(header, "\t{\n");
	fprintf(header, "\t\tthis.data = data;\n");
	fprintf(header, "\t\treset_handlers();\n");
	fprintf(header, "\t}\n");


	fprintf(header, "} // class %s_subscriber\n\n", type_name);
}

void generateCsharpUnionPublisher(FILE* header, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::discriminated_union);


	fprintf(header, "public class %s_publisher : I%s", type_name, type_name);

	fprintf(header, "\n");


	fprintf(header, "{\n");
	fprintf(header, "\tI%s t;\n", type_name);
	fprintf(header, "\tIPublishableComposer composer;\n");
	fprintf(header, "\tUInt64[] address;\n");

	csharpDu_generateUnionAddressEnum(header, s);

	fprintf(header, "\tpublic %s_publisher(I%s t, IPublishableComposer composer, UInt64[] address)\n", type_name, type_name);
	fprintf(header, "\t{\n");
	fprintf(header, "\t\tthis.t = t;\n");
	fprintf(header, "\t\tthis.composer = composer;\n");
	fprintf(header, "\t\tthis.address = address;\n");
	fprintf(header, "\t}\n");

	fprintf(header, "\tpublic I%s.Variants currentVariant() { return t.currentVariant(); }\n", type_name);

	fprintf(header, "\tpublic void setCurrentVariant(I%s.Variants v)\n", type_name);
	fprintf(header, "\t{\n");
	fprintf(header, "\t\tt.setCurrentVariant(v);\n");
	fprintf(header, "\t\tcomposer.composeAddress(address, (UInt64)Address.CurrentVariant);\n");
	fprintf(header, "\t\tcomposer.composeUnsigned(\"value\", (UInt64)v, false);\n");
	fprintf(header, "\t\tcomposer.composeAddressEnd();\n");
	fprintf(header, "\t}\n");


	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		assert(duit->type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", duit->name.c_str());

		for (auto& it : duit->getMembers())
		{
			assert(it != nullptr);
			generateCsharpPublisherMember(header, *it);
		}
	}

	generateCsharpSimpleEquivalentMethod(header, type_name, "t");

	// TODO
	csharpDu_generateCompose(header, s, type_name);
	//fprintf(header, "\tpublic static void compose(IPublishableComposer composer, I%s t)\n", type_name);
	//fprintf(header, "\t{\n");
	//fprintf(header, "\t\tthrow new NotImplementedException();\n");
	//fprintf(header, "\t}\n");


	fprintf(header, "\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
	fprintf(header, "\tpublic void debugOnlySetData(I%s data) { this.t = data; }\n", type_name);

	fprintf(header, "} // class %s_publisher\n\n", type_name);
}

