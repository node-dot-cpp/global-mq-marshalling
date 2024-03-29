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

#ifndef INTEROP1_TEST_COMMON_H_INCLUDED
#define INTEROP1_TEST_COMMON_H_INCLUDED


#include "../../3rdparty/lest/include/lest/lest.hpp"
#include "generated_interop1.h"

lest::tests& specification();

const std::string DataPrefix = "data/";

inline
mtest::structures::struct_one GetSampleStructOne()
{
    //create some sample data to be written to message

    mtest::structures::struct_one msg;

    msg.firstParam = 1;

    msg.secondParam.push_back(0);
    msg.secondParam.push_back(1);
    msg.secondParam.push_back(2);
    msg.secondParam.push_back(3);
    msg.secondParam.push_back(4);
    msg.secondParam.push_back(5);

    msg.thirdParam.push_back( mtest::structures::point3D { 0, 1, 2 });
    msg.thirdParam.push_back( mtest::structures::point3D { 3, 4, 5 });

    msg.forthParam = 3;

    msg.fifthParam = "def";

    msg.sixthParam.push_back( mtest::structures::point { 0, 1 });
    msg.sixthParam.push_back( mtest::structures::point { 2, 3 });
    msg.sixthParam.push_back( mtest::structures::point { 4, 5 });

    msg.seventhParam = 3.1416;

    msg.eighthParam.x = 175;
    msg.eighthParam.y = 186;

    msg.ninethParam.x = 123;
    msg.ninethParam.y = 456;
    msg.ninethParam.z = 789;

    msg.tenthParam.push_back(0.1);
    msg.tenthParam.push_back(1.2);
    msg.tenthParam.push_back(2.3);

    return msg;
}

inline
mtest::structures::StructSix GetPublishableSix()
{
    mtest::structures::StructSix data;

    data.name = "TheName";
            
    // data.basic = data.make_basic();
    data.basic.anInt = -100000;
    data.basic.anUInt = 100000;
    data.basic.aReal = 3.14;
    data.basic.aString = "basic string";

    // data.aggregate = data.make_aggregate();
    data.aggregate.name = "aggregate name";

    // data.aggregate.theAggregate = data.aggregate.make_theAggregate();
    data.aggregate.theAggregate.anInt = -100;
    data.aggregate.theAggregate.anUInt = 100;
    data.aggregate.theAggregate.aReal = 100;
    data.aggregate.theAggregate.aString = "basic string inside aggregate";
    data.aggregate.lastValue = 0;

    return data;
}


inline
mtest::Buffer makeBuffer(const std::string& filename, lest::env & lest_env)
{
    FILE* input_file = fopen(filename.c_str(), "rb");
    if(!input_file)
        EXPECT(false);

    mtest::Buffer b;
    b.read_file(input_file);
    fclose(input_file);

    return b;
}


inline
bool AreEqual(const mtest::Buffer& l, const mtest::Buffer& r)
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

inline
bool AreEqualIgnoreWhite(const mtest::Buffer& l, const mtest::Buffer& r)
{

    auto it1 = const_cast<mtest::Buffer&>(l).getReadIter();
    auto it2 = const_cast<mtest::Buffer&>(r).getReadIter();

    while (it1.isData() && it2.isData())
    {
        if(*it1 != *it2)
        {
            while(it1.isData() && (*it1 == '\r' || *it1 == '\n'|| *it1 == '\t'|| *it1 == ' '))
                ++it1;

            while(it2.isData() && (*it2 == '\r' || *it2 == '\n'|| *it2 == '\t'|| *it2 == ' '))
                ++it2;

            if(!it1.isData() || !it2.isData())
                return !it1.isData() && !it2.isData();

            if(*it1 != *it2)
                return false;
        }

        ++it1;
        ++it2;
    }

    return !it1.isData() && !it2.isData();
}


#endif // INTEROP1_TEST_COMMON_H_INCLUDED
