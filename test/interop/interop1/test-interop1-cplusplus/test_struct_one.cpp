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

#include "test_common.h"

std::string PathJson = DataPrefix + "test_struct_one.json";
std::string PathGmq = DataPrefix + "test_struct_one.gmq";

template<class ComposerT>
void ComposeStructOne(ComposerT& composer, mtest::structures::struct_one& msg)
{
	mtest::STRUCT_struct_one_compose(composer,
		mtest::thirdParam = msg.thirdParam,
		mtest::firstParam = msg.firstParam,
        mtest::fifthParam = msg.fifthParam,
        mtest::forthParam = msg.forthParam,
        mtest::seventhParam = msg.seventhParam,
		mtest::eighthParam = msg.eighthParam,
		mtest::ninethParam = msg.ninethParam,
		mtest::secondParam = msg.secondParam,
		mtest::tenthParam = msg.tenthParam,
		mtest::sixthParam = msg.sixthParam
	);
}


const lest::test test_struct_one[] =
{
    lest_CASE( "TestJsonCompose" )
    {
        mtest::structures::struct_one msg = GetSampleStructOne();

        mtest::Buffer b;
        mtest::JsonComposer composer(b);

        ComposeStructOne(composer, msg);

        auto expected = makeBuffer(PathJson, lest_env);
        EXPECT(AreEqualIgnoreWhite(expected, b));
    },
    lest_CASE( "TestJsonParse" )
    {
        mtest::Buffer b = makeBuffer(PathJson, lest_env);
        auto iter = b.getReadIter();
        mtest::JsonParser parser(iter);

        mtest::structures::struct_one msg = mtest::test_gmq::STRUCT_struct_one_parse(parser);

        auto msg2 = GetSampleStructOne();
        EXPECT(msg == msg2);
    },

    lest_CASE( "TestGmqCompose" )
    {
        mtest::structures::struct_one msg = GetSampleStructOne();

        mtest::Buffer b;
        mtest::GmqComposer composer(b);

        ComposeStructOne(composer, msg);

        auto b2 = makeBuffer(PathGmq, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "TestGmqParse" )
    {
        mtest::Buffer b = makeBuffer(PathGmq, lest_env);
        auto iter = b.getReadIter();
        mtest::GmqParser parser(iter);

        mtest::structures::struct_one msg = mtest::test_gmq::STRUCT_struct_one_parse(parser);

        auto msg2 = GetSampleStructOne();
        EXPECT(msg == msg2);
    },
};

lest_MODULE(specification(), test_struct_one);
