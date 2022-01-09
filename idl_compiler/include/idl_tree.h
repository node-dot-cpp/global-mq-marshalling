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

#ifndef IDL_TREE_H
#define IDL_TREE_H

#include "common.h"

using namespace std;

struct Limit {
	bool inclusive = false;
	double value = 0;
};

struct Location {
	string fileName;
	int lineNumber = 0;
};

class Variant {
public:
	enum KIND { NONE, NUMBER, STRING };
	KIND kind = NONE;

	double numberValue = 0;
	string stringValue;

	Variant() :kind(NONE) {}
	Variant(double value) :kind(NUMBER), numberValue(value) {}
	Variant(string value) :kind(STRING), stringValue(std::move(value)) {}
};

class MessageParameterType
{
public:
	enum KIND { UNDEFINED, EXTENSION, ENUM, INTEGER, UINTEGER, REAL, CHARACTER_STRING, BYTE_ARRAY, BLOB, VECTOR, STRUCT, DISCRIMINATED_UNION };
	static bool isNumericType( KIND kind ) { return kind == KIND::INTEGER || kind == KIND::UINTEGER || kind == KIND::REAL; }
	static bool isNamedType( KIND kind ) { return kind == KIND::ENUM || kind == KIND::STRUCT || kind == KIND::DISCRIMINATED_UNION; }
	KIND kind = UNDEFINED;
	bool isNumericType() { return isNumericType( kind ); }
	bool isNamedType() { return isNamedType( kind ); }
	string name;
	bool hasDefault = false; // INTEGER, UINTEGER, CHARACTER_STRING
	bool hasLowLimit = false; // INTEGER, UINTEGER
	bool hasHighLimit = false; // INTEGER, UINTEGER
	Limit lowLimit; // INTEGER, UINTEGER
	Limit highLimit; // INTEGER, UINTEGER
	double numericalDefault; // INTEGER, UINTEGER

//    CharacterSet characterSet;
	bool hasMaxLength = false; // CHARACTER_STRING
	uint32_t stringMaxLength = 0; // CHARACTER_STRING
	string stringDefault; // CHARACTER_STRING

	uint32_t arrayFixedaxSize = 0; // BYTE_ARRAY

	KIND vectorElemKind = UNDEFINED; // VECTOR
	bool isNonExtendable = false; // VECTOR
	size_t messageIdx; // VECTOR of MESSAGEs, reserved for postprocessing

	map<string, uint32_t> enumValues; // ENUM

	MessageParameterType() = default;
	MessageParameterType(const MessageParameterType& other)
		: kind(other.kind), name(other.name),
		lowLimit(other.lowLimit), highLimit(other.highLimit),
		stringMaxLength(other.stringMaxLength),
		enumValues(other.enumValues)
	{}

	MessageParameterType& operator = (const MessageParameterType& other)
	{
		if (this != &other)
			*this = std::move(MessageParameterType(other));

		return *this;
	}

	MessageParameterType(MessageParameterType&& other) = default;
	MessageParameterType& operator = (MessageParameterType&& other) = default;
};


class ObjectBase
{
public:
	Location location;
	virtual ~ObjectBase() {}
};

class MessageParameter : public ObjectBase
{
public:
	MessageParameterType type;
	string name;
	bool extendTo = false;
	Variant defaultValue;
//	vector<string> whenDiscriminant;
	bool duCaseParam = false; // set at time of tree checking and code generation
	std::string caseName; // for discriminated union case members
};

enum Proto { undefined, json, gmq };

class CompositeType : public ObjectBase
{
private:
	// Note: indeed, CompositeType is a discriminated union itself: all types but discriminated union have members (and not cases), discriminated union has cases only
	//       what's not checked right while parsing, must be ensured at a later checks
	// TODO: think about better representation (if any)
	vector<unique_ptr<MessageParameter>> structMembers; // all but discriminated_union
	vector<unique_ptr<CompositeType>> discriminatedUnionCases; // for discriminated_union only

public:
	static constexpr uint64_t invalid_num_id = (uint64_t)(-1);

public:
	enum Type { undefined = 0,  message = 1, publishable = 2, structure = 3, discriminated_union_case = 4, discriminated_union = 5 };
	Type type = Type::undefined;
	static const char* type2string( Type type_ )
	{
		switch ( type_ )
		{
			case Type::undefined: return "UNDEFINED";
			case Type::message: return "MESSAGE";
			case Type::publishable: return "PUBLISHABLE";
			case Type::structure: return "STRUCT";
			case Type::discriminated_union_case: return "CASE";
			case Type::discriminated_union: return "DISCRIMINATED_UNION";
			default:
				assert( false );
				return "";
		}
	}
	const char* type2string() { return type2string( type ); }

public:
	string scopeName;
	string name;
	uint64_t numID = invalid_num_id; // we will analize/sanitize it at time of code generation
	bool isNonExtendable = false;
	bool isAlias = false;
	string aliasOf;
	uint64_t aliasIdx = invalid_num_id; // set at time of code generation
	
	bool isDiscriminatedUnion() const { assert( ( type == Type::discriminated_union && structMembers.empty() ) || ( type != Type::discriminated_union && discriminatedUnionCases.empty() ) ); return type == Type::discriminated_union; }
	vector<unique_ptr<MessageParameter>>& getMembers() { assert( type != Type::discriminated_union ); assert( discriminatedUnionCases.empty() ); return structMembers; }
	vector<unique_ptr<CompositeType>>& getDiscriminatedUnionCases() { assert( type == Type::discriminated_union ); assert( structMembers.empty() ); return discriminatedUnionCases; }

	// preprocessing for code generation
	bool processingOK = true;
	bool isStruct4Messaging = false; // used for STRUCT only
	bool isStruct4Publishing = false; // used for STRUCT only, filled at time of preprocessing for code generation
	set<Proto> protoList; // populated at time of tree checking and code generation
	int dependsOnCnt = 0;
};

struct Scope : public ObjectBase // used in post-processing
{
public:
	string name;
	std::vector<CompositeType*> objectList;
	Proto proto = Proto::undefined;
	const char* type2string() { return "SCOPE"; }
};

class Root
{
public:
	vector<unique_ptr<CompositeType>> messages;
	vector<unique_ptr<CompositeType>> publishables;
	vector<unique_ptr<CompositeType>> structs;
	vector<unique_ptr<Scope>> scopes;
};

inline
string locationToString(const Location& loc)
{
	if (!loc.fileName.empty()) {
		if (loc.lineNumber != 0)
			return fmt::format("@{}:{}", loc.fileName, loc.lineNumber);
		else
			return string("@") + loc.fileName;
	}
	else
		return "";
}


#endif // IDL_TREE_H
