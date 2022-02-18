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

	void csharpMsg_generateScopeEnum(CsharpFileWritter& f, Scope& scope)
	{


		f.write("\tpublic enum MsgId\n");
		f.write("\t{\n");

		size_t sz = scope.objectList.size();
		for (size_t i = 0; i != sz; ++i)
		{
			auto& msg = scope.objectList[i];
			assert(msg != nullptr);

			string num = std::to_string(msg->numID);
			if(i != sz - 1)
				f.write("\t\t%s = %s,\n", msg->name.c_str(), num.c_str());
			else
				f.write("\t\t%s = %s\n", msg->name.c_str(), num.c_str());
		}
		f.write("\t}\n");
	}


	void csharpMsg_generateStructComposeGmq(CsharpFileWritter& f, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);



		string decl = generateCsharpDeclParams(s);
		f.write("\tpublic static void compose(GmqComposer composer, %s)\n", decl.c_str());

		

		f.write("\t{\n");

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
				const char* method = getIdlPrimitiveType2(param.type.kind);
				f.write("\t\tcomposer.compose%s(%s);\n", method, param.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\t\t%s_message.compose(composer, %s);\n", param.type.name.c_str(), param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = getCSharpPrimitiveType(param.type.vectorElemKind);
					f.write("\t\tGmqCollectionComposer<%s> %s_wrapper = GmqCollectionComposer<%s>.make(%s);\n", elem_type_name, param.name.c_str(), elem_type_name, param.name.c_str());
					f.write("\t\t%s_wrapper.composeGmq(composer);\n", param.name.c_str());
					break;
				}
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* elem_type_name = param.type.name.c_str();

					f.write("\t\tGmqCollectionComposer<I%s> %s_wrapper = new GmqCollectionComposer<I%s>(\n", elem_type_name, param.name.c_str(), elem_type_name);
					f.write("\t\t\t%s, %s_message.compose);\n", param.name.c_str(), elem_type_name);
					f.write("\t\t%s_wrapper.composeGmq(composer);\n", param.name.c_str());
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
		f.write("\t}\n");
	}

	void csharpMsg_generateStructComposeGmq2(CsharpFileWritter& f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);



		f.write("\tpublic static void compose(GmqComposer composer, %s val)\n", type_name);
		f.write("\t{\n");

		string caller = generateCsharpCallerParams(s, true);
		f.write("\t\tcompose(composer, %s);\n", caller.c_str());

		f.write("\t}\n");
	}

	void csharpMsg_generateStructParseGmq3(CsharpFileWritter& f, CompositeType& s, const char* name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);



		f.write("\tpublic static void parse(GmqParser parser, %s val)\n", name);

		f.write("\t{\n");

		int count = 0;
		for (auto& it : s.getMembers())
		{
			assert(it != nullptr);

			MessageParameter& param = *it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			f.write("\t{\n");
			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				f.write("\t\tInt64 tmp;\n");
				f.write("\t\tparser.parseSignedInteger(out tmp);\n");
				f.write("\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				f.write("\t\tUInt64 tmp;\n");
				f.write("\t\tparser.parseUnsignedInteger(out tmp);\n");
				f.write("\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				f.write("\t\tDouble tmp;\n");
				f.write("\t\tparser.parseReal(out tmp);\n");
				f.write("\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				f.write("\t\tString tmp;\n");
				f.write("\t\tparser.parseString(out tmp);\n");
				f.write("\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\t\t%s_message.parse(parser, val.%s);\n", param.type.name.c_str(), param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:

				f.write("\t\t\t\tGmqCollectionParser tmp = new GmqCollectionParser(\n");
				f.write("\t\t\t\t\t(GmqParser parser, int ordinal) =>\n");

				switch (param.type.vectorElemKind)
				{

				case MessageParameterType::KIND::INTEGER:
					f.write("\t\t\t\t\t\t{ Int64 t; parser.parseSignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					f.write("\t\t\t\t\t\t{ UInt64 t; parser.parseUnsignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					f.write("\t\t\t\t\t\t{ Double t; parser.parseReal(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					f.write("\t\t\t\t\t\t{ String t; parse.parseString(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					f.write("\t\t\t\t\t\t{ %s t = new %s(); %s_message.parse(parser, t); val.%s.Add(t); });\n", param.type.name.c_str(), param.type.name.c_str(), param.type.name.c_str(), param.name.c_str());
					break;
				default:
					assert(false); // unexpected
					break;
				}

				f.write("\t\t\t\ttmp.parseGmq(parser);\n");
				break;

			default:
				assert(false); // unexpected
				break;
			}
			f.write("\t}\n");
		}
		f.write("\t}\n");
	}

	void csharpMsg_generateStructComposeJson(CsharpFileWritter& f, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);



		string decl = generateCsharpDeclParams(s);
		f.write("\tpublic static void compose(JsonComposer composer, %s)\n", decl.c_str());
		f.write("\t{\n");

		f.write("\t\tcomposer.append( \"{\\n  \");\n");
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
				f.write("\t\tcomposer.append( \",\\n  \" );\n");


			f.write("\t\tcomposer.addNamePart(\"%s\");\n", param.name.c_str());
			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* method = getIdlPrimitiveType2(param.type.kind);
				f.write("\t\tcomposer.compose%s(%s);\n", method, param.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\t\t%s_message.compose(composer, %s);\n", param.type.name.c_str(), param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
				case MessageParameterType::KIND::UINTEGER:
				case MessageParameterType::KIND::REAL:
				case MessageParameterType::KIND::CHARACTER_STRING:
				{
					const char* elem_type_name = getCSharpPrimitiveType(param.type.vectorElemKind);
					f.write("\t\tJsonCollectionComposer<%s> %s_wrapper = JsonCollectionComposer<%s>.make(%s);\n", elem_type_name, param.name.c_str(), elem_type_name, param.name.c_str());
					f.write("\t\t%s_wrapper.composeJson(composer);\n", param.name.c_str());
					break;
				}

					f.write("\t\tCollectionWrapperForComposing %s_wrapper = SimpleTypeCollection.makeComposer(%s);\n", param.name.c_str(), param.name.c_str());
					f.write("\t\t%s_wrapper.composeJson(composer);\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* elem_type_name = param.type.name.c_str();

					f.write("\t\tJsonCollectionComposer<I%s> %s_wrapper = new JsonCollectionComposer<I%s>(\n", elem_type_name, param.name.c_str(), elem_type_name);
					f.write("\t\t\t%s, %s_message.compose);\n", param.name.c_str(), elem_type_name);
					f.write("\t\t%s_wrapper.composeJson(composer);\n", param.name.c_str());
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

		f.write("\t\tcomposer.append( \"\\n}\" );\n");

		f.write("\t}\n");
	}

	void csharpMsg_generateStructComposeJson2(CsharpFileWritter& f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union_case);



		f.write("\tpublic static void compose(JsonComposer composer, %s val)\n", type_name);

		f.write("\t{\n");
		
		string caller = generateCsharpCallerParams(s, true);
		f.write("\t\tcompose(composer, %s);\n", caller.c_str());
		
		f.write("\t}\n");
	}



	void csharpMsg_generateStructParseJson3(CsharpFileWritter& f, CompositeType& s, const char* name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);



		f.write("\tpublic static void parse(JsonParser parser, %s val)", name);


		f.write("\t{\n");


		f.write("\t\tparser.skipDelimiter( \'{\' );\n");
		f.write("\t\twhile (true)\n");
		f.write("\t\t{\n");
		f.write("\t\t\tstring key;\n");
		f.write("\t\t\tparser.readKeyFromJson( out key );\n");

		int count = 0;
		auto& mem = s.getMembers();
		for (auto it = mem.begin(); it != mem.end(); ++it)
		{
			assert(*it != nullptr);

			MessageParameter& param = **it;
			if (param.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			f.write("\t\t\t%s( key == \"%s\" )\n", count == 1 ? "if " : "else if ", param.name.c_str());

			f.write("\t\t\t{\n");

			switch (param.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				f.write("\t\t\t\tInt64 tmp;\n");
				f.write("\t\t\t\tparser.parseSignedInteger(out tmp);\n");
				f.write("\t\t\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				f.write("\t\t\t\tUInt64 tmp;\n");
				f.write("\t\t\t\tparser.parseUnsignedInteger(out tmp);\n");
				f.write("\t\t\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				f.write("\t\t\t\tDouble tmp;\n");
				f.write("\t\t\t\tparser.parseReal(out tmp);\n");
				f.write("\t\t\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				f.write("\t\t\t\tString tmp;\n");
				f.write("\t\t\t\tparser.parseString(out tmp);\n");
				f.write("\t\t\t\tval.%s = tmp;\n", param.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
				f.write("\t\t\t\tJsonCollectionParser tmp = new JsonCollectionParser(\n");
				f.write("\t\t\t\t\t(JsonParser parser, int ordinal) =>\n");
				switch (param.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					f.write("\t\t\t\t\t{ Int64 t; parser.parseSignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					f.write("\t\t\t\t{ UInt64 t; parser.parseUnsignedInteger(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					f.write("\t\t\t\t{ Double t; parser.parseReal(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					f.write("\t\t\t\t{ String t; parse.parseString(out t); val.%s.Add(t); });\n", param.name.c_str());
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
					f.write("\t\t\t\t{ %s t = new %s(); %s_message.parse(parser, t); val.%s.Add(t); });\n", param.type.name.c_str(), param.type.name.c_str(), param.type.name.c_str(), param.name.c_str());
					break;
				default:
					assert(false); // unexpected
					break;
				}
				f.write("\t\t\t\ttmp.parseJson(parser);\n");
				break;

			case MessageParameterType::KIND::EXTENSION:
				break; // TODO: ...
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\t\t\t\t%s_message.parse(parser, val.%s);\n", param.type.name.c_str(), param.name.c_str());
				break; // TODO: ...
			default:
				assert(false); // unexpected
				break;
			}

			f.write("\t\t\t}\n");
		}

		//TODO if unknown it should be skipped
		f.write("\n");

		f.write("\t\t\tparser.skipSpacesEtc();\n");
		f.write("\t\t\tif ( parser.isDelimiter( \',\' ) )\n");
		f.write("\t\t\t{\n");
		f.write("\t\t\t\tparser.skipDelimiter( \',\' );\n");
		f.write("\t\t\t\tcontinue;\n");
		f.write("\t\t\t}\n");
		f.write("\t\t\tif ( parser.isDelimiter( \'}\' ) )\n");
		f.write("\t\t\t{\n");
		f.write("\t\t\t\tparser.skipDelimiter( \'}\' );\n");
		f.write("\t\t\t\tbreak;\n");
		f.write("\t\t\t}\n");
		f.write("\t\t\tthrow new FormatException(); // bad format\n");

		f.write("\t\t}\n");

		f.write("\t}\n");
	}


	void csharpMsg_generateStructMembers(CsharpFileWritter& f, Root& root, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);



		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			auto& member = *it;
			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
				f.write("\tpublic Int64 %s;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::UINTEGER:
				f.write("\tpublic UInt64 %s;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::REAL:
				f.write("\tpublic Double %s;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::CHARACTER_STRING:
				f.write("\tpublic String %s = String.Empty;\n", member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\tpublic %s %s = new %s();\n", member.type.name.c_str(), member.name.c_str(), member.type.name.c_str());
				break;
			case MessageParameterType::KIND::VECTOR:
			{
				switch (member.type.vectorElemKind)
				{
				case MessageParameterType::KIND::INTEGER:
					f.write("\tpublic List<Int64> %s = new List<Int64>();\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::UINTEGER:
					f.write("\tpublic List<UInt64> %s = new List<UInt64>();\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::REAL:
					f.write("\tpublic List<Double> %s = new List<Double>();\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::CHARACTER_STRING:
					f.write("\tpublic List<String> %s = new List<String>();\n", member.name.c_str());
					break;
				case MessageParameterType::KIND::VECTOR:
					assert(false); // unexpected
					break;
				case MessageParameterType::KIND::STRUCT:
				case MessageParameterType::KIND::DISCRIMINATED_UNION:
				{
					const char* elem_type_name = member.type.name.c_str();
					f.write("\tpublic List<%s> %s = new List<%s>();\n", elem_type_name, member.name.c_str(), elem_type_name);
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

	void csharpMsg_generateComposeMessageMethod(CsharpFileWritter& f, CompositeType& s, const std::string& msgName, Proto proto)
	{
		// when message is an alias we feed structure here
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure);



		string decl = generateCsharpDeclParams(s);
		f.write("\tpublic static void composeMessage_%s(BufferT buffer, %s)\n", msgName.c_str(), decl.c_str());
		f.write("\t{\n");

		if (proto == Proto::gmq)
		{
			f.write("\t\tGmqComposer composer = new GmqComposer(buffer);\n\n");
			f.write("\t\tcomposer.composeUnsignedInteger((UInt64)MsgId.%s);\n", msgName.c_str());

			string caller = generateCsharpCallerParams(s, false);
			f.write("\t\t%s_message.compose(composer, %s);\n", s.name.c_str(), caller.c_str());
		}
		else if (proto == Proto::json)
		{
			f.write("\t\tJsonComposer composer = new JsonComposer(buffer);\n\n");
			f.write("\t\tcomposer.append(\"{\\n  \");\n");
			f.write("\t\tcomposer.addNamePart(\"msgid\");\n");
			f.write("\t\tcomposer.composeUnsignedInteger((UInt64)MsgId.%s);\n", msgName.c_str());
			f.write("\t\tcomposer.append(\",\\n  \");\n");
			f.write("\t\tcomposer.addNamePart(\"msgbody\");\n");

			string caller = generateCsharpCallerParams(s, false);
			f.write("\t\t%s_message.compose(composer, %s);\n", s.name.c_str(), caller.c_str());

			f.write("\t\tcomposer.append(\"\\n}\");\n");
		}
		else
		{
			assert(false);
		}

		f.write("\t}\n\n");
	}


	void csharpMsg_generateParseMessageMethod(CsharpFileWritter& f, const char* msg_name, const char* type_name)
	{
		// when message is an alias we feed structure here



		f.write("\tpublic static %s parseMessage_%s(ParserBase parser)\n", type_name, msg_name);


		f.write("\t{\n");
		f.write("\t\t%s tmp = new %s();\n", type_name, type_name);
		f.write("\t\tif (parser is GmqParser gmqP)\n");
		f.write("\t\t\t%s_message.parse(gmqP, tmp);\n", type_name);
		f.write("\t\telse if (parser is JsonParser jsonP)\n");
		f.write("\t\t\t%s_message.parse(jsonP, tmp);\n", type_name);
		f.write("\t\telse\n");
		f.write("\t\t\tthrow new ArgumentException();\n");
		f.write("\t\treturn tmp;\n");
		f.write("\t}\n");

	}


	void csharpMsg_generateMessageAliasMethods(CsharpFileWritter& f, CompositeType& target, const char* composerType, const char* parserType)
	{


		string decls = generateCsharpDeclParams(target);
		f.write("\tpublic static void compose(%s composer, %s)\n", composerType, decls.c_str());
		
		f.write("\t{\n");

		string caller = generateCsharpCallerParams(target, false);
		f.write("\t\t%s_message.compose(composer, %s);\n", target.name.c_str(), caller.c_str());

		f.write("\t}\n");
	}

	void csharpMsg_generateScopeHandler(CsharpFileWritter& f, Scope& scope)
	{


		f.write("\tpublic static MessageHandler makeMessageHandler( MsgId id, MessageHandler.HandlerDelegate handler )\n");
		f.write("\t{\n");
		f.write("\t\treturn new MessageHandler((ulong)id, handler);\n");
		f.write("\t}\n");

		f.write("\tpublic static MessageHandler makeDefaultMessageHandler( MessageHandler.HandlerDelegate handler)\n");
		f.write("\t{\n");
		f.write("\t\treturn new MessageHandler(MessageHandler.DefaultHandler, handler);\n");
		f.write("\t}\n");

		f.write("\tpublic static void handleMessage( BufferT buffer, params MessageHandler[] handlers )\n");
		f.write("\t{\n");
		f.write("\t\thandleMessage(buffer.getReadIterator(), handlers);\n");
		f.write("\t}\n");


		f.write("\tpublic static void handleMessage( ReadIteratorT riter, params MessageHandler[] handlers )\n");
		f.write("\t{\n");
		switch (scope.proto)
		{
		case Proto::gmq:
			f.write("\t\tGmqParser parser = new GmqParser( riter );\n");
			f.write("\t\tMessageHandler.gmq_handle( parser, handlers );\n");
			break;
		case Proto::json:
			f.write("\t\tJsonParser parser = new JsonParser( riter );\n");
			f.write("\t\tMessageHandler.json_handle( parser, handlers );\n");
			break;
		default:
			assert(false);
		}
		f.write("\t}\n");
	}

	
	void csharpMsg_generateScopeComposer(CsharpFileWritter& f, Scope& scope)
	{


		f.write("\tpublic static void composeMessage( BufferT buffer, MessageHandlerArray handlers )\n");
		f.write("\t{\n");
		f.write("\t\tReadIteratorT riter = buffer.getReadIterator();\n");
		f.write("\t\thandleMessage(riter, handlers);\n");
		f.write("\t}\n");


		f.write("\tpublic static void handleMessage( ReadIteratorT riter, MessageHandlerArray handlers )\n");
		f.write("\t{\n");
		switch (scope.proto)
		{
		case Proto::gmq:
				f.write("\t\tGmqParser parser = new GmqParser( riter );\n");
				f.write("\t\thandlers.gmq_handle(parser);\n");
			break;
		case Proto::json:
				f.write("\t\tJsonParser parser = new JsonParser( riter );\n");
				f.write("\t\thandlers.json_handle(parser);\n");
			break;
		default:
			assert(false);
		}
		f.write("\t}\n");
	}
}

void generateCsharpStructMessage(CsharpFileWritter& f, CompositeType& s, const char* type_name, const char* interface_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);



	f.write("public class %s_message\n", type_name);
	f.write("{\n");

	//csharpMsg_generateStructComposeBase(f, s);
	csharpMsg_generateStructComposeJson(f, s);
	csharpMsg_generateStructComposeGmq(f, s);
	csharpMsg_generateStructComposeJson2(f, s, interface_name);
	csharpMsg_generateStructComposeGmq2(f, s, interface_name);

	csharpMsg_generateStructParseJson3(f, s, interface_name);
	csharpMsg_generateStructParseGmq3(f, s, interface_name);

	f.write("} // class %s_message\n\n", type_name);

}


void generateCsharpMessages(CsharpFileWritter& f, Root& root, const std::string& metascope)
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

				generateCsharpStructMessage(f, *it, type_name.c_str(), interface_name.c_str());
			}
			else if (it->type == CompositeType::Type::discriminated_union)
				generateCsharpUnionMessage(f, *it);
			else
				assert(false);
		}
	}

	impl_insertScopeList(f.getFile(), root);


	for ( auto& scope : root.scopes )
	{
		f.write("public class %s\n", scope->name.c_str() );
		f.write("{\n");

		csharpMsg_generateScopeEnum(f, *scope);
		f.write("\n");
		csharpMsg_generateScopeHandler( f, *scope );
		f.write("\n");

		for ( auto it : scope->objectList )
		{
			assert( it != nullptr );
			assert( it->type == CompositeType::Type::message );

			checkCsharpStruct(*it);

			if (!it->isAlias)
			{
				std::string type_name = getCSharpTypeName(*it);
				std::string interface_name = "I" + type_name;

				generateCsharpStructInterface(f, *it, type_name.c_str());
				generateCsharpStructImpl(f, *it, type_name.c_str(), interface_name.c_str());
				generateCsharpStructMessage(f, *it, type_name.c_str(), interface_name.c_str());
				csharpMsg_generateComposeMessageMethod(f, *it, it->name, scope->proto);
				csharpMsg_generateParseMessageMethod(f, type_name.c_str(), type_name.c_str());
			}
			else
			{
				assert(it->aliasIdx < root.structs.size());
				auto& alias = root.structs[static_cast<decltype(root.structs)::size_type>(it->aliasIdx)];

				csharpMsg_generateComposeMessageMethod(f, *alias, it->name, scope->proto);
				csharpMsg_generateParseMessageMethod(f, it->name.c_str(), alias->name.c_str());
			}

		}

		f.write("} // class %s\n\n", scope->name.c_str());
	}
}

