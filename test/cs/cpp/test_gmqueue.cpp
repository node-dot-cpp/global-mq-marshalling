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

#include "test_idl_common.h"
#include "platforms/inproc_queue.h"



template<class ComposerT>
class Mock_WrapperForPublisher : public mtest::Mock_WrapperForPublisher<mtest::structures::Mock, ComposerT>
{
	typedef mtest::Mock_WrapperForPublisher<mtest::structures::Mock, ComposerT> base;
public:
    virtual void generateStateSyncMessage(ComposerT& composer) { base::compose(composer); }
};

template<class BufferT>
class Mock_WrapperForSubscriber : public mtest::Mock_WrapperForSubscriber<mtest::structures::Mock, BufferT>
{
	typedef mtest::Mock_WrapperForSubscriber<mtest::structures::Mock, BufferT> base;
public:
	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferT>& parser ) 
	{
		base::parseStateSyncMessage(parser);
	}

	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser )
	{
		base::parseStateSyncMessage(parser);
	}
};


GMQ_COLL string getSubcriptionAddress(const char* name)
{
	globalmq::marshalling::GmqPathHelper::PathComponents pc;
	pc.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
	pc.authority = "";
	pc.nodeName = "test_node";
	pc.statePublisherOrConnectionType = name;

	return globalmq::marshalling::GmqPathHelper::compose(pc);

}

template <class BufferT, class MetaPoolT, class MsgQueue>
void deliverAllMessages(MetaPoolT& mp, MsgQueue& queue, const std::string &filePrefix, int &msgCnt, lest::env & lest_env)
{
    constexpr size_t maxMsg = 16;
    ThreadQueueItem<BufferT> messages[maxMsg];
    while (true)
    {

        size_t popped = queue.pop_front(messages, maxMsg, 0);
        if (popped == 0)
            break;
        for (size_t i = 0; i < popped; ++i)
        {
            //		fmt::print( "msg = \"{}\"\n", messages[i].msg.begin() );
            mp.onMessage(messages[i].msg);
            ++msgCnt;
            if (!filePrefix.empty())
            {
                std::string fileName = filePrefix + std::to_string(msgCnt) + ".json";

                auto b2 = makeBuffer(fileName, lest_env);
                EXPECT(EqualsIgnoreEol(b2, messages[i].msg));
            }
        }
    }
}

const lest::test test_gmqueue[] =
{
	lest_CASE("test_gmqueue.TestGmQueueWithMock1"){
		// gmqueue
		using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
		using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
		using ParserT = GMQueueStatePublisherSubscriberTypeInfo::ParserT;

		GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;
		gmqueue.template initStateConcentratorFactory<mtest::StateConcentratorFactory<BufferT, ComposerT>>();
		gmqueue.setAuthority("");

		using PostmanT = globalmq::marshalling::ThreadQueuePostman<BufferT>;
		typename PostmanT::MsgQueue queue;

		globalmq::marshalling::GMQThreadQueueTransport<GMQueueStatePublisherSubscriberTypeInfo> transport(gmqueue, "test_node", queue, 1);

		using MetaPoolT = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
		MetaPoolT mp;
		mp.setTransport(&transport);

		Mock_WrapperForPublisher<ComposerT> publ;
		// mtest::Mock_WrapperForPublisher<mtest::structures::Mock, ComposerT> publ;
		mp.add(&publ);

		GMQ_COLL string path = getSubcriptionAddress(Mock_WrapperForPublisher<ComposerT>::stringTypeID);

		Mock_WrapperForSubscriber<BufferT> subs;
		// mtest::Mock_WrapperForSubscriber<mtest::structures::Mock, BufferT> sub;
		mp.add(&subs);
		mp.subscribe(&subs, path);

		int msgCnt = 0;

		// mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, "test_gmqueue1_", msgCnt, lest_env);

		mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, "test_gmqueue1_", msgCnt, lest_env);
	},
	lest_CASE("test_gmqueue.TestGmQueueWithMock2"){
		// gmqueue
		using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
		using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
		using ParserT = GMQueueStatePublisherSubscriberTypeInfo::ParserT;

		GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;
		gmqueue.template initStateConcentratorFactory<mtest::StateConcentratorFactory<BufferT, ComposerT>>();
		gmqueue.setAuthority("");

		using PostmanT = globalmq::marshalling::ThreadQueuePostman<BufferT>;
		typename PostmanT::MsgQueue queue;

		globalmq::marshalling::GMQThreadQueueTransport<GMQueueStatePublisherSubscriberTypeInfo> transport(gmqueue, "test_node", queue, 1);

		using MetaPoolT = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
		MetaPoolT mp;
		mp.setTransport(&transport);

		Mock_WrapperForPublisher<ComposerT> publ;
		mp.add(&publ);

		GMQ_COLL string path = getSubcriptionAddress(Mock_WrapperForPublisher<ComposerT>::stringTypeID);

		Mock_WrapperForSubscriber<BufferT> subs;
		mp.add(&subs);
		mp.subscribe(&subs, path);

		Mock_WrapperForSubscriber<BufferT> subs2;
		mp.add(&subs2);
		mp.subscribe(&subs2, path);

		int msgCnt = 0;

		// mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, "test_gmqueue2_", msgCnt, lest_env);

		mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, "test_gmqueue2_", msgCnt, lest_env);
	},
	lest_CASE("test_gmqueue.TestGmQueueWithMock3"){
		// gmqueue
		using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
		using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
		using ParserT = GMQueueStatePublisherSubscriberTypeInfo::ParserT;

		GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;
		gmqueue.template initStateConcentratorFactory<mtest::StateConcentratorFactory<BufferT, ComposerT>>();
		gmqueue.setAuthority("");

		using PostmanT = globalmq::marshalling::ThreadQueuePostman<BufferT>;
		typename PostmanT::MsgQueue queue;

		globalmq::marshalling::GMQThreadQueueTransport<GMQueueStatePublisherSubscriberTypeInfo> transport(gmqueue, "test_node", queue, 1);

		using MetaPoolT = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
		MetaPoolT mp;
		mp.setTransport(&transport);

		Mock_WrapperForPublisher<ComposerT> publ;
		mp.add(&publ);
		mp.remove(&publ);
		mp.add(&publ);

		GMQ_COLL string path = getSubcriptionAddress(Mock_WrapperForPublisher<ComposerT>::stringTypeID);

		Mock_WrapperForSubscriber<BufferT> subs;
		mp.add(&subs);
		mp.remove(&subs);
		mp.add(&subs);
		mp.subscribe(&subs, path);

		int msgCnt = 0;

		// mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, "test_gmqueue3_", msgCnt, lest_env);

		mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, "test_gmqueue3_", msgCnt, lest_env);
	}


};

lest_MODULE(specification(), test_gmqueue);
