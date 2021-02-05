#ifndef NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
#define NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H

#include <vector>
#include <marshalling.h>
#include <publishable_impl.h>
//extern thread_local std::vector<globalmq::marshalling::PublisherBase<globalmq::marshalling::GmqComposer<globalmq::marshalling::Buffer>>*> publishers;
//extern thread_local std::vector<globalmq::marshalling::SubscriberBase<globalmq::marshalling::Buffer>*> subscribers;
extern thread_local globalmq::marshalling::PublisherPool<globalmq::marshalling::GmqComposer<globalmq::marshalling::Buffer>> publishers;
extern thread_local globalmq::marshalling::SubscriberPool<globalmq::marshalling::Buffer> subscribers;

class PublisherSubscriberRegistrar
{
public:
	using BufferT = globalmq::marshalling::Buffer;
	using ComposerT = globalmq::marshalling::GmqComposer<BufferT>;
	static void registerPublisher( globalmq::marshalling::PublisherBase<globalmq::marshalling::GmqComposer<globalmq::marshalling::Buffer>>* publisher )
	{
		publishers.registerPublisher( publisher );
	}
	static void unregisterPublisher( globalmq::marshalling::PublisherBase<globalmq::marshalling::GmqComposer<globalmq::marshalling::Buffer>>* publisher )
	{
		publishers.unregisterPublisher( publisher );
	}
	static void registerSubscriber( globalmq::marshalling::SubscriberBase<globalmq::marshalling::Buffer>* subscriber )
	{
		subscribers.registerSubscriber( subscriber );
	}
	static void unregisterSubscriber( globalmq::marshalling::SubscriberBase<globalmq::marshalling::Buffer>* subscriber )
	{
		subscribers.unregisterSubscriber( subscriber );
	}
};

#endif // NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
