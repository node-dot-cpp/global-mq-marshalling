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

#ifndef INTEROP3_STRUCT_DICTIONARY_H_INCLUDED
#define INTEROP3_STRUCT_DICTIONARY_H_INCLUDED

#include "test_common.h"

inline
mtest::structures::struct_dictionary GetDictionary_2()
{
    mtest::structures::struct_dictionary data;

// on linux std::unordered_map reverses the order
// this is a quick hack for testing only
#ifdef _MSC_VER
    data.dictionary_one["hello"] = "world";
    data.dictionary_one["red"] = "blue";
    data.dictionary_one["dog"] = "cat";
#else
    data.dictionary_one["dog"] = "cat";
    data.dictionary_one["red"] = "blue";
    data.dictionary_one["hello"] = "world";
#endif

    mtest::structures::AggregateType aggr1;
    aggr1.name = "aggr1";
    aggr1.theAggregate.anInt = -5;
    aggr1.theAggregate.anUInt = 5;
    aggr1.theAggregate.aReal = 3.14;
    aggr1.theAggregate.aString = "hello!";
    aggr1.lastValue = 99;

    data.dictionary_two[2] = aggr1;
    data.dictionary_two[3] = aggr1;
    data.dictionary_two[4] = aggr1;

    mtest::structures::du_one du;
    du.initAs(mtest::structures::du_one::Variants::two);
    du.Data().push_back(-100.001);
    du.Data().push_back(-200.002);
    du.Data().push_back(-300.003);

    data.dictionary_three[-5] = du;
    data.dictionary_three[-6] = du;
    data.dictionary_three[-7] = du;

    mtest::structures::BasicTypes bt;
    bt.anInt = -99;
    bt.aString = "someName";

    data.dictionary_four["someName"] = bt;

    mtest::structures::BasicTypes bt2;
    bt2.anInt = -98;
    bt2.aString = "otherName";

    data.dictionary_four["otherName"] = bt2;

    return data;
}


#endif // INTEROP3_STRUCT_DICTIONARY_H_INCLUDED
