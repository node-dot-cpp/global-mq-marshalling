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

std::string PathJson = "data/simple/sample_1.json";
std::string PathGmq = "data/simple/sample_1.gmq";


inline
bool operator==(const mtest::structures::SimpleStruct& l, const mtest::structures::SimpleStruct& r)
{
    return l.name == r.name && l.id == r.id;
}

inline
mtest::structures::SimpleStruct GetSample1()
{
    //create some sample data to be written to message

    mtest::structures::SimpleStruct msg;

    msg.name = "Hello world!";
    msg.id = 1;

    return msg;
}


const lest::test test_simple[] =
{
    lest_CASE( "TestJsonCompose" )
    {
        auto msg = GetSample1();

        mtest::Buffer b;
        mtest::JsonComposer<mtest::Buffer> composer(b);

        mtest::json_scope::MESSAGE_SimpleJsonMessage_compose(composer,
            mtest::data =  mtest::MessageWrapperForComposing([&](auto& c) {
                mtest::STRUCT_SimpleStruct_compose(c, mtest::id = msg.id, mtest::name = msg.name);
        }));

        auto b2 = makeBuffer(PathJson, lest_env);
        EXPECT(b2 == b);
    },
    lest_CASE( "TestJsonParse" )
    {
        mtest::Buffer b = makeBuffer(PathJson, lest_env);
        auto iter = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(iter);

        auto msg = mtest::json_scope::MESSAGE_SimpleJsonMessage_parse(parser);

        auto msg2 = GetSample1();
        EXPECT(msg2 == msg.data);
    },

    lest_CASE( "TestGmqCompose" )
    {
        auto msg = GetSample1();

        mtest::Buffer b;
        mtest::GmqComposer<mtest::Buffer> composer(b);

        mtest::gmq_scope::MESSAGE_SimpleGmqMessage_compose(composer,
            mtest::data =  mtest::MessageWrapperForComposing([&](auto& c) {
                mtest::STRUCT_SimpleStruct_compose(c, mtest::id = msg.id, mtest::name = msg.name);
        }));

        auto b2 = makeBuffer(PathGmq, lest_env);
        EXPECT(b2 == b);
    },
    lest_CASE( "TestGmqParse" )
    {
        mtest::Buffer b = makeBuffer(PathGmq, lest_env);
        auto iter = b.getReadIter();
        mtest::GmqParser<mtest::Buffer> parser(iter);

        auto msg = mtest::gmq_scope::MESSAGE_SimpleGmqMessage_parse(parser);

        auto msg2 = GetSample1();
        EXPECT(msg2 == msg.data);
    },
};

lest_MODULE(specification(), test_simple);
