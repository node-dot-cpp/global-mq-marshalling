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

#include "idl_tree_serializer.h"
#include "raiistdiofile.h"

#include <set>



namespace {

	// Message generation
	std::string csharpMsg_MessageNameToDefaultsNamespaceName(const std::string& name)
	{
		return fmt::format("Message_{}_defaults", name);
	}

	std::string csharpMsg_generateComposeFunctionName(CompositeType& s)
	{
		return fmt::format("{}_{}_compose", s.type2string(), s.name);
	}

	std::string csharpMsg_generateParseFunctionName(CompositeType& s)
	{
		return fmt::format("{}_{}_parse", s.type2string(), s.name);
	}

	CompositeType* csharpMsg_findByName(const std::vector<unique_ptr<CompositeType>>& objectList, const std::string& name)
	{
		for (const auto& it : objectList)
		{
			assert(it);
			if (it->name == name)
			{
				return it.get();
			}
		}
		assert(false);
		return nullptr;
	}

	void csharpMsg_generateScopeEnum(FILE* header, Scope& scope)
	{
		fprintf(header, "public enum MessageName  { ");
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
		for (auto it = s.members.begin(); it != s.members.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if (it != s.members.begin())
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
		for (auto it = s.members.begin(); it != s.members.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if (it != s.members.begin())
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
		for (auto it = s.members.begin(); it != s.members.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if (it != s.members.begin())
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

	void csharpMsg_generateGmqComposeMethod(FILE* header, CompositeType& s)
	{
		fprintf(header, "\tpublic static void compose(GmqComposer composer, ");

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");

		int count = 0;
		for (auto it = s.members.begin(); it != s.members.end(); ++it)
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
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t%s.composeGmq(composer);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: treatment
			case MessageParameterType::KIND::STRUCT:
				fprintf(header, "\t\t%s.compose(composer);\n", param.name.c_str());
				break; // TODO: treatment
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateGmqParseMethod(FILE* header, CompositeType& s)
	{

		fprintf(header, "\tpublic static void parse(GmqParser parser, ");

		csharpMsg_generateParseParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");

		int count = 0;
		for (auto& it : s.members)
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
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t%s.parseGmq(parser);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: treatment
			case MessageParameterType::KIND::STRUCT:
				fprintf(header, "\t\t%s.parse(parser);\n", param.name.c_str());
				break; // TODO: ...
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateJsonComposeMethod(FILE* header, CompositeType& s)
	{
		fprintf(header, "\tpublic static void compose(JsonComposer composer, ");

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header, ")\n"
			"\t{\n");

		fprintf(header, "\t\tcomposer.append( \"{\\n  \");\n");
		int count = 0;
		for (auto it = s.members.begin(); it != s.members.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if(it != s.members.begin())
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
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t%s.composeJson(composer);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: ...
			case MessageParameterType::KIND::STRUCT:
				fprintf(header, "\t\t%s.compose(composer);\n", param.name.c_str());
				break; // TODO: ...
			default:
			{
				assert(false); // unexpected
				break;
			}
			}

		}

		fprintf(header, "\t\tcomposer.append( \"\\n}\" );\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateJsonParseMethod(FILE* header, CompositeType& s)
	{
		fprintf(header, "\tpublic static void parse(JsonParser parser, ");

		csharpMsg_generateParseParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");


		fprintf(header, "\t\tparser.skipDelimiter( \'{\' );\n");
		fprintf(header, "\t\twhile (true)\n");
		fprintf(header, "\t\t{\n");
		fprintf(header, "\t\t\tstring key;\n");
		fprintf(header, "\t\t\tparser.readKeyFromJson( out key );\n");

		int count = 0;
		for (auto it = s.members.begin(); it != s.members.end(); ++it)
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
			case MessageParameterType::KIND::BYTE_ARRAY:
				break;
			case MessageParameterType::KIND::BLOB:
				break;
			case MessageParameterType::KIND::ENUM:
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t\t\t%s.parseJson(parser);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: ...
			case MessageParameterType::KIND::STRUCT:
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

	void csharpMsg_generateComposeMethod2(FILE* header, CompositeType& s)
	{
		fprintf(header, "\tpublic static void compose(ComposerBase composer, %s msg)\n", s.name.c_str());

		fprintf(header,
			"\t{\n"
			"\t\tcompose(composer,\n");
		//csharpMsg_generateParamCallBlockForComposingGmq(header, s, "\t");

		int count = 0;
		for (auto it = s.members.begin(); it != s.members.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if(it != s.members.begin())
				fprintf(header, ",\n");

			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\t\t%s: msg.%s", param.name.c_str(), param.name.c_str());
				break;
			//case MessageParameterType::KIND::BYTE_ARRAY:
			//case MessageParameterType::KIND::BLOB:
			//case MessageParameterType::KIND::ENUM:
				//break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\t\t%s: SimpleTypeCollection.makeComposer(msg.%s)", param.name.c_str(), param.name.c_str());
					break;
				//case MessageParameterType::KIND::BYTE_ARRAY:
				//case MessageParameterType::KIND::BLOB:
				//case MessageParameterType::KIND::ENUM:
				case MessageParameterType::KIND::STRUCT:
					//new MessageWrapperForComposing((ComposerBase composer) = > { mtest.STRUCT_point_compose(composer, x: msg.eighthParam.x, y : msg.eighthParam.y); }),
					fprintf(header, "\t\t\t%s: new CollectionWrapperForComposing(\n"
						"\t\t\t\t() => { return msg.%s.Count; },\n", param.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t\t\t(ComposerBase composer, int ordinal) => "
						"{ %s.compose(composer, msg.%s[ordinal]); })", param.type.name.c_str(), param.name.c_str());
					break;
				default:
					assert(false); // unexpected
					break;
				}
				break;
			case MessageParameterType::KIND::STRUCT:
				//new MessageWrapperForComposing((ComposerBase composer) = > { mtest.STRUCT_point_compose(composer, x: msg.eighthParam.x, y : msg.eighthParam.y); }),
				fprintf(header, "\t\t\t%s: new MessageWrapperForComposing(\n", param.name.c_str());
				fprintf(header, "\t\t\t\t(ComposerBase composer) => "
					"{ %s.compose(composer, msg.%s); })", param.type.name.c_str(), param.name.c_str());
				break;
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, "\n\t\t);\n\t}\n");
	}

	void csharpMsg_generateParseMethod2(FILE* header, CompositeType& s)
	{
		fprintf(header, "\tpublic static void parse(ParserBase parser, out %s msg)\n", s.name.c_str());

		//mb: C# has a rule about ref/out params and lambas, so we need to create a tmp here
		// and assign to msg later
		fprintf(header,
			"\t{\n"
			"\t\t%s tmp = new %s();\n"
			"\t\tparse(parser,\n", s.name.c_str(), s.name.c_str());

		int count = 0;
		for (auto it = s.members.begin(); it != s.members.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if (it != s.members.begin())
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
					//new MessageWrapperForComposing((ComposerBase composer) = > { mtest.STRUCT_point_compose(composer, x: msg.eighthParam.x, y : msg.eighthParam.y); }),
					fprintf(header, "\t\t\t%s: new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { tmp.%s = new List<%s>(); },\n", param.name.c_str(), param.name.c_str(), param.type.name.c_str());
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ %s val; %s.parse(parser, out val); tmp.%s.Add(val); })", param.type.name.c_str(), param.type.name.c_str(), param.name.c_str());
					break;
				default:
					assert(false); // unexpected
					break;
				}
				break;
			case MessageParameterType::KIND::STRUCT:
				fprintf(header, "\t\t\t%s: new MessageWrapperForParsing(\n", param.name.c_str());
				fprintf(header, "\t\t\t\t(ParserBase parser) => { %s.parse(parser, out tmp.%s); })", param.type.name.c_str(), param.name.c_str());
				break; // TODO: ...
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, 
			"\n\t\t);\n"
			"\t\tmsg = tmp;\n"
			"\t}\n");
	}


	void csharpMsg_generateComposeFunction(FILE* header, CompositeType& s)
	{
		//std::string funcName = csharpMsg_generateComposeFunctionName(s);

		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure);
		fprintf(header, "\tpublic static void compose(ComposerBase composer, ");

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header,	")\n\t{\n");

		fprintf(header,
			"\t\tif (composer is GmqComposer gmqC)\n"
			"\t\t\tcompose(gmqC, ");

		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");

		fprintf(header,
			"\t\telse if (composer is JsonComposer jsonC)\n"
			"\t\t\tcompose(jsonC, ");

		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");

		fprintf(header,
			"\t\telse\n"
			"\t\t\tthrow new ArgumentException();\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateParseFunction(FILE* header, CompositeType& s)
	{
		//std::string funcName = csharpMsg_generateParseFunctionName(s);

		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure);
		fprintf(header, "\tpublic static void parse(ParserBase parser, ");

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

	void csharpMsg_generateMembers(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure);

		//fprintf(header, "struct %s\n", s.name.c_str());
		//fprintf(header, "{\n");

		for (size_t i = 0; i < s.members.size(); ++i)
		{
			auto& it = s.members[i];
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
				fprintf(header, "\tpublic %s %s;\n", member.type.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "\tpublic IList<Int64> %s;\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "\tpublic IList<UInt64> %s;\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "\tpublic IList<Double> %s;\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\tpublic IList<String> %s;\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::VECTOR:
					assert(false); // unexpected
					break;
				case MessageParameterType::KIND::STRUCT:
					assert(member.type.messageIdx < root.structs.size());
					fprintf(header, "\tpublic IList<%s> %s;\n", root.structs[member.type.messageIdx]->name.c_str(), member.name.c_str());
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

	void csharpMsg_generateStandardMethods(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure);

		fprintf(header, "\tpublic override bool Equals(object obj)\n"
			"\t{\n"
			"\t\treturn obj is %s d && Equals(d);\n"
			"\t}\n", s.name.c_str());

		fprintf(header, "\tpublic static bool operator ==(%s left, %s right)\n"
			"\t{\n"
			"\t\treturn left.Equals(right);\n"
			"\t}\n", s.name.c_str(), s.name.c_str());
		fprintf(header, "\tpublic static bool operator !=(%s left, %s right)\n"
			"\t{\n"
			"\t\treturn !(left == right);\n"
			"\t}\n", s.name.c_str(), s.name.c_str());

		fprintf(header, "\tpublic override int GetHashCode()\n"
			"\t{\n"
			"\t\t// TODO\n"
			"\t\tthrow new InvalidOperationException();\n"
			"\t}\n");

		fprintf(header, "\tpublic bool Equals(%s other)\n"
			"\t{\n"
			"\t\treturn\n", s.name.c_str());

		for (auto it = s.members.begin(); it != s.members.end(); ++it)
		{
			//auto& it = s.members[i];
			assert(*it != nullptr);
			auto& member = **it;

			if(it != s.members.begin())
				fprintf(header, " &&\n");


			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\t\tthis.%s == other.%s", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
				fprintf(header, "\t\t\tthis.%s.Equals(other.%s)", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t\tEnumerable.SequenceEqual(this.%s, other.%s)", member.name.c_str(), member.name.c_str());
				break;
			default:
				assert(false); // not implemented (yet)
			}
		}
		fprintf(header, ";\n\t}\n");
	}

	void csharpMsg_generateComposeMessageMethod(FILE* header, CompositeType& s)
	{
		//mb: here we process messages and struct alias only

		fprintf(header, "\tpublic static void composeMessage(ComposerBase composer, ");

		csharpMsg_generateComposeParamTypeLIst(header, s);

		fprintf(header, ")\n\t{\n");

		fprintf(header,
			"\t\tif (composer is GmqComposer gmqC)\n"
			"\t\t{\n"
			"\t\t\tgmqC.composeUnsignedInteger(MsgId);\n"
			"\t\t\tcompose(gmqC, ");


		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header,
			");\n"
			"\t\t}\n");


		fprintf(header,
			"\t\telse if (composer is JsonComposer jsonC)\n"
			"\t\t{\n"
			"\t\t\tjsonC.append(\"{\\n  \");\n"
			"\t\t\tjsonC.addNamePart(\"msgid\");\n"
			"\t\t\tjsonC.composeUnsignedInteger(MsgId);\n"
			"\t\t\tjsonC.append(\",\\n  \");\n"
			"\t\t\tjsonC.addNamePart(\"msgbody\");\n"
			"\t\t\tcompose(jsonC, ");

		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header,
			");\n"
			"\t\t\tjsonC.append(\"\\n}\");\n"
			"\t\t}\n");

		fprintf(header,
			"\t\telse\n"
			"\t\t\tthrow new ArgumentException();\n");

		fprintf(header, "\t}\n");
	}


	void csharpMsg_generateMessage(FILE* header, Root& root, CompositeType& s)
	{
		bool checked = impl_checkParamNameUniqueness(s);
		checked = impl_checkFollowingExtensionRules(s) && checked;
		if (!checked)
			throw std::exception();

		impl_generateMessageCommentBlock(header, s);

		// TODO generate structs also
		fprintf(header, "public class %s : IEquatable<%s>\n"
			"{\n", s.name.c_str(), s.name.c_str());

		if (s.type == CompositeType::Type::message)
			fprintf(header, "\tpublic static UInt64 MsgId = %lld;\n\n", s.numID);

		csharpMsg_generateMembers(header, root, s);

		fprintf(header, "\n");

		csharpMsg_generateStandardMethods(header, s);
		
		csharpMsg_generateComposeFunction(header, s);
		csharpMsg_generateJsonComposeMethod(header, s);
		csharpMsg_generateGmqComposeMethod(header, s);

		csharpMsg_generateParseFunction(header, s);
		csharpMsg_generateJsonParseMethod(header, s);
		csharpMsg_generateGmqParseMethod(header, s);

		csharpMsg_generateComposeMethod2(header, s);
		csharpMsg_generateParseMethod2(header, s);

		if(s.type == CompositeType::Type::message)
			csharpMsg_generateComposeMessageMethod(header, s);


		fprintf(header, "} // class %s\n\n", s.name.c_str());
	}

	void csharpMsg_generateMessageAlias(FILE* header, CompositeType& s, CompositeType& target, const char* composerType, const char* parserType)
	{
		// compose function
		//std::string compName = csharpMsg_generateComposeFunctionName(s);
		fprintf(header, "\tpublic new static void compose(%s composer, ", composerType);
		csharpMsg_generateComposeParamTypeLIst(header, target);
		fprintf(header, ")\n\t{\n");

		//std::string targetCompName = csharpMsg_generateComposeFunctionName(target);
		fprintf(header, "\t\t%s.compose(composer, ", target.name.c_str());
		csharpMsg_generateCallerParamTypeLIst(header, target, false);
		fprintf(header, ");\n");


		fprintf(header, "\t}\n");

		// parse function
		//std::string parseName = csharpMsg_generateParseFunctionName(s);
		fprintf(header, "\tpublic new static void parse(%s parser, ", parserType);
		csharpMsg_generateParseParamTypeLIst(header, target);
		fprintf(header, ")\n\t{\n");


		//std::string targetParseName = csharpMsg_generateParseFunctionName(target);
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

		fprintf(header, "public static UInt64 MsgId = %lld;\n", s.numID);

		csharpMsg_generateMessageAlias(header, s, target, "ComposerBase", "ParserBase");
		csharpMsg_generateMessageAlias(header, s, target, "GmqComposer", "GmqParser");
		csharpMsg_generateMessageAlias(header, s, target, "JsonComposer", "JsonParser");

		csharpMsg_generateComposeMessageMethod(header, target);

		fprintf(header, "} // class %s\n\n", s.name.c_str());
	}

	void csharpMsg_generateScopeHandler(FILE* header, Scope& scope)
	{
		fprintf(header,
			"\tpublic static void handleMessage( BufferT buffer, MessageHandlerArray handlers )\n"
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
				"\t\thandlers.handleGmq(parser);\n"
			);
			break;
		case Proto::json:
			fprintf(header,
				"\t\tJsonParser parser = new JsonParser( riter );\n"
				"\t\thandlers.handleJson(parser);\n"
			);
			break;
		default:
			assert(false);
		}
		fprintf(header, "\t}\n");
	}


	void csharpMsg_orderStructsByDependency(vector<unique_ptr<CompositeType>>& structs, vector<CompositeType*>& out)
	{
		size_t processed = 0;
		vector<CompositeType*> tmpStructs;
		while (processed < structs.size())
		{
			for (auto& s : structs)
				if (s->dependsOnCnt != -1)
					for (auto& member : s->members)
						if (member->type.kind == MessageParameterType::KIND::STRUCT)
							structs[member->type.messageIdx]->dependsOnCnt = 1;
			for (auto& s : structs)
				if (s->dependsOnCnt == 0)
				{
					tmpStructs.push_back(s.get());
					s->dependsOnCnt = -1;
					++processed;
				}
			for (auto& s : structs)
				if (s->dependsOnCnt != -1)
					s->dependsOnCnt = 0;
		}
		for (size_t i = 0; i < tmpStructs.size(); ++i)
			out.push_back(tmpStructs[tmpStructs.size() - 1 - i]);
		assert(out.size() == structs.size());
	
		for (auto& s : structs)
			s->dependsOnCnt = 0;
	}

}




void generateCsharp( const char* fileName, uint32_t fileChecksum, FILE* header, const char* metascope, std::string platformPrefix, std::string classNotifierName, Root& s )
{
	//std::set<string> msgParams;
	//impl_CollectMessageParamNamesFromRoot( msgParams, s );

	//std::set<string> publishableMembers;
	//impl_CollectPublishableMemberNamesFromRoot( publishableMembers, s );

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
		, metascope);

	//addLibAliasingBlock( header );

	//generateMessageParamNameBlock( header, msgParams );
	//generatePublishableMemberNameBlock( header, publishableMembers );
	//generateNotifierPresenceTesterBlock( header, s );

	vector<CompositeType*> structsOrderedByDependency;
	csharpMsg_orderStructsByDependency( s.structs, structsOrderedByDependency );

	//for ( auto& it : s.structs )
	//{
	//	assert( it != nullptr );
	//	assert( typeid( *(it) ) == typeid( CompositeType ) );
	//	assert( it->type == CompositeType::Type::structure );
	//	if ( it->isStruct4Publishing )
	//	{
	//		impl_generatePublishableStructForwardDeclaration( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	//		impl_GeneratePublishableStructWrapperForwardDeclaration( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	//		impl_GeneratePublishableStructWrapper4SetForwardDeclaration( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	//		fprintf( header, "\n" );
	//	}
	//}

	fprintf( header, "\n" );

	//for ( auto it : structsOrderedByDependency )
	//{
	//	assert( it != nullptr );
	//	assert( typeid( *(it) ) == typeid( CompositeType ) );
	//	assert( it->type == CompositeType::Type::structure );
	//	if ( it->isStruct4Publishing )
	//		impl_generatePublishableStruct( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	//}
	for (auto& it : structsOrderedByDependency)
	{
		assert(it != nullptr);
		assert(typeid(*(it)) == typeid(CompositeType));
		assert(it->type == CompositeType::Type::structure);
		if (it->isStruct4Messaging)
			csharpMsg_generateMessage(header, s, *it);
	}

	impl_insertScopeList(header, s);


	for ( auto& scope : s.scopes )
	{
		fprintf( header, "public class %s\n{\n", scope->name.c_str() );

		//impl_generateScopeEnum( header, *scope );
		csharpMsg_generateScopeHandler( header, *scope );
		//impl_generateScopeComposerForwardDeclaration( header, *scope );
		//csharpMsg_generateScopeComposer(header, *scope);
		
		fprintf(header, "\n");

		for ( auto it : scope->objectList )
		{
			assert( it != nullptr );
			assert( typeid( *(it) ) == typeid( CompositeType ) );
			assert( it->type == CompositeType::Type::message );
			if ( !it->isAlias )
				csharpMsg_generateMessage( header, s, *it );
			else
			{
				auto target = csharpMsg_findByName(s.structs, it->aliasOf);
				csharpMsg_generateMessageAlias(header, *it, *target);
			}
		}

		fprintf(header, "} // class %s\n\n", scope->name.c_str());
	}

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
	//for ( auto it : structsOrderedByDependency )
	//{
	//	assert( it != nullptr );
	//	assert( typeid( *(it) ) == typeid( CompositeType ) );
	//	assert( it->type == CompositeType::Type::structure );
	//	if ( it->isStruct4Publishing )
	//		generatePublishableAsCStruct( header, s, *(dynamic_cast<CompositeType*>(&(*(it)))) );
	//}

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

	fprintf( header, "\n} // namespace %s\n\n",	metascope );
}

