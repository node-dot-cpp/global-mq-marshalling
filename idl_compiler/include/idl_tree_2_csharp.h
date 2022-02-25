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

#ifndef IDL_TREE_2_CSHARP_H
#define IDL_TREE_2_CSHARP_H

#include "idl_tree.h"
#include "idl_tree_serializer.h"

/// Small wrapper to migrate from old fprintf to fmt::fprintf, and add basic auto-indent.
/// Intent is to be relaxed and help the user. Current limitation is that we don't detect for
/// EOL '\n', in text, but need user calling one 'write' for each line. that is a call to 'write'
/// has a one to one relationship with a line of source code.
/// Such limitation can be easily relaxed by scanning the text for '\n' but so far has not been
/// required.

class CsharpWritter
{
	FILE* file = nullptr;
	std::string currentIndent;

public:
	CsharpWritter(FILE* file, size_t initialIndent) : file(file), currentIndent(initialIndent, '\t') {}

	FILE* getFile() { return file; }

	CsharpWritter indent(size_t i = 1);
	// mb: make variadic only if real necesity
	void write(const char* text);
	void write(const char* format, const char* arg0);
	void write(const char* format, const char* arg0, const char* arg1);
	void write(const char* format, const char* arg0, const char* arg1, const char* arg2);
	void write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3);
	void write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3, const char* arg4);
};


//shared by message and publishables

void checkCsharpStruct(CompositeType& s);
std::string getCaseTypeName(CompositeType& s);
const char* getCSharpPrimitiveType(MessageParameterType::KIND kind);
const char* getIdlPrimitiveType(MessageParameterType::KIND kind);
const char* getIdlPrimitiveType2(MessageParameterType::KIND kind);
std::string getCSharpInterfaceType(MessageParameterType& type);
std::string getCSharpElementInterfaceType(MessageParameterType::KIND kind, const std::string& name);
std::string getCSharpElementType(MessageParameterType::KIND kind, const std::string& name);
std::string getCSharpPrefixByType(MessageParameterType& type);

std::string generateCsharpDeclParams(CompositeType& s);
std::string generateCsharpCallerParams(CompositeType& s, bool valPrefix);

void generateCsharpStandardMethods(CsharpWritter f, const char* type_name);
void generateCsharpStructEquivalentExpression(CsharpWritter f, CompositeType& s);
void generateCsharpStructEquivalentMethod(CsharpWritter f, CompositeType& s, const char* type_name);
void generateCsharpSimpleEquivalentMethod(CsharpWritter f, const char* type_name, const char* member_name);
void generateCsharpInterfaceMember(CsharpWritter f, MessageParameter& member);
void generateCsharpStructInterface(CsharpWritter f, CompositeType& s, const char* type_name);
void generateCsharpStructImpl(CsharpWritter f, CompositeType& s, const char* type_name, const char* interface_name);
void generateCsharpStructMessage(CsharpWritter f, CompositeType& s, const char* type_name, const char* interface_name);
void generateCsharpUnionInterface(CsharpWritter f, CompositeType& s);
void generateCsharpUnionImpl(CsharpWritter f, CompositeType& s);
void generateCsharpUnionMessage(CsharpWritter f, CompositeType& s);
void generateCsharpSubscriberFactoryMethod(CsharpWritter f, MessageParameter& member);
void generateCsharpSubscriberEventHandler(CsharpWritter f, MessageParameter& member);
void generateCsharpSubscriberMember(CsharpWritter f, MessageParameter& member);
void generateCsharpPublisherMember(CsharpWritter f, MessageParameter& member);

void generateCsharpCaseSubscriber(CsharpWritter f, CompositeType& s, const char* type_name, const char* du_name);
void generateCsharpCasePublisher(CsharpWritter f, CompositeType& s, const char* type_name, const char* du_name);

void generateCsharpStructSubscriber(CsharpWritter f, CompositeType& s, const char* type_name);
void generateCsharpStructPublisher(CsharpWritter f, CompositeType& s, const char* type_name);
void generateCsharpStructConcentrator(CsharpWritter f, CompositeType& s, const char* type_name);
void generateCsharpConcentratorFactory(CsharpWritter f, Root& root);

void generateCsharpUnionSubscriber(CsharpWritter f, CompositeType& s, const char* type_name);
void generateCsharpUnionPublisher(CsharpWritter f, CompositeType& s, const char* type_name);

// code generation
void generateCsharp(FILE* file, Root& root, const std::string& metascope);
void generateCsharpMessageScope(CsharpWritter f, Root& root, Scope& scope);





#endif // IDL_TREE_2_CSHARP_H