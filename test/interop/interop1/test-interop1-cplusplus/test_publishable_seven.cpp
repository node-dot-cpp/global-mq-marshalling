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


#include "test_common2.h"

namespace
{
std::string PathPubSevenJson = DataPrefix + "test_publishable_seven.json";
std::string PathPubSevenJson1 = DataPrefix + "test_publishable_seven_u1.json";
std::string PathPubSevenJson2 = DataPrefix + "test_publishable_seven_u2.json";
std::string PathPubSevenJson3 = DataPrefix + "test_publishable_seven_u3.json";
std::string PathPubSevenJson4 = DataPrefix + "test_publishable_seven_u4.json";
std::string PathPubSevenJson5 = DataPrefix + "test_publishable_seven_u5.json";

std::string PathPubSevenGmq = DataPrefix + "test_publishable_seven.gmq";
std::string PathPubSevenGmq1 = DataPrefix + "test_publishable_seven_u1.gmq";
std::string PathPubSevenGmq2 = DataPrefix + "test_publishable_seven_u2.gmq";
std::string PathPubSevenGmq3 = DataPrefix + "test_publishable_seven_u3.gmq";
std::string PathPubSevenGmq4 = DataPrefix + "test_publishable_seven_u4.gmq";
std::string PathPubSevenGmq5 = DataPrefix + "test_publishable_seven_u5.gmq";


inline
mtest::structures::publishable_seven GetPublishableSeven()
{
    mtest::structures::publishable_seven data;


    data.intVec.push_back(1);
    data.intVec.push_back(2);
    data.intVec.push_back(3);

    data.uintVec.push_back(4);
    data.uintVec.push_back(5);
    data.uintVec.push_back(6);

    data.realVec.push_back(7);
    data.realVec.push_back(8);
    data.realVec.push_back(9);

    data.strVec.push_back("one");
    data.strVec.push_back("two");
    data.strVec.push_back("three");

    data.structVec.push_back(mtest::structures::point3D{101, 102, 103});
    data.structVec.push_back(mtest::structures::point3D{201, 202, 203});

    return data;
}

enum Events7
{
    notifyUpdated_x, notifyUpdated_y, notifyUpdated_z,
    notifyUpdated_intVec, notifyElementUpdated_intVec, notifyInserted_intVec, notifyErased_intVec,
    notifyUpdated_uintVec, notifyElementUpdated_uintVec, notifyInserted_uintVec, notifyErased_uintVec,
    notifyUpdated_realVec, notifyElementUpdated_realVec, notifyInserted_realVec, notifyErased_realVec,
    notifyUpdated_strVec, notifyElementUpdated_strVec, notifyInserted_strVec, notifyErased_strVec,
    notifyUpdated_structVec, notifyElementUpdated_structVec, notifyInserted_structVec, notifyErased_structVec,
    notifyUpdated, notifyFullyUpdated
};

class user_point3D_handler : public mtest::point3D_subscriber
{
    public:
    std::vector<Events7>& handled7;

    user_point3D_handler(std::vector<Events7>& handled) : handled7(handled) {}


    void notifyUpdated_x(int64_t old) override { handled7.push_back(Events7::notifyUpdated_x); }
    void notifyUpdated_y(int64_t old) override { handled7.push_back(Events7::notifyUpdated_y); }
    void notifyUpdated_z(int64_t old) override { handled7.push_back(Events7::notifyUpdated_z); }
};

class user_publishable_seven_handler :  public mtest::publishable_seven_subscriber
{
    public:

    std::vector<Events7> handled7;

    user_publishable_seven_handler() {}

    const std::vector<Events7>& getEvents() const { return handled7; }

    std::unique_ptr<mtest::point3D_subscriber> make_point3D() override { return std::unique_ptr<mtest::point3D_subscriber>{new user_point3D_handler(handled7)}; }


    void notifyFullyUpdated() override { handled7.push_back(Events7::notifyFullyUpdated); }
    void notifyUpdated() override { handled7.push_back(Events7::notifyUpdated); }
    void notifyUpdated_intVec() override { handled7.push_back(Events7::notifyUpdated_intVec); }
    void notifyElementUpdated_intVec(size_t index, int64_t oldVal) override { handled7.push_back(Events7::notifyElementUpdated_intVec); }
    void notifyInserted_intVec(size_t index) override { handled7.push_back(Events7::notifyInserted_intVec); }
    void notifyErased_intVec(size_t index, int64_t oldVal) override { handled7.push_back(Events7::notifyErased_intVec); }
    void notifyUpdated_uintVec() override { handled7.push_back(Events7::notifyUpdated_uintVec); }
    void notifyElementUpdated_uintVec(size_t index, uint64_t oldVal) override { handled7.push_back(Events7::notifyElementUpdated_uintVec); }
    void notifyInserted_uintVec(size_t index) override { handled7.push_back(Events7::notifyInserted_uintVec); }
    void notifyErased_uintVec(size_t index, uint64_t oldVal) override { handled7.push_back(Events7::notifyErased_uintVec); }
    void notifyUpdated_realVec() override { handled7.push_back(Events7::notifyUpdated_realVec); }
    void notifyElementUpdated_realVec(size_t index, double oldVal) override { handled7.push_back(Events7::notifyElementUpdated_realVec); }
    void notifyInserted_realVec(size_t index) override { handled7.push_back(Events7::notifyInserted_realVec); }
    void notifyErased_realVec(size_t index, double oldVal) override { handled7.push_back(Events7::notifyErased_realVec); }
    void notifyUpdated_strVec() override { handled7.push_back(Events7::notifyUpdated_strVec); }
    void notifyElementUpdated_strVec(size_t index, GMQ_COLL string oldVal) override { handled7.push_back(Events7::notifyElementUpdated_strVec); }
    void notifyInserted_strVec(size_t index) override { handled7.push_back(Events7::notifyInserted_strVec); }
    void notifyErased_strVec(size_t index, GMQ_COLL string oldVal) override { handled7.push_back(Events7::notifyErased_strVec); }
    void notifyUpdated_structVec() override { handled7.push_back(Events7::notifyUpdated_structVec); }
    void notifyElementUpdated_structVec(size_t index) override { handled7.push_back(Events7::notifyElementUpdated_structVec); }
    void notifyInserted_structVec(size_t index) override { handled7.push_back(Events7::notifyInserted_structVec); }
    void notifyErased_structVec(size_t index, GMQ_COLL unique_ptr<mtest::point3D_subscriber> oldVal) override { handled7.push_back(Events7::notifyErased_structVec); }

};

using SubscriberT = user_publishable_seven_handler;
class publishable_seven_json : public types_json
{
    public:
    using DataT = mtest::structures::publishable_seven;
    using PublishableT =  mtest::publishable_seven_WrapperForPublisher<ComposerT>;
    using SubscriberT = user_publishable_seven_handler;
};

class publishable_seven_gmq : public types_gmq
{
    public:
    using DataT = mtest::structures::publishable_seven;
    using PublishableT = mtest::publishable_seven_WrapperForPublisher<ComposerT>;
    using SubscriberT = user_publishable_seven_handler;
};

std::vector<Events7> events0 = {
                    Events7::notifyFullyUpdated
                };

void doUpdate1(mtest::structures::publishable_seven& data)
{
    data.structVec[0].y = 505;
}

template<class T>
void doUpdatePublisher1(T& publ)
{
    //modify substructure inside vector
    publ.get4set_structVec().get4set_at(0).set_y(505);
}

std::vector<Events7> events1 = {
                    Events7::notifyFullyUpdated,
                    Events7::notifyUpdated_y,
                    Events7::notifyElementUpdated_structVec,
                    Events7::notifyUpdated_structVec,
                    Events7::notifyUpdated
                };

void doUpdate2(mtest::structures::publishable_seven& data)
{
    data.intVec[0] = 303;
    data.uintVec[1] = 404;
    data.realVec[2] = 505;
    data.strVec[1] = "four";
    data.structVec[0] = { 901, 902, 903 };
}

template<class T>
void doUpdatePublisher2(T& publ)
{
    //modify existing elements
    publ.get4set_intVec().set_at(303, 0);
    publ.get4set_uintVec().set_at(404, 1);
    publ.get4set_realVec().set_at(505, 2);
    publ.get4set_strVec().set_at("four", 1);

    publ.get4set_structVec().set_at({ 901, 902, 903 }, 0);
}

std::vector<Events7> events2 = {
                    Events7::notifyFullyUpdated,
                    Events7::notifyElementUpdated_intVec,
                    Events7::notifyUpdated_intVec,
                    Events7::notifyUpdated,
                    Events7::notifyElementUpdated_uintVec,
                    Events7::notifyUpdated_uintVec,
                    Events7::notifyUpdated,
                    Events7::notifyElementUpdated_realVec,
                    Events7::notifyUpdated_realVec,
                    Events7::notifyUpdated,
                    Events7::notifyElementUpdated_strVec,
                    Events7::notifyUpdated_strVec,
                    Events7::notifyUpdated,
                    Events7::notifyUpdated_x,
                    Events7::notifyUpdated_y,
                    Events7::notifyUpdated_z,
                    Events7::notifyElementUpdated_structVec,
                    Events7::notifyUpdated_structVec,
                    Events7::notifyUpdated
                };


void doUpdate3(mtest::structures::publishable_seven& data)
{
    decltype(mtest::structures::publishable_seven::strVec) strVec;
    strVec.push_back("four");
    strVec.push_back("five");
    data.strVec = strVec;

    decltype(mtest::structures::publishable_seven::realVec) realVec;
    data.realVec = realVec;

    decltype(mtest::structures::publishable_seven::structVec) structVec;
    structVec.push_back({ 301, 302, 303 });
    data.structVec = structVec;
}

template<class T>
void doUpdatePublisher3(T& publ)
{
    //replace complete vector
    decltype(mtest::structures::publishable_seven::strVec) strVec;
    strVec.push_back("four");
    strVec.push_back("five");
    publ.set_strVec(strVec);

    decltype(mtest::structures::publishable_seven::realVec) realVec;
    publ.set_realVec(realVec);

    decltype(mtest::structures::publishable_seven::structVec) structVec;
    structVec.push_back({ 301, 302, 303 });
    publ.set_structVec(structVec);
}

std::vector<Events7> events3 = {
                    Events7::notifyFullyUpdated,
                    Events7::notifyUpdated_strVec,
                    Events7::notifyUpdated,
                    Events7::notifyUpdated_realVec,
                    Events7::notifyUpdated,
                    Events7::notifyUpdated_x,
                    Events7::notifyUpdated_y,
                    Events7::notifyUpdated_z,
                    Events7::notifyUpdated_structVec,
                    Events7::notifyUpdated
                };


void doUpdate4(mtest::structures::publishable_seven& data)
{
        data.intVec.erase(data.intVec.begin() + 0);
        data.realVec.erase(data.realVec.begin() + 1);
        data.structVec.erase(data.structVec.begin() + 1);
}

template<class T>
void doUpdatePublisher4(T& publ)
{
        //erase elements in vector
        publ.get4set_intVec().remove(0);
        publ.get4set_realVec().remove(1);
        publ.get4set_structVec().remove(1);
}

std::vector<Events7> events4 = {
                    Events7::notifyFullyUpdated,
                    Events7::notifyErased_intVec,
                    Events7::notifyUpdated_intVec,
                    Events7::notifyUpdated,
                    Events7::notifyErased_realVec,
                    Events7::notifyUpdated_realVec,
                    Events7::notifyUpdated,
                    Events7::notifyErased_structVec,
                    Events7::notifyUpdated_structVec,
                    Events7::notifyUpdated
                };


void doUpdate5(mtest::structures::publishable_seven& data)
{
        int64_t v1 = 77;
        uint64_t v2 = 88;
        double v3 = 99;
        GMQ_COLL string v4 = "last";

        data.intVec.insert(data.intVec.begin() + 0, v1);
        data.uintVec.insert(data.uintVec.begin() + 1, v2);
        data.realVec.insert(data.realVec.begin() + 2, v3);
        data.strVec.insert(data.strVec.begin() + 3, v4);

        mtest::structures::point3D v5{301, 302, 303};
        data.structVec.insert(data.structVec.begin() + 1, v5);
}

template<class T>
void doUpdatePublisher5(T& publ)
{
    //insert elements in vector

    int64_t v1 = 77;
    uint64_t v2 = 88;
    double v3 = 99;
    GMQ_COLL string v4 = "last";

    publ.get4set_intVec().insert_before(0, v1);
    publ.get4set_uintVec().insert_before(1, v2);
    publ.get4set_realVec().insert_before(2, v3);
    publ.get4set_strVec().insert_before(3, v4);

    mtest::structures::point3D v5{301, 302, 303};
    publ.get4set_structVec().insert_before(1, v5);
}

std::vector<Events7> events5 = {
                    Events7::notifyFullyUpdated,
                    Events7::notifyInserted_intVec,
                    Events7::notifyUpdated_intVec,
                    Events7::notifyUpdated,
                    Events7::notifyInserted_uintVec,
                    Events7::notifyUpdated_uintVec,
                    Events7::notifyUpdated,
                    Events7::notifyInserted_realVec,
                    Events7::notifyUpdated_realVec,
                    Events7::notifyUpdated,
                    Events7::notifyInserted_strVec,
                    Events7::notifyUpdated_strVec,
                    Events7::notifyUpdated,
                    Events7::notifyUpdated_x,
                    Events7::notifyUpdated_y,
                    Events7::notifyUpdated_z,
                    Events7::notifyInserted_structVec,
                    Events7::notifyUpdated_structVec,
                    Events7::notifyUpdated
                };
}

const lest::test test_publishable_seven[] =
{    
    lest_CASE( "test_publishable_seven.TestStateSync" )
    {
        testStateSync2<publishable_seven_json>(PathPubSevenJson, GetPublishableSeven, lest_env);
        testStateSync2<publishable_seven_gmq>(PathPubSevenGmq, GetPublishableSeven, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestUpdate1" )
    {
        testUpdate2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson1, GetPublishableSeven, doUpdatePublisher1<typename publishable_seven_json::PublishableT>, doUpdate1, lest_env);
        testUpdate2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq1, GetPublishableSeven, doUpdatePublisher1<typename publishable_seven_gmq::PublishableT>, doUpdate1, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestUpdate2" )
    {
        testUpdate2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson2, GetPublishableSeven, doUpdatePublisher2<typename publishable_seven_json::PublishableT>, doUpdate2, lest_env);
        testUpdate2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq2, GetPublishableSeven, doUpdatePublisher2<typename publishable_seven_gmq::PublishableT>, doUpdate2, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestUpdate3" )
    {
        testUpdate2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson3, GetPublishableSeven, doUpdatePublisher3<typename publishable_seven_json::PublishableT>, doUpdate3, lest_env);
        testUpdate2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq3, GetPublishableSeven, doUpdatePublisher3<typename publishable_seven_gmq::PublishableT>, doUpdate3, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestUpdate4" )
    {
        testUpdate2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson4, GetPublishableSeven, doUpdatePublisher4<typename publishable_seven_json::PublishableT>, doUpdate4, lest_env);
        testUpdate2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq4, GetPublishableSeven, doUpdatePublisher4<typename publishable_seven_gmq::PublishableT>, doUpdate4, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestUpdate5" )
    {
        testUpdate2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson5, GetPublishableSeven, doUpdatePublisher5<typename publishable_seven_json::PublishableT>, doUpdate5, lest_env);
        testUpdate2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq5, GetPublishableSeven, doUpdatePublisher5<typename publishable_seven_gmq::PublishableT>, doUpdate5, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestNotifyStateSync" )
    {
        testNotify2<publishable_seven_json>(PathPubSevenJson, "", events0, lest_env);
        testNotify2<publishable_seven_gmq>(PathPubSevenGmq, "", events0, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestNotifyUpdate1" )
    {
        testNotify2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson1, events1, lest_env);
        testNotify2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq1, events1, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestNotifyUpdate2" )
    {
        testNotify2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson2, events2, lest_env);
        testNotify2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq2, events2, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestNotifyUpdate3" )
    {
        testNotify2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson3, events3, lest_env);
        testNotify2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq3, events3, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestNotifyUpdate4" )
    {
        testNotify2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson4, events4, lest_env);
        testNotify2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq4, events4, lest_env);
    },
    lest_CASE( "test_publishable_seven.TestNotifyUpdate5" )
    {
        testNotify2<publishable_seven_json>(PathPubSevenJson, PathPubSevenJson5, events5, lest_env);
        testNotify2<publishable_seven_gmq>(PathPubSevenGmq, PathPubSevenGmq5, events5, lest_env);
    },


}


;

lest_MODULE(specification(), test_publishable_seven);
