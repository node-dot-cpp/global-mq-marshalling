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

#ifndef TEST_IDL_COMMON_H_INCLUDED
#define TEST_IDL_COMMON_H_INCLUDED


#include "../../3rdparty/lest/include/lest/lest.hpp"
#include "generated_interop2.h"

lest::tests& specification();


inline
mtest::Buffer makeBuffer(const std::string& filename, lest::env & lest_env)
{
    FILE* input_file = fopen(filename.c_str(), "rb");
    lest_EXPECT(input_file);

    mtest::Buffer b;
    b.read_file(input_file);
    fclose(input_file);

    return b;
}

template<class T>
bool operator==(const std::vector<T>& l, const std::vector<T>& r)
{
    auto it1 = l.begin();
    auto it2 = r.begin();

    while(it1 != l.end() && it2 != r.end())
    {
        if(!(*it1 == *it2))
            return false;

        ++it1;
        ++it2;
    }

    return it1 == l.end() && it2 == r.end();
}

inline
bool operator==(const mtest::Buffer& l, const mtest::Buffer& r)
{

    auto it1 = const_cast<mtest::Buffer&>(l).getReadIter();
    auto it2 = const_cast<mtest::Buffer&>(r).getReadIter();

    while(it1.isData() && it2.isData())
    {
        if(*it1 != *it2)
            return false;

        ++it1;
        ++it2;
    }

    return !it1.isData() && !it2.isData();
}

/// equality op that ignores eol differences
inline
bool AreEqualIgnoreEol(const mtest::Buffer& l, const mtest::Buffer& r)
{

    auto it1 = const_cast<mtest::Buffer&>(l).getReadIter();
    auto it2 = const_cast<mtest::Buffer&>(r).getReadIter();

    while (it1.isData() && it2.isData())
    {
        // \r\n == \n
        if (*it1 == '\r' && *it2 == '\n')
        {
            ++it1;
            if (!it1.isData() || *it1 != '\n')
                return false;

        }
        // \n == \r\n
        else if (*it1 == '\n' && *it2 == '\r')
        {
            ++it2;
            if (!it2.isData() || *it2 != '\n')
                return false;

        }
        else if (*it1 != *it2)
            return false;

        ++it1;
        ++it2;
    }

    return !it1.isData() && !it2.isData();
}


#endif // TEST_IDL_COMMON_H_INCLUDED
