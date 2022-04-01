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

#ifndef FILE_WRITTER_H
#define FILE_WRITTER_H

#include <string>
#include <cstdio>

/// Small wrapper to migrate from old fprintf to fmt::fprintf, and add basic auto-indent.
/// Intent is to be relaxed and help the user.

class FileWritter
{
	FILE* file = nullptr;
	size_t currentIndent = 0;
	bool lastWasEol = true;

public:
	FileWritter(FILE* file, size_t initialIndent) : file(file), currentIndent(initialIndent) {}

	FILE* getFile() { return file; }

	FileWritter indent(size_t i = 1);
	// mb: make variadic only if real necesity
	void write(std::string text);
	void write(const char* text);
	void write(const char* format, const char* arg0);
	void write(const char* format, const char* arg0, const char* arg1);
	void write(const char* format, const char* arg0, const char* arg1, const char* arg2);
	void write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3);
	void write(const char* format, const char* arg0, const char* arg1, const char* arg2, const char* arg3, const char* arg4);
};


#endif // FILE_WRITTER_H