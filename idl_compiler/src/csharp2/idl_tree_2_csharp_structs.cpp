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

// void checkCsharpStruct(CompositeType& s)
// {
// 	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
// 		s.type == CompositeType::Type::discriminated_union_case ||
// 		s.type == CompositeType::Type::discriminated_union ||
// 		s.type == CompositeType::Type::publishable) ;

// 	// bool checked = impl_checkParamNameUniqueness(s);
// 	// checked = impl_checkFollowingExtensionRules(s) && checked;
// 	// if (!checked)
// 	// 	throw std::exception();
// }

namespace csharp2
{



std::string getCaseTypeName(CompositeType& s)
{
	assert(s.type == CompositeType::Type::discriminated_union_case);
	return fmt::format("CASE_{}", s.name);
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

std::string getCSharpElementInterfaceType(MessageParameterType::KIND kind, const std::string& name)
{
	switch (kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
		return getCSharpPrimitiveType(kind);
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		return fmt::format("I{}", name);
	default:
		assert(false);
	}
	return "";
}


std::string getCSharpInterfaceType(MessageParameterType& type)
{
	switch (type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
		return getCSharpPrimitiveType(type.kind);
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		return fmt::format("I{}", type.name);
	case MessageParameterType::KIND::VECTOR:
		return fmt::format("IList<{}>", getCSharpElementInterfaceType(type.vectorElemKind, type.name));
	case MessageParameterType::KIND::DICTIONARY:
		return fmt::format("IDictionary<{}, {}>", getCSharpPrimitiveType(type.dictionaryKeyKind), getCSharpElementInterfaceType(type.dictionaryValueKind, type.name));
	default:
		assert(false);
	}
	return "";
}


std::string getCSharpPrefixByType(MessageParameterType& type)
{
	switch (type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
		return getCSharpPrimitiveType(type.kind);

	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		return type.name;
		break;
	case MessageParameterType::KIND::VECTOR:
		switch (type.vectorElemKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			return fmt::format("List_{}", getCSharpPrimitiveType(type.vectorElemKind));
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			return fmt::format("List_{}", type.name);
			break;
		default:
			assert(false); // unexpected
			break;
		}
		break;
	case MessageParameterType::KIND::DICTIONARY:
	{
		const char* key = getCSharpPrimitiveType(type.dictionaryKeyKind);
		switch (type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			return fmt::format("Dictionary_{}_{}", key, getCSharpPrimitiveType(type.dictionaryValueKind));
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			return fmt::format("Dictionary_{}_{}", key, type.name);
		default:
			assert(false); // not implemented (yet)
		}
		break;
	}
	default:
		assert(false);
	}
	return "";
}

const char* getIdlPrimitiveType(MessageParameterType::KIND kind)
{
	switch (kind)
	{
	case MessageParameterType::KIND::INTEGER:
		return "Integer";
	case MessageParameterType::KIND::UINTEGER:
		return "Unsigned";
	case MessageParameterType::KIND::REAL:
		return "Real";
	case MessageParameterType::KIND::CHARACTER_STRING:
		return "String";
	default:
		assert(false);
	}
	return nullptr;
}

const char* getIdlPrimitiveType2(MessageParameterType::KIND kind)
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

string generateCsharpDeclParams(CompositeType& s)
{
	string result;

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
			result += ", ";



		switch (member.type.kind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			result += fmt::format("{} {}", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			result += fmt::format("I{} {}", member.type.name.c_str(), member.name.c_str());
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
				result += fmt::format("IList<{}> {}", elem_type_name, member.name.c_str());
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				const char* elem_type_name = member.type.name.c_str();
				result += fmt::format("IList<I{}> {}", elem_type_name, member.name.c_str());
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}
			break;
		}
		case MessageParameterType::KIND::DICTIONARY:
		{
			const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
			switch (member.type.dictionaryValueKind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
				result += fmt::format("IDictionary<{}, {}> {}", key, value, member.name.c_str());

				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				const char* value = member.type.name.c_str();
				result += fmt::format("IDictionary<{}, I{}> {}", key, value, member.name.c_str());
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

	return result;
}


string generateCsharpCallerParams(CompositeType& s, bool valPrefix)
{
	string result;

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
			result +=  ", ";

		if (valPrefix)
			result += "val.";

		result += param.name;
	}

	return result;
}


void generateCsharpStandardMethods(FileWritter f, const char* type_name)
{


	f.write("\tpublic override bool Equals(object obj)\n");
	f.write("\t{\n");
	f.write("\t\treturn Equals(obj as %s);\n", type_name);
	f.write("\t}\n");

	f.write("\tpublic static bool operator ==(%s left, %s right)\n", type_name, type_name);
	f.write("\t{\n");
	f.write("\t\tif (ReferenceEquals(left, right))\n");
	f.write("\t\t\treturn true;\n");
	f.write("\t\telse if (ReferenceEquals(left, null))\n");
	f.write("\t\t\treturn false;\n");
	f.write("\t\telse if (ReferenceEquals(null, right))\n");
	f.write("\t\t\treturn false;\n");
	f.write("\t\telse\n");
	f.write("\t\t\treturn left.Equals(right);\n");
	f.write("\t}\n");

	f.write("\tpublic static bool operator !=(%s left, %s right)\n", type_name, type_name);
	f.write("\t{\n");
	f.write("\t\treturn !(left == right);\n");
	f.write("\t}\n");

	f.write("\tpublic override int GetHashCode()\n");
	f.write("\t{\n");
	f.write("\t\t// TODO\n");
	f.write("\t\tthrow new InvalidOperationException();\n");
	f.write("\t}\n");

}

void generateCsharpStructEqualsMethod(FileWritter f, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);



	f.write("\tpublic bool Equals(%s other)\n", type_name);
	f.write("\t{\n");
	f.write("\t\tif (ReferenceEquals(other, null))\n");
	f.write("\t\t\treturn false;\n");
	f.write("\t\telse if (ReferenceEquals(this, other))\n");
	f.write("\t\t\treturn true;\n");
	f.write("\t\telse\n");
	f.write("\t\t\treturn\n");

	auto& mem = s.getMembers();
	size_t sz = s.getMembers().size();
	for (size_t i = 0; i != sz; ++i)
	{
		auto& it = mem[i];
		assert(it != nullptr);
		auto& member = *it;

		const char* tail = (i == sz - 1) ? ";" : " &&";

		switch (member.type.kind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			f.write("\t\t\t\tthis.%s == other.%s%s\n", member.name.c_str(), member.name.c_str(), tail);
			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			f.write("\t\t\t\tthis.%s.Equals(other.%s)%s\n", member.name.c_str(), member.name.c_str(), tail);
			break;
		case MessageParameterType::KIND::VECTOR:
		case MessageParameterType::KIND::DICTIONARY:
			f.write("\t\t\t\tEnumerable.SequenceEqual(this.%s, other.%s)%s\n", member.name.c_str(), member.name.c_str(), tail);
			break;
		default:
			assert(false); // not implemented (yet)
		}
	}
	f.write("\t}\n");
}

void generateCsharpStructEquivalentExpression(FileWritter f, CompositeType& s)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);

	
	f.write("\t\t\treturn\n");

	auto& mem = s.getMembers();
	size_t sz = s.getMembers().size();
	for (size_t i = 0; i != sz; ++i)
	{
		auto& it = mem[i];
		assert(it != nullptr);
		auto& member = *it;

		const char* tail = (i == sz - 1) ? ";" : " &&";

		switch (member.type.kind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
			f.write("\t\t\t\tthis.%s == other.%s%s\n", member.name.c_str(), member.name.c_str(), tail);
			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			f.write("\t\t\t\tthis.%s.isEquivalent(other.%s)%s\n", member.name.c_str(), member.name.c_str(), tail);
			break;
		case MessageParameterType::KIND::VECTOR:
		{
			switch (member.type.vectorElemKind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				f.write("\t\t\t\tEnumerable.SequenceEqual(this.%s, other.%s)%s\n", member.name.c_str(), member.name.c_str(), tail);
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\t\t\t\tEquivalenceComparer.areEquivalent(this.%s, other.%s)%s\n", member.name.c_str(), member.name.c_str(), tail);
				break;
			default:
				assert(false); // not implemented (yet)
			}
			break;
		}
		case MessageParameterType::KIND::DICTIONARY:
		{
			switch (member.type.dictionaryValueKind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
				f.write("\t\t\t\tEnumerable.SequenceEqual(this.%s, other.%s)%s\n", member.name.c_str(), member.name.c_str(), tail);
				break;
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
				f.write("\t\t\t\tEquivalenceComparer.areEquivalent(this.%s, other.%s)%s\n", member.name.c_str(), member.name.c_str(), tail);
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

void generateCsharpStructEquivalentMethod(FileWritter f, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);



	f.write("\tpublic bool isEquivalent(%s other)\n", type_name);
	f.write("\t{\n");
	f.write("\t\tif (ReferenceEquals(other, null))\n");
	f.write("\t\t\treturn false;\n");
	f.write("\t\telse if (ReferenceEquals(this, other))\n");
	f.write("\t\t\treturn true;\n");
	f.write("\t\telse\n");

	generateCsharpStructEquivalentExpression(f, s);

	f.write("\t}\n");
}

void generateCsharpSimpleEquivalentMethod(FileWritter f, const char* type_name, const char* member_name)
{


	f.write("\tpublic bool isEquivalent(I%s other)\n", type_name);
	f.write("\t{\n");
	f.write("\t\tif (ReferenceEquals(other, null))\n");
	f.write("\t\t\treturn false;\n");
	f.write("\t\telse if (ReferenceEquals(this, other))\n");
	f.write("\t\t\treturn true;\n");
	f.write("\t\telse\n");
	f.write("\t\t\treturn %s.isEquivalent(other);\n", member_name);
	f.write("\t}\n");
}

void generateCsharpInterfaceMember(FileWritter f, MessageParameter& member)
{


	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
		f.write("\t%s %s { get; set; }\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
		break;
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
		f.write("\tI%s %s { get; set; }\n", member.type.name.c_str(), member.name.c_str());
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
			f.write("\tIList<%s> %s { get; set; }\n", elem_type_name, member.name.c_str());
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* elem_type_name = member.type.name.c_str();
			f.write("\tIList<I%s> %s { get; set; }\n", elem_type_name, member.name.c_str());
			break;
		}
		default:
			assert(false); // not implemented (yet)
		}
		break;
	}
	case MessageParameterType::KIND::DICTIONARY:
	{
		const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
			f.write("\tIDictionary<%s, %s> %s { get; set; }\n", key, value, member.name.c_str());
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\tIDictionary<%s, I%s> %s { get; set; }\n", key, value, member.name.c_str());
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


void generateCsharpStructInterface(FileWritter f, CompositeType& s, const char* type_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);



	f.write("public interface I%s : IEquivalenceComparable<I%s>\n", type_name, type_name);
	f.write("{\n");


	auto& mem = s.getMembers();
	for (size_t i = 0; i < mem.size(); ++i)
	{
		auto& it = mem[i];
		assert(it != nullptr);
		auto& member = *it;
		generateCsharpInterfaceMember(f, *it);
	}

	f.write("} // interface %s\n\n", type_name);
}

void generateCsharpStructImpl(FileWritter f, CompositeType& s, const char* type_name, const char* interface_name)
{
	assert(s.type == CompositeType::Type::message || s.type == CompositeType::Type::structure ||
		s.type == CompositeType::Type::discriminated_union_case ||
		s.type == CompositeType::Type::publishable);

	//interface_name is nullptr for discriminated_union_case only
	assert(interface_name != nullptr || s.type == CompositeType::Type::discriminated_union_case);



	if(s.type == CompositeType::Type::discriminated_union_case)
		f.write("public class %s : IEquatable<%s>\n", type_name, type_name);
	else
		f.write("public class %s : %s, IEquatable<%s>\n", type_name, interface_name, type_name);

	f.write("{\n");

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
			f.write("\tpublic %s %s { get; set; }\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
			break;
		case MessageParameterType::KIND::CHARACTER_STRING:
			f.write("\t%s _%s = String.Empty;\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
			f.write("\tpublic %s %s\n", getCSharpPrimitiveType(member.type.kind), member.name.c_str());
			f.write("\t{\n");
			f.write("\t\tget { return _%s; }\n", member.name.c_str());
			f.write("\t\tset\n");
			f.write("\t\t{\n");
			f.write( "\t\t\tif(value == null)\n");
			f.write("\t\t\t\tthrow new ArgumentNullException();\n");
			f.write("\t\t\t_%s = value;\n", member.name.c_str());
			f.write("\t\t}\n");
			f.write("\t}\n");

			break;
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
			f.write("\t%s _%s = new %s();\n", member.type.name.c_str(), member.name.c_str(), member.type.name.c_str());
			f.write("\tpublic I%s %s\n", member.type.name.c_str(), member.name.c_str());

			f.write("\t{\n");
			f.write("\t\tget { return _%s; }\n", member.name.c_str());
			f.write("\t\tset\n");
			f.write("\t\t{\n");
			f.write("\t\t\tif(value == null)\n");
			f.write("\t\t\t\tthrow new ArgumentNullException();\n");
			f.write("\t\t\t_%s = (%s)value;\n", member.name.c_str(), member.type.name.c_str());
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
				f.write("\tList<%s> _%s = new List<%s>();\n", elem_type_name, member.name.c_str(), elem_type_name);
				f.write("\tpublic IList<%s> %s\n", elem_type_name, member.name.c_str());
				f.write("\t{\n");
				f.write("\t\tget { return _%s; }\n", member.name.c_str());
				f.write("\t\tset\n");
				f.write("\t\t{\n");
				f.write("\t\t\tif(value == null)\n");
				f.write("\t\t\t\tthrow new ArgumentNullException();\n");
				f.write("\t\t\t_%s = (List<%s>)value;\n", member.name.c_str(), elem_type_name);
				f.write("\t\t}\n");
				f.write("\t}\n");
				break;
			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				const char* elem_type_name = member.type.name.c_str();
				f.write("\tList<I%s> _%s = new List<I%s>();\n", elem_type_name, member.name.c_str(), elem_type_name);
				f.write("\tpublic IList<I%s> %s\n", elem_type_name, member.name.c_str());
				f.write("\t{\n");
				f.write("\t\tget { return _%s; }\n", member.name.c_str());
				f.write("\t\tset\n");
				f.write("\t\t{\n");
				f.write("\t\t\tif(value == null)\n");
				f.write("\t\t\t\tthrow new ArgumentNullException();\n");

				f.write("\t\t\tList<I%s> tmp = (List<I%s>)value;\n", elem_type_name, elem_type_name);
				f.write("\t\t\ttmp.ForEach((I%s each) => { if(!(each is %s)) throw new InvalidCastException(); });\n", elem_type_name, elem_type_name);
				f.write("\t\t\t_%s = tmp;\n", member.name.c_str());
				f.write("\t\t}\n");
				f.write("\t}\n");
				break;
			}
			default:
				assert(false); // not implemented (yet)
			}
			break;
		}
		case MessageParameterType::KIND::DICTIONARY:
		{
			const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);
			switch (member.type.dictionaryValueKind)
			{
			case MessageParameterType::KIND::INTEGER:
			case MessageParameterType::KIND::UINTEGER:
			case MessageParameterType::KIND::REAL:
			case MessageParameterType::KIND::CHARACTER_STRING:
			{
				const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
				f.write("\tDictionary<%s, %s> _%s = new Dictionary<%s, %s>();\n", key, value, member.name.c_str(), key, value);
				f.write("\tpublic IDictionary<%s, %s> %s\n", key, value, member.name.c_str());
				f.write("\t{\n");
				f.write("\t\tget { return _%s; }\n", member.name.c_str());
				f.write("\t\tset\n");
				f.write("\t\t{\n");
				f.write("\t\t\tif(value == null)\n");
				f.write("\t\t\t\tthrow new ArgumentNullException();\n");
				f.write("\t\t\t_%s = (Dictionary<%s, %s>)value;\n", member.name.c_str(), key, value);
				f.write("\t\t}\n");
				f.write("\t}\n");
				break;

			}
			case MessageParameterType::KIND::STRUCT:
			case MessageParameterType::KIND::DISCRIMINATED_UNION:
			{
				const char* value = member.type.name.c_str();
				f.write("\tDictionary<%s, I%s> _%s = new Dictionary<%s, I%s>();\n", key, value, member.name.c_str(), key, value);
				f.write("\tpublic IDictionary<%s, I%s> %s\n", key, value, member.name.c_str());
				f.write("\t{\n");
				f.write("\t\tget { return _%s; }\n", member.name.c_str());
				f.write("\t\tset\n");
				f.write("\t\t{\n");
				f.write("\t\t\tif(value == null)\n");
				f.write("\t\t\t\tthrow new ArgumentNullException();\n");

				f.write("\t\t\tDictionary<%s, I%s> tmp = (Dictionary<%s, I%s>)value;\n", key, value, key, value);
				f.write("\t\t\tforeach (I%s each in tmp.Values)\n", value);

				f.write("\t\t\t\tif(!(each is %s))\n", value);
				f.write("\t\t\t\t\tthrow new InvalidCastException();\n");
				f.write("\t\t\t_%s = tmp;\n", member.name.c_str());
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


	//default constructor
	f.write("\tpublic %s() { }\n", type_name);

	//constructor with all members
	string decl = generateCsharpDeclParams(s);
	f.write("\tpublic %s(%s)\n", type_name, decl.c_str());
	f.write("\t{\n");

	for (auto& each : s.getMembers())
	{
		assert(each != nullptr);
		auto& member = *each;

		f.write("\t\tthis.%s = %s;\n", member.name.c_str(), member.name.c_str());
	}
	f.write("\t}\n");


	generateCsharpStandardMethods(f, type_name);
	generateCsharpStructEqualsMethod(f, s, type_name);
	if (s.type != CompositeType::Type::discriminated_union_case)
		generateCsharpStructEquivalentMethod(f, s, interface_name);

	f.write("} // class %s\n\n", type_name);
}

void generateCsharpReadOnlyMember(FileWritter f, MessageParameter& member)
{
	const char* name = member.name.c_str();

	switch (member.type.kind)
	{
	case MessageParameterType::KIND::INTEGER:
	case MessageParameterType::KIND::UINTEGER:
	case MessageParameterType::KIND::REAL:
	case MessageParameterType::KIND::CHARACTER_STRING:
	{
		const char* type = getCSharpPrimitiveType(member.type.kind);
		f.write("\tpublic %s %s\n", type, name);
		f.write("\t{\n");
		f.write("\t\tget { return this._data.%s; }\n", name);
		f.write("\t\tset { throw new InvalidOperationException(); }\n");
		f.write("\t}\n");
		break;
	}
	case MessageParameterType::KIND::STRUCT:
	case MessageParameterType::KIND::DISCRIMINATED_UNION:
	{
		const char* type = member.type.name.c_str();
		f.write("\tpublic I%s %s\n", type, name);
		f.write("\t{\n");
		f.write("\t\tget { return new %s_readonly(this._data.%s); }\n", type, name);
		f.write("\t\tset { throw new InvalidOperationException(); }\n");
		f.write("\t}\n");
		break;
	}
	case MessageParameterType::KIND::VECTOR:
	{
		switch (member.type.vectorElemKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* elem = getCSharpPrimitiveType(member.type.vectorElemKind);
			f.write("\tpublic IList<%s> %s\n", elem, name);
			f.write("\t{\n");
			f.write("\t\tget { return new VectorWrapper<%s>(this._data.%s, null); }\n", elem, name);
			f.write("\t\tset { throw new InvalidOperationException(); }\n");
			f.write("\t}\n");
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* elem = member.type.name.c_str();

			f.write("\tpublic IList<I%s> %s\n", elem, name);
			f.write("\t{\n");
			f.write("\t\tget { return new VectorWrapper<I%s>(this._data.%s, (I%s t) => { return new %s_readonly(t); }); }\n", elem, name, elem, elem);
			f.write("\t\tset { throw new InvalidOperationException(); }\n");
			f.write("\t}\n");

			break;
		}
		default:
			assert(false); // not implemented (yet)
		}
		break;
	}
	case MessageParameterType::KIND::DICTIONARY:
	{
		const char* key = getCSharpPrimitiveType(member.type.dictionaryKeyKind);

		switch (member.type.dictionaryValueKind)
		{
		case MessageParameterType::KIND::INTEGER:
		case MessageParameterType::KIND::UINTEGER:
		case MessageParameterType::KIND::REAL:
		case MessageParameterType::KIND::CHARACTER_STRING:
		{
			const char* value = getCSharpPrimitiveType(member.type.dictionaryValueKind);
			f.write("\tpublic IDictionary<%s, %s> %s\n", key, value, name);
			f.write("\t{\n");
			f.write("\t\tget { return new DictionaryWrapper<%s, %s>(this._data.%s, null); }\n", key, value, name);
			f.write("\t\tset { throw new InvalidOperationException(); }\n");
			f.write("\t}\n");
			break;
		}
		case MessageParameterType::KIND::STRUCT:
		case MessageParameterType::KIND::DISCRIMINATED_UNION:
		{
			const char* value = member.type.name.c_str();
			f.write("\tpublic IDictionary<%s, I%s> %s\n", key, value, name);
			f.write("\t{\n");
			f.write("\t\tget { return new DictionaryWrapper<%s, I%s>(this._data.%s, (I%s t) => { return new %s_readonly(t); }); }\n", key, value, name, value, value);
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


void generateCsharpStructReadOnly(FileWritter f, CompositeType& s)
{
	assert(s.type == CompositeType::Type::structure);

	const char* type_name = s.name.c_str();

	f.write("public class %s_readonly : I%s\n", type_name, type_name);
	f.write("{\n");

	f.write("\tprotected I%s _data;\n", type_name);
	f.write("\tpublic %s_readonly(I%s data) { this._data = data; }\n", type_name, type_name);

	auto& mem = s.getMembers();
	for (size_t i = 0; i < mem.size(); ++i)
	{
		auto& it = mem[i];
		assert(it != nullptr);
		generateCsharpReadOnlyMember(f, *it);
	}

	generateCsharpSimpleEquivalentMethod(f, type_name, "this._data");

	f.write("} // class %s_readonly\n\n", type_name);
}

}
