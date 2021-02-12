#ifndef NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
#define NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H

#include <vector>
#include <marshalling.h>
#include <publishable_impl.h>

class PublisherSubscriberRegistrar;
extern thread_local globalmq::marshalling::PublisherPool<PublisherSubscriberRegistrar> publishers;
extern thread_local globalmq::marshalling::SubscriberPool<PublisherSubscriberRegistrar> subscribers;

class PublisherSubscriberRegistrar
{
public:
	using BufferT = globalmq::marshalling::Buffer;

	// protocol is practically defined below
	using ParserT = globalmq::marshalling::JsonParser<BufferT>;
	using ComposerT = globalmq::marshalling::JsonComposer<BufferT>;

	// publishers and subscribers
	using SubscriberT = globalmq::marshalling::SubscriberBase<BufferT>;
	using PublisherT = globalmq::marshalling::PublisherBase<ComposerT>;

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
	static void subscribe( SubscriberT* subscriber )
	{
		subscribers.subscribe( subscriber );
	}
};

#endif // NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
