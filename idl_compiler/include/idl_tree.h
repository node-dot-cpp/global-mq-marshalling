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
	enum KIND { UNDEFINED, EXTENSION, ENUM, INTEGER, UINTEGER, REAL, CHARACTER_STRING, BYTE_ARRAY, BLOB, VECTOR, MESSAGE, PUBLISHABLE, STRUCT };
	KIND kind = UNDEFINED;
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
	string compositeTypeName; // MESSAGEs, PUBLISHABLEs, STRUCTs (incl in VECTORs)
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
	vector<string> whenDiscriminant;
};


class CompositeType : public ObjectBase
{
public:
	vector<unique_ptr<MessageParameter>> members;
	string name;
	enum Proto { json, gmq };
	set<Proto> protoList;
	bool isNonExtendable = false;
};

class Root
{
public:
	vector<unique_ptr<CompositeType>> messages;
	vector<unique_ptr<CompositeType>> publishables;
	vector<unique_ptr<CompositeType>> structs;
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
