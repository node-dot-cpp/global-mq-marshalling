#ifndef NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
#define NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H

#include <vector>
#include <marshalling.h>
#include <publishable_impl.h>

class StatePublisherSubscriberPoolInfo;
//extern thread_local globalmq::marshalling::StatePublisherPool<StatePublisherSubscriberPoolInfo> publishers;
//extern thread_local globalmq::marshalling::StateSubscriberPool<StatePublisherSubscriberPoolInfo> subscribers;

// transporting level simulation (for this test, single-threaded)
constexpr uint32_t publisherPoolAddress = 1;
constexpr uint32_t subscriberPoolAddress = 0;
extern GMQ_COLL vector<globalmq::marshalling::Buffer> likeQueues[2];

class StatePublisherSubscriberInfo
{
public:
	using BufferT = globalmq::marshalling::Buffer;
	using ParserT = globalmq::marshalling::JsonParser<BufferT>;
	using ComposerT = globalmq::marshalling::JsonComposer<BufferT>;
//	using ParserT = globalmq::marshalling::GmqParser<BufferT>;
//	using ComposerT = globalmq::marshalling::GmqComposer<BufferT>;
	using StateSubscriberT = globalmq::marshalling::StateSubscriberBase<BufferT>;
	using StatePublisherT = globalmq::marshalling::StatePublisherBase<ComposerT>;
};

class StatePublisherSubscriberPoolInfo : public StatePublisherSubscriberInfo
{
public:
	// publishers and subscribers

	// addressing (what is kept at publisher's size
	using PublisherAddressT = uint32_t;
	using NodeAddressT = uint32_t;

	// used by pools
	static void sendSubscriptionRequest( BufferT& buff, GMQ_COLL string publisherName )
	{
		// Note: we assume that here or around there is a kind of routing table converting publisherName to some deliverable address
		//       here we just emulate it manually
		PublisherAddressT publAddr = (PublisherAddressT)(-1);
		NodeAddressT subscrAddr = 0;

		if ( publisherName == "publishable_sample" )
			publAddr = publisherPoolAddress;
		else
			assert( false );
		likeQueues[publAddr].push_back( std::move( buff ) );
	}
	static void sendMsgFromPublisherToSubscriber( BufferT& buff, NodeAddressT subscrAddr )
	{
		// Note: we assume that subscrAddr can either be used directly (like in this sample code), or here or around there is a kind of routing table converting subscrAddr to some deliverable address
		//       here we just emulate it manually
		assert( subscrAddr == subscriberPoolAddress ); // by construction in this test
		likeQueues[subscrAddr].push_back( std::move( buff ) );
	}
};

using MetaPoolT = globalmq::marshalling::MetaPool<StatePublisherSubscriberPoolInfo>;
extern globalmq::marshalling::MetaPool<StatePublisherSubscriberPoolInfo> mp;

#endif // NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
