#ifndef NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
#define NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H

#include <vector>
#include <marshalling.h>
#include <publishable_impl.h>
extern thread_local std::vector<globalmq::marshalling::PublisherBase*> publishers;
extern thread_local std::vector<globalmq::marshalling::SubscriberBase<globalmq::marshalling::Buffer>*> subscribers;

class PublisherSubscriberRegistrar
{
public:
	using BufferT = globalmq::marshalling::Buffer;
	static void registerPublisher( globalmq::marshalling::PublisherBase* publisher )
	{
		publishers.push_back( publisher );
	}
	static void unregisterPublisher( globalmq::marshalling::PublisherBase* publisher )
	{
		for ( size_t i=0; i<publishers.size(); ++i )
			if ( publishers[i] == publisher )
			{
				publishers.erase( publishers.begin() + i );
				return;
			}
		assert( false ); // not found
	}
	static void registerSubscriber( globalmq::marshalling::SubscriberBase<globalmq::marshalling::Buffer>* subscriber )
	{
		subscribers.push_back( subscriber );
	}
	static void unregisterSubscriber( globalmq::marshalling::SubscriberBase<globalmq::marshalling::Buffer>* subscriber )
	{
		for ( size_t i=0; i<subscribers.size(); ++i )
			if ( subscribers[i] == subscriber )
			{
				subscribers.erase( subscribers.begin() + i );
				return;
			}
		assert( false ); // not found
	}
};

#endif // NODECPP_PUBLISH_SUBSCRIBE_SUPPORT_H
