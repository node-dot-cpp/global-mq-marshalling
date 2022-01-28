/* -------------------------------------------------------------------------------
* Copyright (c) 2020, OLogN Technologies AG
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the OLogN Technologies AG nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
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


/*
This header is used to isolate lex/yacc parser from rest of the code.
It is necesary because of lex.yy.c and parse.tab.c being pure C code,
while the rest of the project is C++.
Also leaves function calls only at scan.l and parse.y files, and
all code goes inside parser_helper.cpp
*/

#ifndef PARSER_HELPER_H_INCLUDED
#define PARSER_HELPER_H_INCLUDED

struct YyBase;
typedef struct YyBase* YYSTYPE;
#define YYSTYPE_IS_DECLARED 1

void pushState(int state);
int popState();

extern "C" int yywrap();
void yyerror(const char* msg);

void parserError(const char* msg, const char* text, int line);
void parserErrorUnknownChar(char text, int line);

void releaseYys(YYSTYPE yys);
void releaseYys2(YYSTYPE yys0, YYSTYPE yys1);
void releaseYys3(YYSTYPE yys0, YYSTYPE yys1, YYSTYPE yys2);
void releaseYys4(YYSTYPE yys0, YYSTYPE yys1, YYSTYPE yys2, YYSTYPE yys3);
void releaseYys5(YYSTYPE yys0, YYSTYPE yys1, YYSTYPE yys2, YYSTYPE yys3, YYSTYPE yys4);
void releaseYys6(YYSTYPE yys0, YYSTYPE yys1, YYSTYPE yys2, YYSTYPE yys3, YYSTYPE yys4, YYSTYPE yys5);
void releaseYys7(YYSTYPE yys0, YYSTYPE yys1, YYSTYPE yys2, YYSTYPE yys3, YYSTYPE yys4, YYSTYPE yys5, YYSTYPE yys6);
void releaseYys8(YYSTYPE yys0, YYSTYPE yys1, YYSTYPE yys2, YYSTYPE yys3, YYSTYPE yys4, YYSTYPE yys5, YYSTYPE yys6, YYSTYPE yys7);

YYSTYPE createYyToken(const char* text, int line, int token);

YYSTYPE createIdentifier(const char* text, int line);
YYSTYPE createIntegerLiteral(const char* text, int line);
YYSTYPE createHexIntegerLiteral(const char* text, int line);
YYSTYPE createStringLiteral(const char* text, int line);
YYSTYPE createZeroLiteral(const char* text, int line);

YYSTYPE addScopeToFile(YYSTYPE file, YYSTYPE item);
YYSTYPE addMessageToFile(YYSTYPE file, YYSTYPE item);
YYSTYPE addPublishableToFile(YYSTYPE file, YYSTYPE item);
YYSTYPE addStructToFile(YYSTYPE file, YYSTYPE item);
YYSTYPE addDiscriminatedUnionToFile(YYSTYPE file, YYSTYPE item);

void processLineDirective(YYSTYPE line_number, YYSTYPE file_name);

YYSTYPE addToMessage(YYSTYPE decl, YYSTYPE attr);
YYSTYPE addToPublishable(YYSTYPE decl, YYSTYPE attr);
YYSTYPE addToStruct(YYSTYPE decl, YYSTYPE attr);
YYSTYPE addToDiscriminatedUnionCase(YYSTYPE decl, YYSTYPE attr);
YYSTYPE addToDiscriminatedUnion(YYSTYPE decl, YYSTYPE attr);

YYSTYPE createAttribute(YYSTYPE type, YYSTYPE id);
YYSTYPE insertExtensionMarkerToMessage(YYSTYPE decl);
YYSTYPE insertExtensionMarkerToPublishable(YYSTYPE decl);
YYSTYPE insertExtensionMarkerToStruct(YYSTYPE decl);

YYSTYPE createScope(YYSTYPE token, YYSTYPE id, YYSTYPE protoID);

YYSTYPE createMessage(YYSTYPE token, bool isNonExtendable, YYSTYPE scopeName, YYSTYPE id, YYSTYPE numID);
YYSTYPE createPublishable(YYSTYPE token, bool isNonExtendable, YYSTYPE id, YYSTYPE numID);
YYSTYPE createStruct(YYSTYPE token, bool isNonExtendable, YYSTYPE id);
YYSTYPE createDiscriminatedUnionCase(YYSTYPE token, bool isNonExtendable, YYSTYPE id, YYSTYPE numID);
YYSTYPE createDiscriminatedUnion(YYSTYPE token, bool isNonExtendable, YYSTYPE id);

YYSTYPE createMessageAlias(YYSTYPE token, bool isNonExtendable, YYSTYPE scopeName, YYSTYPE id, YYSTYPE numID, YYSTYPE structId);

YYSTYPE createIntegerType(YYSTYPE token);
YYSTYPE createIntegerTypeWithDefault(YYSTYPE token, YYSTYPE default_expr);
YYSTYPE createIntegerTypeWithLimits(YYSTYPE token, bool hasLowLimit, bool low_flag, YYSTYPE low_expr, bool hasHighLimit, YYSTYPE high_expr, bool high_flag);
YYSTYPE createIntegerTypeWithDefaultAndLimits(YYSTYPE token, bool hasLowLimit, bool low_flag, YYSTYPE low_expr, bool hasHighLimit, YYSTYPE high_expr, bool high_flag, YYSTYPE default_expr);

YYSTYPE createUnsignedIntegerType(YYSTYPE token);
YYSTYPE createUnsignedIntegerTypeWithDefault(YYSTYPE token, YYSTYPE default_expr);
YYSTYPE createUnsignedIntegerTypeWithLimits(YYSTYPE token, bool hasLowLimit, bool low_flag, YYSTYPE low_expr, bool hasHighLimit, YYSTYPE high_expr, bool high_flag);
YYSTYPE createUnsignedIntegerTypeWithDefaultAndLimits(YYSTYPE token, bool hasLowLimit, bool low_flag, YYSTYPE low_expr, bool hasHighLimit, YYSTYPE high_expr, bool high_flag, YYSTYPE default_expr);

YYSTYPE createRealType(YYSTYPE token);
YYSTYPE createRealTypeWithDefault(YYSTYPE token, YYSTYPE default_expr);
YYSTYPE createRealTypeWithLimits(YYSTYPE token, bool hasLowLimit, bool low_flag, YYSTYPE low_expr, bool hasHighLimit, YYSTYPE high_expr, bool high_flag);
YYSTYPE createRealTypeWithDefaultAndLimits(YYSTYPE token, bool hasLowLimit, bool low_flag, YYSTYPE low_expr, bool hasHighLimit, YYSTYPE high_expr, bool high_flag, YYSTYPE default_expr);

YYSTYPE createCharacterStringType(YYSTYPE token, bool hasMaxLength, YYSTYPE maxlength_expr, bool hasDefault, YYSTYPE default_expr);

YYSTYPE createByteArrayType(YYSTYPE token, YYSTYPE size_expr);

YYSTYPE createBlobType(YYSTYPE token);

YYSTYPE createVectorOfIntegerType(YYSTYPE token, bool hasDefault);
YYSTYPE createVectorOfUintegerType(YYSTYPE token, bool hasDefault);
YYSTYPE createVectorOfRealType(YYSTYPE token, bool hasDefault);
YYSTYPE createVectorOfCharStringType(YYSTYPE token, bool hasDefault);
YYSTYPE createVectorOfBLOBType(YYSTYPE token, bool hasDefault);
YYSTYPE createVectorOfByteArrayType(YYSTYPE token, bool hasDefault);
YYSTYPE createVectorOfStructsType(YYSTYPE token, YYSTYPE messageName, bool nonext, bool hasDefault);
YYSTYPE createVectorOfDiscriminatedUnionsType(YYSTYPE token, YYSTYPE messageName, bool nonext, bool hasDefault);
YYSTYPE createVectorOfDictionaryType(YYSTYPE token, YYSTYPE messageName, bool nonext, bool hasDefault);

YYSTYPE createDictionaryBeginWithIntegerKey(YYSTYPE token);
YYSTYPE createDictionaryBeginWithUintegerKey(YYSTYPE token);
YYSTYPE createDictionaryBeginWithCharStringKey(YYSTYPE token);

YYSTYPE createDictionaryWithIntegerValue(YYSTYPE token);
YYSTYPE createDictionaryWithUintegerValue(YYSTYPE token);
YYSTYPE createDictionaryWithRealValue(YYSTYPE token);
YYSTYPE createDictionaryWithCharStringValue(YYSTYPE token);
YYSTYPE createDictionaryWithBLOBValue(YYSTYPE token);
YYSTYPE createDictionaryWithByteArrayValue(YYSTYPE token);
YYSTYPE createDictionaryWithStructValue(YYSTYPE token, YYSTYPE structName);
YYSTYPE createDictionaryWithDiscriminatedUnionValue(YYSTYPE token, YYSTYPE structName);

YYSTYPE createStructType(YYSTYPE token, bool isNonExtendable, YYSTYPE structName);
YYSTYPE createDiscriminatedUnionType(YYSTYPE token, bool isNonExtendable, YYSTYPE structName);

YYSTYPE createInlineEnum(YYSTYPE token, YYSTYPE opt_id, YYSTYPE values);
YYSTYPE createInlineEnumWithDefault(YYSTYPE token, YYSTYPE opt_id, YYSTYPE values, YYSTYPE defaultValue);
YYSTYPE addEnumValue(YYSTYPE list, YYSTYPE id, YYSTYPE int_lit);

YYSTYPE addProtoValue(YYSTYPE list, YYSTYPE id);

YYSTYPE createScopeValue(YYSTYPE id);

YYSTYPE addIdentifier(YYSTYPE list, YYSTYPE id);
YYSTYPE addExpression(YYSTYPE list, YYSTYPE id, YYSTYPE expr);
YYSTYPE makeMinusIntLit(YYSTYPE int_lit);

#endif // PARSER_HELPER_H_INCLUDED
