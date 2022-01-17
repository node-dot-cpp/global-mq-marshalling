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

#ifndef COMMON_H
#define COMMON_H

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h> // for assert(); TODO: replace by our own HAREASSERT
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>


#include <exception>
#include "../../3rdparty/fmt/include/fmt/format.h"

namespace global_mq {
class AssertException : public std::exception {
    const char* cond;
    const char* msg;
    const char* file;
    int line;
    std::string longMessage;

public:
    AssertException(const char* cond_, const char* file_, int line_)
        : cond(cond_), msg(nullptr), file(file_), line(line_), longMessage(fmt::format( "Assertion \"{}\" failed. File: \"{}\", line: {}\n", cond, file, line )) {
    }
    AssertException(const char* cond_, const char* msg_, const char* file_, int line_)
        : cond(cond_), msg(msg_), file(file_), line(line_), longMessage(fmt::format( "Assertion \"{}\" failed. Reason: {} File: \"{}\", line: {}\n", cond, msg, file, line )) {
    }

    virtual const char* what() const noexcept override
    {
        return longMessage.c_str();
    }
};
};

//#defining things within namespace is misleading,
//  so they go outside

//Unlike usual assert(), GLOBALMQASSERT():
//  * is ALWAYS on
//  * instead of calling abort(), throws global_mq::AssertException()
//
//NB: if changing GLOBALMQASSERT()/GLOBALMQASSERTM(),
//  make sure to consult with http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
#define GLOBALMQASSERT(cond) \
    do { \
        if(!(cond)) { \
            throw global_mq::AssertException(#cond,__FILE__,__LINE__); \
        } \
    } while(0)

//GLOBALMQASSERTM() stands for "GLOBALMQASSERT with Message"
#define GLOBALMQASSERTM(cond,msg) \
    do { \
        if(!(cond)) { \
            throw global_mq::AssertException(#cond,(msg),__FILE__,__LINE__); \
        } \
    } while(0)

#endif // COMMON_H
