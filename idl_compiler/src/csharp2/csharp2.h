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
#include "file_writter.h"
#include "idl_generators.h"


/// Small wrapper to migrate from old fprintf to fmt::fprintf, and add basic auto-indent.
/// Intent is to be relaxed and help the user. Current limitation is that we don't detect for
/// EOL '\n', in text, but need user calling one 'write' for each line. that is a call to 'write'
/// has a one to one relationship with a line of source code.
/// Such limitation can be easily relaxed by scanning the text for '\n' but so far has not been
/// required.

// class CsharpWritter
// {
// 	FILE* file = nullptr;
// 	std::string currentIndent;

// public:
// 	CsharpWritter(FILE* file, size_t initialIndent) : file(file), currentIndent(initialIndent, '\t') {}

// 	FILE* getFile() { return file; }

// 	CsharpWritter indent(size_t i = 1);
// 	// mb: make variadic only if real necesity
// 	void write(const char* text);
// 	void write(const char* format, const char* arg0);
// 	void write(const char* format, const char* arg0, const char* arg1);
// 	void write(const char* format, const char* arg0, const char* arg1, const char* arg2);
// 	void write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3);
// 	void write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3, const char* arg4);
// };

namespace csharp2
{

//shared by message and publishables

// void checkCsharpStruct(CompositeType& s);
std::string getCaseTypeName(CompositeType& s);
const char* getCSharpPrimitiveType(MessageParameterType::KIND kind);
const char* getIdlPrimitiveType(MessageParameterType::KIND kind);
const char* getIdlPrimitiveType2(MessageParameterType::KIND kind);
std::string getCSharpInterfaceType(MessageParameterType& type);
std::string getCSharpElementInterfaceType(MessageParameterType::KIND kind, const std::string& name);
std::string getCSharpPrefixByType(MessageParameterType& type);

std::string generateCsharpDeclParams(CompositeType& s);
std::string generateCsharpCallerParams(CompositeType& s, bool valPrefix);

void generateCsharpStandardMethods(FileWritter f, const char* type_name);
void generateCsharpStructEquivalentExpression(FileWritter f, CompositeType& s);
void generateCsharpStructEquivalentMethod(FileWritter f, CompositeType& s, const char* type_name);
void generateCsharpSimpleEquivalentMethod(FileWritter f, const char* type_name, const char* member_name);
void generateCsharpInterfaceMember(FileWritter f, MessageParameter& member);
void generateCsharpStructInterface(FileWritter f, CompositeType& s, const char* type_name);
void generateCsharpStructImpl(FileWritter f, CompositeType& s, const char* type_name, const char* interface_name);
void generateCsharpReadOnlyMember(FileWritter f, MessageParameter& member);
void generateCsharpStructReadOnly(FileWritter f, CompositeType& s);
void generateCsharpStructMessage(FileWritter f, CompositeType& s, const char* type_name, const char* interface_name);
void generateCsharpUnionInterface(FileWritter f, CompositeType& s);
void generateCsharpUnionImpl(FileWritter f, CompositeType& s);
void generateCsharpUnionReadOnly(FileWritter f, CompositeType& s);
void generateCsharpUnionMessage(FileWritter f, CompositeType& s);
void generateCsharpSubscriberFactoryMethod(FileWritter f, MessageParameter& member);
void generateCsharpSubscriberEventHandler(FileWritter f, MessageParameter& member);
void generateCsharpSubscriberMember(FileWritter f, MessageParameter& member);
void generateCsharpPublisherMember(FileWritter f, MessageParameter& member);

void generateCsharpCaseSubscriber(FileWritter f, CompositeType& s, const char* type_name, const char* du_name);
void generateCsharpCasePublisher(FileWritter f, CompositeType& s, const char* type_name, const char* du_name);

void generateCsharpStructSubscriber(FileWritter f, CompositeType& s, const char* type_name);
void generateCsharpStructPublisher(FileWritter f, CompositeType& s, const char* type_name);
void generateCsharpStructConcentrator(FileWritter f, CompositeType& s, const char* type_name);
void generateCsharpConcentratorFactory(FileWritter f, Root& root);

void generateCsharpUnionSubscriber(FileWritter f, CompositeType& s, const char* type_name);
void generateCsharpUnionPublisher(FileWritter f, CompositeType& s, const char* type_name);

// code generation
//void generateCsharp(FILE* file, Root& root, const std::string& metascope);
void generateCsharpMessageScope(FileWritter f, Root& root, Scope& scope, const GenerationConfig& config);

}



#endif // IDL_TREE_2_CSHARP_H