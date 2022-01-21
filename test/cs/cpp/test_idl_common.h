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

#include "../../3rdparty/lest/include/lest/lest_basic.hpp"
#include "idl_generated.h"

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

template<class ComposerT>
void ComposeStructOne(ComposerT& composer, mtest::structures::struct_one& msg)
{
	mtest::STRUCT_struct_one_compose(composer,
		mtest::thirdParam = mtest::CollectionWrapperForComposing([&]() { return msg.thirdParam.size(); }, [&](auto& c, size_t ordinal) { mtest::STRUCT_point3D_compose(c, mtest::x = msg.thirdParam[ordinal].x, mtest::y = msg.thirdParam[ordinal].y, mtest::z = msg.thirdParam[ordinal].z); }),
		mtest::firstParam = msg.firstParam, mtest::fifthParam = msg.fifthParam, mtest::forthParam = msg.forthParam, mtest::seventhParam = msg.seventhParam,
		mtest::eighthParam = mtest::MessageWrapperForComposing([&](auto& c) { mtest::STRUCT_point_compose(c, mtest::x = msg.eighthParam.x, mtest::y = msg.eighthParam.y); }),
		mtest::ninethParam = mtest::MessageWrapperForComposing([&](auto& c) { mtest::STRUCT_point3D_compose(c, mtest::x = msg.ninethParam.x, mtest::y = msg.ninethParam.y, mtest::z = msg.ninethParam.z); }),
		mtest::secondParam = mtest::SimpleTypeCollectionWrapper(msg.secondParam),
		mtest::tenthParam = mtest::SimpleTypeCollectionWrapper(msg.tenthParam),
		mtest::sixthParam = mtest::CollectionWrapperForComposing([&]() { return msg.sixthParam.size(); }, [&](auto& c, size_t ordinal) { mtest::STRUCT_point_compose(c, mtest::x = msg.sixthParam[ordinal].x, mtest::y = msg.sixthParam[ordinal].y); })
	);
}

mtest::Buffer makeBuffer(const std::string& filename)
{
    FILE* input_file = fopen(filename.c_str(), "rb");
    EXPECT(input_file);

    mtest::Buffer b;
    b.read_file(input_file);
    fclose(input_file);

    return b;
}

bool operator==(const mtest::structures::point3D& l, const mtest::structures::point3D& r)
{
    return l.x == r.x && l.y == r.y && l.z == r.z;
}

bool operator==(const mtest::structures::point& l, const mtest::structures::point& r)
{
    return l.x == r.x && l.y == r.y;
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

bool operator==(const mtest::structures::struct_one& l, const mtest::structures::struct_one& r)
{
    return (l.firstParam == r.firstParam) &&
            (l.secondParam == r.secondParam) &&
            (l.thirdParam == r.thirdParam) &&
            (l.forthParam == r.forthParam) &&
            (l.fifthParam == r.fifthParam) &&
            (l.sixthParam == r.sixthParam) &&
            (l.seventhParam == r.seventhParam) &&
            (l.eighthParam == r.eighthParam) &&
            (l.ninethParam == r.ninethParam) &&
            (l.tenthParam == r.tenthParam);
}

bool operator==(mtest::Buffer& l, mtest::Buffer& r)
{
    auto it1 = l.getReadIter();
    auto it2 = r.getReadIter();

    while(it1.isData() && it2.isData()) {
        if(*it1 != *it2)
            return false;
    
        ++it1;
        ++it2;
    }

    return it1.isData() == it2.isData();
}

#endif // TEST_IDL_COMMON_H_INCLUDED
