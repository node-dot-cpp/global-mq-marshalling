/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
* Copyright (c) 2021, OLogN Technologies AG
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

#ifndef GMQUEUE_H
#define GMQUEUE_H

#include "global_mq_common.h"
#include "marshalling.h"
#include <thread>
#include <mutex>
#include <condition_variable>


#ifndef GMQUEUE_CUSTOMIZED_Q_TYPES
class GMQueueStatePublisherSubscriberTypeInfo
{
public:
	using BufferT = globalmq::marshalling::Buffer;
	using ParserT = globalmq::marshalling::JsonParser<BufferT>;
	using ComposerT = globalmq::marshalling::JsonComposer<BufferT>;
	template<class T>
	using OwningPtrT = ::std::unique_ptr<T>;
};
#else
#include GMQUEUE_CUSTOMIZED_Q_TYPES
#endif

namespace globalmq::marshalling2
{
	class ParserBase;
	class ComposerBase;

	template<class BufferT>
	class IComposer2;

	template<class BufferT>
	class GmqComposer2;

	template <class BufferT>
    class JsonComposer2;

	template<class BufferT>
	class IParser2;

	template<class BufferT>
	class GmqParser2;

	template <class BufferT>
    class JsonParser2;
}


namespace globalmq::marshalling {

using MessageBufferT = typename GMQueueStatePublisherSubscriberTypeInfo::BufferT;

struct PublishableStateMessageHeader
{
	static constexpr uint64_t invalidValue = 0xFFFFFFFFFFFFFFULL;
	enum MsgType { undefined = 0, subscriptionRequest = 1, subscriptionResponse = 2, stateUpdate = 3, connectionRequest = 4, connectionAccepted = 5, connectionMessage = 6 };
	enum ConnMsgDirection { toServer = 0, toClient = 1 };
	MsgType type = MsgType::undefined;
	uint64_t state_type_id_or_direction = invalidValue; // Note: may be removed in future versions
	uint64_t priority = invalidValue;
	GMQ_COLL string path;  // subscriptionRequest only
	uint64_t ref_id_at_subscriber = invalidValue; // updatable
	uint64_t ref_id_at_publisher = invalidValue; // updatable

	struct UpdatedData
	{
		uint64_t ref_id_at_subscriber = invalidValue;
		uint64_t ref_id_at_publisher = invalidValue;
		bool update_ref_id_at_subscriber = false;
		bool update_ref_id_at_publisher = false;
	};

	template<class ParserT>
	void parse( ParserT& parser )
	{
		globalmq::marshalling::impl::parsePublishableStructBegin( parser, "hdr" );
		size_t msgType;
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &state_type_id_or_direction, "state_type_id" );
				break;
			case MsgType::connectionRequest:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &state_type_id_or_direction, "direction" );
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &priority, "priority" );
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			case MsgType::connectionRequest:
			{
				type = (MsgType)(msgType);
				globalmq::marshalling::impl::publishableParseString<ParserT, GMQ_COLL string>( parser, &path, "path" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				break;
			}
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
			{
				type = (MsgType)(msgType);
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &ref_id_at_publisher, "ref_id_at_publisher" );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		globalmq::marshalling::impl::parsePublishableStructEnd( parser );
	}

	template<class ParserT>
	void parse2( ParserT& parser )
	{
		parser.structBegin();
		// globalmq::marshalling::impl::parsePublishableStructBegin( parser, "hdr" );
		parser.namedParamBegin("msg_type");
		uint64_t msgType = parser.parseUnsignedInteger();
		parser.nextElement();
		// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
				parser.namedParamBegin("state_type_id");
				state_type_id_or_direction = parser.parseUnsignedInteger();
				parser.nextElement();
				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &state_type_id_or_direction, "state_type_id" );
				break;
			case MsgType::connectionRequest:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
				parser.namedParamBegin("direction");
				state_type_id_or_direction = parser.parseUnsignedInteger();
				parser.nextElement();
				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &state_type_id_or_direction, "direction" );
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		parser.namedParamBegin("priority");
		priority = parser.parseUnsignedInteger();
		parser.nextElement();
		// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &priority, "priority" );
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			case MsgType::connectionRequest:
				type = (MsgType)(msgType);
				parser.namedParamBegin("path");
				path = parser.parseString();
				parser.nextElement();
				parser.namedParamBegin("ref_id_at_subscriber");
				ref_id_at_subscriber = parser.parseUnsignedInteger();

				// globalmq::marshalling::impl::publishableParseString<ParserT, GMQ_COLL string>( parser, &path, "path" );
				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				break;
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
				type = (MsgType)(msgType);
				parser.namedParamBegin("ref_id_at_subscriber");
				ref_id_at_subscriber = parser.parseUnsignedInteger();
				parser.nextElement();
				parser.namedParamBegin("ref_id_at_publisher");
				ref_id_at_publisher = parser.parseUnsignedInteger();
				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &ref_id_at_publisher, "ref_id_at_publisher" );
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		parser.structEnd();
		// globalmq::marshalling::impl::parsePublishableStructEnd( parser );
	}

	template<class ParserT, class ComposerT>
	static void parseAndUpdate( ParserT& msgStartParser, ParserT& parser, typename ComposerT::BufferType& buff, const UpdatedData& udata )
	{
		ComposerT composer( buff );
//		ParserT parser2 = parser;
		globalmq::marshalling::impl::parsePublishableStructBegin( parser, "hdr" );
		size_t msgType;
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		uint64_t dummy;
		GMQ_COLL string dummyStr;
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "state_type_id" );
				break;
			case MsgType::connectionRequest:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "direction" );
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "priority" );
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			case MsgType::connectionRequest:
			{
				globalmq::marshalling::impl::publishableParseString<ParserT, GMQ_COLL string>( parser, &dummyStr, "path" );
				assert( !udata.update_ref_id_at_publisher );
				size_t offset = parser.getCurrentOffset();
				::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( msgStartParser.getIterator(), buff, offset );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "ref_id_at_subscriber" );
				if ( udata.update_ref_id_at_subscriber )
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, udata.ref_id_at_subscriber, "ref_id_at_subscriber", false );
				else
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, dummy, "ref_id_at_subscriber", false );
				break;
			}
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
			{
				size_t offset = parser.getCurrentOffset();
				::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( msgStartParser.getIterator(), buff, offset );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "ref_id_at_subscriber" );
				if ( udata.update_ref_id_at_subscriber )
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, udata.ref_id_at_subscriber, "ref_id_at_subscriber", true );
				else
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, dummy, "ref_id_at_subscriber", true );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "ref_id_at_publisher" );
				if ( udata.update_ref_id_at_publisher )
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, udata.ref_id_at_publisher, "ref_id_at_publisher", false );
				else
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, dummy, "ref_id_at_publisher", false );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( parser.getIterator(), buff );
	}

	template<class ParserT, class ComposerT>
	static void parseAndUpdate2( ParserT& msgStartParser, ParserT& parser, typename ComposerT::BufferType& buff, const UpdatedData& udata )
	{
		ComposerT composer( buff );
//		ParserT parser2 = parser;
		// globalmq::marshalling::impl::parsePublishableStructBegin( parser, "hdr" );
		parser.structBegin();
		// size_t msgType;
		// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		parser.namedParamBegin("msg_type");
		uint64_t msgType = parser.parseUnsignedInteger();
		parser.nextElement();

		uint64_t dummy;
		GMQ_COLL string dummyStr;
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
				parser.namedParamBegin("state_type_id");
				dummy = parser.parseUnsignedInteger();
				parser.nextElement();
				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "state_type_id" );
				break;
			case MsgType::connectionRequest:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
				parser.namedParamBegin("direction");
				dummy = parser.parseUnsignedInteger();
				parser.nextElement();
				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "direction" );
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		parser.namedParamBegin("priority");
		dummy = parser.parseUnsignedInteger();
		parser.nextElement();
		// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "priority" );
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			case MsgType::connectionRequest:
			{
				parser.namedParamBegin("path");
				dummyStr = parser.parseString();
				parser.nextElement();

				// globalmq::marshalling::impl::publishableParseString<ParserT, GMQ_COLL string>( parser, &dummyStr, "path" );
				assert( !udata.update_ref_id_at_publisher );
				size_t offset = parser.getCurrentOffset();

				composer.appendRaw(msgStartParser.getIterator(), offset);
				// ::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( msgStartParser.getIterator(), buff, offset );


				parser.namedParamBegin("ref_id_at_subscriber");
				dummy = parser.parseUnsignedInteger();

				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "ref_id_at_subscriber" );
				composer.namedParamBegin("ref_id_at_subscriber");
				if ( udata.update_ref_id_at_subscriber )
					composer.composeUnsignedInteger(udata.ref_id_at_subscriber);
					// globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, udata.ref_id_at_subscriber, "ref_id_at_subscriber", false );
				else
					composer.composeUnsignedInteger(dummy);
					// globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, dummy, "ref_id_at_subscriber", false );
				break;
			}
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
			{
				size_t offset = parser.getCurrentOffset();
				composer.appendRaw(msgStartParser.getIterator(), offset);
				// ::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( msgStartParser.getIterator(), buff, offset );
				
				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "ref_id_at_subscriber" );
				parser.namedParamBegin("ref_id_at_subscriber");
				dummy = parser.parseUnsignedInteger();
				parser.nextElement();

				composer.namedParamBegin("ref_id_at_subscriber");
				if ( udata.update_ref_id_at_subscriber )
					composer.composeUnsignedInteger(udata.ref_id_at_subscriber);
					// globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, udata.ref_id_at_subscriber, "ref_id_at_subscriber", true );
				else
					composer.composeUnsignedInteger(dummy);
					// globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, dummy, "ref_id_at_subscriber", true );

				composer.nextElement();

				parser.namedParamBegin("ref_id_at_publisher");
				dummy = parser.parseUnsignedInteger();
				// globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, uint64_t>( parser, &dummy, "ref_id_at_publisher" );

				composer.namedParamBegin("ref_id_at_publisher");
				if ( udata.update_ref_id_at_publisher )
					composer.composeUnsignedInteger(udata.ref_id_at_publisher);
					// globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, udata.ref_id_at_publisher, "ref_id_at_publisher", false );
				else
					composer.composeUnsignedInteger(dummy);
					// globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, dummy, "ref_id_at_publisher", false );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}

		// copy everything else
		composer.appendRaw(parser.getIterator());
		// ::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( parser.getIterator(), buff );
	}

	template<class ComposerT>
	void compose(ComposerT& composer, bool addSeparator) const
	{
		globalmq::marshalling::impl::composePublishableStructBegin( composer, "hdr" );
		globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, (uint32_t)(type), "msg_type", true );
		switch ( type )
		{
			case MsgType::subscriptionRequest:
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, state_type_id_or_direction, "state_type_id", true );
				break;
			case MsgType::connectionRequest:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, state_type_id_or_direction, "direction", true );
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, priority, "priority", true );
		switch ( type )
		{
			case MsgType::subscriptionRequest:
			case MsgType::connectionRequest:
			{
				globalmq::marshalling::impl::publishableStructComposeString( composer, path, "path", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, ref_id_at_subscriber, "ref_id_at_subscriber", false );
				break;
			}
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
			{
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, ref_id_at_subscriber, "ref_id_at_subscriber", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, ref_id_at_publisher, "ref_id_at_publisher", false );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		globalmq::marshalling::impl::composePublishableStructEnd( composer, addSeparator );
	}

	template<class ComposerT>
	void compose2(ComposerT& composer) const
	{
		composer.structBegin();
		composer.namedParamBegin("msg_type");
		composer.composeUnsignedInteger((uint32_t)(type));
		composer.nextElement();
		switch ( type )
		{
			case MsgType::subscriptionRequest:
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
				composer.namedParamBegin("state_type_id");
				composer.composeUnsignedInteger(state_type_id_or_direction);
				composer.nextElement();
				break;
			case MsgType::connectionRequest:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
				composer.namedParamBegin("direction");
				composer.composeUnsignedInteger(state_type_id_or_direction);
				composer.nextElement();
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}

		composer.namedParamBegin("priority");
		composer.composeUnsignedInteger(priority);
		composer.nextElement();

		switch ( type )
		{
			case MsgType::subscriptionRequest:
			case MsgType::connectionRequest:
				composer.namedParamBegin("path");
				composer.composeString(path);
				composer.nextElement();
				composer.namedParamBegin("ref_id_at_subscriber");
				composer.composeUnsignedInteger(ref_id_at_subscriber);
				break;
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
			case MsgType::connectionAccepted:
			case MsgType::connectionMessage:
				composer.namedParamBegin("ref_id_at_subscriber");
				composer.composeUnsignedInteger(ref_id_at_subscriber);
				composer.nextElement();
				composer.namedParamBegin("ref_id_at_publisher");
				composer.composeUnsignedInteger(ref_id_at_publisher);
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		composer.structEnd();
	}
};

template<class ComposerT>
void helperComposePublishableStateMessageBegin(ComposerT& composer, const PublishableStateMessageHeader& header)
{
	if constexpr ( ComposerT::proto == Proto::GMQ || ComposerT::proto == Proto::JSON )
	{
		globalmq::marshalling::impl::composeStructBegin( composer );
		if ( header.type == PublishableStateMessageHeader::MsgType::subscriptionResponse || header.type == PublishableStateMessageHeader::MsgType::stateUpdate || header.type == PublishableStateMessageHeader::MsgType::connectionMessage )
		{
			header.compose( composer, true );
			globalmq::marshalling::impl::composeKey( composer, "data" );
			// next call would be generateXXXMessage()
		}
		else
			header.compose( composer, false );
	}
	else
	{
		composer.stateSyncBegin();
		header.compose2(composer);

		if ( header.type == PublishableStateMessageHeader::MsgType::subscriptionResponse || header.type == PublishableStateMessageHeader::MsgType::stateUpdate || header.type == PublishableStateMessageHeader::MsgType::connectionMessage )
		{
			composer.nextElement();
			composer.namedParamBegin("data");
			// next call would be generateXXXMessage()
		}
	}
}

template<class ComposerT>
void helperComposePublishableStateMessageEnd(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ || ComposerT::proto == Proto::JSON )
		globalmq::marshalling::impl::composeStructEnd( composer );
	else
		composer.stateSyncEnd();
}

template<class ParserT>
void helperParsePublishableStateMessageBegin( ParserT& parser, PublishableStateMessageHeader& header ) // leaves parser pos at the beginning of message data part (if any)
{
	if constexpr ( ParserT::proto == Proto::GMQ || ParserT::proto == Proto::JSON )
	{
		globalmq::marshalling::impl::parseStructBegin( parser );
		header.parse( parser );
		if ( header.type == PublishableStateMessageHeader::MsgType::subscriptionResponse || header.type == PublishableStateMessageHeader::MsgType::stateUpdate || header.type == PublishableStateMessageHeader::MsgType::connectionMessage )
			globalmq::marshalling::impl::parseKey( parser, "data" );
	}
	else
	{
		parser.stateSyncBegin();
		header.parse2( parser );
		if ( header.type == PublishableStateMessageHeader::MsgType::subscriptionResponse || header.type == PublishableStateMessageHeader::MsgType::stateUpdate || header.type == PublishableStateMessageHeader::MsgType::connectionMessage )
		{
			parser.nextElement();
			parser.namedParamBegin("data");
		}
	}
}

template<class ParserT>
void helperParsePublishableStateMessageEnd(ParserT& parser)
{
	if constexpr ( ParserT::proto == Proto::GMQ || ParserT::proto == Proto::JSON )
		globalmq::marshalling::impl::parseStructEnd( parser );
	else
		parser.stateSyncEnd();
}

template<class ParserT, class ComposerT>
void helperParseAndUpdatePublishableStateMessage( typename ParserT::BufferType& buffFrom, typename ComposerT::BufferType& buffTo, const PublishableStateMessageHeader::UpdatedData& udata )
{
	if constexpr (ParserT::proto == Proto::JSON || ParserT::proto == Proto::GMQ)
	{
		auto riter = buffFrom.getReadIter();
		auto riter1 = buffFrom.getReadIter();
		ParserT parser( riter );
		ParserT parserCurrent( riter1 );
		PublishableStateMessageHeader header;
		globalmq::marshalling::impl::parseStructBegin( parserCurrent );
		header.parseAndUpdate<ParserT, ComposerT>( parser, parserCurrent, buffTo, udata );
	}
	else
	{
		auto riter = buffFrom.getReadIter();
		auto riter1 = buffFrom.getReadIter();
		ParserT parser( riter );
		ParserT parserCurrent( riter1 );
		PublishableStateMessageHeader header;
		// globalmq::marshalling::impl::parseStructBegin( parserCurrent );
		parserCurrent.stateSyncBegin();
		header.parseAndUpdate2<ParserT, ComposerT>( parser, parserCurrent, buffTo, udata );
	}
}


struct GmqPathHelper
{
	/*
		(global)
		globalmq://example.com/node23?sp=pub1
		sp stands for State Publisher 
		or (another global, with different - future - resolution)
		globalmq://ourname!gmq:4567/somenode?sp=somepub 
		!gmq is a non-standard extension which means we'll resolve the name using our own means
		or (local)
		globalmq:/lobbynode?sp=worldlist
	*/
	struct PathComponents
	{
		PublishableStateMessageHeader::MsgType type = PublishableStateMessageHeader::MsgType::undefined;
		GMQ_COLL string authority;
		bool furtherResolution = false;
		bool hasPort = false;
		uint16_t port = 0xFFFF;
		GMQ_COLL string nodeName;
		GMQ_COLL string statePublisherOrConnectionType; // for subscription request
	};

	static GMQ_COLL string compose( const PathComponents& components )
	{
		// TODO: check components
		GMQ_COLL string ret = "globalmq:";
		if ( !components.authority.empty() )
		{
			ret += "//";
			ret += components.authority;
		}
		if ( components.furtherResolution )
			ret += "!gmq";
		if ( components.hasPort )
		{
			auto str = fmt::format( ":{}", components.port );
			ret += str;
		}
		assert( !components.nodeName.empty() );
		assert( !components.statePublisherOrConnectionType.empty() );
		ret += '/';
		ret += localPart( components );
		return ret;
	}

	static GMQ_COLL string localPart( const PathComponents& components )
	{
		switch ( components.type )
		{
			case PublishableStateMessageHeader::MsgType::subscriptionRequest:
				return fmt::format( "{}?sp={}", components.nodeName, components.statePublisherOrConnectionType );
			case PublishableStateMessageHeader::MsgType::connectionRequest:
				return fmt::format( "{}?ct={}", components.nodeName, components.statePublisherOrConnectionType );
			default:
				assert( false );
				return "";
		}
	}

	static bool parse( GMQ_COLL string path, PathComponents& components )
	{
		size_t pos = path.find( "globalmq:" );
		if ( pos != 0 )
			return false;
		pos += sizeof( "globalmq:" ) - 1;
		if ( path.size() <= pos )
			return false;
		if ( path[pos++] != '/' )
			return false;
		if ( path[pos] == '/' ) // double-slash, authority component is present
		{
			++pos;
			size_t pos1 = path.find( "/", pos );
			if ( pos1 == GMQ_COLL string::npos )
				return false;
			components.authority = path.substr( pos, pos1 - pos );
			pos = pos1 + 1;
			pos1 = components.authority.find_last_of( ':' );
			if ( pos1 != GMQ_COLL string::npos )
			{
				char* end = nullptr;
				size_t port = strtol( components.authority.c_str() + pos1 + 1, &end, 10 );
				if ( components.authority.c_str() + pos1 + 1 == end )
					return false;
				if ( end < components.authority.c_str() + components.authority.size() ) // there are remaining chars
					return false;
				if ( port >= UINT16_MAX )
					return false;
				components.hasPort = true;
				components.port = (uint16_t)port;
				components.authority.erase( pos1 );
			}
			else
			{
				components.hasPort = false;
				components.port = 0xFFFF;
			}

			size_t pos2 = components.authority.find_last_of( '!' );
			if ( pos2 != GMQ_COLL string::npos )
			{
				if ( components.authority.size() - pos2 < sizeof( "gmq" ) - 1 )
					return false;
				if ( components.authority.substr( pos2 + 1 ) != "gmq" )
					return false;
				components.furtherResolution = true;
				components.authority.erase( pos2 );
			}
			else
			{
				components.furtherResolution = false;
			}
		}
		else
		{
			components.authority = "";
			components.hasPort = false;
			components.furtherResolution = false;
			components.port = 0xFFFF;
		}

		// node name
		size_t pos1 = path.find( '?', pos );
		if ( pos1 == GMQ_COLL string::npos )
			return false;
		components.nodeName = path.substr( pos, pos1 - pos );
		pos = pos1;

		// statePublisherOrConnectionType
		switch ( components.type )
		{
			case PublishableStateMessageHeader::MsgType::subscriptionRequest: 
				pos = path.find( "sp=", pos ); 
				pos += sizeof( "sp=" ) - 1;
				break;
			case PublishableStateMessageHeader::MsgType::connectionRequest:
				pos = path.find( "ct=", pos ); 
				pos += sizeof( "ct=" ) - 1;
				break;
			default:
				assert( false );
		}
		if ( pos == GMQ_COLL string::npos )
			return false;
		pos1 = path.find( '&', pos );
		if ( pos1 == GMQ_COLL string::npos )
			components.statePublisherOrConnectionType = path.substr( pos );
		else
			components.statePublisherOrConnectionType = path.substr( pos, pos1 - pos );
		return true;
	}
};

template<class InputBufferT, class ComposerT>
class StateConcentratorBase
{
	// using OutputBufferT = typename ComposerT::BufferType;

public:
	virtual ~StateConcentratorBase() {}
	// as subscriber
	virtual void applyGmqMessageWithUpdates( GmqParser<InputBufferT>& parser ) = 0;
	virtual void applyJsonMessageWithUpdates( JsonParser<InputBufferT>& parser ) = 0;
	virtual void applyGmqStateSyncMessage( GmqParser<InputBufferT>& parser ) = 0;
	virtual void applyJsonStateSyncMessage( JsonParser<InputBufferT>& parser ) = 0;

	// as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ) = 0;

	virtual const char* name() { throw std::exception(); }

	// mb: using just 'name' prevents idl from using member named 'name'
    virtual const char* publishableName() { return name(); }

	// new interface with default implementation to avoid breaking old code
	virtual void publishableApplyUpdates( globalmq::marshalling2::ParserBase& parser ) { throw std::exception(); }
	virtual void publishableApplyStateSync( globalmq::marshalling2::ParserBase& parser ) { throw std::exception(); }
	virtual void publishableGenerateStateSync( globalmq::marshalling2::ComposerBase& composer ) { throw std::exception(); }
};

template<class InputBufferT, class ComposerT>
class StateConcentratorFactoryBase
{
public:
	virtual StateConcentratorBase<InputBufferT, ComposerT>* createConcentrator( uint64_t typeID ) = 0;
	virtual ~StateConcentratorFactoryBase() = default;
};

class InProcessMessagePostmanBase
{
public:
	InProcessMessagePostmanBase() {};
	virtual void postMessage( MessageBufferT&& ) = 0;
	virtual ~InProcessMessagePostmanBase() {}
};

struct AddressableLocation
{
	InProcessMessagePostmanBase* postman = nullptr;
	uint64_t reincarnation = 0;
};

struct SlotIdx
{
	static constexpr size_t invalid_idx = (size_t)(-1);
	static constexpr size_t invalid_reincarnation = (size_t)(-1);
	size_t idx = invalid_idx;
	uint64_t reincarnation = invalid_reincarnation;
	bool isInitialized() { return idx != invalid_idx && reincarnation != invalid_reincarnation; }
	bool operator == ( const SlotIdx& other ) const { return idx == other.idx && reincarnation == other.reincarnation; }
	bool operator != ( const SlotIdx& other ) const { return idx != other.idx || reincarnation != other.reincarnation; }
	void invalidate() { idx = invalid_idx; reincarnation = invalid_reincarnation; }
};

class AddressableLocations // one per process; provides process-unique Slot with Postman and returns its SlotIdx
{
	GMQ_COLL vector<AddressableLocation> slots; // mx-protected!
public:
	SlotIdx add( InProcessMessagePostmanBase* postman )
	{ 
		// essentially add to slots and return its idx
		for ( size_t i = 0; i<slots.size(); ++i )
			if ( slots[i].postman == nullptr )
			{
				slots[i].postman = postman;
				++(slots[i].reincarnation);
				return SlotIdx({i,slots[i].reincarnation});
			}
		slots.push_back({postman, 0});
		return SlotIdx({slots.size() - 1, 0});
	}
	void remove( SlotIdx idx )
	{ 
		// find by idx.idx, check reincarnaion, set postman to null
		assert ( idx.idx < slots.size() ); 
		assert ( idx.reincarnation == slots[idx.idx].reincarnation ); 
		slots[idx.idx].postman = nullptr;
	}
	InProcessMessagePostmanBase* getPostman( SlotIdx idx )
	{
		// access, verify, return
		assert ( idx.idx < slots.size() ); 
		assert ( idx.reincarnation == slots[idx.idx].reincarnation ); 
		return slots[idx.idx].postman;
	}
};

template<class PlatformSupportT>
class GMQueue
{
	using InputBufferT = typename PlatformSupportT::BufferT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using ParserT = typename PlatformSupportT::ParserT;

	AddressableLocations addressableLocations;

	GMQ_COLL string myAuthority;
	bool isMyAuthority( GMQ_COLL string authority )
	{
		// TODO: regexp comparison (note: myAuthority could be '*', etc );
		return authority == myAuthority || authority.empty();
	}

	class ConcentratorWrapper
	{
		friend class GMQueue<PlatformSupportT>;

		StateConcentratorBase<InputBufferT, ComposerT>* ptr = nullptr;
		bool subscriptionResponseReceived = false;
		static constexpr uint64_t invalidValue = 0xFFFFFFFFFFFFFFFFULL;
		uint64_t idAtPublisher = invalidValue; // one for all subscriber using this concentrator

	public:
		struct SubscriberData
		{
			uint64_t ref_id_at_subscriber = invalidValue;
			uint64_t ref_id_at_publisher = invalidValue;
			SlotIdx senderSlotIdx;
		};
		GMQ_COLL vector<SubscriberData> subscribers;

	public:
		ConcentratorWrapper( StateConcentratorBase<InputBufferT, ComposerT>* ptr_ ) : ptr( ptr_ ) {}
		ConcentratorWrapper( const ConcentratorWrapper& ) = delete;
		ConcentratorWrapper& operator = ( const ConcentratorWrapper& ) = delete;
		ConcentratorWrapper( ConcentratorWrapper&& other ) noexcept { ptr = other.ptr; other.ptr = nullptr; }
		ConcentratorWrapper& operator = ( ConcentratorWrapper&& other ) noexcept { ptr = other.ptr; other.ptr = nullptr; return *this; }
		~ConcentratorWrapper() { if ( ptr ) delete ptr; }

		// Gmqueue part (indeed, we need it only if 'remove concentrator' event may actually happen (conditions?))
		GMQ_COLL string address;
		uint64_t idInQueue = invalidValue; // used as a key for finding this concentrator; reported to publisher as a ref_id_at_subscriber (note: concentrator plays role of subscriber in this case)

	public:
		uint64_t addSubscriber( SubscriberData sd )
		{
			subscribers.push_back( sd );
			return subscribers.size() - 1;
		}

		bool isSsubscriptionResponseReceived() { return subscriptionResponseReceived; }

		void generateStateSyncMessage( ComposerT& composer )
		{
			assert( ptr != nullptr );
			assert( subscriptionResponseReceived );
			if constexpr ( ComposerT::proto == globalmq::marshalling::Proto::JSON ||
							ComposerT::proto == globalmq::marshalling::Proto::GMQ )
				ptr->generateStateSyncMessage( composer );
			else
				ptr->publishableGenerateStateSync( composer );
		}

		void onSubscriptionResponseMessage( ParserT& parser, uint64_t idAtPublisher_ ) 
		{
			assert( ptr != nullptr );
			assert( subscribers.size() != 0 ); // current implementation does not practically assume removing subscribers 
			if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
				ptr->applyJsonStateSyncMessage( parser );
			else if constexpr ( ParserT::proto == globalmq::marshalling::Proto::GMQ )
				ptr->applyGmqStateSyncMessage( parser );
			else
				ptr->publishableApplyStateSync( parser );

			subscriptionResponseReceived = true;
			idAtPublisher = idAtPublisher_;
		}

		void onStateUpdateMessage( ParserT& parser ) 
		{
			assert( ptr != nullptr );
			if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
				ptr->applyJsonMessageWithUpdates( parser );
			else if constexpr ( ParserT::proto == globalmq::marshalling::Proto::GMQ )
				ptr->applyGmqMessageWithUpdates( parser );
			else
				ptr->publishableApplyUpdates( parser );
		}
	};

	class Connections
	{
		static constexpr uint64_t invalidValue = 0xFFFFFFFFFFFFFFFFULL;
		struct Connection
		{
			enum Status {uninitialized, connRequestSent, connected };
			Status status = Status::uninitialized;

			// conn initiator info
			uint64_t ref_id_at_conn_initiator = invalidValue;
			uint64_t ref_id_at_conn_acceptor = invalidValue;
			uint64_t idInQueueForInitiator = 0; // reported to initiator
			uint64_t idInQueueForAcceptor = 0; // reported to aceptor

			SlotIdx initiatorSlotIdx;
			SlotIdx acceptorSlotIdx;

			GMQ_COLL string address;
		};

		GMQ_COLL unordered_map<uint64_t, Connection> idToConnectionStorage; // used as storage, mx-protected
		GMQ_COLL unordered_map<uint64_t, Connection*> idToConnection; // used for searching, mx-protected
		uint64_t connectionIDBase = 0;

	public:
		Connections() {}
		Connections( const Connections& ) = delete;
		Connections& operator = ( const Connections& ) = delete;
		Connections( Connections&& ) = delete;
		Connections& operator = ( Connections&& ) = delete;
		~Connections() {}

		struct FieldsForSending
		{
			uint64_t idAtSource = invalidValue;
			uint64_t idAtTarget = invalidValue;
			SlotIdx targetSlotIdx;
		};

		FieldsForSending onConnRequest( uint64_t ref_id_at_conn_initiator, SlotIdx initiatorSlotIdx, SlotIdx acceptorSlotIdx, GMQ_COLL string address )
		{
			Connection conn;
			conn.status = Connection::Status::connRequestSent;
			conn.ref_id_at_conn_initiator = ref_id_at_conn_initiator;
			conn.initiatorSlotIdx = initiatorSlotIdx;
			conn.acceptorSlotIdx = acceptorSlotIdx;
			conn.address = address;
			conn.idInQueueForAcceptor = ++connectionIDBase;
			auto ins = idToConnectionStorage.insert( std::make_pair( conn.idInQueueForAcceptor, conn ) );
			assert( ins.second );
			auto ins2 = idToConnection.insert( std::make_pair( conn.idInQueueForAcceptor, &(ins.first->second) ) );
			assert( ins2.second );
			FieldsForSending ffs;
			ffs.idAtSource = conn.idInQueueForAcceptor;
			ffs.idAtTarget = 0; // TODO: invalid value
			ffs.targetSlotIdx = acceptorSlotIdx;
			return ffs;
		}
		
		FieldsForSending onConnAccepted( uint64_t connID, uint64_t ref_id_at_conn_acceptor, SlotIdx acceptorSlotIdx )
		{
			auto f = idToConnection.find( connID );
			if ( f == idToConnection.end() )
				throw std::exception();
			auto& conn = *(f->second);
			assert( connID == conn.idInQueueForAcceptor ); // self-consistency
			conn.status = Connection::Status::connRequestSent;
			if ( conn.status != Connection::Status::connRequestSent )
				throw std::exception();
			conn.status = Connection::Status::connected;
			if ( conn.acceptorSlotIdx != acceptorSlotIdx )
				throw std::exception();
			conn.ref_id_at_conn_acceptor = ref_id_at_conn_acceptor;
			conn.idInQueueForInitiator = ++connectionIDBase;
			auto ins = idToConnection.insert( std::make_pair( conn.idInQueueForInitiator, &(*(f->second)) ) );
			assert( ins.second );
			FieldsForSending ffs;
			ffs.idAtSource = conn.idInQueueForInitiator;
			ffs.idAtTarget = conn.ref_id_at_conn_initiator;
			ffs.targetSlotIdx = conn.initiatorSlotIdx;
			return ffs;
		}
		FieldsForSending onConnMsg( uint64_t connID, uint64_t refIdAtCaller, SlotIdx callerSlotIdx )
		{
			auto f = idToConnection.find( connID );
			if ( f == idToConnection.end() )
				throw std::exception();
			auto& conn = *(f->second);
			assert( connID == conn.idInQueueForInitiator || connID == conn.idInQueueForAcceptor );
			if ( conn.status != Connection::Status::connected )
				throw std::exception();
			FieldsForSending ffs;
			if ( connID == conn.idInQueueForAcceptor ) // from acceptor to initiator
			{
				if ( refIdAtCaller != conn.ref_id_at_conn_acceptor )
					throw std::exception();
				if ( callerSlotIdx != conn.acceptorSlotIdx )
					throw std::exception();
				ffs.idAtSource = conn.idInQueueForInitiator;
				ffs.idAtTarget = conn.ref_id_at_conn_initiator;
				ffs.targetSlotIdx = conn.initiatorSlotIdx;
			}
			else
			{
				if ( refIdAtCaller != conn.ref_id_at_conn_initiator )
					throw std::exception();
				if ( callerSlotIdx != conn.initiatorSlotIdx )
					throw std::exception();
				ffs.idAtSource = conn.idInQueueForAcceptor;
				ffs.idAtTarget = conn.ref_id_at_conn_acceptor;
				ffs.targetSlotIdx = conn.acceptorSlotIdx;
			}
			return ffs;
		}
	};
	Connections connections;

	std::mutex mx;


	GMQ_COLL unordered_map<GMQ_COLL string, ConcentratorWrapper> addressesToStateConcentrators; // address to concentrator mapping, 1 - 1, mx-protected
	GMQ_COLL unordered_map<uint64_t, ConcentratorWrapper*> idToStateConcentrators; // id to concentrator mapping, many - 1, mx-protected
	uint64_t concentratorIDBase = 0;

//	GMQ_COLL unordered_map<GMQ_COLL string, AddressableLocation> namedRecipients; // node name to location, mx-protected
	GMQ_COLL unordered_map<GMQ_COLL string, SlotIdx> namedRecipients; // node name to location, mx-protected

	GMQ_COLL unordered_map<uint64_t, SlotIdx> senders; // node name to location, mx-protected
	uint64_t senderIDBase = 0;

	GMQ_COLL unordered_map<uint64_t, std::pair<uint64_t, uint64_t>> ID2ConcentratorSubscriberPairMapping;
	uint64_t publisherAndItsConcentratorBase = 0;

	StateConcentratorFactoryBase<InputBufferT, ComposerT>* stateConcentratorFactory = nullptr;

	void addConcentratorSubscriberPair( uint64_t id, uint64_t concentratorID, uint64_t subscriberDataID ) {
		auto ins = ID2ConcentratorSubscriberPairMapping.insert( std::make_pair( id, std::make_pair( concentratorID, subscriberDataID ) ) );
		assert( ins.second );
	}
	void removeConcentratorSubscriberPair( uint64_t ID ) {
		ID2ConcentratorSubscriberPairMapping.erase( ID );
	}
	std::pair<uint64_t, uint64_t> findConcentratorSubscriberPair( uint64_t ID ) {
		auto f = ID2ConcentratorSubscriberPairMapping.find( ID );
		if ( f != ID2ConcentratorSubscriberPairMapping.end() )
			return f->second;
		else
			throw std::exception();
	}

	// concentrators (address2concentrators)
	std::pair<ConcentratorWrapper*, bool> findOrAddStateConcentrator( GMQ_COLL string path, uint64_t stateTypeID ) {
		assert( !path.empty() );
		auto f = addressesToStateConcentrators.find( path );
		if ( f != addressesToStateConcentrators.end() )
			return std::make_pair(&(f->second), true);
		else
		{
			assert( !path.empty() );
			assert( stateConcentratorFactory != nullptr );
			auto concentrator = stateConcentratorFactory->createConcentrator( stateTypeID );
			assert( concentrator != nullptr );
			ConcentratorWrapper cwrapper( concentrator );
			auto ins = addressesToStateConcentrators.insert( std::make_pair( path, std::move( cwrapper ) ) );
			assert( ins.second );
			ConcentratorWrapper* c = &(ins.first->second);
			uint64_t concentratorID = ++concentratorIDBase;
			auto ins1 = idToStateConcentrators.insert( std::make_pair( concentratorID, c ) );
			assert( ins1.second );
			c->address = path;
			c->idInQueue = concentratorID;
			return std::make_pair(c, false);
		}
	}
	ConcentratorWrapper* findStateConcentrator( uint64_t id ) {
		assert( id != 0 );
		auto f = idToStateConcentrators.find( id );
		if ( f != idToStateConcentrators.end() )
			return f->second;
		else
			return nullptr;
	}
	/*void removeStateConcentrator( GMQ_COLL string path ) { // TODO: rework
		assert( !path.empty() );
		std::unique_lock<std::mutex> lock(mxAddressesToStateConcentrators);
		addressesToStateConcentrators.erase( path );
		idToStateConcentrators.erase( id );
	}*/

	// named local objects (namedRecipients)
	void addNamedLocation( GMQ_COLL string name, SlotIdx idx ) {
		assert( !name.empty() );
		auto ins = namedRecipients.insert( std::make_pair( name, idx ) );
		assert( ins.second );
	}
	void removeNamedLocation( GMQ_COLL string name ) {
		assert( !name.empty() );
		namedRecipients.erase( name );
	}

	public:
	SlotIdx locationNameToSlotIdx( GMQ_COLL string name ) {
		assert( !name.empty() );
		auto f = namedRecipients.find( name );
		if ( f != namedRecipients.end() )
			return f->second;
		else
			return SlotIdx();
	}

	private:
	uint64_t addSender( SlotIdx idx ) {
		uint64_t id = ++senderIDBase;
		auto ins = senders.insert( std::make_pair( id, idx ) );
		assert( ins.second );
		return id;
	}
	void removeSender( uint64_t id, SlotIdx idx ) {
		auto f = senders.find( id );
		assert( f != senders.end() );
		assert( f->second.idx == idx.idx );
		assert( f->second.reincarnation == idx.reincarnation );
		senders.erase( id );
	}
	public:
	SlotIdx senderIDToSlotIdx( uint64_t id ) {
		auto f = senders.find( id );
		if ( f != senders.end() )
			return f->second;
		else
			return SlotIdx();
	}

public:
	GMQueue() {}
	~GMQueue()
	{ 
		std::unique_lock<std::mutex> lock(mx);

		if ( stateConcentratorFactory != nullptr )
			delete stateConcentratorFactory;
	}

	template<class StateFactoryT>
	void initStateConcentratorFactory() // Note: potentially, temporary solution
	{
		std::unique_lock<std::mutex> lock(mx);

		assert( stateConcentratorFactory == nullptr ); // must be called just once
		stateConcentratorFactory = new StateFactoryT;
	}
	void setAuthority( GMQ_COLL string authority )
	{ 
		std::unique_lock<std::mutex> lock(mx);

		assert( myAuthority.empty() ); // set just once
		myAuthority = authority;
	}

	void postMessage( MessageBufferT&& msg, uint64_t senderID, SlotIdx senderSlotIdx )
	{
		PublishableStateMessageHeader mh;
		auto riter = msg.getReadIter();
		ParserT parser( riter );
		helperParsePublishableStateMessageBegin( parser, mh );

		std::unique_lock<std::mutex> lock(mx);

		SlotIdx senderIdx = senderIDToSlotIdx( senderID );
		assert( senderIdx.idx == senderSlotIdx.idx );
		assert( senderIdx.reincarnation == senderSlotIdx.reincarnation );

		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::subscriptionRequest:
			{
				GmqPathHelper::PathComponents pc;
				pc.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
				bool pathOK = GmqPathHelper::parse( mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)

				GMQ_COLL string addr = GmqPathHelper::localPart( pc );
				if ( isMyAuthority( pc.authority ) ) // local
				{
					assert( !pc.nodeName.empty() );

					auto findCr = findOrAddStateConcentrator( addr, mh.state_type_id_or_direction );
					ConcentratorWrapper* concentrator = findCr.first;
					assert( concentrator != nullptr );

					typename ConcentratorWrapper::SubscriberData sd;
					sd.ref_id_at_subscriber = mh.ref_id_at_subscriber;
					sd.ref_id_at_publisher = ++publisherAndItsConcentratorBase;
					sd.senderSlotIdx = senderSlotIdx;
					uint64_t sid = concentrator->addSubscriber( sd );
					addConcentratorSubscriberPair( sd.ref_id_at_publisher, sid, concentrator->idInQueue );

					if ( findCr.second )
					{
						if ( concentrator->isSsubscriptionResponseReceived() )
						{
							PublishableStateMessageHeader hdrBack;
							hdrBack.type = PublishableStateMessageHeader::MsgType::subscriptionResponse;
							hdrBack.priority = mh.priority;
							hdrBack.ref_id_at_subscriber = mh.ref_id_at_subscriber;
							hdrBack.ref_id_at_publisher = sd.ref_id_at_publisher;

							typename ComposerT::BufferType msgBack;
							ComposerT composer( msgBack );
							helperComposePublishableStateMessageBegin( composer, hdrBack );
							concentrator->generateStateSyncMessage( composer );
							helperComposePublishableStateMessageEnd( composer );

							InProcessMessagePostmanBase* postman = addressableLocations.getPostman( senderSlotIdx );
							lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
							postman->postMessage( std::move( msgBack ) );
						}
					}
					else
					{
						// TODO: revise!
						SlotIdx targetIdx = locationNameToSlotIdx( pc.nodeName );
						if ( targetIdx.idx == SlotIdx::invalid_idx )
							throw std::exception(); // TODO: post permanent error message to sender instead or in addition; remove concentrator

						globalmq::marshalling::PublishableStateMessageHeader::UpdatedData ud;
						ud.ref_id_at_subscriber = concentrator->idInQueue;
						ud.update_ref_id_at_subscriber = true;

						typename ComposerT::BufferType msgForward;
						helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( msg, msgForward, ud );

						InProcessMessagePostmanBase* postman = addressableLocations.getPostman( targetIdx );
						lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
						postman->postMessage( std::move( msgForward ) );
					}
				}
				else
				{
					assert( false ); // not yet implemented (but up to Postman should go along the same lines)
				}
				break;
			}
			case PublishableStateMessageHeader::MsgType::subscriptionResponse:
			{
				ConcentratorWrapper* concentrator = findStateConcentrator( mh.ref_id_at_publisher );
				concentrator->onSubscriptionResponseMessage( parser, mh.ref_id_at_publisher );

				// forward message to all concentrator's subscribers
				PublishableStateMessageHeader::UpdatedData ud;
				ud.update_ref_id_at_publisher = true;
				ud.update_ref_id_at_subscriber = true;
				for ( auto& subscriber : concentrator->subscribers )
				{
					ud.ref_id_at_subscriber = subscriber.ref_id_at_subscriber;
					ud.ref_id_at_publisher = subscriber.ref_id_at_publisher;
					typename ComposerT::BufferType msgForward;
					helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( msg, msgForward, ud );

					InProcessMessagePostmanBase* postman = addressableLocations.getPostman( subscriber.senderSlotIdx );
					lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
					postman->postMessage( std::move( msgForward ) );
					lock.lock();
				}

				break;
			}
			case PublishableStateMessageHeader::MsgType::stateUpdate: // so far we have the same processing
			{
				ConcentratorWrapper* concentrator = findStateConcentrator( mh.ref_id_at_publisher );
				if ( concentrator == nullptr )
					throw std::exception(); // TODO: ?
				concentrator->onStateUpdateMessage( parser );

				// forward message to all concentrator's subscribers
				PublishableStateMessageHeader::UpdatedData ud;
				ud.update_ref_id_at_publisher = true;
				ud.update_ref_id_at_subscriber = true;
				for ( auto& subscriber : concentrator->subscribers )
				{
					ud.ref_id_at_subscriber = subscriber.ref_id_at_subscriber;
					ud.ref_id_at_publisher = subscriber.ref_id_at_publisher;
					typename ComposerT::BufferType msgForward;
					helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( msg, msgForward, ud );

					InProcessMessagePostmanBase* postman = addressableLocations.getPostman( subscriber.senderSlotIdx );
					lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
					postman->postMessage( std::move( msgForward ) );
					lock.lock();
				}

				break;
			}
			case PublishableStateMessageHeader::MsgType::connectionRequest:
			{
				GmqPathHelper::PathComponents pc;
				pc.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
				bool pathOK = GmqPathHelper::parse( mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)

				GMQ_COLL string addr = GmqPathHelper::localPart( pc );
				if ( isMyAuthority( pc.authority ) ) // local
				{
					assert( !pc.nodeName.empty() );

					SlotIdx targetIdx = locationNameToSlotIdx( pc.nodeName );
					if ( targetIdx.idx == SlotIdx::invalid_idx )
						throw std::exception(); // TODO: post permanent error message to sender instead or in addition

					typename Connections::FieldsForSending fields = connections.onConnRequest( mh.ref_id_at_subscriber, senderSlotIdx, targetIdx, mh.path );
					globalmq::marshalling::PublishableStateMessageHeader::UpdatedData ud;
					ud.ref_id_at_subscriber = fields.idAtSource;
					ud.update_ref_id_at_subscriber = true;

					typename ComposerT::BufferType msgForward;
					helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( msg, msgForward, ud );

					InProcessMessagePostmanBase* postman = addressableLocations.getPostman( targetIdx );
					lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
					postman->postMessage( std::move( msgForward ) );
				}
				else
				{
					assert( false ); // not yet implemented (but up to Postman should go along the same lines)
				}
				break;
			}
			case PublishableStateMessageHeader::MsgType::connectionAccepted:
			{
				typename Connections::FieldsForSending fields = connections.onConnAccepted( mh.ref_id_at_subscriber, mh.ref_id_at_publisher, senderSlotIdx );

				PublishableStateMessageHeader::UpdatedData ud;
				ud.update_ref_id_at_publisher = true;
				ud.update_ref_id_at_subscriber = true;
				ud.ref_id_at_subscriber = fields.idAtTarget;
				ud.ref_id_at_publisher = fields.idAtSource;

				typename ComposerT::BufferType msgForward;
				helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( msg, msgForward, ud );

				InProcessMessagePostmanBase* postman = addressableLocations.getPostman( fields.targetSlotIdx );
				lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
				postman->postMessage( std::move( msgForward ) );

				break;
			}
			case PublishableStateMessageHeader::MsgType::connectionMessage:
			{
				PublishableStateMessageHeader::UpdatedData ud;
				ud.update_ref_id_at_publisher = true;
				ud.update_ref_id_at_subscriber = true;

				typename Connections::FieldsForSending fields;
				if ( mh.state_type_id_or_direction == PublishableStateMessageHeader::ConnMsgDirection::toServer )
				{
					fields = connections.onConnMsg( mh.ref_id_at_publisher, mh.ref_id_at_subscriber, senderSlotIdx );
					ud.ref_id_at_subscriber = fields.idAtSource;
					ud.ref_id_at_publisher = fields.idAtTarget;
				}
				else
				{
					if ( mh.state_type_id_or_direction != PublishableStateMessageHeader::ConnMsgDirection::toClient )
						throw std::exception();
					fields = connections.onConnMsg( mh.ref_id_at_subscriber, mh.ref_id_at_publisher, senderSlotIdx );
					ud.ref_id_at_subscriber = fields.idAtTarget;
					ud.ref_id_at_publisher = fields.idAtSource;
				}

				typename ComposerT::BufferType msgForward;
				helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( msg, msgForward, ud );

				InProcessMessagePostmanBase* postman = addressableLocations.getPostman( fields.targetSlotIdx );
				lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
				postman->postMessage( std::move( msgForward ) );

				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
	}

	uint64_t add( GMQ_COLL string name, InProcessMessagePostmanBase* postman, SlotIdx& idx )
	{
		assert( !name.empty() );

		std::unique_lock<std::mutex> lock(mx);

		idx = addressableLocations.add( postman );
		addNamedLocation( name, idx );
		return addSender( idx );
	}
	uint64_t add( InProcessMessagePostmanBase* postman, SlotIdx& idx )
	{
		std::unique_lock<std::mutex> lock(mx);

		idx = addressableLocations.add( postman );
		return addSender( idx );
	}
	void remove( GMQ_COLL string name, SlotIdx idx )
	{
		std::unique_lock<std::mutex> lock(mx);

		if ( !name.empty() )
			removeNamedLocation( name );
		addressableLocations.remove( idx );
		// TODO: revise and finalize implementation
	}

	template<class PostmanT, class ... Args>
	static InProcessMessagePostmanBase* allocPostman(Args&& ... args)
	{
		static_assert( alignof( PostmanT ) <= sizeof(::std::max_align_t) );
		PostmanT* ret = static_cast<PostmanT *>( ::malloc( sizeof(PostmanT) ) );
		new (ret) PostmanT( std::forward<Args>( args )...);
		return ret;
	}

};

struct InProcTransferrable
{
	GMQ_COLL string name;
	uint64_t id;
	void serialize( uint8_t* buff, size_t maxSz ) // NOTE: temporary solution
	{
		assert( name.size() + 1 + sizeof( id ) <= maxSz );
		memcpy( buff, &id, sizeof( id ) );
		memcpy( buff + sizeof( id ), name.c_str(), name.size() );
		buff[ sizeof( id ) + name.size() ] = 0;
	}
	void deserilaize( uint8_t* buff ) // NOTE: temporary solution
	{
		memcpy( &id, buff, sizeof( id ) );
		name = (char*)(buff + sizeof( id ) );
	}
};

template<class PlatformSupportT>
class GMQTransportBase
{
protected:
	GMQueue<PlatformSupportT>& gmq;
	GMQ_COLL string name;
	SlotIdx idx;
	uint64_t id;
	GMQTransportBase( GMQueue<PlatformSupportT>& queue, GMQ_COLL string name_, SlotIdx idx_, uint64_t id_ ) : gmq( queue ), name( name_ ), idx( idx_ ), id ( id_ ) {}

public:
	GMQTransportBase( GMQueue<PlatformSupportT>& queue ) : gmq( queue ) {}
	virtual ~GMQTransportBase() {
		if ( idx.isInitialized() )
			gmq.remove( name, idx );
	}
protected:
	GMQTransportBase( GMQueue<PlatformSupportT>& queue, GMQ_COLL string name_, InProcessMessagePostmanBase* postman ) : gmq( queue ), name( name_ ) {
		assert( !name_.empty() );
		id = gmq.add( name, postman, idx );
	};
	GMQTransportBase( GMQueue<PlatformSupportT>& queue, InProcessMessagePostmanBase* postman ) : gmq( queue ) {
		id = gmq.add( postman, idx );
	};

public:
	void postMessage( MessageBufferT&& msg ){
		assert( idx.isInitialized() );
		gmq.postMessage( std::move( msg ), id, idx );
	}

public:
	InProcTransferrable makeTransferrable()
	{
		InProcTransferrable ret;
		ret.name = name;
		ret.id = id;
		idx.invalidate();
		return ret;
	}

	void restore( const InProcTransferrable& t, GMQueue<PlatformSupportT>& queue_ ) {
		assert( &gmq == &queue_ );
		assert( !idx.isInitialized() );
		if ( t.name.empty() )
		{
			idx = gmq.senderIDToSlotIdx( t.id );
			assert( idx.isInitialized() );
			id = t.id;
		}
		else
		{
			idx = gmq.locationNameToSlotIdx( t.name );
			assert( idx.isInitialized() );
			SlotIdx idx2 = gmq.senderIDToSlotIdx( t.id );
			assert( idx2.isInitialized() );
			assert( idx == idx2 );
			name = t.name;
			id = t.id;
		}
	}
};



} // namespace globalmq::marshalling

#endif // GMQUEUE_H
