/* -------------------------------------------------------------------------------
* Copyright (c) 2022, OLogN Technologies AG
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
#include <fmt/printf.h>
//#include "idl_tree_serializer.h"

using namespace std;

void CsharpFileWritter::increment()
{
	currentIndent.push_back('\t');
}
void CsharpFileWritter::decrement()
{
	if (!currentIndent.empty())
		currentIndent.pop_back();
}

void  CsharpFileWritter::writeBr(char brace)
{
	if (brace == '{')
	{
		// when opening brace, first write the brace then increase indent
		if (!currentIndent.empty())
			fmt::fprintf(file, currentIndent);
		fmt::fprintf(file, "{\n");

		currentIndent.push_back('\t');
	}
	else if (brace == '}')
	{
		// when closing brace, first decrease indent then write the brace
		if (!currentIndent.empty())
			currentIndent.pop_back();

		if (!currentIndent.empty())
			fmt::fprintf(file, currentIndent);
		fmt::fprintf(file, "}\n");
	}
	else
		assert(false);
}


void CsharpFileWritter::write(const char* text)
{
	if(!currentIndent.empty())
		fmt::fprintf(file, currentIndent);
	fmt::fprintf(file, text);
}

void CsharpFileWritter::write(const char* format, const char* arg0)
{
	if (!currentIndent.empty())
		fmt::fprintf(file, currentIndent);
	fmt::fprintf(file, format, arg0);
}

void CsharpFileWritter::write(const char* format, const char* arg0, const char* arg1)
{
	if (!currentIndent.empty())
		fmt::fprintf(file, currentIndent);
	fmt::fprintf(file, format, arg0, arg1);
}
void CsharpFileWritter::write(const char* format, const char* arg0, const char* arg1, const char* arg2)
{
	if (!currentIndent.empty())
		fmt::fprintf(file, currentIndent);
	fmt::fprintf(file, format, arg0, arg1, arg2);
}
void CsharpFileWritter::write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3)
{
	if (!currentIndent.empty())
		fmt::fprintf(file, currentIndent);
	fmt::fprintf(file, format, arg0, arg1, arg2, arg3);
}
void CsharpFileWritter::write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3, const char* arg4)
{
	if (!currentIndent.empty())
		fmt::fprintf(file, currentIndent);
	fmt::fprintf(file, format, arg0, arg1, arg2, arg3, arg4);
}


