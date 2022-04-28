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

namespace
{
std::string StructDuPrefix = "data/struct_dunion/";

mtest::structures::struct_du GetStructDu_0()
{
    mtest::structures::struct_du msg;
    return msg;
}

mtest::structures::struct_du GetStructDu_1()
{
    mtest::structures::struct_du msg;

    msg.disc_union.initAs(mtest::structures::du_one::Variants::one);
    msg.disc_union.D1() = 1.2;
    msg.disc_union.D2() = 3.4;
    msg.disc_union.D3() = 5.6;

    return msg;
}

mtest::structures::struct_du GetStructDu_2()
{
    mtest::structures::struct_du msg;

    msg.disc_union.initAs(mtest::structures::du_one::Variants::two);

    msg.disc_union.Data().push_back(1.2);
    msg.disc_union.Data().push_back(3.4);
    msg.disc_union.Data().push_back(5.6);

    return msg;
}


template<class T>
void TestComposeParse(const std::string& fileName, std::function<mtest::structures::struct_du()> getState, lest::env& lest_env)
{
    auto msg = getState();

    mtest::Buffer b;
    typename T::ComposerT composer(b);

    mtest::publishable_STRUCT_struct_du::compose(composer, msg);

    auto expected = makeBuffer(fileName);
    T::ExpectAreEqual(expected, b);


    auto iter = b.getReadIter();
    typename T::ParserT parser(iter);

    mtest::structures::struct_du msg2;
    mtest::publishable_STRUCT_struct_du::parseForStateSyncOrMessageInDepth(parser, msg2);

    EXPECT(msg == msg2);
}
}

const lest::test test_struct_du[] =
{
    lest_CASE( "StructDu_0" )
    {
        TestComposeParse<types_json>(StructDuPrefix + "test_struct_du_0.json", GetStructDu_0, lest_env);
        TestComposeParse<types_gmq>(StructDuPrefix + "test_struct_du_0.gmq", GetStructDu_0, lest_env);
    },
    lest_CASE( "StructDu_1" )
    {
        TestComposeParse<types_json>(StructDuPrefix + "test_struct_du_1.json", GetStructDu_1, lest_env);
        TestComposeParse<types_gmq>(StructDuPrefix + "test_struct_du_1.gmq", GetStructDu_1, lest_env);
    },
    lest_CASE( "StructDu_2" )
    {
        TestComposeParse<types_json>(StructDuPrefix + "test_struct_du_2.json", GetStructDu_2, lest_env);
        TestComposeParse<types_gmq>(StructDuPrefix + "test_struct_du_2.gmq", GetStructDu_2, lest_env);
    },
};

lest_MODULE(specification(), test_struct_du);
