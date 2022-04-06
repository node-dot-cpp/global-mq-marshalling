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
#include "platforms/inproc_queue.h"


class TestPlatformSupport
{
public:
	using BufferT = globalmq::marshalling::Buffer;
	using ParserT = globalmq::marshalling2::JsonParser2<BufferT>;
	using ComposerT = globalmq::marshalling2::JsonComposer2<BufferT>;
	template<class T>
	using OwningPtrT = std::unique_ptr<T>;
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

				std::string fileName = DataPrefix + filePrefix + std::to_string(msgCnt) + ".json";

				auto expected = makeBuffer(fileName, lest_env);
				EXPECT(AreEqualIgnoreWhite(expected, messages[i].msg));
            }
        }
    }
}

const lest::test test_gmqueue[] =
{
	lest_CASE("test_gmqueue.TestGmQueueWithMock1"){
		// gmqueue
		using BufferT = TestPlatformSupport::BufferT;
		using ComposerT = TestPlatformSupport::ComposerT;
		using ParserT = TestPlatformSupport::ParserT;

		GMQueue<TestPlatformSupport> gmqueue;
		gmqueue.template initStateConcentratorFactory<mtest::StateConcentratorFactory<BufferT, ComposerT>>();
		gmqueue.setAuthority("");

		using PostmanT = globalmq::marshalling::ThreadQueuePostman<BufferT>;
		typename PostmanT::MsgQueue queue;

		globalmq::marshalling::GMQThreadQueueTransport<TestPlatformSupport> transport(gmqueue, "test_node", queue, 1);

		using MetaPoolT = globalmq::marshalling::MetaPool<TestPlatformSupport>;
		MetaPoolT mp;
		mp.setTransport(&transport);

		mtest::Mock_WrapperForPublisher<ComposerT> publ;
		// mtest::Mock_WrapperForPublisher<mtest::structures::Mock, ComposerT> publ;
		mp.add(&publ);

		GMQ_COLL string path = getSubcriptionAddress(publ.publishableName());

		mtest::Mock_subscriber subs;
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
		using BufferT = TestPlatformSupport::BufferT;
		using ComposerT = TestPlatformSupport::ComposerT;
		using ParserT = TestPlatformSupport::ParserT;

		GMQueue<TestPlatformSupport> gmqueue;
		gmqueue.template initStateConcentratorFactory<mtest::StateConcentratorFactory<BufferT, ComposerT>>();
		gmqueue.setAuthority("");

		using PostmanT = globalmq::marshalling::ThreadQueuePostman<BufferT>;
		typename PostmanT::MsgQueue queue;

		globalmq::marshalling::GMQThreadQueueTransport<TestPlatformSupport> transport(gmqueue, "test_node", queue, 1);

		using MetaPoolT = globalmq::marshalling::MetaPool<TestPlatformSupport>;
		MetaPoolT mp;
		mp.setTransport(&transport);

		mtest::Mock_WrapperForPublisher<ComposerT> publ;
		mp.add(&publ);

		GMQ_COLL string path = getSubcriptionAddress(publ.publishableName());

		mtest::Mock_subscriber subs;
		mp.add(&subs);
		mp.subscribe(&subs, path);

		mtest::Mock_subscriber subs2;
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
		using BufferT = TestPlatformSupport::BufferT;
		using ComposerT = TestPlatformSupport::ComposerT;
		using ParserT = TestPlatformSupport::ParserT;

		GMQueue<TestPlatformSupport> gmqueue;
		gmqueue.template initStateConcentratorFactory<mtest::StateConcentratorFactory<BufferT, ComposerT>>();
		gmqueue.setAuthority("");

		using PostmanT = globalmq::marshalling::ThreadQueuePostman<BufferT>;
		typename PostmanT::MsgQueue queue;

		globalmq::marshalling::GMQThreadQueueTransport<TestPlatformSupport> transport(gmqueue, "test_node", queue, 1);

		using MetaPoolT = globalmq::marshalling::MetaPool<TestPlatformSupport>;
		MetaPoolT mp;
		mp.setTransport(&transport);

		mtest::Mock_WrapperForPublisher<ComposerT> publ;
		mp.add(&publ);
		mp.remove(&publ);
		mp.add(&publ);

		GMQ_COLL string path = getSubcriptionAddress(publ.publishableName());

		mtest::Mock_subscriber subs;
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
