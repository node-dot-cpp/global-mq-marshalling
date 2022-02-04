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

//shared by message and publishables

void checkCsharpStruct(CompositeType& s);
std::string getCSharpTypeName(CompositeType& s);
const char* getCSharpPrimitiveType(MessageParameterType::KIND kind);
void generateCsharpDeclParams(FILE* header, Root& root, CompositeType& s);
void generateCsharpStandardMethods(FILE* header, const char* type_name);
void generateCsharpStructInterface(FILE* header, Root& root, CompositeType& s, const char* type_name);
void generateCsharpStructImpl(FILE* header, Root& root, CompositeType& s, const char* type_name, const char* interface_name);



// code generation
void generateCsharp(FILE* header, Root& root, const std::string& metascope);
void generateCsharpMessages(FILE* header, Root& root, const std::string& metascope);
void generateCsharpPublishables(FILE* header, Root& root, const std::string& metascope);
//void generateMessage( FILE* header, CompositeType& s );
//void generatePublishable( FILE* header, Root& root, CompositeType& s, std::string platformPrefix, std::string classNotifierName );
//void generatePublishableAsCStruct( FILE* header, Root& root, CompositeType& s );
//void generateMessageAlias( FILE* header, CompositeType& s );
//void generateMessageParameter( FILE* header, MessageParameter& s );
//void generateMessageMembers( FILE* header, CompositeType& s );
//void generateLimit( FILE* header, Limit& s );
//void generateLocation( FILE* header, Location& s );
//void generateDataType( FILE* header, MessageParameterType& s );
//void generate__unique_ptr_DataType( FILE* header, unique_ptr<MessageParameterType>& s );
//void generate__unique_ptr_MessageParameter( FILE* header, unique_ptr<MessageParameter>& s );
//
//void generateDataType( FILE* header, MessageParameterType& s );
//void generateLimit( FILE* header, Limit& s );
//void generateVariant( FILE* header, Variant& s );

#endif // IDL_TREE_2_CSHARP_H