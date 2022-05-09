/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
* Copyright (c) 2020-2022, OLogN Technologies AG
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
#include "idl_tree_common.h"

#include <set>

namespace csharp
{

	void csharpMsg_generateScopeEnum(FileWritter f, Scope& scope)
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


	void csharpMsg_generateStructComposeGmq(FileWritter f, CompositeType& s)
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

			MessageParameter& member = **it;
			if (member.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* idl = getIdlPrimitiveType2(member.type.kind);
				f.write("\t\tcomposer.compose%s(%s);\n", idl, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			case MessageParameterType::KIND::VECTOR:
			case MessageParameterType::KIND::DICTIONARY:
			{
				string p = getCSharpPrefixByType(member.type);
				f.write("\t\t%s_message.compose(composer, %s);\n", p.c_str(), member.name.c_str());
				break;
			}
			default:
				assert(false); // unexpected
				break;
			}
		}
		f.write("\t}\n");
	}

	void csharpMsg_generateStructComposeGmq2(FileWritter f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);



		f.write("\tpublic static void compose(GmqComposer composer, %s val)\n", type_name);
		f.write("\t{\n");

		string caller = generateCsharpCallerParams(s, true);
		f.write("\t\tcompose(composer, %s);\n", caller.c_str());

		f.write("\t}\n");
	}

	void csharpMsg_generateStructParseGmq3(FileWritter f, CompositeType& s, const char* name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure || s.type == CompositeType::Type::discriminated_union_case);



		f.write("\tpublic static void parse(GmqParser parser, %s val)\n", name);

		f.write("\t{\n");

		int count = 0;
		for (auto& it : s.getMembers())
		{
			assert(it != nullptr);

			MessageParameter& member = *it;
			if (member.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* type = getCSharpPrimitiveType(member.type.kind);
				const char* idl = getIdlPrimitiveType2(member.type.kind);
				f.write("\t{\n");
				f.write("\t\t%s tmp;\n", type);
				f.write("\t\tparser.parse%s(out tmp);\n", idl);
				f.write("\t\tval.%s = tmp;\n", member.name.c_str());
				f.write("\t}\n");
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			case MessageParameterType::KIND::VECTOR:
			case MessageParameterType::KIND::DICTIONARY:
			{
				string p = getCSharpPrefixByType(member.type);
				f.write("\t%s_message.parse(parser, val.%s);\n", p.c_str(), member.name.c_str());
				break;
			}
			default:
				assert(false); // unexpected
				break;
			}
		}
		f.write("\t}\n");
	}

	void csharpMsg_generateStructComposeJson(FileWritter f, CompositeType& s)
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

			MessageParameter& member = **it;
			if (member.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			if(it != mem.begin())
				f.write("\t\tcomposer.append( \",\\n  \" );\n");


			f.write("\t\tcomposer.addNamePart(\"%s\");\n", member.name.c_str());
			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* method = getIdlPrimitiveType2(member.type.kind);
				f.write("\t\tcomposer.compose%s(%s);\n", method, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			case MessageParameterType::KIND::VECTOR:
			case MessageParameterType::KIND::DICTIONARY:
			{
				string p = getCSharpPrefixByType(member.type);
				f.write("\t\t%s_message.compose(composer, %s);\n", p.c_str(), member.name.c_str());
				break;
			}
			default:
				assert(false); // unexpected
				break;
			}
		}

		f.write("\t\tcomposer.append( \"\\n}\" );\n");

		f.write("\t}\n");
	}

	void csharpMsg_generateStructComposeJson2(FileWritter f, CompositeType& s, const char* type_name)
	{
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
			s.type == CompositeType::Type::discriminated_union_case);



		f.write("\tpublic static void compose(JsonComposer composer, %s val)\n", type_name);

		f.write("\t{\n");
		
		string caller = generateCsharpCallerParams(s, true);
		f.write("\t\tcompose(composer, %s);\n", caller.c_str());
		
		f.write("\t}\n");
	}



	void csharpMsg_generateStructParseJson3(FileWritter f, CompositeType& s, const char* name)
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

			MessageParameter& member = **it;
			if (member.type.kind == MessageParameterType::KIND::EXTENSION)
				continue;
			++count;

			f.write("\t\t\t%s( key == \"%s\" )\n", count == 1 ? "if " : "else if ", member.name.c_str());

			switch (member.type.kind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* type = getCSharpPrimitiveType(member.type.kind);
				const char* idl = getIdlPrimitiveType2(member.type.kind);

				f.write("\t\t\t{\n");
				f.write("\t\t\t\t%s tmp;\n", type);
				f.write("\t\t\t\tparser.parse%s(out tmp);\n", idl);
				f.write("\t\t\t\tval.%s = tmp;\n", member.name.c_str());
				f.write("\t\t\t}\n");
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			case MessageParameterType::KIND::VECTOR:
			case MessageParameterType::KIND::DICTIONARY:
			{
				string p = getCSharpPrefixByType(member.type);

				f.write("\t\t\t%s_message.parse(parser, val.%s);\n", p.c_str(), member.name.c_str());
				break;
			}
			default:
				assert(false); // unexpected
				break;
			}

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

	void csharpMsg_generateComposeMessageMethod(FileWritter f, CompositeType& s, const std::string& msgName, Scope& scope)
	{
		// when message is an alias we feed structure here
		assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure);



		string decl = generateCsharpDeclParams(s);
		f.write("\tpublic static void composeMessage_%s(BufferT buffer, %s)\n", msgName.c_str(), decl.c_str());
		f.write("\t{\n");

		if (scope.proto == Proto::gmq)
		{
			f.write("\t\tGmqComposer composer = new GmqComposer(buffer);\n\n");
			f.write("\t\tcomposer.composeUnsignedInteger((UInt64)MsgId.%s);\n", msgName.c_str());

			string caller = generateCsharpCallerParams(s, false);
			f.write("\t\t%s_message.compose(composer, %s);\n", s.name.c_str(), caller.c_str());
		}
		else if (scope.proto == Proto::json)
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


	void csharpMsg_generateParseMessageMethod(FileWritter f, const char* msg_name, const char* type_name, Scope& scope)
	{
		// when message is an alias we feed structure here

		if (scope.proto == Proto::gmq)
			f.write("\tpublic static %s parseMessage_%s(GmqParser parser)\n", type_name, msg_name);
		else if (scope.proto == Proto::json)
			f.write("\tpublic static %s parseMessage_%s(JsonParser parser)\n", type_name, msg_name);
		else
			assert(false);

		f.write("\t{\n");
		f.write("\t\t%s tmp = new %s();\n", type_name, type_name);
		f.write("\t\t%s_message.parse(parser, tmp);\n", type_name);
		f.write("\t\treturn tmp;\n");
		f.write("\t}\n");

	}


	void csharpMsg_generateMessageAliasMethods(FileWritter f, CompositeType& target, const char* composerType, const char* parserType)
	{


		string decls = generateCsharpDeclParams(target);
		f.write("\tpublic static void compose(%s composer, %s)\n", composerType, decls.c_str());
		
		f.write("\t{\n");

		string caller = generateCsharpCallerParams(target, false);
		f.write("\t\t%s_message.compose(composer, %s);\n", target.name.c_str(), caller.c_str());

		f.write("\t}\n");
	}

	void csharpMsg_generateScopeHandler(FileWritter f, const char* prefix)
	{
		f.write("\tpublic static %sMessageHandler makeMessageHandler( MsgId id, %sMessageHandler.HandlerDelegate handler )\n", prefix, prefix);
		f.write("\t{\n");
		f.write("\t\treturn new %sMessageHandler((ulong)id, handler);\n", prefix);
		f.write("\t}\n");

		f.write("\tpublic static %sMessageHandler makeDefaultMessageHandler( %sMessageHandler.HandlerDelegate handler)\n", prefix, prefix);
		f.write("\t{\n");
		f.write("\t\treturn new %sMessageHandler(%sMessageHandler.DefaultHandler, handler);\n", prefix, prefix);
		f.write("\t}\n");

		f.write("\tpublic static void handleMessage( BufferT buffer, params %sMessageHandler[] handlers )\n", prefix);
		f.write("\t{\n");
		f.write("\t\thandleMessage(buffer.getReadIterator(), handlers);\n");
		f.write("\t}\n");

		f.write("\tpublic static void handleMessage( ReadIteratorT riter, params %sMessageHandler[] handlers )\n", prefix);
		f.write("\t{\n");
		f.write("\t\t%sParser parser = new %sParser( riter );\n", prefix, prefix);
		f.write("\t\t%sMessageHandler.handle( parser, handlers );\n", prefix);
		f.write("\t}\n");
	}


	void csharpMsg_generateScopeHandler(FileWritter f, Scope& scope)
	{
		switch (scope.proto)
		{
		case Proto::gmq:
			csharpMsg_generateScopeHandler(f, "Gmq");
			break;
		case Proto::json:
			csharpMsg_generateScopeHandler(f, "Json");
			break;
		default:
			assert(false);
		}
	}

	void csharpMsg_generateDictionaryMessage(FileWritter f, MessageParameter& member, std::set<std::string>& alreadyDone)
	{
		assert(member.type.kind == MessageParameterType::KIND::DICTIONARY);

		string prefix = getCSharpPrefixByType(member.type);
		string iface = getCSharpInterfaceType(member.type);

		if(alreadyDone.find(prefix) != alreadyDone.end())
			return;

		alreadyDone.insert(prefix);

		f.write("public class %s_message\n", prefix.c_str());
		f.write("{\n");

		f.write("\tpublic static void compose(JsonComposer composer, %s val)\n", iface.c_str());
		f.write("\t{\n");

		f.write("\t\tcomposer.append( \"{\\n  \");\n");
		f.write("\t\tint i = 0;\n");

		const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
		const char* idlKey = getIdlPrimitiveType2(member.type.dictionaryKeyKind);


		int count = 0;
		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
			const char* idlValue = getIdlPrimitiveType2(member.type.dictionaryValueKind);
			f.write("\t\tforeach(KeyValuePair<%s, %s> each in val)\n", key, value);
			f.write("\t\t{\n");
			f.write("\t\t\tif (i != 0)\n");
			f.write("\t\t\t\tcomposer.append(\", \\n  \");\n");
			f.write("\t\t\tcomposer.append('{');\n");
			f.write("\t\t\tcomposer.compose%s(each.Key);\n", idlKey);
			f.write("\t\t\tcomposer.append(',');\n");
			f.write("\t\t\tcomposer.compose%s(each.Value);\n", idlValue);
			f.write("\t\t\tcomposer.append('}');\n");
			f.write("\t\t\t++i;\n");
			f.write("\t\t}\n");
			break;

		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\t\tforeach(KeyValuePair<%s, I%s> each in val)\n", key, value);
			f.write("\t\t{\n");
			f.write("\t\t\tif (i != 0)\n");
			f.write("\t\t\t\tcomposer.append(\", \\n  \");\n");
			f.write("\t\t\tcomposer.append('{');\n");
			f.write("\t\t\tcomposer.compose%s(each.Key);\n", idlKey);
			f.write("\t\t\tcomposer.append(',');\n");
			f.write("\t\t\t%s_message.compose(composer, each.Value);\n",  value);
			f.write("\t\t\tcomposer.append('}');\n");
			f.write("\t\t\t++i;\n");
			f.write("\t\t}\n");
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}


		f.write("\t\tcomposer.append(\"\\n}\");\n");

		f.write("\t}\n");


		f.write("\tpublic static void compose(GmqComposer composer, %s val)\n", iface.c_str());
		f.write("\t{\n");
		f.write("\t\tcomposer.composeUnsignedInteger((UInt64)val.Count);\n");

		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
			const char* idlValue = getIdlPrimitiveType2(member.type.dictionaryValueKind);
			f.write("\t\tforeach(KeyValuePair<%s, %s> each in val)\n", key, value);
			f.write("\t\t{\n");
			f.write("\t\t\tcomposer.compose%s(each.Key);\n", idlKey);
			f.write("\t\t\tcomposer.compose%s(each.Value);\n", idlValue);
			f.write("\t\t}\n");
			break;

		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\t\tforeach(KeyValuePair<%s, I%s> each in val)\n", key, value);
			f.write("\t\t{\n");
			f.write("\t\t\tcomposer.compose%s(each.Key);\n", idlKey);
			f.write("\t\t\t%s_message.compose(composer, each.Value);\n", value);
			f.write("\t\t}\n");
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}

		f.write("\t}\n");


		f.write("\tpublic static void parse(JsonParser parser, %s res)\n", iface.c_str());
		f.write("\t{\n");

		f.write("\t\tparser.skipDelimiter('{');\n");
		f.write("\t\tif (parser.isDelimiter('}'))\n");
		f.write("\t\t{\n");
		f.write("\t\t\tparser.skipDelimiter('}');\n");
		//f.write("\t\t\tif (parser.isDelimiter(','))\n");
		//f.write("\t\t\t\tparser.skipDelimiter(',');\n");
		f.write("\t\t\treturn;\n");
		f.write("\t\t}\n");

		f.write("\t\twhile(true)\n");
		f.write("\t\t{\n");
		f.write("\t\t\tparser.skipDelimiter('{');\n");
		f.write("\t\t\t%s key;\n", key);
		f.write("\t\t\tparser.parse%s(out key);\n", idlKey);
		f.write("\t\t\tparser.skipDelimiter(',');\n");

		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
			const char* idlValue = getIdlPrimitiveType2(member.type.dictionaryValueKind);
			f.write("\t\t\t%s val;\n", value);
			f.write("\t\t\tparser.parse%s(out val);\n", idlValue);
			break;

		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\t\t\t%s val = new %s();\n", value, value);
			f.write("\t\t\t%s_message.parse(parser, val);\n", value);
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}

		f.write("\t\t\tparser.skipDelimiter('}');\n");
		f.write("\t\t\tres.Add(key, val);\n");
	
		f.write("\t\t\tif (parser.isDelimiter(','))\n");
		f.write("\t\t\t{\n");
		f.write("\t\t\t\tparser.skipDelimiter(',');\n");
		f.write("\t\t\t\tcontinue;\n");
		f.write("\t\t\t}\n");

		f.write("\t\t\tif (parser.isDelimiter('}'))\n");
		f.write("\t\t\t{\n");
		f.write("\t\t\t\tparser.skipDelimiter('}');\n");
		f.write("\t\t\t\tbreak;\n");
		f.write("\t\t\t}\n");
		f.write("\t\t}\n");

		//f.write("\t\tif (parser.isDelimiter(','))\n");
		//f.write("\t\t\tparser.skipDelimiter(',');\n");
		
		f.write("\t}\n");


		f.write("\tpublic static void parse(GmqParser parser, %s res)\n", iface.c_str());
		f.write("\t{\n");
		f.write("\t\tint sz;\n");
		f.write("\t\tparser.parseUnsignedInteger(out sz);\n");
		f.write("\t\tfor(int i = 0; i < sz; ++i)\n");
		f.write("\t\t{\n");
		f.write("\t\t\t%s key;\n", key);
		f.write("\t\t\tparser.parse%s(out key);\n", idlKey);

		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
			const char* idlValue = getIdlPrimitiveType2(member.type.dictionaryValueKind);
			f.write("\t\t\t%s val;\n", value);
			f.write("\t\t\tparser.parse%s(out val);\n", idlValue);
			break;

		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\t\t\t%s val = new %s();\n", value, value);
			f.write("\t\t\t%s_message.parse(parser, val);\n", value);
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}

		f.write("\t\t\tres.Add(key, val);\n");
		f.write("\t\t}\n");
		f.write("\t}\n");

		f.write("} // class %s_message\n\n", prefix.c_str());
	}

	void csharpMsg_generateVectorMessage(FileWritter f, MessageParameter& member, std::set<std::string>& alreadyDone)
	{
		assert(member.type.kind == MessageParameterType::KIND::VECTOR);

		string type = getCSharpInterfaceType(member.type);
		string prefix = getCSharpPrefixByType(member.type);
		
		if(alreadyDone.find(prefix) != alreadyDone.end())
			return;

		alreadyDone.insert(prefix);

		f.write("public class %s_message\n", prefix.c_str());
		f.write("{\n");


		f.write("\tpublic static void compose(JsonComposer composer, %s val)\n", type.c_str());
		f.write("\t{\n");

		f.write("\t\tcomposer.append('[');\n");
		f.write("\t\tfor (int i = 0; i < val.Count; ++i)\n");
		f.write("\t\t{\n");
		f.write("\t\t\tif (i != 0)\n");
		f.write("\t\t\t\tcomposer.append(\", \");\n");

		int count = 0;
		switch (member.type.vectorElemKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* idlValue = getIdlPrimitiveType2(member.type.vectorElemKind);
			f.write("\t\t\tcomposer.compose%s(val[i]);\n", idlValue);
			break;

		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\t\t\t%s_message.compose(composer, val[i]);\n", value);
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}

		f.write("\t\t}\n");
		f.write("\t\tcomposer.append(']');\n");
		f.write("\t}\n");


		f.write("\tpublic static void compose(GmqComposer composer, %s val)\n", type.c_str());
		f.write("\t{\n");
		f.write("\t\tcomposer.composeUnsignedInteger((UInt64)val.Count);\n");
		f.write("\t\tfor (int i = 0; i < val.Count; ++i)\n");
		f.write("\t\t{\n");

		switch (member.type.vectorElemKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* idlValue = getIdlPrimitiveType2(member.type.vectorElemKind);
			f.write("\t\t\tcomposer.compose%s(val[i]);\n", idlValue);
			break;

		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\t\t\t%s_message.compose(composer, val[i]);\n", value);
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}
		f.write("\t\t}\n");
		f.write("\t}\n");


		f.write("\tpublic static void parse(JsonParser parser, %s res)\n", type.c_str());
		f.write("\t{\n");

		f.write("\t\tparser.skipDelimiter('[');\n");
		f.write("\t\tif (parser.isDelimiter(']'))\n");
		f.write("\t\t{\n");
		f.write("\t\t\tparser.skipDelimiter(']');\n");
		//f.write("\t\t\tif (parser.isDelimiter(','))\n");
		//f.write("\t\t\t\tparser.skipDelimiter(',');\n");
		f.write("\t\t\treturn;\n");
		f.write("\t\t}\n");

		f.write("\t\twhile(true)\n");
		f.write("\t\t{\n");
		//f.write("\t\t\tparser.skipDelimiter('{');\n");
		//f.write("\t\t\t%s key;\n", key);
		//f.write("\t\t\tparser.parse%s(out key);\n", idlKey);
		//f.write("\t\t\tparser.skipDelimiter(',');\n");

		switch (member.type.vectorElemKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.vectorElemKind);
			const char* idlValue = getIdlPrimitiveType2(member.type.vectorElemKind);
			f.write("\t\t\t%s val;\n", value);
			f.write("\t\t\tparser.parse%s(out val);\n", idlValue);
			break;

		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\t\t\t%s val = new %s();\n", value, value);
			f.write("\t\t\t%s_message.parse(parser, val);\n", value);
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}

		//f.write("\t\t\tparser.skipDelimiter('}');\n");
		f.write("\t\t\tres.Add(val);\n");

		f.write("\t\t\tif (parser.isDelimiter(','))\n");
		f.write("\t\t\t{\n");
		f.write("\t\t\t\tparser.skipDelimiter(',');\n");
		f.write("\t\t\t\tcontinue;\n");
		f.write("\t\t\t}\n");

		f.write("\t\t\tif (parser.isDelimiter(']'))\n");
		f.write("\t\t\t{\n");
		f.write("\t\t\t\tparser.skipDelimiter(']');\n");
		f.write("\t\t\t\tbreak;\n");
		f.write("\t\t\t}\n");
		f.write("\t\t}\n");

		//f.write("\t\tif (parser.isDelimiter(','))\n");
		//f.write("\t\t\tparser.skipDelimiter(',');\n");

		f.write("\t}\n");


		f.write("\tpublic static void parse(GmqParser parser, %s res)\n", type.c_str());
		f.write("\t{\n");
		f.write("\t\tint sz;\n");
		f.write("\t\tparser.parseUnsignedInteger(out sz);\n");
		f.write("\t\tfor(int i = 0; i < sz; ++i)\n");
		f.write("\t\t{\n");

		switch (member.type.vectorElemKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.vectorElemKind);
			const char* idlValue = getIdlPrimitiveType2(member.type.vectorElemKind);
			f.write("\t\t\t%s val;\n", value);
			f.write("\t\t\tparser.parse%s(out val);\n", idlValue);
			break;

		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\t\t\t%s val = new %s();\n", value, value);
			f.write("\t\t\t%s_message.parse(parser, val);\n", value);
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}

		f.write("\t\t\tres.Add(val);\n");
		f.write("\t\t}\n");
		f.write("\t}\n");

		f.write("} // class %s_message\n\n", prefix.c_str());
	}

void generateCsharpStructMessage(FileWritter f, CompositeType& s, const char* type_name, const char* interface_name)
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

	// this should be made only once for each type, and
	// put at namespace scope
	std::set<std::string> alreadyDone;
	auto& mem = s.getMembers();
	for (auto it = mem.begin(); it != mem.end(); ++it)
	{
		assert(*it != nullptr);

		MessageParameter& member = **it;
		if (member.type.kind == MessageParameterType::KIND::EXTENSION)
			continue;
		switch (member.type.kind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			break;
		case MessageParameterType::KIND::VECTOR:
			csharpMsg_generateVectorMessage(f, member, alreadyDone);
			break;
		case MessageParameterType::KIND::DICTIONARY:
			csharpMsg_generateDictionaryMessage(f, member, alreadyDone);
			break;
		default:
			break;
		}
	}


	f.write("} // class %s_message\n\n", type_name);



}

void generateCsharpMessageScope(FileWritter f, Root& root, Scope& scope)
{
	f.write("public class %s\n", scope.name.c_str());
	f.write("{\n");

	csharpMsg_generateScopeEnum(f, scope);
	f.write("\n");
	csharpMsg_generateScopeHandler(f, scope);
	f.write("\n");

	for (auto it : scope.objectList)
	{
		assert(it != nullptr);
		assert(it->type == CompositeType::Type::message);

		// checkCsharpStruct(*it);

		if (!it->isAlias)
		{
			std::string type_name = it->name;
			std::string interface_name = "I" + type_name;

			generateCsharpStructInterface(f.indent(), *it, type_name.c_str());
			generateCsharpStructImpl(f.indent(), *it, type_name.c_str(), interface_name.c_str());
			generateCsharpStructMessage(f.indent(), *it, type_name.c_str(), interface_name.c_str());
			csharpMsg_generateComposeMessageMethod(f, *it, it->name, scope);
			csharpMsg_generateParseMessageMethod(f, type_name.c_str(), type_name.c_str(), scope);
		}
		else
		{
			assert(it->aliasIdx < root.structs.size());
			auto& alias = root.structs[static_cast<decltype(root.structs)::size_type>(it->aliasIdx)];

			csharpMsg_generateComposeMessageMethod(f, *alias, it->name, scope);
			csharpMsg_generateParseMessageMethod(f, it->name.c_str(), alias->name.c_str(), scope);
		}

	}

	f.write("} // class %s\n\n", scope.name.c_str());
}

}