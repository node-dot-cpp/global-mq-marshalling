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


std::string PathGmQueuePrefix1 = "test_gmqueue1_";
std::string PathGmQueuePrefix2 = "test_gmqueue2_";


template<class ComposerT>
class Mock_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>
{
	using BufferT = typename ComposerT::BufferType;
	BufferT buffer;
	ComposerT composer;


public:
	static constexpr uint64_t numTypeID = 5;
	static constexpr const char* stringTypeID = "Mock";

	Mock_WrapperForPublisher() : composer( buffer ){}
	ComposerT& getComposer() { return composer; }
	void startTick( BufferT&& buff ) { buffer = std::move( buff ); composer.reset(); ::globalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( composer );}
	BufferT&& endTick() { ::globalmq::marshalling::impl::composeStateUpdateMessageEnd( composer ); return std::move( buffer ); }
	const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }

	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );
		::globalmq::marshalling::impl::publishableStructComposeString( composer, "value", "TestStateSync", false );
		::globalmq::marshalling::impl::composeStructEnd( composer );
	}
    virtual void generateStateSyncMessage(ComposerT& composer) { compose(composer); }

};

template<class BufferT>
class Mock_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>
{
public:
	static constexpr uint64_t numTypeID = 5;
	static constexpr const char* stringTypeID = "Mock";

	Mock_WrapperForSubscriber() {}
	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferT>& parser ) 
	{
		parseStateSyncMessage(parser);
	}

	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser )
	{
		parseStateSyncMessage(parser);
	}

	template<typename ParserT>
	void applyMessageWithUpdates(ParserT& parser)
	{
		::globalmq::marshalling::impl::parseStateUpdateMessageBegin( parser );
		GMQ_COLL vector<size_t> addr;
		while( ::globalmq::marshalling::impl::parseAddressInPublishable<ParserT, GMQ_COLL vector<size_t>>( parser, addr ) )
		{
			addr.clear();
		}
	}


	template<class ParserT>
	void parseStateSyncMessage( ParserT& parser )
	{
		::globalmq::marshalling::impl::parseStructBegin( parser );

        std::string value;
		::globalmq::marshalling::impl::publishableParseString( parser, &value, "TestStateSync" );

		::globalmq::marshalling::impl::parseStructEnd( parser );
	}
};

template<class InputBufferT, class ComposerT>
class Mock_WrapperForConcentrator : public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>
{
	using BufferT = typename ComposerT::BufferType;

public:
	static constexpr uint64_t numTypeID = 5;

	Mock_WrapperForConcentrator() {}
	const char* name() {return "Mock";}
	
	// Acting as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ){ compose(composer); }
	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, "value", "TestStateSync", false );


		::globalmq::marshalling::impl::composeStructEnd( composer );
	}

	// Acting as subscriber
	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferT>& parser ) { parseStateSyncMessage(parser); }
	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser ) { parseStateSyncMessage(parser); }

	template<typename ParserT>
	void applyMessageWithUpdates(ParserT& parser)
	{
		::globalmq::marshalling::impl::parseStateUpdateMessageBegin( parser );
		GMQ_COLL vector<size_t> addr;
		while( ::globalmq::marshalling::impl::parseAddressInPublishable<ParserT, GMQ_COLL vector<size_t>>( parser, addr ) )
		{
			addr.clear();
		}
	}

	template<class ParserT>
	void parseStateSyncMessage( ParserT& parser )
	{
		::globalmq::marshalling::impl::parseStructBegin( parser );

        std::string value;
		::globalmq::marshalling::impl::publishableParseString( parser, &value, "TestStateSync" );

		::globalmq::marshalling::impl::parseStructEnd( parser );
	}
};


template<class InputBufferT, class ComposerT>
class MockStateConcentratorFactory : public ::globalmq::marshalling::StateConcentratorFactoryBase<InputBufferT, ComposerT>
{
public:
	virtual StateConcentratorBase<InputBufferT, ComposerT>* createConcentrator( uint64_t typeID )
	{
		switch( typeID )
		{
			case 5:
				return new Mock_WrapperForConcentrator<InputBufferT, ComposerT>();
			default:
				return nullptr;
		}
	}
};

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
                EXPECT(messages[i].msg == b2);
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
		gmqueue.template initStateConcentratorFactory<MockStateConcentratorFactory<BufferT, ComposerT>>();
		gmqueue.setAuthority("");

		using PostmanT = globalmq::marshalling::ThreadQueuePostman<BufferT>;
		typename PostmanT::MsgQueue queue;

		globalmq::marshalling::GMQThreadQueueTransport<GMQueueStatePublisherSubscriberTypeInfo> transport(gmqueue, "test_node", queue, 1);

		using MetaPoolT = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
		MetaPoolT mp;
		mp.setTransport(&transport);

		Mock_WrapperForPublisher<ComposerT> publ;
		mp.add(&publ);

		globalmq::marshalling::GmqPathHelper::PathComponents pc;
		pc.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
		pc.authority = "";
		pc.nodeName = "test_node";
		pc.statePublisherOrConnectionType = Mock_WrapperForPublisher<ComposerT>::stringTypeID;
		GMQ_COLL string path = globalmq::marshalling::GmqPathHelper::compose(pc);

		Mock_WrapperForSubscriber<BufferT> subs;
		mp.add(&subs);
		mp.subscribe(&subs, path);

		int msgCnt = 0;

		mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, PathGmQueuePrefix1, msgCnt, lest_env);

		mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, PathGmQueuePrefix1, msgCnt, lest_env);
	},
		lest_CASE("test_gmqueue.TestGmQueueWithMock2"){
		// gmqueue
		using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
		using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
		using ParserT = GMQueueStatePublisherSubscriberTypeInfo::ParserT;

		GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;
		gmqueue.template initStateConcentratorFactory<MockStateConcentratorFactory<BufferT, ComposerT>>();
		gmqueue.setAuthority("");

		using PostmanT = globalmq::marshalling::ThreadQueuePostman<BufferT>;
		typename PostmanT::MsgQueue queue;

		globalmq::marshalling::GMQThreadQueueTransport<GMQueueStatePublisherSubscriberTypeInfo> transport(gmqueue, "test_node", queue, 1);

		using MetaPoolT = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
		MetaPoolT mp;
		mp.setTransport(&transport);

		Mock_WrapperForPublisher<ComposerT> publ;
		mp.add(&publ);

		globalmq::marshalling::GmqPathHelper::PathComponents pc;
		pc.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
		pc.authority = "";
		pc.nodeName = "test_node";
		pc.statePublisherOrConnectionType = Mock_WrapperForPublisher<ComposerT>::stringTypeID;
		GMQ_COLL string path = globalmq::marshalling::GmqPathHelper::compose(pc);

		Mock_WrapperForSubscriber<BufferT> subs;
		mp.add(&subs);
		mp.subscribe(&subs, path);

		Mock_WrapperForSubscriber<BufferT> subs2;
		mp.add(&subs2);
		mp.subscribe(&subs2, path);

		int msgCnt = 0;

		mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, PathGmQueuePrefix2, msgCnt, lest_env);

		mp.postAllUpdates();
		deliverAllMessages<BufferT>(mp, queue, PathGmQueuePrefix2, msgCnt, lest_env);
	}


};

lest_MODULE(specification(), test_gmqueue);
