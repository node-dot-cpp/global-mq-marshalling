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

	const char* csharpDu_getCSharpType(MessageParameterType::KIND kind)
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

	const char* csharpDu_getMethodForKind(MessageParameterType::KIND kind)
	{
		switch (kind)
		{
		case MessageParameterType::KIND::INTEGER:
			return "SignedInteger";
		case MessageParameterType::KIND::UINTEGER:
			return "UnsignedInteger";
		case MessageParameterType::KIND::REAL:
			return "Real";
		case MessageParameterType::KIND::CHARACTER_STRING:
			return "String";
		default:
			assert(false);
		}
		return nullptr;
	}


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

	void csharpDu_generateUnionComposeJson2(FILE* header, Root& root, CompositeType& s, const char* type_name)
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

	void csharpDu_generateUnionComposeGmq2(FILE* header, Root& root, CompositeType& s, const char* type_name)
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

	void csharpDu_generateUnionCaseProperties(FILE* header, Root& root, CompositeType& s, bool forInterface)
	{
		assert(s.type == CompositeType::Type::discriminated_union_case);

		std::string type_name = getCSharpTypeName(s);

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;

			if (!forInterface)
				fprintf(header, "\tpublic ");
			else
				fprintf(header, "\t");

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "Int64 %s", member.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "UInt64 %s", member.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "Double %s", member.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "String %s", member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "I%s %s", member.type.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "IList<Int64> %s", member.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "IList<UInt64> %s", member.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "IList<Double> %s", member.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "IList<String> %s", member.name.c_str());
					break;
				case MessageParameterType::KIND::VECTOR:
					assert(false); // unexpected
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					assert(member.type.structIdx < root.structs.size());
					fprintf(header, "IList<I%s> %s", root.structs[member.type.structIdx]->name.c_str(), member.name.c_str());
					break;
				default:
					assert(false); // not implemented (yet)
				}
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}

			if (forInterface)
			{
				fprintf(header, " { get; set; }\n");
			}
			else
			{
				fprintf(header,
					"\n"
					"\t{\n"
					"\t\tget { return ((%s)this.mem).%s; }\n", type_name.c_str(), member.name.c_str());
				fprintf(header,
					"\t\tset { ((%s)this.mem).%s = value; }\n", type_name.c_str(), member.name.c_str());
				fprintf(header,
					"\t}\n");

			}


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
}


void generateCsharpUnionInterface(FILE* header, Root& root, CompositeType& s)
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

	fprintf(header, "\tpublic enum Variants { ");

	for (auto& it : s.getDiscriminatedUnionCases())
	{
		assert(it != nullptr);
		CompositeType& cs = *it;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "%s = %lld, ", cs.name.c_str(), cs.numID);
	}

	fprintf(header, "unknown };\n");

	fprintf(header, "\tVariants currentVariant();\n");

	fprintf(header, "\tvoid setCurrentVariant(Variants v);\n");

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

		std::string case_type_name = getCSharpTypeName(cs);
		csharpDu_generateUnionCaseProperties(header, root, cs, true);
		//generateCsharpStructInterface(header, root, cs, case_type_name.c_str());
	}
	fprintf(header, "} // interface I%s\n\n", s.name.c_str());
}

void generateCsharpUnionImpl(FILE* header, Root& root, CompositeType& s)
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

		csharpDu_generateUnionCaseProperties(header, root, cs, false);
	}

	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

		std::string case_type_name = getCSharpTypeName(cs);

		generateCsharpStructImpl(header, root, cs, case_type_name.c_str(), nullptr);
		//csharpDu_generateStructMessage(header, root, cs, case_type_name.c_str(), interface_name.c_str());
	}

	fprintf(header, "} // class %s\n\n", s.name.c_str());
}

void generateCsharpUnionMessage(FILE* header, Root& root, CompositeType& s)
{
	assert(s.isDiscriminatedUnion());

	std::string type_name = getCSharpTypeName(s);


	fprintf(header, "public class %s_message\n", type_name.c_str());
	fprintf(header, "{\n");

	csharpDu_generateUnionParseJson3(header, s);
	csharpDu_generateUnionParseGmq3(header, s);

	csharpDu_generateUnionComposeJson2(header, root, s, type_name.c_str());
	csharpDu_generateUnionComposeGmq2(header, root, s, type_name.c_str());


	for (auto& duit : s.getDiscriminatedUnionCases())
	{
		assert(duit != nullptr);
		auto& cs = *duit;
		assert(cs.type == CompositeType::Type::discriminated_union_case);
		fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

		std::string case_type_name = getCSharpTypeName(cs);
		std::string interface_name = "I" + type_name;

		generateCsharpStructMessage(header, root, cs, case_type_name.c_str(), interface_name.c_str());
	}


	fprintf(header, "} // class %s_message\n\n", s.name.c_str());
}


//void generateCsharpUnionSubscriber(FILE* header, Root& root, CompositeType& s, const char* type_name)
//{
//	assert(s.type == CompositeType::Type::discriminated_union);
//
//	fprintf(header, "public class %s_subscriber : I%s", type_name, type_name);
//
//	if (s.type == CompositeType::Type::publishable)
//		fprintf(header, ", StateSubscriberBase");
//
//	fprintf(header, "\n{\n");
//
//	fprintf(header, "\n\t/////////////////////////////////  begin user override section /////////////////////////////////\n\n");
//
//	csharpPub_generateFactoryMethodSubs(header, root, s, type_name);
//	csharpPub_generateEventHandlerSubs(header, root, s, type_name);
//
//	fprintf(header, "\n\t/////////////////////////////////   end user override section  /////////////////////////////////\n\n\n");
//
//
//	fprintf(header, "\tprotected I%s data;\n", type_name.c_str());
//
//	csharpPub_generateAddressEnum(header, s);
//
//	if (s.type == CompositeType::Type::publishable)
//		fprintf(header, "\tpublic %s_subscriber() { this.data = new %s(); }\n", type_name.c_str(), type_name.c_str());
//	else
//		fprintf(header, "\tpublic %s_subscriber(I%s data) { this.data = data; }\n", type_name.c_str(), type_name.c_str());
//
//
//	auto& mem = s.getMembers();
//	for (size_t i = 0; i < mem.size(); ++i)
//	{
//		auto& it = mem[i];
//		assert(it != nullptr);
//		auto& member = *it;
//		switch (member.type.kind)
//		{
//		case MessageParameterType::KIND::INTEGER:
//		case MessageParameterType::KIND::UINTEGER:
//		case MessageParameterType::KIND::REAL:
//		case MessageParameterType::KIND::CHARACTER_STRING:
//		{
//			const char* type_name = csharpPub_getCSharpType(member.type.kind);
//			fprintf(header, "\tpublic %s %s\n", type_name, member.name.c_str());
//			fprintf(header, "\t{\n");
//			fprintf(header, "\t\tget { return data.%s; }\n", member.name.c_str());
//			fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
//			fprintf(header, "\t}\n");
//			fprintf(header, "\tbool update_%s(%s newVal)\n", member.name.c_str(), csharpPub_getCSharpType(member.type.kind));
//			fprintf(header, "\t{\n");
//			fprintf(header, "\t\tif (newVal != data.%s)\n", member.name.c_str());
//			fprintf(header, "\t\t{\n");
//			fprintf(header, "\t\t\t%s oldVal = data.%s;\n", type_name, member.name.c_str());
//			fprintf(header, "\t\t\tdata.%s = newVal;\n", member.name.c_str());
//			fprintf(header, "\t\t\tnotifyUpdated_%s(oldVal);\n", member.name.c_str());
//			fprintf(header, "\t\t\treturn true;\n");
//			fprintf(header, "\t\t}\n");
//			fprintf(header, "\t\telse\n");
//			fprintf(header, "\t\t\treturn false;\n");
//			fprintf(header, "\t}\n");
//			break;
//		}
//		case MessageParameterType::KIND::STRUCT:
//		case MessageParameterType::KIND::DISCRIMINATED_UNION:
//			fprintf(header, "\t%s_subscriber %s_handler;\n", member.type.name.c_str(), member.name.c_str());
//			fprintf(header, "\t%s_subscriber lazy_%s_handler()\n", member.type.name.c_str(), member.name.c_str());
//			fprintf(header, "\t{ // mb: lazy because can't call virtual 'makeHandler' in ctor\n");
//			fprintf(header, "\t\tif (%s_handler == null)\n", member.name.c_str());
//			fprintf(header, "\t\t\t%s_handler = makeHandler_%s(data.%s);\n", member.name.c_str(), member.name.c_str(), member.name.c_str());
//			fprintf(header, "\t\treturn %s_handler;\n", member.name.c_str());
//			fprintf(header, "\t}\n");
//
//
//			fprintf(header, "\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());
//			fprintf(header, "\t{\n");
//			fprintf(header, "\t\tget { return lazy_%s_handler(); }\n", member.name.c_str());
//			fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
//			fprintf(header, "\t}\n");
//			break;
//		case MessageParameterType::KIND::VECTOR:
//		{
//			switch (member.type.vectorElemKind)
//			{
//			case MessageParameterType::KIND::INTEGER:
//			case MessageParameterType::KIND::UINTEGER:
//			case MessageParameterType::KIND::REAL:
//			case MessageParameterType::KIND::CHARACTER_STRING:
//			{
//				const char* elem_type_name = csharpPub_getCSharpType(member.type.vectorElemKind);
//				fprintf(header, "\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
//				fprintf(header, "\t{\n");
//				fprintf(header, "\t\tget { return new SubscriberVectorWrapper<%s, %s>(data.%s); }\n", elem_type_name, elem_type_name, member.name.c_str());
//				fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
//				fprintf(header, "\t}\n");
//				break;
//			}
//			case MessageParameterType::KIND::STRUCT:
//			case MessageParameterType::KIND::DISCRIMINATED_UNION:
//			{
//				assert(member.type.structIdx < root.structs.size());
//				const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();
//				fprintf(header, "\tList<%s_subscriber> %s_handlers;\n", elem_type_name, member.name.c_str());
//				fprintf(header, "\tList<%s_subscriber> lazy_%s_handlers()\n", elem_type_name, member.name.c_str());
//				fprintf(header, "\t{ // mb: lazy because can't call virtual 'makeElementHandler' in ctor\n");
//				fprintf(header, "\t\tif (%s_handlers == null)\n", member.name.c_str());
//				fprintf(header, "\t\t{\n");
//				fprintf(header, "\t\t\t%s_handlers = new List<%s_subscriber>();\n", member.name.c_str(), elem_type_name);
//				fprintf(header, "\t\t\tfor (int i = 0; i < data.%s.Count; ++i)\n", member.name.c_str());
//				fprintf(header, "\t\t\t{\n");
//				fprintf(header, "\t\t\t\t%s_subscriber handler = makeElementHandler_%s(data.%s[i]);\n", elem_type_name, member.name.c_str(), member.name.c_str());
//				fprintf(header, "\t\t\t\t%s_handlers.Add(handler);\n", member.name.c_str());
//				fprintf(header, "\t\t\t}\n");
//				fprintf(header, "\t\t}\n");
//				fprintf(header, "\t\treturn %s_handlers;\n", member.name.c_str());
//				fprintf(header, "\t}\n");
//				fprintf(header, "\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
//				fprintf(header, "\t{\n");
//				fprintf(header, "\t\tget { return new SubscriberVectorWrapper<I%s, %s_subscriber>(lazy_%s_handlers()); }\n", elem_type_name, elem_type_name, member.name.c_str());
//				fprintf(header, "\t\tset { throw new InvalidOperationException(); }\n");
//				fprintf(header, "\t}\n");
//				break;
//			}
//			default:
//				assert(false); // not implemented (yet)
//			}
//			break;
//		}
//		default:
//			assert(false); // not implemented (yet)
//		}
//	}
//
//	std::string interface_name = "I" + type_name;
//	generateCsharpStructEquivalentMethod(header, root, s, interface_name.c_str());
//
//	csharpPub_generateParseStateSync(header, root, s, type_name);
//	csharpPub_generateParse1(header, root, s, type_name);
//	csharpPub_generateParse2(header, root, s, type_name);
//
//	if (s.type == CompositeType::Type::publishable)
//		csharpPub_generateStateSubscriberBase(header, s, type_name);
//
//	csharpPub_generateDebugMethodSubs(header, root, s, type_name);
//
//	fprintf(header, "} // class %s_subscriber\n\n", type_name.c_str());
//}
