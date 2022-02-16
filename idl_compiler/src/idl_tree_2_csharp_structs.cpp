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

void checkCsharpStruct(CompositeType& s)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::discriminated_union ||
		s.type == CompositeType::Type::publishable) ;

	bool checked = impl_checkParamNameUniqueness(s);
	checked = impl_checkFollowingExtensionRules(s) && checked;
	if (!checked)
		throw std::exception();
}

std::string getCSharpTypeName(CompositeType& s)
{
	if (s.type == CompositeType::Type::discriminated_union_case)
		return fmt::format("{}_{}", s.type2string(), s.name);
	else
		return s.name;
}
const char* getCSharpPrimitiveType(MessageParameterType::KIND kind)
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


void generateCsharpDeclParams(FILE* header, Root& root, CompositeType& s)
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


void generateCsharpCallerParams(FILE* header, CompositeType& s, bool valPrefix)
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

		if (valPrefix)
			fprintf(header, "val.");

		fprintf(header, "%s", param.name.c_str());
	}
}


void generateCsharpStandardMethods(FILE* header, const char* type_name)
{

	fprintf(header, "\tpublic override bool Equals(object obj)\n"
		"\t{\n"
		"\t\treturn Equals(obj as %s);\n"
		"\t}\n", type_name);

	fprintf(header, "\tpublic static bool operator ==(%s left, %s right)\n", type_name, type_name);
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
		"\t}\n", type_name, type_name);

	fprintf(header, "\tpublic override int GetHashCode()\n"
		"\t{\n"
		"\t\t// TODO\n"
		"\t\tthrow new InvalidOperationException();\n"
		"\t}\n");

}

void generateCsharpStructEqualsMethod(FILE* header, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);

	fprintf(header, "\tpublic bool Equals(%s other)\n", type_name);
	fprintf(header,
		"\t{\n"
		"\t\tif (ReferenceEquals(other, null))\n"
		"\t\t\treturn false;\n"
		"\t\telse if (ReferenceEquals(this, other))\n"
		"\t\t\treturn true;\n"
		"\t\telse\n"
		"\t\t\treturn\n");

	auto& mem = s.getMembers();
	for (auto it = mem.begin(); it != mem.end(); ++it)
	{
		//auto& it = s.members[i];
		assert(*it != nullptr);
		auto& member = **it;

		if (it != mem.begin())
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

void generateCsharpStructEquivalentExpression(FILE* header, CompositeType& s)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);

	auto& mem = s.getMembers();
	for (auto it = mem.begin(); it != mem.end(); ++it)
	{
		//auto& it = s.members[i];
		assert(*it != nullptr);
		auto& member = **it;

		if (it != mem.begin())
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
			fprintf(header, "\t\t\t\tthis.%s.isEquivalent(other.%s)", member.name.c_str(), member.name.c_str());
			break;
		case MessageParameterType::KIND::VECTOR:
		{
			switch (member.type.vectorElemKind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				fprintf(header, "\t\t\t\tEnumerable.SequenceEqual(this.%s, other.%s)", member.name.c_str(), member.name.c_str());
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				fprintf(header, "\t\t\t\tEquivalenceComparer.areEquivalent(this.%s, other.%s)", member.name.c_str(), member.name.c_str());
				break;
			default:
				assert(false); // not implemented (yet)
			}
			break;
		}

		break;
		default:
			assert(false); // not implemented (yet)
		}
	}
}


void generateCsharpStructEquivalentMethod(FILE* header, Root& root, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);

	fprintf(header, "\tpublic bool isEquivalent(%s other)\n", type_name);
	fprintf(header,
		"\t{\n"
		"\t\tif (ReferenceEquals(other, null))\n"
		"\t\t\treturn false;\n"
		"\t\telse if (ReferenceEquals(this, other))\n"
		"\t\t\treturn true;\n"
		"\t\telse\n"
		"\t\t\treturn\n");

	generateCsharpStructEquivalentExpression(header, s);

	fprintf(header, ";\n\t}\n");
}

void generateCsharpStructInterface(FILE* header, Root& root, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);

	fprintf(header, "public interface I%s : IEquivalenceComparable<I%s>\n", type_name, type_name);
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
			fprintf(header, "\t%s %s { get; set; }\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			fprintf(header, "\tI%s %s { get; set; }\n", member.type.name.c_str(), member.name.c_str());
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
				fprintf(header, "\tIList<%s> %s { get; set; }\n", elem_type_name, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				assert(member.type.structIdx < root.structs.size());
				const std::string type_name = root.structs[member.type.structIdx]->name;
				fprintf(header, "\tIList<I%s> %s { get; set; }\n", type_name.c_str(), member.name.c_str());
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

	fprintf(header, "} // interface %s\n\n", type_name);
}

void generateCsharpStructImpl(FILE* header, Root& root, CompositeType& s, const char* type_name, const char* interface_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);

	if(interface_name)
		fprintf(header, "public class %s : %s, IEquatable<%s>\n", type_name, interface_name, type_name);
	else
		fprintf(header, "public class %s : IEquatable<%s>\n", type_name, type_name);
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
			fprintf(header, "\tpublic %s %s { get; set; }\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
			break;
		case MessageParameterType::KIND::CHARACTER_STRING:
			fprintf(header, "\t%s _%s = String.Empty;\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
			fprintf(header, "\tpublic %s %s\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
			fprintf(header, "\t{\n");
			fprintf(header, "\t\tget { return _%s; }\n", member.name.c_str());
			fprintf(header, "\t\tset\n");
			fprintf(header, "\t\t{\n");
			fprintf(header, "\t\t\tif(value == null)\n");
			fprintf(header, "\t\t\t\tthrow new ArgumentNullException();\n");
			fprintf(header, "\t\t\t_%s = value;\n", member.name.c_str());
			fprintf(header, "\t\t}\n");
			fprintf(header, "\t}\n");

			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			fprintf(header, "\t%s _%s = new %s();\n", member.type.name.c_str(), member.name.c_str(), member.type.name.c_str());
			fprintf(header, "\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());

			fprintf(header, "\t{\n");
			fprintf(header, "\t\tget { return _%s; }\n", member.name.c_str());
			fprintf(header, "\t\tset\n");
			fprintf(header, "\t\t{\n");
			fprintf(header, "\t\t\tif(value == null)\n");
			fprintf(header, "\t\t\t\tthrow new ArgumentNullException();\n");
			fprintf(header, "\t\t\t_%s = (%s)value;\n", member.name.c_str(), member.type.name.c_str());
			fprintf(header, "\t\t}\n");
			fprintf(header, "\t}\n");

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
				fprintf(header, "\tList<%s> _%s = new List<%s>();\n", elem_type_name, member.name.c_str(), elem_type_name);
				fprintf(header, "\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return _%s; }\n", member.name.c_str());
				fprintf(header, "\t\tset\n");
				fprintf(header, "\t\t{\n");
				fprintf(header, "\t\t\tif(value == null)\n");
				fprintf(header, "\t\t\t\tthrow new ArgumentNullException();\n");
				fprintf(header, "\t\t\t_%s = (List<%s>)value;\n", member.name.c_str(), elem_type_name);
				fprintf(header, "\t\t}\n");
				fprintf(header, "\t}\n");
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				assert(member.type.structIdx < root.structs.size());
				const char* elem_type_name = root.structs[member.type.structIdx]->name.c_str();
				fprintf(header, "\tList<I%s> _%s = new List<I%s>();\n", elem_type_name, member.name.c_str(), elem_type_name);
				fprintf(header, "\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
				fprintf(header, "\t{\n");
				fprintf(header, "\t\tget { return _%s; }\n", member.name.c_str());
				fprintf(header, "\t\tset\n");
				fprintf(header, "\t\t{\n");
				fprintf(header, "\t\t\tif(value == null)\n");
				fprintf(header, "\t\t\t\tthrow new ArgumentNullException();\n");

				fprintf(header, "\t\t\tList<I%s> tmp = (List<I%s>)value;\n", elem_type_name, elem_type_name);
				fprintf(header, "\t\t\ttmp.ForEach((I%s each) => { if(!(each is %s)) throw new InvalidCastException(); });\n", elem_type_name, elem_type_name);
				fprintf(header, "\t\t\t_%s = tmp;\n", member.name.c_str());
				fprintf(header, "\t\t}\n");
				fprintf(header, "\t}\n");
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


	//default constructor
	fprintf(header, "\tpublic %s() { }\n", type_name);

	//constructor with all members
	fprintf(header, "\tpublic %s(", type_name);
	generateCsharpDeclParams(header, root, s);
	fprintf(header, ")\n");

	fprintf(header, "\t{\n");

	for (auto& each : s.getMembers())
	{
		assert(each != nullptr);
		auto& member = *each;

		fprintf(header, "\t\tthis.%s = %s;\n", member.name.c_str(), member.name.c_str());
	}
	fprintf(header, "\t}\n");


	generateCsharpStandardMethods(header, type_name);
	generateCsharpStructEqualsMethod(header, s, type_name);
	if (interface_name)
		generateCsharpStructEquivalentMethod(header, root, s, interface_name);

	fprintf(header, "} // class %s\n\n", type_name);
}


void generateCsharp(FILE* header, Root& root, const std::string& metascope)
{
	fprintf(header, "//////////////////////////////////////////////////////////////\n");
	fprintf(header, "//\n");
	fprintf(header, "//  Do not edit! file automatically generated by idl_compiler\n");
	fprintf(header, "//\n");
	fprintf(header, "//////////////////////////////////////////////////////////////\n\n");

	fprintf(header,
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
	std::unordered_set<size_t> collElementTypes;
	orderStructsByDependency(root.structs, structsOrderedByDependency, collElementTypes);

	fprintf(header, "//////////////////////////////////////////////////////////////\n");
	fprintf(header, "//\n");
	fprintf(header, "//                 Structures\n");
	fprintf(header, "//\n");
	fprintf(header, "//////////////////////////////////////////////////////////////\n\n");

	for (auto& it : structsOrderedByDependency)
	{
		assert(it != nullptr);
		assert(typeid(*(it)) == typeid(CompositeType));

		if (it->isStruct4Messaging || it->isStruct4Publishing)
		{
			checkCsharpStruct(*it);
			if (it->type == CompositeType::Type::structure)
			{

				std::string type_name = getCSharpTypeName(*it);
				std::string interface_name = "I" + type_name;

				generateCsharpStructInterface(header, root, *it, type_name.c_str());
				generateCsharpStructImpl(header, root, *it, type_name.c_str(), interface_name.c_str());
			}
			else if (it->type == CompositeType::Type::discriminated_union)
			{
				generateCsharpUnionInterface(header, root, *it);
				generateCsharpUnionImpl(header, root, *it);
			}
			else
				assert(false);
		}
	}

	fprintf(header, "//////////////////////////////////////////////////////////////\n");
	fprintf(header, "//\n");
	fprintf(header, "//                 Messages\n");
	fprintf(header, "//\n");
	fprintf(header, "//////////////////////////////////////////////////////////////\n\n");

	generateCsharpMessages(header, root, metascope);

	fprintf(header, "//////////////////////////////////////////////////////////////\n");
	fprintf(header, "//\n");
	fprintf(header, "//                 Publishables\n");
	fprintf(header, "//\n");
	fprintf(header, "//////////////////////////////////////////////////////////////\n\n");

	generateCsharpPublishables(header, root, metascope);

	fprintf(header, "\n} // namespace %s\n\n", metascope.c_str());
}
