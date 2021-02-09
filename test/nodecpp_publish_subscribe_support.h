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

	static void registerPublisher( globalmq::marshalling::PublisherBase<ComposerT>* publisher )
	{
		publishers.registerPublisher( publisher );
	}
	static void unregisterPublisher( globalmq::marshalling::PublisherBase<ComposerT>* publisher )
	{
		publishers.unregisterPublisher( publisher );
	}
	static void registerSubscriber( globalmq::marshalling::SubscriberBase<BufferT>* subscriber )
	{
		subscribers.registerSubscriber( subscriber );
	}
	static void unregisterSubscriber( globalmq::marshalling::SubscriberBase<BufferT>* subscriber )
	{
		subscribers.unregisterSubscriber( subscriber );
	}
};

#endif // NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
