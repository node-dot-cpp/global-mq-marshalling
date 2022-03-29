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
#include "struct_dictionary.h"

std::string StructDictionaryPrefix = "data/struct_dictionary/";


template<class T>
void TestComposeParse(const std::string& fileName, std::function<mtest::structures::struct_dictionary()> getState, lest::env& lest_env)
{
    auto msg = getState();

    mtest::Buffer b;
    typename T::ComposerT composer(b);

    mtest::publishable_STRUCT_struct_dictionary::compose(composer, msg);

    auto expected = makeBuffer(fileName);
    T::ExpectAreEqual(expected, b);


    auto iter = b.getReadIter();
    typename T::ParserT parser(iter);

    mtest::structures::struct_dictionary msg2;
    mtest::publishable_STRUCT_struct_dictionary::parseForStateSyncOrMessageInDepth(parser, msg2);

    EXPECT(msg == msg2);
}


const lest::test test_struct_dictionary[] =
{
    lest_CASE( "StructDictionary_2" )
    {
        TestComposeParse<types_json>(StructDictionaryPrefix + "struct_2.json", GetDictionary_2, lest_env);
        TestComposeParse<types_gmq>(StructDictionaryPrefix + "struct_2.gmq", GetDictionary_2, lest_env);
    },
};

lest_MODULE(specification(), test_struct_dictionary);
