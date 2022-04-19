/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
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

#include "file_writter.h"
#include <fmt/printf.h>

using namespace std;


FileWritter FileWritter::indent(size_t i)
{
	return {file, currentIndent + i};
}

void FileWritter::write(std::string text)
{
	if(text.empty())
		return;

	if(currentIndent != 0)
	{

		size_t ix = text.find_first_not_of('\n', 0);
		if(ix == 0 && lastWasEol)
			text.insert(0, currentIndent, '\t');

		while (true)
		{
			ix = text.find('\n', ix);
			if(ix != std::string::npos && ix != text.size() - 1)
			{
				if(text.at(ix + 1) != '\n')
				{
					text.insert(ix + 1, currentIndent, '\t');
					ix += currentIndent;
				}
				ix += 1;
			}
			else
				break;
		}
	}

	if(text.back() == '\n')
		lastWasEol = true;

	fmt::fprintf(file, text);
}

void FileWritter::write(const char* text)
{
	write(std::string(text));
}

void FileWritter::write(const char* format, const char* arg0)
{
	write(fmt::sprintf(format, arg0));
}

void FileWritter::write(const char* format, const char* arg0, const char* arg1)
{
	write(fmt::sprintf(format, arg0, arg1));
}
void FileWritter::write(const char* format, const char* arg0, const char* arg1, const char* arg2)
{
	write(fmt::sprintf(format, arg0, arg1, arg2));
}
void FileWritter::write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3)
{
	write(fmt::sprintf(format, arg0, arg1, arg2, arg3));
}
void FileWritter::write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3, const char* arg4)
{
	write(fmt::sprintf(format, arg0, arg1, arg2, arg3, arg4));
}

