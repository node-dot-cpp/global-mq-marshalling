/* -------------------------------------------------------------------------------
* Copyright (c) 2018, OLogN Technologies AG
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


%{
#include "global_mq_parser_helper.h"
#include "global_mq_idl.tab.h"
#include <stdlib.h>

void jumpToState(int state)
{
	pushState(YYSTATE);
	BEGIN(state);
}

void backToPreviousState()
{
	BEGIN(popState());
}

int createToken(int token)
{
	yylval = createYyToken(yytext, yylineno, token);
	return token;
}

%}

%option yylineno
%option never-interactive
%s COMMENT SLCOMMENT

%%

<COMMENT>{
	"*/" { backToPreviousState(); }
	.|\n { /* ignore */ }
}

<SLCOMMENT>{
	\n { backToPreviousState(); }
	.  { /* ignore */ }
}

<INITIAL>{

	"/*" { jumpToState( COMMENT ) ; }
	"//" { jumpToState( SLCOMMENT ) ; }

	"#line" { return createToken( KW_HASH_LINE ); }

	"FILE" { return createToken( KW_FILE ); }

	"MESSAGE" { return createToken( KW_MESSAGE ); }
	"PUBLISHABLE-STRUCT" { return createToken( KW_PUBLISHABLE_STRUCT ); }
	"EXTENSION" { return createToken( KW_EXTENSION ); }
	"NONEXTENDABLE" { return createToken( KW_NONEXTENDABLE ); }
	"enum" { return createToken( KW_ENUM ); }

	"PROTO" { return createToken( KW_PROTO ); }

	"DEFAULT" { return createToken( KW_DEFAULT ); }
	"MAX_LENGTH" { return createToken( KW_MAX_LENGTH ); }
	"EMPTY" { return createToken( KW_EMPTY ); }

	"INTEGER" { return createToken( KW_INTEGER ); }
	"UINTEGER" { return createToken( KW_UINTEGER ); }
	"REAL" { return createToken( KW_REAL ); }
	"CHARACTER-STRING" { return createToken( KW_CHARACTER_STRING ); }
	"BYTE_ARRAY" { return createToken( KW_BYTE_ARRAY ); }
	"BLOB" { return createToken( KW_BLOB ); }
	"VECTOR" { return createToken( KW_VECTOR ); }


	/* INT32, DOUBLE, etc will be parsed as IDENTIFIER (easier),
		but currently idetifiers do not support hypen,
		so PRINTABLE-ASCII-STRING and UNICODE-STRING must be keywords */

	[;<>,:=&\[\]\(\)\{\}\?\.] { yylval = 0; return yytext[0];}
	[&|!~/%\*\+\-] { return createToken( yytext[0] );}

	[_a-zA-Z][_0-9a-zA-Z]* { yylval = createIdentifier(yytext, yylineno); return IDENTIFIER; }

	0[0-9]+ { parserError("Octal integer literal not supported '%s'.", yytext, yylineno); }

	[1-9][0-9]* { yylval = createIntegerLiteral(yytext, yylineno); return INTEGER_LITERAL; }
	0 { yylval = createZeroLiteral(yytext, yylineno); return INTEGER_LITERAL; }
	0x[a-fA-F0-9]+ { yylval = createHexIntegerLiteral(yytext, yylineno); return INTEGER_LITERAL; }


	\"([^\\"]|\\.)*\" { yylval = createStringLiteral(yytext, yylineno); return STRING_LITERAL; }

	[\ \t\r\n]+ { /* ignore */ }

	. { parserErrorUnknownChar(*yytext, yylineno); }
}
