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

	const char* csharpMsg_getCSharpType(MessageParameterType::KIND kind)
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

	const char* csharpMsg_getMethodForKind(MessageParameterType::KIND kind)
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

	void csharpMsg_generateScopeEnum(FILE* header, Scope& scope)
	{
		fprintf(header, "\tpublic enum MsgId { ");
		for (auto msg = scope.objectList.begin(); msg != scope.objectList.end(); ++msg)
		{
			if(msg != scope.objectList.begin())
				fprintf(header, ", ");

			fprintf(header, "%s = %lld", (*msg)->name.c_str(), (*msg)->numID);
		}
		fprintf(header, " }\n");
	}


	void csharpMsg_generateStructComposeGmq(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose(GmqComposer composer, ");

		generateCsharpDeclParams(header, root, s);

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
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* method = csharpMsg_getMethodForKind(param.type.kind);
				fprintf(header, "\t\tcomposer.compose%s(%s);\n", method, param.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t%s_message.compose(composer, %s);\n", param.type.name.c_str(), param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = csharpMsg_getCSharpType(param.type.vectorElemKind);
					fprintf(header, "\t\tGmqCollectionComposer<%s> %s_wrapper = GmqCollectionComposer<%s>.make(%s);\n", elem_type_name, param.name.c_str(), elem_type_name, param.name.c_str());
					fprintf(header, "\t\t%s_wrapper.composeGmq(composer);\n", param.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(param.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[param.type.structIdx]->name.c_str();

					fprintf(header, "\t\tGmqCollectionComposer<I%s> %s_wrapper = new GmqCollectionComposer<I%s>(\n", elem_type_name, param.name.c_str(), elem_type_name);
					fprintf(header, "\t\t\t%s, %s_message.compose);\n", param.name.c_str(), elem_type_name);
					fprintf(header, "\t\t%s_wrapper.composeGmq(composer);\n", param.name.c_str());
					break;
				}
				default:
					assert(false); // unexpected
					break;
				}
				break;
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructComposeGmq2(FILE* header, Root& root, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose(GmqComposer composer, %s val)\n", type_name);
		fprintf(header, "\t{\n");

		fprintf(header, "\t\tcompose(composer, ");
		generateCsharpCallerParams(header, s, true);
		fprintf(header, ");\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructParseGmq3(FILE* header, CompositeType& s, const char* name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void parse(GmqParser parser, %s val)\n", name);

		fprintf(header, "\t{\n");

		int count = 0;
		for (auto& it : s.getMembers())
		{
			assert(it != nullptr);

			MessageParameter& param = *it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			fprintf(header, "\t{\n");
			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\t\tInt64 tmp;\n");
				fprintf(header, "\t\tparser.parseSignedInteger(out tmp);\n");
				fprintf(header, "\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\t\tUInt64 tmp;\n");
				fprintf(header, "\t\tparser.parseUnsignedInteger(out tmp);\n");
				fprintf(header, "\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "\t\tDouble tmp;\n");
				fprintf(header, "\t\tparser.parseReal(out tmp);\n");
				fprintf(header, "\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\tString tmp;\n");
				fprintf(header, "\t\tparser.parseString(out tmp);\n");
				fprintf(header, "\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t%s_message.parse(parser, val.%s);\n", param.type.name.c_str(), param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:

				fprintf(header, "\t\t\t\tGmqCollectionParser tmp = new GmqCollectionParser(\n");
				fprintf(header, "\t\t\t\t\t(GmqParser parser, int ordinal) =>\n");

				switch (param.type.vectorElemKind)
				{

				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "\t\t\t\t\t\t{ Int64 t; parser.parseSignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "\t\t\t\t\t\t{ UInt64 t; parser.parseUnsignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "\t\t\t\t\t\t{ Double t; parser.parseReal(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\t\t\t\t\t{ String t; parse.parseString(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					fprintf(header, "\t\t\t\t\t\t{ %s t = new %s(); %s_message.parse(parser, t); val.%s.Add(t); });\n", param.type.name.c_str(), param.type.name.c_str(), param.type.name.c_str(), param.name.c_str());
					break;
				default:
					assert(false); // unexpected
					break;
				}

				fprintf(header, "\t\t\t\ttmp.parseGmq(parser);\n");
				break;

			default:
				assert(false); // unexpected
				break;
			}
			fprintf(header, "\t}\n");
		}
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructComposeJson(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose(JsonComposer composer, ");

		generateCsharpDeclParams(header, root, s);

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
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* method = csharpMsg_getMethodForKind(param.type.kind);
				fprintf(header, "\t\tcomposer.compose%s(%s);\n", method, param.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t%s_message.compose(composer, %s);\n", param.type.name.c_str(), param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = csharpMsg_getCSharpType(param.type.vectorElemKind);
					fprintf(header, "\t\tJsonCollectionComposer<%s> %s_wrapper = JsonCollectionComposer<%s>.make(%s);\n", elem_type_name, param.name.c_str(), elem_type_name, param.name.c_str());
					fprintf(header, "\t\t%s_wrapper.composeJson(composer);\n", param.name.c_str());
					break;
				}

					fprintf(header, "\t\tCollectionWrapperForComposing %s_wrapper = SimpleTypeCollection.makeComposer(%s);\n", param.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t%s_wrapper.composeJson(composer);\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(param.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[param.type.structIdx]->name.c_str();

					fprintf(header, "\t\tJsonCollectionComposer<I%s> %s_wrapper = new JsonCollectionComposer<I%s>(\n", elem_type_name, param.name.c_str(), elem_type_name);
					fprintf(header, "\t\t\t%s, %s_message.compose);\n", param.name.c_str(), elem_type_name);
					fprintf(header, "\t\t%s_wrapper.composeJson(composer);\n", param.name.c_str());
					break;
				}
				default:
					assert(false); // unexpected
					break;
				}
				break;
			default:
				assert(false); // unexpected
				break;
			}
		}

		fprintf(header, "\t\tcomposer.append( \"\\n}\" );\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructComposeJson2(FILE* header, Root& root, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose(JsonComposer composer, %s val)\n", type_name);

		fprintf(header, "\t{\n");
		
		fprintf(header, "\t\tcompose(composer, ");
		generateCsharpCallerParams(header, s, true);
		fprintf(header, ");\n");

		fprintf(header, "\t}\n");
	}



	void csharpMsg_generateStructParseJson3(FILE* header, CompositeType& s, const char* name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void parse(JsonParser parser, %s val)", name);


		fprintf(header, "\t{\n");


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

			fprintf(header, "\t\t\t{\n");

			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				fprintf(header, "\t\t\t\tInt64 tmp;\n");
				fprintf(header, "\t\t\t\tparser.parseSignedInteger(out tmp);\n");
				fprintf(header, "\t\t\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				fprintf(header, "\t\t\t\tUInt64 tmp;\n");
				fprintf(header, "\t\t\t\tparser.parseUnsignedInteger(out tmp);\n");
				fprintf(header, "\t\t\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				fprintf(header, "\t\t\t\tDouble tmp;\n");
				fprintf(header, "\t\t\t\tparser.parseReal(out tmp);\n");
				fprintf(header, "\t\t\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\t\t\tString tmp;\n");
				fprintf(header, "\t\t\t\tparser.parseString(out tmp);\n");
				fprintf(header, "\t\t\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t\t\tJsonCollectionParser tmp = new JsonCollectionParser(\n");
				fprintf(header, "\t\t\t\t\t(JsonParser parser, int ordinal) =>\n");
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "\t\t\t\t\t{ Int64 t; parser.parseSignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "\t\t\t\t{ UInt64 t; parser.parseUnsignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "\t\t\t\t{ Double t; parser.parseReal(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\t\t\t{ String t; parse.parseString(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					fprintf(header, "\t\t\t\t{ %s t = new %s(); %s_message.parse(parser, t); val.%s.Add(t); });\n", param.type.name.c_str(), param.type.name.c_str(), param.type.name.c_str(), param.name.c_str());
					break;
				default:
					assert(false); // unexpected
					break;
				}
				fprintf(header, "\t\t\t\ttmp.parseJson(parser);\n");
				break;

			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: ...
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\t\t%s_message.parse(parser, val.%s);\n", param.type.name.c_str(), param.name.c_str());
				break; // TODO: ...
			default:
				assert(false); // unexpected
				break;
			}

			fprintf(header, "\t\t\t}\n");
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
				fprintf(header, "\tpublic String %s = String.Empty;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\tpublic %s %s = new %s();\n", member.type.name.c_str(), member.name.c_str(), member.type.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "\tpublic List<Int64> %s = new List<Int64>();\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "\tpublic List<UInt64> %s = new List<UInt64>();\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "\tpublic List<Double> %s = new List<Double>();\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\tpublic List<String> %s = new List<String>();\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::VECTOR:
					assert(false); // unexpected
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* type_name = root.structs[member.type.structIdx]->name.c_str();
					fprintf(header, "\tpublic List<%s> %s = new List<%s>();\n", type_name, member.name.c_str(), type_name);
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

	void csharpMsg_generateComposeMessageMethod(FILE* header, Root& root, CompositeType& s, const std::string& msgName, Proto proto)
	{
		// when message is an alias we feed structure here
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure);

		fprintf(header, "\tpublic static void composeMessage_%s(BufferT buffer, ", msgName.c_str());

		generateCsharpDeclParams(header, root, s);

		fprintf(header, ")\n\t{\n");

		if (proto == Proto::gmq)
		{
			fprintf(header,
				"\t\tGmqComposer composer = new GmqComposer(buffer);\n\n"
				"\t\tcomposer.composeUnsignedInteger((UInt64)MsgId.%s);\n"
				"\t\t%s_message.compose(composer, ", msgName.c_str(), s.name.c_str());


			generateCsharpCallerParams(header, s, false);
			fprintf(header,");\n");
		}
		else if (proto == Proto::json)
		{
			fprintf(header,
				"\t\tJsonComposer composer = new JsonComposer(buffer);\n\n"
				"\t\tcomposer.append(\"{\\n  \");\n"
				"\t\tcomposer.addNamePart(\"msgid\");\n"
				"\t\tcomposer.composeUnsignedInteger((UInt64)MsgId.%s);\n"
				"\t\tcomposer.append(\",\\n  \");\n"
				"\t\tcomposer.addNamePart(\"msgbody\");\n"
				"\t\t%s_message.compose(composer, ", msgName.c_str(), s.name.c_str());

			generateCsharpCallerParams(header, s, false);
			fprintf(header,	");\n"
				"\t\tcomposer.append(\"\\n}\");\n");
		}
		else
		{
			assert(false);
		}

		fprintf(header, "\t}\n\n");
	}


	void csharpMsg_generateParseMessageMethod(FILE* header, const char* msg_name, const char* type_name)
	{
		// when message is an alias we feed structure here

		fprintf(header, "\tpublic static %s parseMessage_%s(ParserBase parser)\n", type_name, msg_name);


		fprintf(header, "\t{\n");
		fprintf(header, "\t\t%s tmp = new %s();\n", type_name, type_name);
		fprintf(header, "\t\tif (parser is GmqParser gmqP)\n");
		fprintf(header, "\t\t\t%s_message.parse(gmqP, tmp);\n", type_name);
		fprintf(header, "\t\telse if (parser is JsonParser jsonP)\n");
		fprintf(header, "\t\t\t%s_message.parse(jsonP, tmp);\n", type_name);
		fprintf(header, "\t\telse\n");
		fprintf(header, "\t\t\tthrow new ArgumentException();\n");
		fprintf(header, "\t\treturn tmp;\n");
		fprintf(header, "\t}\n");

	}


	void csharpMsg_generateMessageAliasMethods(FILE* header, Root& root, CompositeType& target, const char* composerType, const char* parserType)
	{

		fprintf(header, "\tpublic static void compose(%s composer, ", composerType);
		generateCsharpDeclParams(header, root, target);
		fprintf(header, ")\n\t{\n");

		fprintf(header, "\t\t%s_message.compose(composer, ", target.name.c_str());
		generateCsharpCallerParams(header, target, false);
		fprintf(header, ");\n");


		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateScopeHandler(FILE* header, Scope& scope)
	{
		fprintf(header,
			"\tpublic static MessageHandler makeMessageHandler( MsgId id, MessageHandler.HandlerDelegate handler )\n"
			"\t{\n"
			"\t\treturn new MessageHandler((ulong)id, handler);\n"
			"\t}\n");
		fprintf(header,
			"\tpublic static MessageHandler makeDefaultMessageHandler( MessageHandler.HandlerDelegate handler)\n"
			"\t{\n"
			"\t\treturn new MessageHandler(MessageHandler.DefaultHandler, handler);\n"
			"\t}\n");

		fprintf(header,
			"\tpublic static void handleMessage( BufferT buffer, params MessageHandler[] handlers )\n"
			"\t{\n"
			"\t\thandleMessage(buffer.getReadIterator(), handlers);\n"
			"\t}\n");


		fprintf(header,
			"\tpublic static void handleMessage( ReadIteratorT riter, params MessageHandler[] handlers )\n"
			"\t{\n"
		);
		switch (scope.proto)
		{
		case Proto::gmq:
			fprintf(header,
				"\t\tGmqParser parser = new GmqParser( riter );\n"
				"\t\tMessageHandler.gmq_handle( parser, handlers );\n"
			);
			break;
		case Proto::json:
			fprintf(header,
				"\t\tJsonParser parser = new JsonParser( riter );\n"
				"\t\tMessageHandler.json_handle( parser, handlers );\n"
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

void generateCsharpStructMessage(FILE* header, Root& root, CompositeType& s, const char* type_name, const char* interface_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

	fprintf(header, "public class %s_message\n", type_name);
	fprintf(header, "{\n");

	//csharpMsg_generateStructComposeBase(header, s);
	csharpMsg_generateStructComposeJson(header, root, s);
	csharpMsg_generateStructComposeGmq(header, root, s);
	csharpMsg_generateStructComposeJson2(header, root, s, interface_name);
	csharpMsg_generateStructComposeGmq2(header, root, s, interface_name);

	csharpMsg_generateStructParseJson3(header, s, interface_name);
	csharpMsg_generateStructParseGmq3(header, s, interface_name);

	fprintf(header, "} // class %s_message\n\n", type_name);

}


void generateCsharpMessages(FILE* header, Root& root, const std::string& metascope)
{
	vector<CompositeType*> structsOrderedByDependency;
	std::unordered_set<size_t> collElementTypes;
	orderStructsByDependency( root.structs, structsOrderedByDependency, collElementTypes );

	for (auto& it : structsOrderedByDependency)
	{
		assert(it != nullptr);
		assert(typeid(*(it)) == typeid(CompositeType));

		if (it->isStruct4Messaging)
		{
			checkCsharpStruct(*it);
			if (it->type == CompositeType::Type::structure)
			{

				std::string type_name = getCSharpTypeName(*it);
				std::string interface_name = "I" + type_name;

				generateCsharpStructMessage(header, root, *it, type_name.c_str(), interface_name.c_str());
			}
			else if (it->type == CompositeType::Type::discriminated_union)
				generateCsharpUnionMessage(header, root, *it);
			else
				assert(false);
		}
	}

	impl_insertScopeList(header, root);


	for ( auto& scope : root.scopes )
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

			checkCsharpStruct(*it);

			if (!it->isAlias)
			{
				std::string type_name = getCSharpTypeName(*it);
				std::string interface_name = "I" + type_name;

				generateCsharpStructInterface(header, root, *it, type_name.c_str());
				generateCsharpStructImpl(header, root, *it, type_name.c_str(), interface_name.c_str());
				generateCsharpStructMessage(header, root, *it, type_name.c_str(), interface_name.c_str());
				csharpMsg_generateComposeMessageMethod(header, root, *it, it->name, scope->proto);
				csharpMsg_generateParseMessageMethod(header, type_name.c_str(), type_name.c_str());
			}
			else
			{
				assert(it->aliasIdx < root.structs.size());
				auto& alias = root.structs[static_cast<decltype(root.structs)::size_type>(it->aliasIdx)];

				csharpMsg_generateComposeMessageMethod(header, root, *alias, it->name, scope->proto);
				csharpMsg_generateParseMessageMethod(header, it->name.c_str(), alias->name.c_str());
			}

		}

		fprintf(header, "} // class %s\n\n", scope->name.c_str());
	}
}

