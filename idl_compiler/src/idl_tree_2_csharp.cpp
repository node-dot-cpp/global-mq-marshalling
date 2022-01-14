/* -------------------------------------------------------------------------------
* Copyright (c) 2020, OLogN Technologies AG
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

	std::string csharpMsg_getTypeName(CompositeType& s)
	{
		if (s.type == CompositeType::Type::discriminated_union_case)
			return fmt::format("{}_{}", s.type2string(), s.name);
		else
			return s.name;
	}

	void csharpMsg_generateScopeEnum(FILE* header, Scope& scope)
	{
		fprintf(header, "public enum MsgIds { ");
		for (auto msg = scope.objectList.begin(); msg != scope.objectList.end(); ++msg)
		{
			if(msg != scope.objectList.begin())
				fprintf(header, ", ");

			fprintf(header, "%s = %lld", (*msg)->name.c_str(), (*msg)->numID);
		}
		fprintf(header, " }\n");
	}


	void csharpMsg_generateComposeParamTypeLIst(FILE* header, CompositeType& s)
	{
		int count = 0;
		auto& mem = s.getMembers();
		for (auto it = mem.begin(); it != mem.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if (it != mem.begin())
				fprintf(header, ", ");

			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "Int64 %s", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "UInt64 %s", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "Double %s", param.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "String %s", param.name.c_str());
				break;
			//case MessageParameterType::KIND::BYTE_ARRAY:
			//case MessageParameterType::KIND::BLOB:
			//case MessageParameterType::KIND::ENUM:
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "ICollectionCompose %s", param.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "IMessageCompose %s", param.name.c_str());
				break;
			default:
				assert(false); // unexpected
				break;
			}
		}
	}

	void csharpMsg_generateParseParamTypeLIst(FILE* header, CompositeType& s)
	{
		int count = 0;
		auto& mem = s.getMembers();
		for (auto it = mem.begin(); it != mem.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if (it != mem.begin())
				fprintf(header, ", ");


			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "ref Int64 %s", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "ref UInt64 %s", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "ref Double %s", param.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "ref String %s", param.name.c_str());
				break;
			//case MessageParameterType::KIND::BYTE_ARRAY:
			//case MessageParameterType::KIND::BLOB:
			//case MessageParameterType::KIND::ENUM:
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "ICollectionParse %s", param.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "IMessageParse %s", param.name.c_str());
				break;
			default:
				assert(false); // unexpected
				break;
			}
		}
	}

	void csharpMsg_generateCallerParamTypeLIst(FILE* header, CompositeType& s, bool useRef)
	{
		int count = 0;
		auto& mem = s.getMembers();
		for (auto it = mem.begin(); it != mem.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if (it != mem.begin())
				fprintf(header, ", ");

			using KIND = MessageParameterType::KIND;
			switch (param.type.kind)
			{
			case KIND::INTEGER:
			case KIND::UINTEGER:
			case KIND::REAL:
			case KIND::CHARACTER_STRING:

				if (useRef)
					fprintf(header, "ref ");

				fprintf(header, "%s", param.name.c_str());
				break;
			default:
				fprintf(header, "%s", param.name.c_str());
				break;
			}
		}
	}

	void csharpMsg_generateStructComposeGmq(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose(GmqComposer composer, ");

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");

		int count = 0;
		auto& mem = s.getMembers();
		for (auto it = mem.begin(); it != mem.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\t\tcomposer.composeSignedInteger(%s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\t\tcomposer.composeUnsignedInteger(%s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
			{
				fprintf(header, "\t\tcomposer.composeReal(%s);\n", param.name.c_str());
				break;
			}
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\tcomposer.composeString(%s);\n", param.name.c_str());
				break;
			//case MessageParameterType::KIND::BYTE_ARRAY:
			//	break;
			//case MessageParameterType::KIND::BLOB:
			//	break;
			//case MessageParameterType::KIND::ENUM:
			//	break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t%s.composeGmq(composer);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: treatment
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t%s.compose(composer);\n", param.name.c_str());
				break; // TODO: treatment
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructParseGmq(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tprotected static void parse(GmqParser parser, ");

		csharpMsg_generateParseParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");

		int count = 0;
		for (auto& it : s.getMembers())
		{
			assert(it != nullptr);

			MessageParameter& param = *it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\t\tparser.parseSignedInteger(out %s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\t\tparser.parseUnsignedInteger(out %s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "\t\tparser.parseReal(out %s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\tparser.parseString(out %s);\n", param.name.c_str());
				break;
			//case MessageParameterType::KIND::BYTE_ARRAY:
			//	break;
			//case MessageParameterType::KIND::BLOB:
			//	break;
			//case MessageParameterType::KIND::ENUM:
			//	break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t%s.parseGmq(parser);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: treatment
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t%s.parse(parser);\n", param.name.c_str());
				break; // TODO: ...
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructComposeJson(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose(JsonComposer composer, ");

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header, ")\n"
			"\t{\n");

		fprintf(header, "\t\tcomposer.append( \"{\\n  \");\n");
		int count = 0;
		auto& mem = s.getMembers();
		for (auto it = mem.begin(); it != mem.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if(it != mem.begin())
				fprintf(header, "\t\tcomposer.append( \",\\n  \" );\n");


			fprintf(header, "\t\tcomposer.addNamePart(\"%s\");\n", param.name.c_str());
			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\t\tcomposer.composeSignedInteger(%s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\t\tcomposer.composeUnsignedInteger(%s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
			{
				fprintf(header, "\t\tcomposer.composeReal(%s);\n", param.name.c_str());
				break;
			}
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\tcomposer.composeString(%s);\n", param.name.c_str());
				break;
			//case MessageParameterType::KIND::BYTE_ARRAY:
			//	break;
			//case MessageParameterType::KIND::BLOB:
			//	break;
			//case MessageParameterType::KIND::ENUM:
			//	break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t%s.composeJson(composer);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: ...
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t%s.compose(composer);\n", param.name.c_str());
				break; // TODO: ...
			default:
				assert(false); // unexpected
				break;
			}

		}

		fprintf(header, "\t\tcomposer.append( \"\\n}\" );\n");

		fprintf(header, "\t}\n");
	}



	void csharpMsg_generateUnionCaseComposeJson(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose_%s(JsonComposer composer, ", s.name.c_str());

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header, ")\n"
			"\t{\n");

		fprintf(header, "\t\tcomposer.append( \"{\\n  \");\n");
		fprintf(header, "\t\tcomposer.addNamePart(\"caseid\");\n");
		fprintf(header, "\t\tcomposer.composeSignedInteger((Int64)Variants.%s);\n", s.name.c_str());
		fprintf(header, "\t\tcomposer.append( \",\\n  \" );\n");
		fprintf(header, "\t\tcomposer.addNamePart(\"caseData\");\n");

		std::string type_name = csharpMsg_getTypeName(s);
		fprintf(header, "\t\t%s.compose(composer, ", type_name.c_str());
		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");

		fprintf(header, "\t\tcomposer.append( \"\\n}\" );\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateUnionCaseComposeGmq(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose_%s(GmqComposer composer, ", s.name.c_str());

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");

		fprintf(header, "\t\tcomposer.composeSignedInteger((Int64)Variants.%s);\n", s.name.c_str());

		std::string type_name = csharpMsg_getTypeName(s);
		fprintf(header, "\t\t%s.compose(composer, ", type_name.c_str());
		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");

		fprintf(header, "\t}\n");
	}


	void csharpMsg_generateStructParseJson(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tprotected static void parse(JsonParser parser, ");

		csharpMsg_generateParseParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");


		fprintf(header, "\t\tparser.skipDelimiter( \'{\' );\n");
		fprintf(header, "\t\twhile (true)\n");
		fprintf(header, "\t\t{\n");
		fprintf(header, "\t\t\tstring key;\n");
		fprintf(header, "\t\t\tparser.readKeyFromJson( out key );\n");

		int count = 0;
		auto& mem = s.getMembers();
		for (auto it = mem.begin(); it != mem.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			fprintf(header, "\t\t\t%s( key == \"%s\" )\n", count == 1 ? "if " : "else if ", param.name.c_str());

			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\t\t\t\tparser.parseSignedInteger(out %s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\t\t\t\tparser.parseUnsignedInteger(out %s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "\t\t\t\tparser.parseReal(out %s);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\t\t\tparser.parseString(out %s);\n", param.name.c_str());
				break;
			//case MessageParameterType::KIND::BYTE_ARRAY:
			//	break;
			//case MessageParameterType::KIND::BLOB:
			//	break;
			//case MessageParameterType::KIND::ENUM:
			//	break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t\t\t%s.parseJson(parser);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: ...
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\t\t%s.parse(parser);\n", param.name.c_str());
				break; // TODO: ...
			default:
				assert(false); // unexpected
				break;
			}
		}

		//TODO if unknown it should be skipped
		fprintf(header, "\n");

		fprintf(header, "\t\t\tparser.skipSpacesEtc();\n");
		fprintf(header, "\t\t\tif ( parser.isDelimiter( \',\' ) )\n");
		fprintf(header, "\t\t\t{\n");
		fprintf(header, "\t\t\t\tparser.skipDelimiter( \',\' );\n");
		fprintf(header, "\t\t\t\tcontinue;\n");
		fprintf(header, "\t\t\t}\n");
		fprintf(header, "\t\t\tif ( parser.isDelimiter( \'}\' ) )\n");
		fprintf(header, "\t\t\t{\n");
		fprintf(header, "\t\t\t\tparser.skipDelimiter( \'}\' );\n");
		fprintf(header, "\t\t\t\tbreak;\n");
		fprintf(header, "\t\t\t}\n");
		fprintf(header, "\t\t\tthrow new FormatException(); // bad format\n");

		fprintf(header, "\t\t}\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateUnionParseJson(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string type_name = csharpMsg_getTypeName(s);

		fprintf(header, "\tprotected static %s parse(JsonParser parser)\n", type_name.c_str());

		fprintf(header, "\t{\n");
		fprintf(header, "\t\t%s val = new %s();\n\n", type_name.c_str(), type_name.c_str());

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

		fprintf(header, "\t\tswitch ((Variants)caseID)\n");
		fprintf(header, "\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = csharpMsg_getTypeName(cs);

			fprintf(header, "\t\t\tcase Variants.%s: val.mem = %s.parse(parser); break;\n",
				cs.name.c_str(), case_name.c_str());

		}

		fprintf(header, "\t\t\tdefault: throw new System.Exception();\n");
		fprintf(header, "\t\t}\n\n");

		fprintf(header, "\t\tparser.skipDelimiter('}');\n\n");
		
		fprintf(header, "\t\treturn val;\n");
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateUnionParseGmq(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string type_name = csharpMsg_getTypeName(s);

		fprintf(header, "\tprotected static %s parse(GmqParser parser)\n", type_name.c_str());

		fprintf(header, "\t{\n");
		fprintf(header, "\t\t%s val = new %s();\n\n", type_name.c_str(), type_name.c_str());


		fprintf(header, "\t\tInt64 caseID;\n");
		fprintf(header, "\t\tparser.parseSignedInteger(out caseID);\n\n");

		fprintf(header, "\t\tswitch ((Variants)caseID)\n");
		fprintf(header, "\t\t{\n");

		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;

			std::string case_name = csharpMsg_getTypeName(cs);

			fprintf(header, "\t\t\tcase Variants.%s: val.mem = %s.parse(parser); break;\n",
				cs.name.c_str(), case_name.c_str());

		}

		fprintf(header, "\t\t\tdefault: throw new System.Exception();\n");
		fprintf(header, "\t\t}\n\n");

		fprintf(header, "\t\treturn val;\n");
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructParseBase2(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		std::string name = csharpMsg_getTypeName(s);

		fprintf(header, "\tpublic static %s parse(ParserBase parser)\n", name.c_str());

		fprintf(header,
			"\t{\n"
			"\t\t%s tmp = new %s();\n"
			"\t\tparse(parser,\n", name.c_str(), name.c_str());

		int count = 0;
		auto& mem = s.getMembers();
		for (auto it = mem.begin(); it != mem.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if (it != mem.begin())
				fprintf(header, ",\n");

			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\t\t%s: ref tmp.%s", param.name.c_str(), param.name.c_str());
				break;
			//case MessageParameterType::KIND::BYTE_ARRAY:
			//case MessageParameterType::KIND::BLOB:
			//case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "\t\t\t%s: new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { tmp.%s = new List<Int64>(); },\n", param.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ Int64 val; parser.parseSignedInteger(out val); tmp.%s.Add(val); })", param.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "\t\t\t%s: new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { tmp.%s = new List<UInt64>(); },\n", param.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ UInt64 val; parser.parseUnsignedInteger(out val); tmp.%s.Add(val); })", param.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "\t\t\t%s: new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { tmp.%s = new List<Double>(); },\n", param.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ Double val; parser.parseReal(out val); tmp.%s.Add(val); })", param.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\t\t%s: new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { tmp.%s = new List<String>(); },\n", param.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ String val; parse.parseString(out val); tmp.%s.Add(val); })", param.name.c_str());
					break;
					//case MessageParameterType::KIND::BYTE_ARRAY:
					//case MessageParameterType::KIND::BLOB:
					//case MessageParameterType::KIND::ENUM:
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					//new MessageWrapperForComposing((ComposerBase composer) = > { mtest.STRUCT_point_compose(composer, x: msg.eighthParam.x, y : msg.eighthParam.y); }),
					fprintf(header, "\t\t\t%s: new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { tmp.%s = new List<%s>(); },\n", param.name.c_str(), param.name.c_str(), param.type.name.c_str());
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ %s val = %s.parse(parser); tmp.%s.Add(val); })", param.type.name.c_str(), param.type.name.c_str(), param.name.c_str());
					break;
				default:
					assert(false); // unexpected
					break;
				}
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\t%s: new MessageWrapperForParsing(\n", param.name.c_str());
				fprintf(header, "\t\t\t\t(ParserBase parser) => { tmp.%s = %s.parse(parser); })", param.name.c_str(), param.type.name.c_str());
				break; // TODO: ...
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, 
			"\n\t\t);\n"
			"\t\treturn tmp;\n"
			"\t}\n");
	}


	void csharpMsg_generateStructComposeBase(FILE* header, CompositeType& s, bool add_sufix = false)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		std::string funcName = add_sufix ? "compose_" + s.name : "compose";

		fprintf(header, "\tpublic static void %s(ComposerBase composer, ", funcName.c_str());

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header,	")\n\t{\n");

		fprintf(header,
			"\t\tif (composer is GmqComposer gmqC)\n"
			"\t\t\t%s(gmqC, ", funcName.c_str());

		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");

		fprintf(header,
			"\t\telse if (composer is JsonComposer jsonC)\n"
			"\t\t\t%s(jsonC, ", funcName.c_str());

		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");

		fprintf(header,
			"\t\telse\n"
			"\t\t\tthrow new ArgumentException();\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructParseBase(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tprotected static void parse(ParserBase parser, ");

		csharpMsg_generateParseParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");

		fprintf(header,
			"\t\tif (parser is GmqParser gmqP)\n"
			"\t\t\tparse(gmqP, ");

		csharpMsg_generateCallerParamTypeLIst(header, s, true);
		fprintf(header, ");\n");

		fprintf(header,
			"\t\telse if (parser is JsonParser jsonP)\n"
			"\t\t\tparse(jsonP, ");

		csharpMsg_generateCallerParamTypeLIst(header, s, true);
		fprintf(header, ");\n");

		fprintf(header,
			"\t\telse\n"
			"\t\t\tthrow new ArgumentException();\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateUnionParseBase(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string type_name = csharpMsg_getTypeName(s);

		fprintf(header, "\tpublic static %s parse(ParserBase parser)\n", type_name.c_str());

		fprintf(header,
			"\t{\n"
			"\t\tif (parser is GmqParser gmqP)\n"
			"\t\t\treturn parse(gmqP);\n"
			"\t\telse if (parser is JsonParser jsonP)\n"
			"\t\t\treturn parse(jsonP);\n"
			"\t\telse\n"
			"\t\t\tthrow new ArgumentException();\n"
			"\t}\n"
		);
	}


	void csharpMsg_generateStructMembers(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		//fprintf(header, "struct %s\n", s.name.c_str());
		//fprintf(header, "{\n");

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;
			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\tpublic Int64 %s;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\tpublic UInt64 %s;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "\tpublic Double %s;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\tpublic String %s;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic %s %s;\n", member.type.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "\tpublic List<Int64> %s;\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "\tpublic List<UInt64> %s;\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "\tpublic List<Double> %s;\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\tpublic List<String> %s;\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::VECTOR:
					assert(false); // unexpected
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					assert(member.type.messageIdx < root.structs.size());
					fprintf(header, "\tpublic List<%s> %s;\n", root.structs[member.type.messageIdx]->name.c_str(), member.name.c_str());
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

	void csharpMsg_generateUnionCaseProperties(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union_case);

		std::string type_name = csharpMsg_getTypeName(s);

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;
			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\tpublic Int64 %s\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\tpublic UInt64 %s\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "\tpublic Double %s\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\tpublic String %s\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic %s %s\n", member.type.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "\tpublic List<Int64> %s\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "\tpublic List<UInt64> %s\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "\tpublic List<Double> %s\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\tpublic List<String> %s\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::VECTOR:
					assert(false); // unexpected
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					assert(member.type.messageIdx < root.structs.size());
					fprintf(header, "\tpublic List<%s> %s\n", root.structs[member.type.messageIdx]->name.c_str(), member.name.c_str());
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
				"\t{\n"
				"\t\tget { return ((%s)this.mem).%s; }\n", type_name.c_str(), member.name.c_str());
			fprintf(header,
				"\t\tset { ((%s)this.mem).%s = value; }\n", type_name.c_str(), member.name.c_str());
			fprintf(header,
				"\t}\n");


		}
	}


	void csharpMsg_generateStandardMethods(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union || s.type == CompositeType::Type::discriminated_union_case);

		std::string name = csharpMsg_getTypeName(s);

		fprintf(header, "\tpublic override bool Equals(object obj)\n"
			"\t{\n"
			"\t\treturn Equals(obj as %s);\n"
			"\t}\n", name.c_str());

		fprintf(header, "\tpublic static bool operator ==(%s left, %s right)\n", name.c_str(), name.c_str());
		fprintf(header, 
			"\t{\n"
			"\t\tif (ReferenceEquals(left, right))\n"
			"\t\t\treturn true;\n"
			"\t\telse if (ReferenceEquals(left, null))\n"
			"\t\t\treturn false;\n"
			"\t\telse if (ReferenceEquals(null, right))\n"
			"\t\t\treturn false;\n"
			"\t\telse\n"
			"\t\t\treturn left.Equals(right);\n"
			"\t}\n");

		fprintf(header, "\tpublic static bool operator !=(%s left, %s right)\n"
			"\t{\n"
			"\t\treturn !(left == right);\n"
			"\t}\n", name.c_str(), name.c_str());

		fprintf(header, "\tpublic override int GetHashCode()\n"
			"\t{\n"
			"\t\t// TODO\n"
			"\t\tthrow new InvalidOperationException();\n"
			"\t}\n");

	}

	void csharpMsg_generateStructEqualsMethod(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union_case);

		std::string name = csharpMsg_getTypeName(s);

		fprintf(header, "\tpublic bool Equals(%s other)\n", name.c_str());
		fprintf(header, 
			"\t{\n"
			"\t\tif (ReferenceEquals(this, other))\n"
			"\t\t\treturn true;\n"
			"\t\telse if (ReferenceEquals(null, other))\n"
			"\t\t\treturn false;\n"
			"\t\telse\n"
			"\t\t\treturn\n");

		auto& mem = s.getMembers();
		for (auto it = mem.begin(); it != mem.end(); ++it)
		{
			//auto& it = s.members[i];
			assert(*it != nullptr);
			auto& member = **it;

			if(it != mem.begin())
				fprintf(header, " &&\n");


			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\t\t\tthis.%s == other.%s", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\t\tthis.%s.Equals(other.%s)", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t\t\tEnumerable.SequenceEqual(this.%s, other.%s)", member.name.c_str(), member.name.c_str());
				break;
			default:
				assert(false); // not implemented (yet)
			}
		}
		fprintf(header, ";\n\t}\n");
	}

	void csharpMsg_generateUnionEqualsMethod(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string name = csharpMsg_getTypeName(s);

		fprintf(header, "\tpublic bool Equals(%s other)\n", name.c_str());
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

	void csharpMsg_generateComposeMessageMethod(FILE* header, const std::string& msgName, CompositeType& s, Proto proto)
	{
		// when message is an alias we feed structure here
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure);

		fprintf(header, "\tpublic static void composeMessage_%s(BufferT buffer, ", msgName.c_str());

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");

		if (proto == Proto::gmq)
		{
			fprintf(header,
				"\t\tGmqComposer composer = new GmqComposer(buffer);\n\n"
				"\t\tcomposer.composeUnsignedInteger((UInt64)MsgIds.%s);\n"
				"\t\t%s.compose(composer, ", msgName.c_str(), msgName.c_str());


			csharpMsg_generateCallerParamTypeLIst(header, s, false);
			fprintf(header,");\n");
		}
		else if (proto == Proto::json)
		{
			fprintf(header,
				"\t\tJsonComposer composer = new JsonComposer(buffer);\n\n"
				"\t\tcomposer.append(\"{\\n  \");\n"
				"\t\tcomposer.addNamePart(\"msgid\");\n"
				"\t\tcomposer.composeUnsignedInteger((UInt64)MsgIds.%s);\n"
				"\t\tcomposer.append(\",\\n  \");\n"
				"\t\tcomposer.addNamePart(\"msgbody\");\n"
				"\t\t%s.compose(composer, ", msgName.c_str(), msgName.c_str());

			csharpMsg_generateCallerParamTypeLIst(header, s, false);
			fprintf(header,	");\n"
				"\t\tcomposer.append(\"\\n}\");\n");
		}
		else
		{
			assert(false);
		}

		fprintf(header, "\t}\n\n");
	}

	void csharpMsg_generateStruct(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		//mb: we use this function to create outer and inner classes.
		// i.e. scope class has inner messages classes and
		// discriminated_union class has inner union_cases classes.
		// Inner classes should be indented inside their outer class,
		// but that goes bad in readability of this function, so I prefer to leave it as it is for now.

		bool checked = impl_checkParamNameUniqueness(s);
		checked = impl_checkFollowingExtensionRules(s) && checked;
		if (!checked)
			throw std::exception();

		std::string name = csharpMsg_getTypeName(s);

		if(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure)
			impl_generateMessageCommentBlock(header, s);

		fprintf(header, "public class %s : IEquatable<%s>\n"
			"{\n", name.c_str(), name.c_str());

		csharpMsg_generateStructMembers(header, root, s);

		fprintf(header, "\n");

		csharpMsg_generateStructEqualsMethod(header, s);
		csharpMsg_generateStandardMethods(header, s);
		
		csharpMsg_generateStructComposeBase(header, s);
		csharpMsg_generateStructComposeJson(header, s);
		csharpMsg_generateStructComposeGmq(header, s);

		csharpMsg_generateStructParseBase2(header, s);
		csharpMsg_generateStructParseBase(header, s);
		csharpMsg_generateStructParseJson(header, s);
		csharpMsg_generateStructParseGmq(header, s);

		fprintf(header, "} // class %s\n\n", name.c_str());
	}



	void csharpMsg_generateUnion(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.isDiscriminatedUnion());

		bool checked = impl_checkParamNameUniqueness(s);
		checked = impl_checkFollowingExtensionRules(s) && checked;
		if (!checked)
			throw std::exception();


		impl_generateMessageCommentBlock(header, s);

		// TODO generate structs also
		fprintf(header, "public class %s : IEquatable<%s>\n"
			"{\n", s.name.c_str(), s.name.c_str());

		fprintf(header, "\tpublic enum Variants { ");

		std::string concatenatedNames;

		for (auto& it : s.getDiscriminatedUnionCases())
		{
			assert(it != nullptr);
			CompositeType& cs = *it;
			assert(cs.type == CompositeType::Type::discriminated_union_case);
			fprintf(header, "%s = %lld, ", cs.name.c_str(), cs.numID);

			concatenatedNames += cs.name;
			concatenatedNames += "_";
		}

		fprintf(header, "unknown };\n");

		fprintf(header, "\tObject mem;\n");


		fprintf(header, "\n");

		for (auto& it : s.getDiscriminatedUnionCases())
		{
			assert(it != nullptr);
			CompositeType& cs = *it;
			assert(cs.type == CompositeType::Type::discriminated_union_case);
			csharpMsg_generateStruct(header, root, cs);
		}

		csharpMsg_generateStandardMethods(header, s);
		csharpMsg_generateUnionEqualsMethod(header, s);

		// currentVariant()
		fprintf(header,
			"\tpublic Variants currentVariant()\n"
			"\t{\n"
			"\t\tif(this.mem == null)\n"
			"\t\t\treturn Variants.unknown;\n");

		for (auto& it : s.getDiscriminatedUnionCases())
		{
			assert(it != nullptr);
			CompositeType& cs = *it;
			assert(cs.type == CompositeType::Type::discriminated_union_case);

			std::string type_name = csharpMsg_getTypeName(cs);
			fprintf(header,
				"\t\telse if(this.mem is %s)\n"
				"\t\t\treturn Variants.%s;\n", type_name.c_str(), cs.name.c_str());
		}

		fprintf(header,
			"\t\telse\n"
			"\t\t\treturn Variants.unknown;\n"
			"\t}\n");


		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;
			fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

			csharpMsg_generateUnionCaseProperties(header, root, cs);
		}

		csharpMsg_generateUnionParseBase(header, s);
		csharpMsg_generateUnionParseJson(header, s);
		csharpMsg_generateUnionParseGmq(header, s);
		
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;
			fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

			csharpMsg_generateStructComposeBase(header, cs, true);
			csharpMsg_generateUnionCaseComposeJson(header, cs);
			csharpMsg_generateUnionCaseComposeGmq(header, cs);
		}
		

		fprintf(header, "} // class %s\n\n", s.name.c_str());
	}

	void csharpMsg_generateMessageAliasMethods(FILE* header, CompositeType& s, CompositeType& target, const char* composerType, const char* parserType)
	{

		fprintf(header, "\tpublic new static void compose(%s composer, ", composerType);
		csharpMsg_generateComposeParamTypeLIst(header, target);
		fprintf(header, ")\n\t{\n");

		fprintf(header, "\t\t%s.compose(composer, ", target.name.c_str());
		csharpMsg_generateCallerParamTypeLIst(header, target, false);
		fprintf(header, ");\n");


		fprintf(header, "\t}\n");

		// parse function
		fprintf(header, "\tprotected new static void parse(%s parser, ", parserType);
		csharpMsg_generateParseParamTypeLIst(header, target);
		fprintf(header, ")\n\t{\n");


		fprintf(header, "\t\t%s.parse(parser, ", target.name.c_str());
		csharpMsg_generateCallerParamTypeLIst(header, target, true);
		fprintf(header, ");\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateMessageAlias(FILE* header, CompositeType& s, CompositeType& target)
	{
		bool checked = impl_checkFollowingExtensionRules(s);
		if (!checked)
			throw std::exception();

		impl_generateMessageCommentBlock(header, s);

		fprintf(header, "public class %s : %s\n"
			"{\n", s.name.c_str(), target.name.c_str());

		csharpMsg_generateMessageAliasMethods(header, s, target, "ComposerBase", "ParserBase");
		csharpMsg_generateMessageAliasMethods(header, s, target, "GmqComposer", "GmqParser");
		csharpMsg_generateMessageAliasMethods(header, s, target, "JsonComposer", "JsonParser");

		fprintf(header, "} // class %s\n\n", s.name.c_str());
	}

	void csharpMsg_generateScopeHandler(FILE* header, Scope& scope)
	{
		fprintf(header,
			"\tpublic static void handleMessage( BufferT buffer, MessageHandlerArray handlers )\n"
			"\t{\n"
			"\t\thandleMessage(buffer.getReadIterator(), handlers);\n"
			"\t}\n");


		fprintf(header,
			"\tpublic static void handleMessage( ReadIteratorT riter, MessageHandlerArray handlers )\n"
			"\t{\n"
		);
		switch (scope.proto)
		{
		case Proto::gmq:
			fprintf(header,
				"\t\tGmqParser parser = new GmqParser( riter );\n"
				"\t\thandlers.gmq_handle(parser);\n"
			);
			break;
		case Proto::json:
			fprintf(header,
				"\t\tJsonParser parser = new JsonParser( riter );\n"
				"\t\thandlers.json_handle(parser);\n"
			);
			break;
		default:
			assert(false);
		}
		fprintf(header, "\t}\n");
	}

	
	void csharpMsg_generateScopeComposer(FILE* header, Scope& scope)
	{
		fprintf(header,
			"\tpublic static void composeMessage( BufferT buffer, MessageHandlerArray handlers )\n"
			"\t{\n"
			"\t\tReadIteratorT riter = buffer.getReadIterator();\n"
			"\t\thandleMessage(riter, handlers);\n"
			"\t}\n");


		fprintf(header,
			"\tpublic static void handleMessage( ReadIteratorT riter, MessageHandlerArray handlers )\n"
			"\t{\n"
		);
		switch (scope.proto)
		{
		case Proto::gmq:
			fprintf(header,
				"\t\tGmqParser parser = new GmqParser( riter );\n"
				"\t\thandlers.gmq_handle(parser);\n"
			);
			break;
		case Proto::json:
			fprintf(header,
				"\t\tJsonParser parser = new JsonParser( riter );\n"
				"\t\thandlers.json_handle(parser);\n"
			);
			break;
		default:
			assert(false);
		}
		fprintf(header, "\t}\n");
	}
}


void generateCsharp( const char* fileName, uint32_t fileChecksum, FILE* header, const std::string& metascope, const std::string& platformPrefix, const std::string& classNotifierName, Root& s )
{
	fprintf(header, "//////////////////////////////////////////////////////////////\n");
	fprintf(header, "//\n");
	fprintf(header, "//  Do not edit! file automatically generated by idl_compiler\n");
	fprintf(header, "//\n");
	fprintf(header, "//////////////////////////////////////////////////////////////\n\n");

	fprintf( header,
		"using globalmq.marshalling;\n"
		"using System;\n"
		"using System.Collections.Generic;\n"
		"using System.Diagnostics;\n"
		"using System.Linq;\n"
		"\n"
		"namespace %s\n"
		"{\n"
		"\n"
		, metascope.c_str());


	vector<CompositeType*> structsOrderedByDependency;
	orderStructsByDependency( s.structs, structsOrderedByDependency );

	fprintf(header, "//////////////////////////////////////////////////////////////\n");
	fprintf(header, "//\n");
	fprintf(header, "//                 Messages\n");
	fprintf(header, "//\n");
	fprintf(header, "//////////////////////////////////////////////////////////////\n\n");

	for (auto& it : structsOrderedByDependency)
	{
		assert(it != nullptr);
		assert(typeid(*(it)) == typeid(CompositeType));

		if (it->isStruct4Messaging)
		{
			if (it->type == CompositeType::Type::structure)
				csharpMsg_generateStruct(header, s, *it);
			else if (it->type == CompositeType::Type::discriminated_union)
				csharpMsg_generateUnion(header, s, *it);
			else
				assert(false);
		}
	}

	impl_insertScopeList(header, s);


	for ( auto& scope : s.scopes )
	{
		fprintf( header, "public class %s\n{\n", scope->name.c_str() );

		csharpMsg_generateScopeEnum(header, *scope);
		fprintf(header, "\n");
		csharpMsg_generateScopeHandler( header, *scope );
		fprintf(header, "\n");

		for ( auto it : scope->objectList )
		{
			assert( it != nullptr );
			assert( it->type == CompositeType::Type::message );
			if (!it->isAlias)
			{
				csharpMsg_generateStruct(header, s, *it);
				csharpMsg_generateComposeMessageMethod(header, it->name, *it, scope->proto);
			}
			else
			{
				assert(it->aliasIdx < s.structs.size());
				auto& alias = s.structs[it->aliasIdx];
				csharpMsg_generateMessageAlias(header, *it, *alias);
				csharpMsg_generateComposeMessageMethod(header, it->name, *alias, scope->proto);
			}

		}

		fprintf(header, "} // class %s\n\n", scope->name.c_str());
	}


	// temporary remove
	//generateCsharpPublishables(fileName, fileChecksum, header, metascope, platformPrefix, classNotifierName, s);

	fprintf( header, "\n} // namespace %s\n\n",	metascope.c_str() );
}

