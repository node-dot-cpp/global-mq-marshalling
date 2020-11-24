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


%token KW_FILE
%token KW_MESSAGE
%token KW_PUBLISHABLE
%token KW_STRUCT
%token KW_NONEXTENDABLE
%token KW_EXTENSION
%token KW_HASH_LINE
%token KW_DEFAULT
%token KW_EMPTY
%token KW_MAX_LENGTH
%token KW_ENUM
%token KW_BYTE_ARRAY
%token IDENTIFIER
%token KW_IDENTIFIER
%token STRING_LITERAL INTEGER_LITERAL
%token KW_INTEGER KW_UINTEGER KW_REAL
%token KW_CHARACTER_STRING
%token KW_BLOB
%token KW_VECTOR
%token KW_PROTO
%token KW_SCOPE

%error-verbose
%start file
%destructor { releaseYys($$); } <>

%{
#include "global_mq_parser_helper.h"
extern int yylex();
%}

%%

file : { $$ = 0; }
	| file line_directive { $$ = 0; releaseYys2($1, $2); }
	| file message { $$ = addMessageToFile($1, $2); }
	| file message_alias { $$ = addMessageToFile($1, $2); }
	| file publishable { $$ = addPublishableToFile($1, $2); }
	| file struct { $$ = addStructToFile($1, $2); }
	| file scope { $$ = addScopeToFile($1, $2); }
;

line_directive
	: KW_HASH_LINE INTEGER_LITERAL ';' { $$ = 0; processLineDirective($2, 0); releaseYys2($1, $3); }
	| KW_HASH_LINE INTEGER_LITERAL STRING_LITERAL ';' { $$ = 0; processLineDirective($2, $3); releaseYys2($1, $4); }
;


scope
	: KW_SCOPE IDENTIFIER KW_PROTO '=' proto_values ';' { $$ = createScope($1, $2, $5); releaseYys3($3, $4, $6); }

message_alias
	: KW_MESSAGE IDENTIFIER ':' IDENTIFIER '=' INTEGER_LITERAL '=' KW_STRUCT IDENTIFIER ';' { $$ = createMessageAlias($1, false, $2, $4, $6, $9); releaseYys5($3, $5, $7, $8, $10); }
	| KW_MESSAGE KW_NONEXTENDABLE IDENTIFIER ':' IDENTIFIER '=' INTEGER_LITERAL '=' KW_STRUCT IDENTIFIER ';' { $$ = createMessageAlias($1, true, $3, $5, $7, $10); releaseYys6($2, $4, $6, $8, $9, $11); }
;

message_begin
	: KW_MESSAGE IDENTIFIER ':' IDENTIFIER '=' INTEGER_LITERAL '{' { $$ = createMessage($1, false, $2, $4, $6); releaseYys3($3, $5, $7); }
	| KW_MESSAGE KW_NONEXTENDABLE IDENTIFIER ':' IDENTIFIER '=' INTEGER_LITERAL '{' { $$ = createMessage($1, true, $3, $5, $7); releaseYys4($2, $4, $6, $8); }
;

message_and_body
	: message_begin data_type IDENTIFIER ';' { $$ = addToMessage($1, createAttribute($2, $3)); releaseYys($4); }
	| message_begin KW_EXTENSION ':' { $$ = insertExtensionMarkerToMessage($1); releaseYys2($2, $3); }
	| message_and_body data_type IDENTIFIER ';' { $$ = addToMessage($1, createAttribute($2, $3)); releaseYys($4); }
	| message_and_body KW_EXTENSION ':' { $$ = insertExtensionMarkerToMessage($1); releaseYys2($2, $3); }
;

message
	: message_begin '}' ';' { $$ = $1; releaseYys2($2, $3); }
	| message_and_body '}' ';' { $$ = $1; releaseYys2($2, $3); }
;

publishable_begin
	: KW_PUBLISHABLE IDENTIFIER '{' { $$ = createPublishable($1, false, $2); releaseYys($3); }
	| KW_PUBLISHABLE KW_NONEXTENDABLE IDENTIFIER '{' { $$ = createPublishable($1, true, $3); releaseYys2($2, $4); }
	| publishable_begin data_type IDENTIFIER ';' { $$ = addToPublishable($1, createAttribute($2, $3)); releaseYys($4); }
	| publishable_begin KW_EXTENSION ':' { $$ = insertExtensionMarkerToPublishable($1); releaseYys2($2, $3); }
;

publishable
	: publishable_begin '}' ';' { $$ = $1; releaseYys2($2, $3); }
;

struct_begin
	: KW_STRUCT IDENTIFIER '{' { $$ = createStruct($1, false, $2); releaseYys($3); }
	| KW_STRUCT KW_NONEXTENDABLE IDENTIFIER '{' { $$ = createStruct($1, true, $3); releaseYys2($2, $4); }
	| struct_begin data_type IDENTIFIER ';' { $$ = addToStruct($1, createAttribute($2, $3)); releaseYys($4); }
	| struct_begin KW_EXTENSION ':' { $$ = insertExtensionMarkerToStruct($1); releaseYys2($2, $3); }
;

struct
	: struct_begin '}' ';' { $$ = $1; releaseYys2($2, $3); }
;

data_type
	: integer_type
	| unsigned_integer_type
	| real_type
	| character_string_type
	| byte_array_type
	| inline_enum_type
	| blob_type
	| vector_type
	| struct_type
;

integer_type
	: KW_INTEGER { $$ = createIntegerType($1); }
	| KW_INTEGER KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefault($1, $4); releaseYys2($2, $3); }
	| KW_INTEGER '[' expr ',' expr ']' { $$ = createIntegerTypeWithLimits($1, true, true, $3, true, $5, true); releaseYys3($2, $4, $6); }
	| KW_INTEGER '(' expr ',' expr ']' { $$ = createIntegerTypeWithLimits($1, true, false, $3, true, $5, true); releaseYys3($2, $4, $6); }
	| KW_INTEGER '[' expr ',' expr ')' { $$ = createIntegerTypeWithLimits($1, true, true, $3, true, $5, false); releaseYys3($2, $4, $6); }
	| KW_INTEGER '(' expr ',' expr ')' { $$ = createIntegerTypeWithLimits($1, true, false, $3, true, $5, false); releaseYys3($2, $4, $6); }
	| KW_INTEGER '[' ',' expr ']' { $$ = createIntegerTypeWithLimits($1, false, true, 0, true, $4, true); releaseYys3($2, $3, $5); }
	| KW_INTEGER '(' ',' expr ']' { $$ = createIntegerTypeWithLimits($1, false, false, 0, true, $4, true); releaseYys3($2, $3, $5); }
	| KW_INTEGER '[' ',' expr ')' { $$ = createIntegerTypeWithLimits($1, false, true, 0, true, $4, false); releaseYys3($2, $3, $5); }
	| KW_INTEGER '(' ',' expr ')' { $$ = createIntegerTypeWithLimits($1, false, false, 0, true, $4, false); releaseYys3($2, $3, $5); }
	| KW_INTEGER '[' expr ',' ']' { $$ = createIntegerTypeWithLimits($1, true, true, $3, false, 0, true); releaseYys3($2, $4, $5); }
	| KW_INTEGER '(' expr ',' ']' { $$ = createIntegerTypeWithLimits($1, true, false, $3, false, 0, true); releaseYys3($2, $4, $5); }
	| KW_INTEGER '[' expr ',' ')' { $$ = createIntegerTypeWithLimits($1, true, true, $3, false, 0, false); releaseYys3($2, $4, $5); }
	| KW_INTEGER '(' expr ',' ')' { $$ = createIntegerTypeWithLimits($1, true, false, $3, false, 0, false); releaseYys3($2, $4, $5); }
	| KW_INTEGER '[' expr ',' expr ']' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, true, true, $3, true, $5, true, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_INTEGER '(' expr ',' expr ']' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, true, false, $3, true, $5, true, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_INTEGER '[' expr ',' expr ')' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, true, true, $3, true, $5, false, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_INTEGER '(' expr ',' expr ')' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, true, false, $3, true, $5, false, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_INTEGER '[' ',' expr ']' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, false, true, 0, true, $4, true, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_INTEGER '(' ',' expr ']' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, false, false, 0, true, $4, true, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_INTEGER '[' ',' expr ')' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, false, true, 0, true, $4, false, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_INTEGER '(' ',' expr ')' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, false, false, 0, true, $4, false, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_INTEGER '[' expr ',' ']' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, true, true, $3, false, 0, true, $8); releaseYys5($2, $4, $5, $6, $7); }
	| KW_INTEGER '(' expr ',' ']' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, true, false, $3, false, 0, true, $8); releaseYys5($2, $4, $5, $6, $7); }
	| KW_INTEGER '[' expr ',' ')' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, true, true, $3, false, 0, false, $8); releaseYys5($2, $4, $5, $6, $7); }
	| KW_INTEGER '(' expr ',' ')' KW_DEFAULT '=' expr { $$ = createIntegerTypeWithDefaultAndLimits($1, true, false, $3, false, 0, false, $8); releaseYys5($2, $4, $5, $6, $7); }
;

unsigned_integer_type
	: KW_UINTEGER { $$ = createUnsignedIntegerType($1); }
	| KW_UINTEGER KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefault($1, $4); releaseYys2($2, $3); }
	| KW_UINTEGER '[' expr ',' expr ']' { $$ = createUnsignedIntegerTypeWithLimits($1, true, true, $3, true, $5, true); releaseYys3($2, $4, $6); }
	| KW_UINTEGER '(' expr ',' expr ']' { $$ = createUnsignedIntegerTypeWithLimits($1, true, false, $3, true, $5, true); releaseYys3($2, $4, $6); }
	| KW_UINTEGER '[' expr ',' expr ')' { $$ = createUnsignedIntegerTypeWithLimits($1, true, true, $3, true, $5, false); releaseYys3($2, $4, $6); }
	| KW_UINTEGER '(' expr ',' expr ')' { $$ = createUnsignedIntegerTypeWithLimits($1, true, false, $3, true, $5, false); releaseYys3($2, $4, $6); }
	| KW_UINTEGER '[' ',' expr ']' { $$ = createUnsignedIntegerTypeWithLimits($1, false, true, 0, true, $4, true); releaseYys3($2, $3, $5); }
	| KW_UINTEGER '(' ',' expr ']' { $$ = createUnsignedIntegerTypeWithLimits($1, false, false, 0, true, $4, true); releaseYys3($2, $3, $5); }
	| KW_UINTEGER '[' ',' expr ')' { $$ = createUnsignedIntegerTypeWithLimits($1, false, true, 0, true, $4, false); releaseYys3($2, $3, $5); }
	| KW_UINTEGER '(' ',' expr ')' { $$ = createUnsignedIntegerTypeWithLimits($1, false, false, 0, true, $4, false); releaseYys3($2, $3, $5); }
	| KW_UINTEGER '[' expr ',' ']' { $$ = createUnsignedIntegerTypeWithLimits($1, true, true, $3, false, 0, true); releaseYys3($2, $4, $5); }
	| KW_UINTEGER '(' expr ',' ']' { $$ = createUnsignedIntegerTypeWithLimits($1, true, false, $3, false, 0, true); releaseYys3($2, $4, $5); }
	| KW_UINTEGER '[' expr ',' ')' { $$ = createUnsignedIntegerTypeWithLimits($1, true, true, $3, false, 0, false); releaseYys3($2, $4, $5); }
	| KW_UINTEGER '(' expr ',' ')' { $$ = createUnsignedIntegerTypeWithLimits($1, true, false, $3, false, 0, false); releaseYys3($2, $4, $5); }
	| KW_UINTEGER '[' expr ',' expr ']' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, true, true, $3, true, $5, true, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_UINTEGER '(' expr ',' expr ']' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, true, false, $3, true, $5, true, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_UINTEGER '[' expr ',' expr ')' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, true, true, $3, true, $5, false, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_UINTEGER '(' expr ',' expr ')' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, true, false, $3, true, $5, false, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_UINTEGER '[' ',' expr ']' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, false, true, 0, true, $4, true, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_UINTEGER '(' ',' expr ']' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, false, false, 0, true, $4, true, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_UINTEGER '[' ',' expr ')' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, false, true, 0, true, $4, false, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_UINTEGER '(' ',' expr ')' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, false, false, 0, true, $4, false, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_UINTEGER '[' expr ',' ']' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, true, true, $3, false, 0, true, $8); releaseYys5($2, $4, $5, $6, $7); }
	| KW_UINTEGER '(' expr ',' ']' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, true, false, $3, false, 0, true, $8); releaseYys5($2, $4, $5, $6, $7); }
	| KW_UINTEGER '[' expr ',' ')' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, true, true, $3, false, 0, false, $8); releaseYys5($2, $4, $5, $6, $7); }
	| KW_UINTEGER '(' expr ',' ')' KW_DEFAULT '=' expr { $$ = createUnsignedIntegerTypeWithDefaultAndLimits($1, true, false, $3, false, 0, false, $8); releaseYys5($2, $4, $5, $6, $7); }
;

real_type
	: KW_REAL { $$ = createRealType($1); }
	| KW_REAL KW_DEFAULT '=' expr { $$ = createRealTypeWithDefault($1, $4); releaseYys2($2, $3); }
	| KW_REAL '[' expr ',' expr ']' { $$ = createRealTypeWithLimits($1, true, true, $3, true, $5, true); releaseYys3($2, $4, $6); }
	| KW_REAL '(' expr ',' expr ']' { $$ = createRealTypeWithLimits($1, true, false, $3, true, $5, true); releaseYys3($2, $4, $6); }
	| KW_REAL '[' expr ',' expr ')' { $$ = createRealTypeWithLimits($1, true, true, $3, true, $5, false); releaseYys3($2, $4, $6); }
	| KW_REAL '(' expr ',' expr ')' { $$ = createRealTypeWithLimits($1, true, false, $3, true, $5, false); releaseYys3($2, $4, $6); }
	| KW_REAL '[' ',' expr ']' { $$ = createRealTypeWithLimits($1, false, true, 0, true, $4, true); releaseYys3($2, $3, $5); }
	| KW_REAL '(' ',' expr ']' { $$ = createRealTypeWithLimits($1, false, false, 0, true, $4, true); releaseYys3($2, $3, $5); }
	| KW_REAL '[' ',' expr ')' { $$ = createRealTypeWithLimits($1, false, true, 0, true, $4, false); releaseYys3($2, $3, $5); }
	| KW_REAL '(' ',' expr ')' { $$ = createRealTypeWithLimits($1, false, false, 0, true, $4, false); releaseYys3($2, $3, $5); }
	| KW_REAL '[' expr ',' ']' { $$ = createRealTypeWithLimits($1, true, true, $3, false, 0, true); releaseYys3($2, $4, $5); }
	| KW_REAL '(' expr ',' ']' { $$ = createRealTypeWithLimits($1, true, false, $3, false, 0, true); releaseYys3($2, $4, $5); }
	| KW_REAL '[' expr ',' ')' { $$ = createRealTypeWithLimits($1, true, true, $3, false, 0, false); releaseYys3($2, $4, $5); }
	| KW_REAL '(' expr ',' ')' { $$ = createRealTypeWithLimits($1, true, false, $3, false, 0, false); releaseYys3($2, $4, $5); }
	| KW_REAL '[' expr ',' expr ']' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, true, true, $3, true, $5, true, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_REAL '(' expr ',' expr ']' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, true, false, $3, true, $5, true, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_REAL '[' expr ',' expr ')' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, true, true, $3, true, $5, false, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_REAL '(' expr ',' expr ')' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, true, false, $3, true, $5, false, $9); releaseYys5($2, $4, $6, $7, $8); }
	| KW_REAL '[' ',' expr ']' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, false, true, 0, true, $4, true, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_REAL '(' ',' expr ']' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, false, false, 0, true, $4, true, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_REAL '[' ',' expr ')' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, false, true, 0, true, $4, false, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_REAL '(' ',' expr ')' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, false, false, 0, true, $4, false, $8); releaseYys5($2, $3, $5, $6, $7); }
	| KW_REAL '[' expr ',' ']' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, true, true, $3, false, 0, true, $8); releaseYys5($2, $4, $5, $6, $7); }
	| KW_REAL '(' expr ',' ']' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, true, false, $3, false, 0, true, $8); releaseYys5($2, $4, $5, $6, $7); }
	| KW_REAL '[' expr ',' ')' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, true, true, $3, false, 0, false, $8); releaseYys5($2, $4, $5, $6, $7); }
	| KW_REAL '(' expr ',' ')' KW_DEFAULT '=' expr { $$ = createRealTypeWithDefaultAndLimits($1, true, false, $3, false, 0, false, $8); releaseYys5($2, $4, $5, $6, $7); }
;


character_string_type
	: KW_CHARACTER_STRING { $$ = createCharacterStringType($1, false, 0, false, 0); }
	| KW_CHARACTER_STRING KW_MAX_LENGTH '=' expr { $$ = createCharacterStringType($1, true, $4, false, 0); releaseYys2($2, $3); }
	| KW_CHARACTER_STRING KW_DEFAULT '=' STRING_LITERAL { $$ = createCharacterStringType($1, false, 0, true, $4); releaseYys2($2, $3); }
	| KW_CHARACTER_STRING KW_MAX_LENGTH '=' expr KW_DEFAULT '=' STRING_LITERAL { $$ = createCharacterStringType($1, true, $4, true, $7); releaseYys4($2, $3, $5, $6); }
	| KW_CHARACTER_STRING KW_DEFAULT '=' STRING_LITERAL KW_MAX_LENGTH '=' expr { $$ = createCharacterStringType($1, true, $7, true, $4); releaseYys4($2, $3, $5, $6); }
/*	| KW_CHARACTER_STRING '{' STRING_LITERAL '}' { $$ = createCharacterStringType4($1, $3, 0, 0); releaseYys2($2, $4); }
	| KW_CHARACTER_STRING '{' STRING_LITERAL '}' '[' expr ',' expr ']' { $$ = createCharacterStringType4($1, $3, $6, $8); releaseYys5($2, $4, $5, $7, $9); }
	| KW_CHARACTER_STRING '{' character_set '}' { $$ = createCharacterStringType4($1, $3, 0, 0); releaseYys2($2, $4); }
	| KW_CHARACTER_STRING '{' character_set '}' '[' expr ',' expr ']' { $$ = createCharacterStringType4($1, $3, $6, $8); releaseYys5($2, $4, $5, $7, $9); }*/
;

byte_array_type
	: KW_BYTE_ARRAY '[' expr ']' { $$ = createByteArrayType($1, $3); releaseYys2($2, $4); }
;

vector_type
	: KW_VECTOR '<' KW_INTEGER '>' { $$ = createVectorOfIntegerType($1, false); releaseYys3($2, $3, $4); }
	| KW_VECTOR '<' KW_UINTEGER '>' { $$ = createVectorOfUintegerType($1, false); releaseYys3($2, $3, $4); }
	| KW_VECTOR '<' KW_REAL '>' { $$ = createVectorOfRealType($1, false); releaseYys3($2, $3, $4); }
	| KW_VECTOR '<' KW_CHARACTER_STRING '>' { $$ = createVectorOfCharStringType($1, false); releaseYys3($2, $3, $4); }
	| KW_VECTOR '<' KW_BLOB '>' { $$ = createVectorOfBLOBType($1, false); releaseYys3($2, $3, $4); }
	| KW_VECTOR '<' KW_BYTE_ARRAY '>' { $$ = createVectorOfByteArrayType($1, false); releaseYys3($2, $3, $4); }
	| KW_VECTOR '<' KW_STRUCT IDENTIFIER '>' { $$ = createVectorOfStructsType($1, $4, false, false); releaseYys3($2, $3, $5); }
	| KW_VECTOR '<' KW_STRUCT KW_NONEXTENDABLE IDENTIFIER '>' { $$ = createVectorOfStructsType($1, $5, true, false); releaseYys4($2, $3, $4, $6); }
	| KW_VECTOR '<' KW_INTEGER '>' KW_DEFAULT '=' KW_EMPTY { $$ = createVectorOfIntegerType($1, true); releaseYys6($2, $3, $4, $5, $6, $7); }
	| KW_VECTOR '<' KW_UINTEGER '>' KW_DEFAULT '=' KW_EMPTY { $$ = createVectorOfUintegerType($1, true); releaseYys6($2, $3, $4, $5, $6, $7); }
	| KW_VECTOR '<' KW_REAL '>' KW_DEFAULT '=' KW_EMPTY { $$ = createVectorOfRealType($1, true); releaseYys6($2, $3, $4, $5, $6, $7); }
	| KW_VECTOR '<' KW_CHARACTER_STRING '>' KW_DEFAULT '=' KW_EMPTY { $$ = createVectorOfCharStringType($1, true); releaseYys6($2, $3, $4, $5, $6, $7); }
	| KW_VECTOR '<' KW_BLOB '>' KW_DEFAULT '=' KW_EMPTY { $$ = createVectorOfBLOBType($1, true); releaseYys6($2, $3, $4, $5, $6, $7); }
	| KW_VECTOR '<' KW_BYTE_ARRAY '>' KW_DEFAULT '=' KW_EMPTY { $$ = createVectorOfByteArrayType($1, true); releaseYys6($2, $3, $4, $5, $6, $7); }
	| KW_VECTOR '<' KW_STRUCT IDENTIFIER '>' KW_DEFAULT '=' KW_EMPTY { $$ = createVectorOfStructsType($1, $4, false, true); releaseYys6($2, $3, $5, $6, $7, $8); }
	| KW_VECTOR '<' KW_STRUCT KW_NONEXTENDABLE IDENTIFIER '>' KW_DEFAULT '=' KW_EMPTY { $$ = createVectorOfStructsType($1, $5, true, true); releaseYys7($2, $3, $4, $6, $7, $8, $9); }
	| KW_VECTOR '<' KW_NONEXTENDABLE KW_STRUCT IDENTIFIER '>' KW_DEFAULT '=' KW_EMPTY { $$ = createVectorOfStructsType($1, $5, true, true); releaseYys7($2, $3, $4, $6, $7, $8, $9); }
;

struct_type
	: KW_STRUCT IDENTIFIER { $$ = createStructType($1, false, $2); }
	| KW_STRUCT KW_NONEXTENDABLE IDENTIFIER { $$ = createStructType($1, true, $3); releaseYys( $2 );}
	| KW_NONEXTENDABLE KW_STRUCT IDENTIFIER { $$ = createStructType($2, true, $3); releaseYys( $1 ); }

blob_type
	: KW_BLOB { $$ = createBlobType($1); }
;

inline_enum_type
	: KW_ENUM qname '{' enum_values '}' { $$ = createInlineEnum($1, $2, $4); releaseYys2($3, $5); }
	| KW_ENUM qname '{' enum_values '}' KW_DEFAULT '=' IDENTIFIER { $$ = createInlineEnumWithDefault($1, $2, $4, $8); releaseYys4($3, $5, $6, $7); }
;

enum_values
	: IDENTIFIER '=' INTEGER_LITERAL { $$ = addEnumValue(0, $1, $3); releaseYys($2); }
	| enum_values ',' IDENTIFIER '=' INTEGER_LITERAL { $$ = addEnumValue($1, $3, $5); releaseYys2($2, $4); }
	| KW_IDENTIFIER '(' STRING_LITERAL ')' '=' INTEGER_LITERAL { $$ = addEnumValue(0, $3, $6); releaseYys4($1, $2, $4, $5); }
	| enum_values ',' KW_IDENTIFIER '(' STRING_LITERAL ')' '=' INTEGER_LITERAL { $$ = addEnumValue($1, $5, $8); releaseYys3($2, $3, $4); releaseYys2($6, $7); }
;

proto_values
	: IDENTIFIER { $$ = addProtoValue(0, $1); }
	| proto_values ',' IDENTIFIER { $$ = addProtoValue($1, $3); releaseYys($2); }
;

expr
	: INTEGER_LITERAL { $$ = $1; }
	| '+' INTEGER_LITERAL { $$ = $2; releaseYys($1); }
	| '-' INTEGER_LITERAL { $$ = makeMinusIntLit($2); releaseYys($1); }
	| STRING_LITERAL { $$ = $1; }
;

qname
	: IDENTIFIER { $$ = $1; }
;