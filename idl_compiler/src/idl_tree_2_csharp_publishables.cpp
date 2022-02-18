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

	void csharpPub_generateAddressEnum(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		f.write("\tenum Address\n");
		f.write("\t{\n");

		auto& mem = s.getMembers();
		size_t sz = mem.size();
		for (size_t i = 0; i != sz; ++i)
		{
			auto& member = mem[i];
			assert(member != nullptr);

			string number = std::to_string(i);
			if(i != sz - 1)
				f.write("%s = %s,\n", member->name.c_str(), number.c_str());
			else
				f.write("%s = %s\n", member->name.c_str(), number.c_str());
		}
		f.write("}\n");
	}

	void csharpPub_generateParseStateSync(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic static void parseForStateSync(IPublishableParser parser, %s_subscriber subscriber)\n", type_name.c_str());
		f.write("\t{\n");

		generateCsharpSubscriberParseForStateSync(header, s);
		
		f.write("\t}\n");
	}

	void csharpPub_generateParse1(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber)\n", type_name.c_str());
		f.write("\t{\n");

		f.write("\t\tbool changed = false;\n");

		generateCsharpSubscriberParse1(header, s);

		f.write("\t\treturn changed;\n");
		f.write("\t}\n");
	}

	void csharpPub_generateParse2(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic static bool parse(IPublishableParser parser, %s_subscriber subscriber, UInt64[] addr, int offset)\n", type_name.c_str());
		f.write("\t{\n");

		f.write("\t\tbool changed = false;\n");

		generateCsharpSubscriberParse2(header, s);

		f.write("\t\treturn changed;\n");
		f.write("\t}\n");
	}

	void csharpPub_generateStateSubscriberBase(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic String stateSubscriberName() { return \"%s\"; }\n", s.name.c_str());

		string number = std::to_string(s.numID);
		f.write("\tpublic UInt64 stateTypeID() { return %s; }\n", number.c_str());

		f.write("\tpublic void applyMessageWithUpdates(IPublishableParser parser)\n");
		f.write("\t{\n");
		f.write("\t\tparser.parseStateUpdateMessageBegin();\n");
		f.write("\t\tUInt64[] addr = null;\n");
		f.write("\t\tbool changed = false;\n");
		f.write("\t\twhile(parser.parseAddress(ref addr))\n");
		f.write("\t\t{\n");
		f.write("\t\t\tchanged = %s_subscriber.parse(parser, this, addr, 0) | changed;\n", type_name.c_str());
		f.write("\t\t\tparser.parseAddressEnd();\n");
		f.write("\t\t\taddr = null;\n");
		f.write("\t\t}\n");
		f.write("\t\tparser.parseStateUpdateMessageEnd();\n");
		f.write("\t\tif(changed)\n");
		f.write("\t\t\tthis.notifyUpdated();\n");
		f.write("\t}\n");

		f.write("\tpublic void applyStateSyncMessage(IPublishableParser parser)\n");
		f.write("\t{\n");
		f.write("\t\tparser.parseStructBegin();\n");
		f.write("\t\t%s_subscriber.parseForStateSync(parser, this);\n", type_name.c_str());
		f.write("\t\tparser.parseStructEnd();\n");
		f.write("\t\tthis.notifyFullyUpdated();\n");
		f.write("\t}\n");
	}

	void csharpPub_generateEventHandlerSubs(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		if (s.type == CompositeType::Type::publishable)
		{
			f.write("\tpublic virtual void notifyFullyUpdated() { }\n");
			f.write("\tpublic virtual void notifyUpdated() { }\n");
		}
		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			generateCsharpSubscriberEventHandler(header, *it);
		}
	}



	void csharpPub_generateFactoryMethodSubs(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			generateCsharpSubscriberFactoryMethod(header, *it);
		}
	}

	void csharpPub_generateSubscriberResetHandlers(FILE* header, CompositeType& s)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic void reset_handlers()\n");
		f.write("\t{\n");

		for (auto& each : s.getMembers())
		{
			auto& member = *each;
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
		f.write("\t}\n");
	}

	void csharpPub_generateStructSubs(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		if(s.type == CompositeType::Type::publishable)
			f.write("public class %s_subscriber : I%s, StateSubscriberBase\n", type_name.c_str(), type_name.c_str());
		else
			f.write("public class %s_subscriber : I%s\n", type_name.c_str(), type_name.c_str());


		f.write("{\n");

		f.write("\n");
		f.write("\t/////////////////////////////////  begin user override section /////////////////////////////////\n");
		f.write("\n");

		csharpPub_generateFactoryMethodSubs(header, s, type_name);
		csharpPub_generateEventHandlerSubs(header, s, type_name);

		f.write("\n");
		f.write("\t/////////////////////////////////   end user override section  /////////////////////////////////\n");
		f.write("\n");
		f.write("\n");


		f.write("\tprotected I%s data;\n", type_name.c_str());

		csharpPub_generateAddressEnum(header, s);

		if (s.type == CompositeType::Type::publishable)
			f.write("\tpublic %s_subscriber() { this.data = new %s(); }\n", type_name.c_str(), type_name.c_str());
		else
			f.write("\tpublic %s_subscriber(I%s data) { this.data = data; }\n", type_name.c_str(), type_name.c_str());


		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			generateCsharpSubscriberMember(header, *it);
		}

		generateCsharpSimpleEquivalentMethod(header, type_name.c_str(), "data");

		csharpPub_generateParseStateSync(header, s, type_name);
		csharpPub_generateParse1(header, s, type_name);
		csharpPub_generateParse2(header, s, type_name);

		if (s.type == CompositeType::Type::publishable)
			csharpPub_generateStateSubscriberBase(header, s, type_name);

		csharpPub_generateSubscriberResetHandlers(header, s);

		f.write("\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
		f.write("\tpublic void debugOnlySetData(I%s data)\n", type_name.c_str());
		f.write("\t{\n");
		f.write("\t\tthis.data = data;\n");
		f.write("\t\treset_handlers();\n");
		f.write("\t}\n");


		f.write("} // class %s_subscriber\n\n", type_name.c_str());
	}

	void csharpPub_generateCompose(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic static void compose(IPublishableComposer composer, I%s t)\n", type_name.c_str());
		f.write("\t{\n");

		generateCsharpPublisherCompose(header, s);

		f.write("\t}\n");
	}


	void csharpPub_generateStatePublishableBase(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic int idx { get; set; } // for use in pools, etc\n");
		f.write("\tpublic String statePublisherName() { return \"%s\"; }\n", s.name.c_str());
		string number = std::to_string(s.numID);
		f.write("\tpublic UInt64 stateTypeID() { return %s; }\n", number.c_str());

		f.write("\tpublic void generateStateSyncMessage(IPublishableComposer composer)\n");
		f.write("\t{\n");
		f.write("\t\tcomposer.composeStructBegin();\n");
		f.write("\t\t%s_publisher.compose(composer, this.t);\n", type_name.c_str());
		f.write("\t\tcomposer.composeStructEnd();\n");
		f.write("\t}\n");
		
		f.write("\tpublic void startTick(IPublishableComposer composer)\n");
		f.write("\t{\n");
		f.write("\t\tthis.composer = composer;\n");
		f.write("\t\tcomposer.composeStateUpdateMessageBegin();\n");
		f.write("\t}\n");

		f.write("\tpublic IPublishableComposer endTick()\n");
		f.write("\t{\n");
		f.write("\t\tcomposer.composeStateUpdateMessageEnd();\n");
		f.write("\t\tIPublishableComposer tmp = composer;\n");
		f.write("\t\tthis.composer = null;\n");
		f.write("\t\treturn tmp;\n");
		f.write("\t}\n");
	}


	void csharpPub_generateStructPubl(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		if (s.type == CompositeType::Type::publishable)
			f.write("public class %s_publisher : I%s, StatePublisherBase\n", type_name.c_str(), type_name.c_str());
		else
			f.write("public class %s_publisher : I%s\n", type_name.c_str(), type_name.c_str());


		f.write("\n");


		f.write("{\n");
		f.write("\tI%s t;\n", type_name.c_str());
		f.write("\tIPublishableComposer composer;\n");
		f.write("\tUInt64[] address;\n");

		csharpPub_generateAddressEnum(header, s);

		if (s.type == CompositeType::Type::publishable)
		{
			f.write("\tpublic %s_publisher()\n", type_name.c_str());
			f.write("\t{\n");
			f.write("\t\tthis.t = new %s();\n", type_name.c_str());
			f.write("\t\tthis.composer = null;\n");
			f.write("\t\tthis.address = new UInt64[] { };\n");
			f.write("\t}\n");
		}
		else
		{
			f.write("\tpublic %s_publisher(I%s t, IPublishableComposer composer, UInt64[] address)\n", type_name.c_str(), type_name.c_str());
			f.write("\t{\n");
			f.write("\t\tthis.t = t;\n");
			f.write("\t\tthis.composer = composer;\n");
			f.write("\t\tthis.address = address;\n");
			f.write("\t}\n");
		}
		auto& mem = s.getMembers();
		for (size_t i = 0; i < mem.size(); ++i)
		{
			auto& it = mem[i];
			assert(it != nullptr);
			generateCsharpPublisherMember(header, *it);
		}

		generateCsharpSimpleEquivalentMethod(header, type_name.c_str(), "t");

		csharpPub_generateCompose(header, s, type_name);

		if (s.type == CompositeType::Type::publishable)
			csharpPub_generateStatePublishableBase(header, s, type_name);


		//f.write("\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
		//f.write("\tpublic I%s debugOnlyGetData() { return this.t; }\n", type_name.c_str());

		f.write("\t/// <summary>This method is for testing and debugging only. Do not use!</summary>\n");
		f.write("\tpublic void debugOnlySetData(I%s data) { this.t = data; }\n", type_name.c_str());


		f.write("} // class %s_publisher\n\n", type_name.c_str());
	}

	void csharpPub_generateStateConcentratorBase(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable);

		CsharpFileWritter f(header, 0);

		f.write("\tpublic void generateStateSyncMessage(IPublishableComposer composer)\n");
		f.write("\t{\n");
		f.write("\t\tcomposer.composeStructBegin();\n");
		f.write("\t\t%s_publisher.compose(composer, this.data);\n", type_name.c_str());
		f.write("\t\tcomposer.composeStructEnd();\n");
		f.write("\t}\n");

	}


	void csharpPub_generateStructConcentrator(FILE* header, CompositeType& s, const std::string& type_name)
	{
		assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure);

		CsharpFileWritter f(header, 0);

		f.write("public class %s_concentrator : %s_subscriber, StateConcentratorBase\n", type_name.c_str(), type_name.c_str());

		f.write("{\n");
		csharpPub_generateCompose(header, s, type_name);
		csharpPub_generateStateConcentratorBase(header, s, type_name);

		f.write("} // class %s_concentrator\n\n", type_name.c_str());
	}

	void csharpPub_generateConcentratorFactory(FILE* header, Root& root)
	{
		assert(!root.publishables.empty());

		CsharpFileWritter f(header, 0);

		f.write("public class StateConcentratorFactory : IStateConcentratorFactory\n");
		f.write("{\n");
		f.write("\tpublic StateConcentratorBase createConcentrator(UInt64 typeID)\n");
		f.write("\t{\n");
		f.write("\t\tswitch(typeID)\n");
		f.write("\t\t{\n");
		for (auto& it : root.publishables)
		{
			auto& obj_1 = it;
			assert(it != nullptr);
			assert(typeid(*(it)) == typeid(CompositeType));
			assert(it->type == CompositeType::Type::publishable);
			string number = std::to_string(it->numID);
			f.write("\t\tcase %s:\n", number.c_str());
			f.write("\t\t\treturn new %s_concentrator();\n", it->name.c_str());
		}
		f.write("\t\tdefault:\n");
		f.write("\t\t\treturn null;\n");
		f.write("\t\t}\n");

		f.write("\t}\n");
		f.write("} // class StateConcentratorFactory\n\n");
	}
}

void generateCsharpSubscriberFactoryMethod(FILE* header, MessageParameter& member)
{
	CsharpFileWritter f(header, 0);

	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
		break;
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		f.write("\tpublic virtual %s_subscriber makeHandler_%s(I%s data) { return new %s_subscriber(data); }\n", member.type.name.c_str(), member.name.c_str(), member.type.name.c_str(), member.type.name.c_str());
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
		{
			const char* elem_type_name = member.type.name.c_str();
			f.write("\tpublic virtual %s_subscriber makeElementHandler_%s(I%s data) { return new %s_subscriber(data); }\n", elem_type_name, member.name.c_str(), elem_type_name, elem_type_name);
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

void generateCsharpSubscriberEventHandler(FILE* header, MessageParameter& member)
{
	CsharpFileWritter f(header, 0);

	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
	{
		const char* type_name = getCSharpPrimitiveType(member.type.kind);
		f.write("\tpublic virtual void notifyUpdated_%s(%s old) { }\n", member.name.c_str(), type_name);
		break;
	}
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		f.write("\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
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
			f.write("\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyElementUpdated_%s(int index, %s old) { }\n", member.name.c_str(), elem_type_name);
			f.write("\tpublic virtual void notifyInserted_%s(int index) { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyErased_%s(int index) { }\n", member.name.c_str());
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			f.write("\tpublic virtual void notifyUpdated_%s() { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyElementUpdated_%s(int index) { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyInserted_%s(int index) { }\n", member.name.c_str());
			f.write("\tpublic virtual void notifyErased_%s(int index) { }\n", member.name.c_str());
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

void generateCsharpSubscriberMember(FILE* header, MessageParameter& member)
{
	CsharpFileWritter f(header, 0);

	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
	{
		const char* type_name = getCSharpPrimitiveType(member.type.kind);
		f.write("\tpublic %s %s\n", type_name, member.name.c_str());
		f.write("\t{\n");
		f.write("\t\tget { return data.%s; }\n", member.name.c_str());
		f.write("\t\tset { throw new InvalidOperationException(); }\n");
		f.write("\t}\n");
		f.write("\tbool update_%s(%s newVal)\n", member.name.c_str(), getCSharpPrimitiveType(member.type.kind));
		f.write("\t{\n");
		f.write("\t\tif (newVal != data.%s)\n", member.name.c_str());
		f.write("\t\t{\n");
		f.write("\t\t\t%s oldVal = data.%s;\n", type_name, member.name.c_str());
		f.write("\t\t\tdata.%s = newVal;\n", member.name.c_str());
		f.write("\t\t\tnotifyUpdated_%s(oldVal);\n", member.name.c_str());
		f.write("\t\t\treturn true;\n");
		f.write("\t\t}\n");
		f.write("\t\telse\n");
		f.write("\t\t\treturn false;\n");
		f.write("\t}\n");
		break;
	}
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		f.write("\t%s_subscriber %s_handler;\n", member.type.name.c_str(), member.name.c_str());
		f.write("\t%s_subscriber lazy_%s_handler()\n", member.type.name.c_str(), member.name.c_str());
		f.write("\t{ // mb: lazy because can't call virtual 'makeHandler' in ctor\n");
		f.write("\t\tif (%s_handler == null)\n", member.name.c_str());
		f.write("\t\t\t%s_handler = makeHandler_%s(data.%s);\n", member.name.c_str(), member.name.c_str(), member.name.c_str());
		f.write("\t\treturn %s_handler;\n", member.name.c_str());
		f.write("\t}\n");


		f.write("\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());
		f.write("\t{\n");
		f.write("\t\tget { return lazy_%s_handler(); }\n", member.name.c_str());
		f.write("\t\tset { throw new InvalidOperationException(); }\n");
		f.write("\t}\n");
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
			f.write("\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget { return new SubscriberVectorWrapper<%s, %s>(data.%s); }\n", elem_type_name, elem_type_name, member.name.c_str());
			f.write("\t\tset { throw new InvalidOperationException(); }\n");
			f.write("\t}\n");
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* elem_type_name = member.type.name.c_str();
			f.write("\tList<%s_subscriber> %s_handlers;\n", elem_type_name, member.name.c_str());
			f.write("\tList<%s_subscriber> lazy_%s_handlers()\n", elem_type_name, member.name.c_str());
			f.write("\t{ // mb: lazy because can't call virtual 'makeElementHandler' in ctor\n");
			f.write("\t\tif (%s_handlers == null)\n", member.name.c_str());
			f.write("\t\t{\n");
			f.write("\t\t\t%s_handlers = new List<%s_subscriber>();\n", member.name.c_str(), elem_type_name);
			f.write("\t\t\tfor (int i = 0; i < data.%s.Count; ++i)\n", member.name.c_str());
			f.write("\t\t\t{\n");
			f.write("\t\t\t\t%s_subscriber handler = makeElementHandler_%s(data.%s[i]);\n", elem_type_name, member.name.c_str(), member.name.c_str());
			f.write("\t\t\t\t%s_handlers.Add(handler);\n", member.name.c_str());
			f.write("\t\t\t}\n");
			f.write("\t\t}\n");
			f.write("\t\treturn %s_handlers;\n", member.name.c_str());
			f.write("\t}\n");
			f.write("\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget { return new SubscriberVectorWrapper<I%s, %s_subscriber>(lazy_%s_handlers()); }\n", elem_type_name, elem_type_name, member.name.c_str());
			f.write("\t\tset { throw new InvalidOperationException(); }\n");
			f.write("\t}\n");
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

void generateCsharpPublisherMember(FILE* header, MessageParameter& member)
{
	CsharpFileWritter f(header, 0);

	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
		f.write("\tpublic %s %s\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
		f.write("\t{\n");
		f.write("\t\tget { return t.%s; }\n", member.name.c_str());
		f.write("\t\tset\n");
		f.write("\t\t{\n");
		//f.write("\t\t\tif (value != t.%s)\n", member.name.c_str());
		//f.write("\t\t\t{\n");
		f.write("\t\t\tt.%s = value;\n", member.name.c_str());
		f.write("\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
		f.write("\t\t\tcomposer.compose%s(\"value\", value, false);\n", getIdlPrimitiveType(member.type.kind));
		f.write("\t\t\tcomposer.composeAddressEnd();\n");
		//f.write("\t\t\t}\n");
		f.write("\t\t}\n");
		f.write("\t}\n");
		break;
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		f.write("\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());
		f.write("\t{\n");
		f.write("\t\tget { return new %s_publisher(t.%s, composer, Publishable.makeAddress(address, (UInt64)Address.%s)); }\n", member.type.name.c_str(), member.name.c_str(), member.name.c_str());
		f.write("\t\tset\n");
		f.write("\t\t{\n");
		//f.write("\t\t\tif (value != t.%s)\n", member.name.c_str());
		//f.write("\t\t\t{\n");
		f.write("\t\t\tt.%s = value;\n", member.name.c_str());
		f.write("\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
		f.write("\t\t\tcomposer.composePublishableStructBegin(\"value\");\n");
		f.write("\t\t\t%s_publisher.compose(composer, value);\n", member.type.name.c_str());
		f.write("\t\t\tcomposer.composePublishableStructEnd(false);\n");
		f.write("\t\t\tcomposer.composeAddressEnd();\n");
		//f.write("\t\t\t}\n");
		f.write("\t\t}\n");
		f.write("\t}\n");

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
			const char* compose_method = getIdlPrimitiveType(member.type.vectorElemKind);


			f.write("\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget\n");
			f.write("\t\t{\n");
			f.write("\t\t\treturn new PublisherVectorWrapper<%s>(t.%s, composer,\n", elem_type_name, member.name.c_str());
			f.write("\t\t\t\tPublishable.makeAddress(address, (UInt64)Address.%s),\n", member.name.c_str());
			f.write("\t\t\t\t(IPublishableComposer composer, %s val) => { composer.compose%s(\"value\", val, false); },\n", elem_type_name, compose_method);
			f.write("\t\t\t\tnull\n");
			f.write("\t\t\t);\n");
			f.write("\t\t}\n");
			f.write("\t\tset\n");
			f.write("\t\t{\n");
			f.write("\t\t\tt.%s = value;\n", member.name.c_str());
			f.write("\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
			f.write("\t\t\tcomposer.composeSimpleVector(\"value\", value, false);\n");
			f.write("\t\t\tcomposer.composeAddressEnd();\n");
			f.write("\t\t}\n");
			f.write("\t}\n");
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* elem_type_name = member.type.name.c_str();

			f.write("\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget\n");
			f.write("\t\t{\n");
			f.write("\t\t\treturn new PublisherVectorWrapper<I%s>(t.%s, composer,\n", elem_type_name, member.name.c_str());
			f.write("\t\t\t\tPublishable.makeAddress(address, (UInt64)Address.%s),\n", member.name.c_str());
			f.write("\t\t\t\t(IPublishableComposer composer, I%s val) =>\n", elem_type_name);
			f.write("\t\t\t\t{\n");
			f.write("\t\t\t\t\tcomposer.composePublishableStructBegin(\"value\");\n");
			f.write("\t\t\t\t\t%s_publisher.compose(composer, val);\n", elem_type_name);
			f.write("\t\t\t\t\tcomposer.composePublishableStructEnd(false);\n");
			f.write("\t\t\t\t},\n");
			f.write("\t\t\t\t(I%s val, IPublishableComposer composer, UInt64[] addr) =>\n", elem_type_name);
			f.write("\t\t\t\t{ return new %s_publisher(val, composer, addr); }\n", elem_type_name);
			f.write("\t\t\t);\n");
			f.write("\t\t}\n");
			f.write("\t\tset\n");
			f.write("\t\t{\n");
			f.write("\t\t\tt.%s = value;\n", member.name.c_str());
			f.write("\t\t\tcomposer.composeAddress(address, (UInt64)Address.%s);\n", member.name.c_str());
			f.write("\t\t\tcomposer.composeVector(\"value\", value.Count,\n");
			f.write("\t\t\t\t(IPublishableComposer composer, int ordinal) => { %s_publisher.compose(composer, value[ordinal]); },\n", elem_type_name);
			f.write("\t\t\t\tfalse);\n");
			f.write("\t\t\tcomposer.composeAddressEnd();\n");
			f.write("\t\t}\n");
			f.write("\t}\n");
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


void generateCsharpPublisherCompose(FILE* header, CompositeType& s)
{
	assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case);

	CsharpFileWritter f(header, 0);

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
			f.write("\t\tcomposer.compose%s(\"%s\", t.%s, ", getIdlPrimitiveType(member.type.kind), member.name.c_str(), member.name.c_str());
			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			f.write("\t\tcomposer.composePublishableStructBegin(\"%s\");\n", member.name.c_str());
			f.write("\t\t%s_publisher.compose(composer, t.%s);\n", member.type.name.c_str(), member.name.c_str());
			f.write("\t\tcomposer.composePublishableStructEnd(");
			break;
		case MessageParameterType::KIND::VECTOR:
		{
			switch (member.type.vectorElemKind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				f.write("\t\tcomposer.composeSimpleVector(\"%s\", t.%s, ", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				const char* elem_type_name = member.type.name.c_str();
				f.write("\t\tcomposer.composeVector(\"%s\", t.%s.Count,\n", member.name.c_str(), member.name.c_str());
				f.write("\t\t\t(IPublishableComposer composer, int ordinal) => { %s_publisher.compose(composer, t.%s[ordinal]); }, ", elem_type_name, member.name.c_str());
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

		if (i == mem.size() - 1)
			f.write("false);\n");
		else
			f.write("true);\n");

	}
}

void generateCsharpSubscriberParseForStateSync(FILE* header, CompositeType& s)
{
	assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case);

	CsharpFileWritter f(header, 0);

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
			const char* parse_method = getIdlPrimitiveType(member.type.kind);
			f.write("\t\tsubscriber.data.%s = parser.parse%s(\"%s\");\n", member.name.c_str(), parse_method, member.name.c_str());
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			f.write("\t\tparser.parsePublishableStructBegin(\"%s\");\n", member.name.c_str());
			f.write("\t\t%s_subscriber.parseForStateSync(parser, subscriber.lazy_%s_handler());\n", member.type.name.c_str(), member.name.c_str());
			f.write("\t\tparser.parsePublishableStructEnd();\n");
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
				f.write("\t\tparser.parseSimpleVector(\"%s\", subscriber.data.%s);\n", member.name.c_str(), member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				const char* elem_type_name = member.type.name.c_str();

				f.write("\t\tparser.parseVector(\"%s\", (IPublishableParser parser, int index) =>\n", member.name.c_str());
				f.write("\t\t\t{\n");
				f.write("\t\t\t\tparser.parseStructBegin();\n");
				f.write("\t\t\t\tI%s val = new %s();\n", elem_type_name, elem_type_name);
				f.write("\t\t\t\t%s_subscriber handler = subscriber.makeElementHandler_%s(val);\n", elem_type_name, member.name.c_str());
				f.write("\t\t\t\t%s_subscriber.parseForStateSync(parser, handler);\n", elem_type_name);
				f.write("\t\t\t\t// mb: lazy initialization always first\n");
				f.write("\t\t\t\tsubscriber.lazy_%s_handlers().Add(handler);\n", member.name.c_str());
				f.write("\t\t\t\tsubscriber.data.%s.Add(val);\n", member.name.c_str());
				f.write("\t\t\t\tparser.parseStructEnd();\n");
				f.write("\t\t\t}\n");
				f.write("\t\t);\n");
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

void generateCsharpSubscriberParse1(FILE* header, CompositeType& s)
{
	assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case);

	CsharpFileWritter f(header, 0);

	auto& mem = s.getMembers();
	for (size_t i = 0; i < mem.size(); ++i)
	{
		auto& it = mem[i];
		assert(it != nullptr);
		auto& member = *it;

		f.write("\t\t{\n");

		switch (member.type.kind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* type_name = getCSharpPrimitiveType(member.type.kind);
			const char* parse_method = getIdlPrimitiveType(member.type.kind);

			f.write("\t\t\t%s newVal = parser.parse%s(\"%s\");\n", type_name, parse_method, member.name.c_str());
			f.write("\t\t\tchanged = subscriber.update_%s(newVal) | changed;\n", member.name.c_str());
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			f.write("\t\t\tparser.parsePublishableStructBegin(\"%s\");\n", member.name.c_str());
			f.write("\t\t\tbool currentChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handler());\n", member.type.name.c_str(), member.name.c_str());
			f.write("\t\t\tparser.parsePublishableStructEnd();\n");
			f.write("\t\t\tif(currentChanged)\n");
			f.write("\t\t\t{\n");
			f.write("\t\t\t\t\tchanged = true;\n");
			f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
			f.write("\t\t\t}\n");
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

				f.write("\t\tIList<%s> newVal = new List<%s>();\n", elem_type_name, elem_type_name);
				f.write("\t\tparser.parseSimpleVector(\"%s\", newVal);\n", member.name.c_str());
				f.write("\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.data.%s))\n", member.name.c_str());
				f.write("\t\t{\n");
				f.write("\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
				f.write("\t\t\tchanged = true;\n");
				f.write("\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
				f.write("\t\t}\n");

				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				const char* elem_type_name = member.type.name.c_str();

				f.write("\t\tList<I%s> newVal = new List<I%s>();\n", elem_type_name, elem_type_name);
				f.write("\t\tList<%s_subscriber> newHandlers = new List<%s_subscriber>();\n", elem_type_name, elem_type_name);
				f.write("\t\tparser.parseVector(\"%s\", (IPublishableParser parser, int index) =>\n", member.name.c_str());
				f.write("\t\t\t{\n");
				f.write("\t\t\t\tparser.parseStructBegin();\n");
				f.write("\t\t\t\tI%s val = new %s();\n", elem_type_name, elem_type_name);
				f.write("\t\t\t\t%s_subscriber handler = subscriber.makeElementHandler_%s(val);\n", elem_type_name, member.name.c_str());
				f.write("\t\t\t\t%s_subscriber.parseForStateSync(parser, handler);\n", elem_type_name);
				f.write("\t\t\t\tnewVal.Add(val);\n");
				f.write("\t\t\t\tnewHandlers.Add(handler);\n");
				f.write("\t\t\t\tparser.parseStructEnd();\n");
				f.write("\t\t\t}\n");
				f.write("\t\t);\n");
				f.write("\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.data.%s))\n", member.name.c_str());
				f.write("\t\t{\n");
				f.write("\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
				f.write("\t\t\tsubscriber.%s_handlers = newHandlers;\n", member.name.c_str());
				f.write("\t\t\tchanged = true;\n");
				f.write("\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
				f.write("\t\t}\n");
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
		f.write("\t\t}\n");
	}
}

void generateCsharpSubscriberParse2(FILE* header, CompositeType& s)
{
	assert(s.type == CompositeType::Type::publishable || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case);

	CsharpFileWritter f(header, 0);

	f.write("\t\tswitch ((Address)addr[offset])\n");
	f.write("\t\t{\n");

	auto& mem = s.getMembers();
	for (size_t i = 0; i < mem.size(); ++i)
	{
		auto& it = mem[i];
		assert(it != nullptr);
		auto& member = *it;


		f.write("\t\t\tcase Address.%s:\n", member.name.c_str());
		f.write("\t\t\t{\n");

		switch (member.type.kind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* type_name = getCSharpPrimitiveType(member.type.kind);
			const char* parse_method = getIdlPrimitiveType(member.type.kind);

			f.write("\t\t\t\tif(addr.Length != offset + 1)\n");
			f.write("\t\t\t\t\tthrow new Exception();\n");
			f.write("\t\t\t\t%s newVal = parser.parse%s(\"value\");\n", type_name, parse_method);
			f.write("\t\t\t\tchanged = subscriber.update_%s(newVal) | changed;\n", member.name.c_str());
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			f.write("\t\t\t\tbool currentChanged = false;\n");
			f.write("\t\t\t\tif(addr.Length == offset + 1) // full element replace\n");
			f.write("\t\t\t\t{\n");
			f.write("\t\t\t\t\tparser.parsePublishableStructBegin(\"value\");\n");
			f.write("\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handler());\n", member.type.name.c_str(), member.name.c_str());
			f.write("\t\t\t\t\tparser.parsePublishableStructEnd();\n");
			f.write("\t\t\t\t}\n");
			f.write("\t\t\t\telse if(addr.Length > offset + 1) // let child continue parsing\n");
			f.write("\t\t\t\t{\n");
			f.write("\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handler(), addr, offset + 1);\n", member.type.name.c_str(), member.name.c_str());
			f.write("\t\t\t\t}\n");
			f.write("\t\t\t\telse\n");
			f.write("\t\t\t\t\tthrow new Exception();\n\n");

			f.write("\t\t\t\tif(currentChanged)\n");
			f.write("\t\t\t\t{\n");
			f.write("\t\t\t\t\tchanged = true;\n");
			f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
			f.write("\t\t\t\t}\n");

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
				const char* parse_method = getIdlPrimitiveType(member.type.vectorElemKind);

				f.write("\t\t\t\tbool currentChanged = false;\n");
				f.write("\t\t\t\tif(addr.Length == offset + 1) // full vector replace\n");
				f.write("\t\t\t\t{\n");
				f.write("\t\t\t\t\tIList<%s> newVal = new List<%s>();\n", elem_type_name, elem_type_name);
				f.write("\t\t\t\t\tparser.parseSimpleVector(\"value\", newVal);\n");
				f.write("\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.data.%s))\n", member.name.c_str());
				f.write("\t\t\t\t\t{\n");
				f.write("\t\t\t\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
				f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
				f.write("\t\t\t\t\t}\n");

				f.write("\t\t\t\t}\n");
				f.write("\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
				f.write("\t\t\t\t{\n");
				f.write("\t\t\t\t\tint index = (int)addr[offset + 1];\n");
				f.write("\t\t\t\t\tcurrentChanged = SubscriberVectorHelper.parseVectorPrimitive<%s>(\n", elem_type_name);
				f.write("\t\t\t\t\t\tparser, subscriber.data.%s, index,\n", member.name.c_str());
				f.write("\t\t\t\t\t\t(IPublishableParser parser) => { return parser.parse%s(\"value\"); },\n", parse_method);
				f.write("\t\t\t\t\t\tsubscriber.notifyElementUpdated_%s,\n", member.name.c_str());
				f.write("\t\t\t\t\t\tsubscriber.notifyInserted_%s,\n", member.name.c_str());
				f.write("\t\t\t\t\t\tsubscriber.notifyErased_%s\n", member.name.c_str());
				f.write("\t\t\t\t\t);\n");
				f.write("\t\t\t\t}\n");
				f.write("\t\t\t\telse // simple type can't handle deeper address\n");
				f.write("\t\t\t\t\tthrow new Exception();\n\n");

				f.write("\t\t\t\tif(currentChanged)\n");
				f.write("\t\t\t\t{\n");
				f.write("\t\t\t\t\tchanged = true;\n");
				f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
				f.write("\t\t\t\t}\n");

				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				const char* elem_type_name = member.type.name.c_str();

				f.write("\t\t\t\tbool currentChanged = false;\n");
				f.write("\t\t\t\tif(addr.Length == offset + 1) // full vector replace\n");
				f.write("\t\t\t\t{\n");
				f.write("\t\t\t\t\tList<I%s> newVal = new List<I%s>();\n", elem_type_name, elem_type_name);
				f.write("\t\t\t\t\tList<%s_subscriber> newHandlers = new List<%s_subscriber>();\n", elem_type_name, elem_type_name);
				f.write("\t\t\t\t\tparser.parseVector(\"value\",\n");
				f.write("\t\t\t\t\t\t(IPublishableParser parser, int index) =>\n");
				f.write("\t\t\t\t\t\t{\n");
				f.write("\t\t\t\t\t\t\tparser.parseStructBegin();\n");
				f.write("\t\t\t\t\t\t\tI%s val = new %s();\n", elem_type_name, elem_type_name);
				f.write("\t\t\t\t\t\t\t%s_subscriber handler = subscriber.makeElementHandler_%s(val);\n", elem_type_name, member.name.c_str());
				f.write("\t\t\t\t\t\t\t%s_subscriber.parseForStateSync(parser, handler);\n", elem_type_name);
				f.write("\t\t\t\t\t\t\tnewVal.Add(val);\n");
				f.write("\t\t\t\t\t\t\tnewHandlers.Add(handler);\n");
				f.write("\t\t\t\t\t\t\tparser.parseStructEnd();\n");
				f.write("\t\t\t\t\t\t}\n");
				f.write("\t\t\t\t\t);\n");

				f.write("\t\t\t\t\tif(!Enumerable.SequenceEqual(newVal, subscriber.data.%s))\n", member.name.c_str());
				f.write("\t\t\t\t\t{\n");
				f.write("\t\t\t\t\t\tsubscriber.data.%s = newVal;\n", member.name.c_str());
				f.write("\t\t\t\t\t\tsubscriber.%s_handlers = newHandlers;\n", member.name.c_str());
				f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
				f.write("\t\t\t\t\t}\n");

				f.write("\t\t\t\t}\n");
				f.write("\t\t\t\telse if(addr.Length == offset + 2) // action over one of the elements\n");
				f.write("\t\t\t\t{\n");
				f.write("\t\t\t\t\tint index = (int)addr[offset + 1];\n");
				f.write("\t\t\t\t\tPublishable.ActionOnVector action = (Publishable.ActionOnVector)parser.parseActionInPublishable();\n");

				f.write("\t\t\t\t\tswitch (action)\n");
				f.write("\t\t\t\t\t{\n");
				f.write("\t\t\t\t\tcase Publishable.ActionOnVector.update_at:\n");
				f.write("\t\t\t\t\t{\n");
				f.write("\t\t\t\t\t\tparser.parsePublishableStructBegin(\"value\");\n");
				f.write("\t\t\t\t\t\tbool elemChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handlers()[index]);\n", elem_type_name, member.name.c_str());
				f.write("\t\t\t\t\t\tparser.parsePublishableStructEnd();\n");
				f.write("\t\t\t\t\t\tif (elemChanged)\n");
				f.write("\t\t\t\t\t\t{\n");
				f.write("\t\t\t\t\t\t\tcurrentChanged = true;\n");
				f.write("\t\t\t\t\t\t\tsubscriber.notifyElementUpdated_%s(index);\n", member.name.c_str());
				f.write("\t\t\t\t\t\t}\n");
				f.write("\t\t\t\t\t\tbreak;\n");
				f.write("\t\t\t\t\t}\n");


				f.write("\t\t\t\t\tcase Publishable.ActionOnVector.insert_single_before:\n");
				f.write("\t\t\t\t\t{\n");
				f.write("\t\t\t\t\t\tparser.parsePublishableStructBegin(\"value\");\n");
				f.write("\t\t\t\t\t\tI%s newVal = new %s();\n", elem_type_name, elem_type_name);
				f.write("\t\t\t\t\t\t%s_subscriber handler = subscriber.makeElementHandler_%s(newVal);\n", elem_type_name, member.name.c_str());
				f.write("\t\t\t\t\t\t%s_subscriber.parse(parser, handler);\n", elem_type_name);
				f.write("\t\t\t\t\t\t// mb: lazy initialization always first\n");
				f.write("\t\t\t\t\t\tsubscriber.lazy_%s_handlers().Insert(index, handler);\n", member.name.c_str());
				f.write("\t\t\t\t\t\tsubscriber.data.%s.Insert(index, newVal);\n", member.name.c_str());
				f.write("\t\t\t\t\t\tparser.parsePublishableStructEnd();\n");
				f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
				f.write("\t\t\t\t\t\tsubscriber.notifyInserted_%s(index);\n", member.name.c_str());
				f.write("\t\t\t\t\t\tbreak;\n");
				f.write("\t\t\t\t\t}\n");



				f.write("\t\t\t\t\tcase Publishable.ActionOnVector.remove_at:\n");
				f.write("\t\t\t\t\t{\n");
				f.write("\t\t\t\t\t\t// mb: lazy initialization always first\n");
				f.write("\t\t\t\t\t\tsubscriber.lazy_%s_handlers().RemoveAt(index);\n", member.name.c_str());
				f.write("\t\t\t\t\t\tsubscriber.data.%s.RemoveAt(index);\n", member.name.c_str());
				f.write("\t\t\t\t\t\tcurrentChanged = true;\n");
				f.write("\t\t\t\t\t\tsubscriber.notifyErased_%s(index);\n", member.name.c_str());
				f.write("\t\t\t\t\t\tbreak;\n");
				f.write("\t\t\t\t\t}\n");
				f.write("\t\t\t\t\tdefault:\n");
				f.write("\t\t\t\t\t\tthrow new Exception();\n");
				f.write("\t\t\t\t\t}\n");


				f.write("\t\t\t\t}\n");
				f.write("\t\t\t\telse if(addr.Length > offset + 2) // let child continue parsing\n");
				f.write("\t\t\t\t{\n");
				f.write("\t\t\t\t\tint index = (int)addr[offset + 1];\n");
				f.write("\t\t\t\t\tcurrentChanged = %s_subscriber.parse(parser, subscriber.lazy_%s_handlers()[index], addr, offset + 2);\n", elem_type_name, member.name.c_str());
				f.write("\t\t\t\t}\n");
				f.write("\t\t\t\telse\n");
				f.write("\t\t\t\t\tthrow new Exception();\n\n");

				f.write("\t\t\t\tif(currentChanged)\n");
				f.write("\t\t\t\t{\n");
				f.write("\t\t\t\t\tchanged = true;\n");
				f.write("\t\t\t\t\tsubscriber.notifyUpdated_%s();\n", member.name.c_str());
				f.write("\t\t\t\t}\n");


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
		f.write("\t\t\t}\n");
		f.write("\t\t\tbreak;\n");

	}

	f.write("\t\t\tdefault:\n");
	f.write("\t\t\t\tthrow new Exception();\n");

	f.write("\t\t}\n");
}


void generateCsharpPublishables( FILE* header, Root& root, const std::string& metascope )
{
	vector<CompositeType*> structsOrderedByDependency;
	std::unordered_set<size_t> collElementTypes;
	orderStructsByDependency( root.structs, structsOrderedByDependency, collElementTypes );

	CsharpFileWritter f(header, 0);

	for ( auto it : structsOrderedByDependency )
	{
		assert( it != nullptr );
		if ( it->isStruct4Publishing )
		{
			if (it->type == CompositeType::Type::structure)
			{
				csharpPub_generateStructSubs(header, *it, it->name);
				csharpPub_generateStructPubl(header, *it, it->name);

			}
			else if (it->type == CompositeType::Type::discriminated_union)
			{
				generateCsharpUnionSubscriber(header, *it, it->name.c_str());
				generateCsharpUnionPublisher(header, *it, it->name.c_str());
			}
			else
				assert(false);
		}
	}

	for ( auto& it : root.publishables )
	{
		auto& obj_1 = it;
		assert(it != nullptr );
		assert( typeid( *(it) ) == typeid( CompositeType ) );
		assert(it->type == CompositeType::Type::publishable );

		checkCsharpStruct(*it);

		std::string type_name = getCSharpTypeName(*it);
		std::string interface_name = "I" + type_name;

		generateCsharpStructInterface(header, *it, type_name.c_str());
		generateCsharpStructImpl(header, *it, type_name.c_str(), interface_name.c_str());

		impl_generatePublishableCommentBlock(header, *it);

		csharpPub_generateStructSubs(header, *it, type_name);
		csharpPub_generateStructPubl(header, *it, type_name);
		csharpPub_generateStructConcentrator(header, *it, type_name);
	}

	if (!root.publishables.empty())
		csharpPub_generateConcentratorFactory(header, root);

}

