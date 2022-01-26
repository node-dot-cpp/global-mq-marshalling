#ifndef NODECPP_GMQUEUE_CUSTOMIZED_TYPES_H
#define NODECPP_GMQUEUE_CUSTOMIZED_TYPES_H

#include <marshalling.h>

class GMQueueStatePublisherSubscriberTypeInfo
{
public:
	using BufferT = globalmq::marshalling::Buffer;
	using ParserT = globalmq::marshalling::JsonParser<BufferT>;
	using ComposerT = globalmq::marshalling::JsonComposer<BufferT>;
//	using ParserT = globalmq::marshalling::GmqParser<BufferT>;
//	using ComposerT = globalmq::marshalling::GmqComposer<BufferT>;
	template<class T>
	using OwningPtrT = ::std::unique_ptr<T>;
//	using StateSubscriberT = globalmq::marshalling::StateSubscriberBase<BufferT>;
//	using StatePublisherT = globalmq::marshalling::StatePublisherBase<ComposerT>;
};

#endif // NODECPP_GMQUEUE_CUSTOMIZED_TYPES_H
