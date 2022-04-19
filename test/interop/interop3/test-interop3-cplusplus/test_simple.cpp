/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
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

#include "test_common.h"

std::string SimplePrefix = "data/simple/";

namespace
{
std::string PathGmq = "data/simple/sample_1.gmq";

mtest::structures::SimpleStruct GetSample_1()
{
    //create some sample data to be written to message

    mtest::structures::SimpleStruct msg;

    msg.name = "Hello world!";
    msg.id = 1;

    return msg;
}
}

template<class T>
void TestComposeParse(const std::string& fileName, std::function<mtest::structures::SimpleStruct()> getState, lest::env& lest_env)
{
    auto msg = getState();

    mtest::Buffer b;
    typename T::ComposerT composer(b);

    mtest::publishable_STRUCT_SimpleStruct::compose(composer, msg);

    auto expected = makeBuffer(fileName);
    T::ExpectAreEqual(expected, b);


    auto iter = b.getReadIter();
    typename T::ParserT parser(iter);

    mtest::structures::SimpleStruct msg2;
    mtest::publishable_STRUCT_SimpleStruct::parseForStateSyncOrMessageInDepth(parser, msg2);

    EXPECT(msg == msg2);
}


const lest::test test_simple[] =
{
    lest_CASE( "Simple" )
    {
        TestComposeParse<types_json>(SimplePrefix + "sample_1.json", GetSample_1, lest_env);
        TestComposeParse<types_gmq>(SimplePrefix + "sample_1.gmq", GetSample_1, lest_env);
    }
};

lest_MODULE(specification(), test_simple);
