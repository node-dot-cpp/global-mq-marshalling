#ifndef NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
#define NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H

#include <vector>
#include <marshalling.h>
#include <publishable_impl.h>

class PublisherSubscriberRegistrar;
extern thread_local globalmq::marshalling::PublisherPool<PublisherSubscriberRegistrar> publishers;
extern thread_local globalmq::marshalling::SubscriberPool<PublisherSubscriberRegistrar> subscribers;

// transporting level simulation (for this test, single-threaded)
constexpr uint32_t publisherPoolAddress = 1;
constexpr uint32_t subscriberPoolAddress = 0;
extern GMQ_COLL vector<globalmq::marshalling::Buffer> likeQueues[2];

class PublisherSubscriberRegistrar
{
public:
	using BufferT = globalmq::marshalling::Buffer;
	using InternalBufferT = globalmq::marshalling::Buffer;

	// protocol is practically defined below
	using ParserT = globalmq::marshalling::JsonParser<BufferT>;
	using ComposerT = globalmq::marshalling::JsonComposer<BufferT>;

	// publishers and subscribers
	using SubscriberT = globalmq::marshalling::SubscriberBase<BufferT>;
	using PublisherT = globalmq::marshalling::PublisherBase<ComposerT>;

	// addressing (what is kept at publisher's size
//	using PublisherAddressT = GMQ_COLL string;
//	using SubscriberNodeAddressT = GMQ_COLL string;
	using PublisherAddressT = uint32_t;
	using SubscriberNodeAddressT = uint32_t;

	// used by generated code
	static void registerPublisher( PublisherT* publisher )
	{
		publishers.registerPublisher( publisher );
	}
	static void unregisterPublisher( PublisherT* publisher )
	{
		publishers.unregisterPublisher( publisher );
	}
	static void registerSubscriber( SubscriberT* subscriber )
	{
		subscribers.registerSubscriber( subscriber );
	}
	static void unregisterSubscriber( SubscriberT* subscriber )
	{
		subscribers.unregisterSubscriber( subscriber );
	}

	// used by State owner
	static void subscribe( SubscriberT* subscriber )
	{
		subscribers.subscribe( subscriber );
	}

	// used by pools
	static void sendSubscriptionRequest( BufferT& buff, GMQ_COLL string publisherName )
	{
		// Note: we assume that here or around there is a kind of routing table converting publisherName to some deliverable address
		//       here we just emulate it manually
		PublisherAddressT publAddr = (PublisherAddressT)(-1);
		SubscriberNodeAddressT subscrAddr = 0;

		if ( publisherName == "publishable_sample" )
			publAddr = publisherPoolAddress;
		else
			assert( false );
		likeQueues[publAddr].push_back( std::move( buff ) );
	}
	static void sendMsgFromPublisherToSubscriber( BufferT& buff, SubscriberNodeAddressT subscrAddr )
	{
		// Note: we assume that subscrAddr can either be used directly (like in this sample code), or here or around there is a kind of routing table converting subscrAddr to some deliverable address
		//       here we just emulate it manually
		assert( subscrAddr == subscriberPoolAddress ); // by construction in this test
		likeQueues[subscrAddr].push_back( std::move( buff ) );
	}
};

#endif // NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
