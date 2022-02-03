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

	void csharpMsg_generateComposeParamTypeList2(FILE* header, Root& root, CompositeType& s)
	{
		int count = 0;
		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;

			if (member.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if (i != 0)
				fprintf(header, ", ");



			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "%s %s", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
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
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = getCSharpPrimitiveType(member.type.vectorElemKind);
					fprintf(header, "IList<%s> %s", elem_type_name, member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const std::string type_name = root.structs[member.type.structIdx]->name;
					fprintf(header, "IList<I%s> %s", type_name.c_str(), member.name.c_str());
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

	void csharpMsg_generateComposeParamTypeList5(FILE* header, Root& root, CompositeType& s)
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
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* type = csharpMsg_getCSharpType(param.type.kind);
				fprintf(header, "%s %s", type, param.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "%s_composer %s", param.type.name.c_str(), param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type = csharpMsg_getCSharpType(param.type.vectorElemKind);
					fprintf(header, "CollectionWrapperForComposing %s", param.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(param.type.structIdx < root.structs.size());
					const char* elem_type = root.structs[param.type.structIdx]->name.c_str();

					fprintf(header, "CollectionComposer<%s_composer> %s", elem_type, param.name.c_str());
					break;
				}
				default:
					assert(false); // unexpected
					break;
				}
				break;

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

	void csharpMsg_generateStructComposeGmq(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose(GmqComposer composer, ");

		csharpMsg_generateComposeParamTypeList2(header, root, s);

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
					fprintf(header, "\t\tCollectionWrapperForComposing %s_wrapper = SimpleTypeCollection.makeComposer(%s);\n", param.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t%s_wrapper.composeGmq(composer);\n", param.name.c_str());
					break;
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
				fprintf(header, "\t\tcomposer.compose%s(val.%s);\n", method, param.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t%s_message.compose(composer, val.%s);\n", param.type.name.c_str(), param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\tCollectionWrapperForComposing %s_wrapper = SimpleTypeCollection.makeComposer(val.%s);\n", param.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t%s_wrapper.composeGmq(composer);\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(param.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[param.type.structIdx]->name.c_str();

					fprintf(header, "\t\tGmqCollectionComposer<I%s> %s_wrapper = new GmqCollectionComposer<I%s>(\n", elem_type_name, param.name.c_str(), elem_type_name);
					fprintf(header, "\t\t\tval.%s, %s_message.compose);\n", param.name.c_str(), elem_type_name);
					fprintf(header, "\t\t%s_wrapper.composeGmq(composer);\n", param.name.c_str());
					break;
				}
				default:
					assert(false); // unexpected
					break;
				}
				break;

				break;
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructComposeGmq5(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose5(GmqComposer composer, ");

		csharpMsg_generateComposeParamTypeList5(header, root, s);

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
				fprintf(header, "\t\t%s.composeGmq(composer);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t%s.composeGmq(composer);\n", param.name.c_str());
				break;
			default:
				assert(false); // unexpected
				break;
			}
		}
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructParseGmq3(FILE* header, CompositeType& s, const char* name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tprotected static void parse(GmqParser parser, %s val)\n", name);

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
			//case MessageParameterType::KIND::BYTE_ARRAY:
			//	break;
			//case MessageParameterType::KIND::BLOB:
			//	break;
			//case MessageParameterType::KIND::ENUM:
			//	break;
			case MessageParameterType::KIND::VECTOR:

				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ Int64 t; parser.parseSignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseGmq(parser);\n");
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ UInt64 t; parser.parseUnsignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseGmq(parser);\n");
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ Double t; parser.parseReal(out t); val.%s.Add(t); });\n", param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseGmq(parser);\n");
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ String t; parse.parseString(out t); val.%s.Add(t); });\n", param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseGmq(parser);\n");
					break;
					//case MessageParameterType::KIND::BYTE_ARRAY:
					//case MessageParameterType::KIND::BLOB:
					//case MessageParameterType::KIND::ENUM:
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					//new MessageWrapperForComposing((ComposerBase composer) = > { mtest.STRUCT_point_compose(composer, x: msg.eighthParam.x, y : msg.eighthParam.y); }),
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ %s t = new %s(); %s_message.parse(parser, t); val.%s.Add(t); });\n", param.type.name.c_str(), param.type.name.c_str(), param.type.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseGmq(parser);\n");
					break;
				default:
					assert(false); // unexpected
					break;
				}
				break;

			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: treatment
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t%s_message.parse(parser, val.%s);\n", param.type.name.c_str(), param.name.c_str());
				break; // TODO: ...
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

		csharpMsg_generateComposeParamTypeList2(header, root, s);

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

			if (it != mem.begin())
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
				fprintf(header, "\t\tcomposer.compose%s(val.%s);\n", method, param.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t%s_message.compose(composer, val.%s);\n", param.type.name.c_str(), param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\tCollectionWrapperForComposing %s_wrapper = SimpleTypeCollection.makeComposer(val.%s);\n", param.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t%s_wrapper.composeJson(composer);\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(param.type.structIdx < root.structs.size());
					const char* elem_type_name = root.structs[param.type.structIdx]->name.c_str();

					fprintf(header, "\t\tJsonCollectionComposer<I%s> %s_wrapper = new JsonCollectionComposer<I%s>(\n", elem_type_name, param.name.c_str(), elem_type_name);
					fprintf(header, "\t\t\tval.%s, %s_message.compose);\n", param.name.c_str(), elem_type_name);
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

	void csharpMsg_generateStructComposeJson5(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose5(JsonComposer composer, ");

		csharpMsg_generateComposeParamTypeList5(header, root, s);

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

			if (it != mem.begin())
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
				fprintf(header, "\t\t%s.composeJson(composer);\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				fprintf(header, "\t\t%s.composeJson(composer);\n", param.name.c_str());
				break;
   			default:
				assert(false); // unexpected
				break;
			}

		}

		fprintf(header, "\t\tcomposer.append( \"\\n}\" );\n");

		fprintf(header, "\t}\n");
	}


	void csharpMsg_generateUnionCaseComposeJson(FILE* header, Root& root, CompositeType& s, const char* scope_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose_%s(JsonComposer composer, ", s.name.c_str());

		csharpMsg_generateComposeParamTypeList2(header, root, s);

		fprintf(header, ")\n"
			"\t{\n");

		fprintf(header, "\t\tcomposer.append( \"{\\n  \");\n");
		fprintf(header, "\t\tcomposer.addNamePart(\"caseid\");\n");
		fprintf(header, "\t\tcomposer.composeSignedInteger((Int64)I%s.Variants.%s);\n", scope_name, s.name.c_str());
		fprintf(header, "\t\tcomposer.append( \",\\n  \" );\n");
		fprintf(header, "\t\tcomposer.addNamePart(\"caseData\");\n");

		std::string type_name = getCSharpTypeName(s);
		fprintf(header, "\t\t%s.%s_message.compose(composer, ", scope_name, type_name.c_str());
		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");

		fprintf(header, "\t\tcomposer.append( \"\\n}\" );\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateUnionCaseComposeGmq(FILE* header, Root& root, CompositeType& s, const char* scope_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void compose_%s(GmqComposer composer, ", s.name.c_str());

		csharpMsg_generateComposeParamTypeList2(header, root, s);

		fprintf(header, ")\n\t{\n");

		fprintf(header, "\t\tcomposer.composeSignedInteger((Int64)I%s.Variants.%s);\n", scope_name, s.name.c_str());

		std::string type_name = getCSharpTypeName(s);
		fprintf(header, "\t\t%s.%s_message.compose(composer, ", scope_name, type_name.c_str());
		csharpMsg_generateCallerParamTypeLIst(header, s, false);
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
				//case MessageParameterType::KIND::BYTE_ARRAY:
				//	break;
				//case MessageParameterType::KIND::BLOB:
				//	break;
				//case MessageParameterType::KIND::ENUM:
				//	break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ Int64 t; parser.parseSignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseJson(parser);\n");
					break;
				case MessageParameterType::KIND::UINTEGER:
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ UInt64 t; parser.parseUnsignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseJson(parser);\n");
					break;
				case MessageParameterType::KIND::REAL:
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ Double t; parser.parseReal(out t); val.%s.Add(t); });\n", param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseJson(parser);\n");
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ String t; parse.parseString(out t); val.%s.Add(t); });\n", param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseJson(parser);\n");
					break;
					//case MessageParameterType::KIND::BYTE_ARRAY:
					//case MessageParameterType::KIND::BLOB:
					//case MessageParameterType::KIND::ENUM:
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					//new MessageWrapperForComposing((ComposerBase composer) = > { mtest.STRUCT_point_compose(composer, x: msg.eighthParam.x, y : msg.eighthParam.y); }),
					fprintf(header, "\t\t\t\tICollectionParse tmp = new CollectionWrapperForParsing(\n"
						"\t\t\t\t() => { },\n");
					fprintf(header, "\t\t\t\t(ParserBase parser, int ordinal) => "
						"{ %s t = new %s(); %s_message.parse(parser, t); val.%s.Add(t); });\n", param.type.name.c_str(), param.type.name.c_str(), param.type.name.c_str(), param.name.c_str());
					fprintf(header, "\t\t\t\ttmp.parseJson(parser);\n");
					break;
				default:
					assert(false); // unexpected
					break;
				}
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

	void csharpMsg_generateUnionParseJson3(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string type_name = getCSharpTypeName(s);

		fprintf(header, "\tprotected static void parse(JsonParser parser, I%s val)\n", type_name.c_str());

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
			fprintf(header, "\t\t\t\t%s.%s tmp = new %s.%s();\n", type_name.c_str(), case_name.c_str(), type_name.c_str(), case_name.c_str());
			fprintf(header, "\t\t\t\t%s.%s_message.parse(parser, tmp);\n", type_name.c_str(), case_name.c_str());
			fprintf(header, "\t\t\t\tval.setVariant(tmp);\n");
			fprintf(header, "\t\t\t\tbreak;\n");
			fprintf(header, "\t\t\t}\n");
		}

		fprintf(header, "\t\t\tdefault: throw new System.Exception();\n");
		fprintf(header, "\t\t}\n\n");

		fprintf(header, "\t\tparser.skipDelimiter('}');\n\n");

		//fprintf(header, "\t\treturn val;\n");
		fprintf(header, "\t}\n");
	}
	void csharpMsg_generateUnionParseGmq3(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string type_name = getCSharpTypeName(s);

		fprintf(header, "\tprotected static void parse(GmqParser parser, I%s val)\n", type_name.c_str());

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
			fprintf(header, "\t\t\t\t%s.%s tmp = new %s.%s();\n", type_name.c_str(), case_name.c_str(), type_name.c_str(), case_name.c_str());
			fprintf(header, "\t\t\t\t%s.%s_message.parse(parser, tmp);\n", type_name.c_str(), case_name.c_str());
			fprintf(header, "\t\t\t\tval.setVariant(tmp);\n");
			fprintf(header, "\t\t\t\tbreak;\n");
			fprintf(header, "\t\t\t}\n");
		}

		fprintf(header, "\t\t\tdefault: throw new System.Exception();\n");
		fprintf(header, "\t\t}\n\n");

		//fprintf(header, "\t\treturn val;\n");
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateUnionComposeJson2(FILE* header, Root& root, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		fprintf(header, "\tpublic static void compose(JsonComposer composer, %s val)\n", type_name);

		fprintf(header, "\t{\n");
		fprintf(header, "\t\t// TODO\n");
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateUnionComposeGmq2(FILE* header, Root& root, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		fprintf(header, "\tpublic static void compose(GmqComposer composer, %s val)\n", type_name);

		fprintf(header, "\t{\n");
		fprintf(header, "\t\t// TODO\n");
		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateStructParseBase4(FILE* header, CompositeType& s, const char* name)
	{
		assert(s.type == CompositeType::Type::message);

		fprintf(header, "\tpublic static %s parse(ParserBase parser)\n", name);

		fprintf(header, "\t{\n");
		fprintf(header, "\t\t%s tmp = new %s();\n", name, name);
		fprintf(header, "\t\t%s_message.parse(parser, tmp);\n", name);
		fprintf(header,	"\t\treturn tmp;\n");
		fprintf(header, "\t}\n");
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


	void csharpMsg_generateStructParseBase3(FILE* header, CompositeType& s, const char* name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "\tpublic static void parse(ParserBase parser, %s val)\n", name);

		fprintf(header, "\t{\n");

		fprintf(header,
			"\t\tif (parser is GmqParser gmqP)\n"
			"\t\t\tparse(gmqP, val);\n");

		fprintf(header,
			"\t\telse if (parser is JsonParser jsonP)\n"
			"\t\t\tparse(jsonP, val);\n");

		fprintf(header,
			"\t\telse\n"
			"\t\t\tthrow new ArgumentException();\n");

		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateUnionParseBase3(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		std::string type_name = getCSharpTypeName(s);

		fprintf(header, "\tpublic static void parse(ParserBase parser, I%s val)\n", type_name.c_str());

		fprintf(header,
			"\t{\n"
			"\t\tif (parser is GmqParser gmqP)\n"
			"\t\t\tparse(gmqP, val);\n"
			"\t\telse if (parser is JsonParser jsonP)\n"
			"\t\t\tparse(jsonP, val);\n"
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

	void csharpMsg_generateUnionCaseProperties(FILE* header, Root& root, CompositeType& s, bool forInterface)
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



	void csharpMsg_generateUnionEqualsMethod(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::discriminated_union);

		generateCsharpStandardMethods(header, type_name.c_str());

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

	void csharpMsg_generateComposeMessageMethod(FILE* header, Root& root, CompositeType& s, const std::string& msgName, Proto proto)
	{
		// when message is an alias we feed structure here
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure);

		fprintf(header, "\tpublic static void composeMessage_%s(BufferT buffer, ", msgName.c_str());

		csharpMsg_generateComposeParamTypeList2(header, root, s);

		fprintf(header, ")\n\t{\n");

		if (proto == Proto::gmq)
		{
			fprintf(header,
				"\t\tGmqComposer composer = new GmqComposer(buffer);\n\n"
				"\t\tcomposer.composeUnsignedInteger((UInt64)MsgId.%s);\n"
				"\t\t%s_message.compose(composer, ", msgName.c_str(), s.name.c_str());


			csharpMsg_generateCallerParamTypeLIst(header, s, false);
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

	void csharpMsg_generateStructComposer(FILE* header, Root& root, CompositeType& s, const char* type_name, const char* interface_name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);


		fprintf(header, "public struct %s_composer : IMessageCompose\n", type_name);
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
			{
				const char* type_name = getCSharpPrimitiveType(member.type.kind);
				fprintf(header, "\t%s %s;\n", type_name, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t%s_composer %s;\n", member.type.name.c_str(), member.name.c_str());
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
					const char* elem_type = getCSharpPrimitiveType(member.type.vectorElemKind);
					fprintf(header, "\tCollectionWrapperForComposing %s;", member.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					assert(member.type.structIdx < root.structs.size());
					const char* elem_type = root.structs[member.type.structIdx]->name.c_str();

					fprintf(header, "\tCollectionComposer<%s_composer> %s;", elem_type, member.name.c_str());
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



		fprintf(header, "\tpublic %s_composer(", type_name);
		csharpMsg_generateComposeParamTypeList5(header, root, s);
		fprintf(header, ")\n");

		fprintf(header, "\t{\n");

		for (auto& each : s.getMembers())
		{
			assert(each != nullptr);
			auto& member = *each;

			fprintf(header, "\t\tthis.%s = %s;\n", member.name.c_str(), member.name.c_str());
		}

		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic void composeJson(JsonComposer composer)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\t%s_message.compose5(composer, ", type_name);
		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic void composeGmq(GmqComposer composer)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\t%s_message.compose5(composer, ", type_name);
		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic static %s_composer make(", type_name);

		csharpMsg_generateComposeParamTypeList5(header, root, s);
		fprintf(header, ")\n");

		fprintf(header, "\t{\n");
		fprintf(header, "\t\treturn new %s_composer(", type_name);
		csharpMsg_generateCallerParamTypeLIst(header, s, false);
		fprintf(header, ");\n");
		fprintf(header, "\t}\n");

		fprintf(header, "} // class %s_composer\n\n", type_name);
	}


	void csharpMsg_generateStructMessage(FILE* header, Root& root, CompositeType& s, const char* type_name, const char* interface_name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);

		fprintf(header, "public class %s_message\n", type_name);
		fprintf(header, "{\n");

		//csharpMsg_generateStructComposeBase(header, s);
		csharpMsg_generateStructComposeJson(header, root, s);
		csharpMsg_generateStructComposeGmq(header, root, s);
		csharpMsg_generateStructComposeJson2(header, root, s, interface_name);
		csharpMsg_generateStructComposeGmq2(header, root, s, interface_name);
		csharpMsg_generateStructComposeJson5(header, root, s);
		csharpMsg_generateStructComposeGmq5(header, root, s);

		//csharpMsg_generateStructParseBase2(header, s, type_name, false);

		if (s.type == CompositeType::Type::message)
			csharpMsg_generateStructParseBase4(header, s, type_name);
		
		//csharpMsg_generateStructParseBase(header, s);
		csharpMsg_generateStructParseBase3(header, s, interface_name);
		//csharpMsg_generateStructParseJson(header, s);
		csharpMsg_generateStructParseJson3(header, s, interface_name);
		//csharpMsg_generateStructParseGmq(header, s);
		csharpMsg_generateStructParseGmq3(header, s, interface_name);

		fprintf(header, "} // class %s_message\n\n", type_name);

	}



	void csharpMsg_generateUnion(FILE* header, Root& root, CompositeType& s)
	{
		assert(s.isDiscriminatedUnion());

		bool checked = impl_checkParamNameUniqueness(s);
		checked = impl_checkFollowingExtensionRules(s) && checked;
		if (!checked)
			throw std::exception();


		impl_generateMessageCommentBlock(header, s);

		std::string type_name = getCSharpTypeName(s);


		fprintf(header, "public interface I%s\n", type_name.c_str());
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
		
		for (auto& it : s.getDiscriminatedUnionCases())
		{
			assert(it != nullptr);
			CompositeType& cs = *it;
			assert(cs.type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(cs);
			fprintf(header, "\tvoid setVariant(I%s obj);\n", case_type_name.c_str());
		}


		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;
			assert(cs.type == CompositeType::Type::discriminated_union_case);
			fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

			std::string case_type_name = getCSharpTypeName(cs);
			csharpMsg_generateUnionCaseProperties(header, root, cs, true);
			generateCsharpStructInterface(header, root, cs, case_type_name.c_str());
		}
		fprintf(header, "} // interface I%s\n\n", s.name.c_str());


		fprintf(header, "public class %s : I%s, IEquatable<%s>\n"
			"{\n", type_name.c_str(), type_name.c_str(), type_name.c_str());


		fprintf(header, "\tObject mem;\n");


		fprintf(header, "\n");


		csharpMsg_generateUnionEqualsMethod(header, s, type_name);

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

		for (auto& it : s.getDiscriminatedUnionCases())
		{
			assert(it != nullptr);
			CompositeType& cs = *it;
			assert(cs.type == CompositeType::Type::discriminated_union_case);

			std::string case_type_name = getCSharpTypeName(cs);
			fprintf(header, "\tpublic void setVariant(I%s.I%s obj) { this.mem = obj; }\n", type_name.c_str(), case_type_name.c_str());
		}


		// each case attributes
		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;
			assert(cs.type == CompositeType::Type::discriminated_union_case);
			fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

			std::string case_type_name = getCSharpTypeName(cs);
			std::string interface_name = "I" + type_name + ".I" + case_type_name;

			csharpMsg_generateUnionCaseProperties(header, root, cs, false);
			generateCsharpStructImpl(header, root, cs, case_type_name.c_str(), interface_name.c_str());
			csharpMsg_generateStructComposer(header, root, cs, case_type_name.c_str(), interface_name.c_str());
			csharpMsg_generateStructMessage(header, root, cs, case_type_name.c_str(), interface_name.c_str());
		}

		fprintf(header, "} // class %s\n\n", s.name.c_str());

		fprintf(header, "public struct %s_composer : IMessageCompose\n", type_name.c_str());

		fprintf(header, "{\n");

		fprintf(header, "\tint dummy;// TODO, same attributes as base type\n");

		fprintf(header, "\tpublic %s_composer(int dummy)\n", type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\tthis.dummy = dummy;// TODO\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic void composeJson(JsonComposer composer)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\t// TODO\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic void composeGmq(GmqComposer composer)\n");
		fprintf(header, "\t{\n");
		fprintf(header, "\t\t// TODO\n");
		fprintf(header, "\t}\n");

		fprintf(header, "\tpublic static %s_composer make(/* TODO */)\n", type_name.c_str());
		fprintf(header, "\t{\n");
		fprintf(header, "\t\t// TODO\n");
		fprintf(header, "\t\treturn new %s_composer(0);\n", type_name.c_str());
		fprintf(header, "\t}\n");

		fprintf(header, "} // class %s_composer\n\n", s.name.c_str());


		fprintf(header, "public class %s_message\n", type_name.c_str());
		fprintf(header, "{\n");

		//csharpMsg_generateUnionParseBase(header, s);
		csharpMsg_generateUnionParseBase3(header, s);
		//csharpMsg_generateUnionParseJson(header, s);
		csharpMsg_generateUnionParseJson3(header, s);
		//csharpMsg_generateUnionParseGmq(header, s);
		csharpMsg_generateUnionParseGmq3(header, s);
		std::string interface_name = "I" + type_name;

		csharpMsg_generateUnionComposeJson2(header, root, s, interface_name.c_str());
		csharpMsg_generateUnionComposeGmq2(header, root, s, interface_name.c_str());


		for (auto& duit : s.getDiscriminatedUnionCases())
		{
			assert(duit != nullptr);
			auto& cs = *duit;
			fprintf(header, "\n\t// IDL CASE %s:\n", cs.name.c_str());

			std::string type_name = getCSharpTypeName(s);

			//csharpMsg_generateStructComposeBase(header, cs, true);
			csharpMsg_generateUnionCaseComposeJson(header, root, cs, type_name.c_str());
			csharpMsg_generateUnionCaseComposeGmq(header, root, cs, type_name.c_str());
		}
		

		fprintf(header, "} // class %s_message\n\n", s.name.c_str());
	}

	void csharpMsg_generateMessageAliasMethods(FILE* header, Root& root, CompositeType& target, const char* composerType, const char* parserType)
	{

		fprintf(header, "\tpublic static void compose(%s composer, ", composerType);
		csharpMsg_generateComposeParamTypeList2(header, root, target);
		fprintf(header, ")\n\t{\n");

		fprintf(header, "\t\t%s_message.compose(composer, ", target.name.c_str());
		csharpMsg_generateCallerParamTypeLIst(header, target, false);
		fprintf(header, ");\n");


		fprintf(header, "\t}\n");
	}

	void csharpMsg_generateMessageAlias(FILE* header, Root& root, CompositeType& s, CompositeType& target)
	{
		bool checked = impl_checkFollowingExtensionRules(s);
		if (!checked)
			throw std::exception();

		impl_generateMessageCommentBlock(header, s);

		fprintf(header, "public class %s_message\n"
			"{\n", s.name.c_str());

		std::string type_name = getCSharpTypeName(s);
		//csharpMsg_generateStructParseBase2(header, target, type_name, true);
		csharpMsg_generateStructParseBase4(header, s, target.name.c_str());
		//csharpMsg_generateMessageAliasMethods(header, root, target, "ComposerBase", "ParserBase");
		//csharpMsg_generateMessageAliasMethods(header, root, target, "GmqComposer", "GmqParser");
		//csharpMsg_generateMessageAliasMethods(header, root, target, "JsonComposer", "JsonParser");

		fprintf(header, "} // class %s\n\n", s.name.c_str());
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

				//generateCsharpStructInterface(header, s, *it, type_name.c_str());
				//generateCsharpStructImpl(header, s, *it, type_name.c_str(), interface_name.c_str());
				csharpMsg_generateStructComposer(header, root, *it, type_name.c_str(), interface_name.c_str());
				csharpMsg_generateStructMessage(header, root, *it, type_name.c_str(), interface_name.c_str());
			}
			else if (it->type == CompositeType::Type::discriminated_union)
				csharpMsg_generateUnion(header, root, *it);
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
				csharpMsg_generateStructMessage(header, root, *it, type_name.c_str(), interface_name.c_str());
				csharpMsg_generateComposeMessageMethod(header, root, *it, it->name, scope->proto);
			}
			else
			{
				assert(it->aliasIdx < root.structs.size());
				auto& alias = root.structs[static_cast<decltype(root.structs)::size_type>(it->aliasIdx)];
				csharpMsg_generateMessageAlias(header, root, *it, *alias);
				csharpMsg_generateComposeMessageMethod(header, root, *alias, it->name, scope->proto);
			}

		}

		fprintf(header, "} // class %s\n\n", scope->name.c_str());
	}
}

