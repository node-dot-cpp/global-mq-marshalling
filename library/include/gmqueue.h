/* -------------------------------------------------------------------------------
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
#include "publishable_impl.h"


namespace globalmq::marshalling {

struct GmqPathHelper
{
	struct PathComponents
	{
		GMQ_COLL string authority;
		bool furtherResolution = false;
		bool hasPort = false;
		uint16_t port = 0xFFFF;
		GMQ_COLL string localPart; // TODO: revise
		GMQ_COLL string nodeName;
		GMQ_COLL string statePublisherName;
	};

	static GMQ_COLL string compose( GMQ_COLL string authority, GMQ_COLL string nodeName, GMQ_COLL string statePublisherName )
	{
		// TODO: check components
		GMQ_COLL string ret = "globalmq:";
		if ( !authority.empty() )
		{
			ret += "//";
			ret += authority;
		}
		assert( !nodeName.empty() );
		assert( !statePublisherName.empty() );
		ret += '/';
		ret += localPart( nodeName, statePublisherName );
		return ret;
	}

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
		assert( !components.statePublisherName.empty() );
		ret += '/';
		ret += localPart( components.nodeName, components.statePublisherName );
		return ret;
	}

	static GMQ_COLL string localPart( GMQ_COLL string nodeName, GMQ_COLL string statePublisherName )
	{
		return fmt::format( "{}?sp={}", nodeName, statePublisherName );
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
		if ( path[pos++] == '/' ) // double-slash, authority component is present
		{
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
				if ( end - components.authority.c_str() < components.authority.size() ) // there are remaining chars
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

		// statePublisherName
		pos = path.find( "sp=", pos );
		if ( pos == GMQ_COLL string::npos )
			return false;
		pos += sizeof( "sp=" ) - 1;
		pos1 = path.find( '&', pos );
		if ( pos1 == GMQ_COLL string::npos )
			components.statePublisherName = path.substr( pos );
		else
			components.statePublisherName = path.substr( pos, pos1 - pos );
		return true;
	}
};

struct MessageHeader
{
	enum MsgType { undefined = 0, subscriptionRequest = 1, subscriptionResponse = 2, stateUpdate = 3 };
	MsgType type;
	uint64_t state_type_id; // Note: may be removed in future versions
	uint64_t priority;
	GMQ_COLL string path;  // subscriptionRequest only
	uint64_t ref_id_at_subscriber; // updatable
	uint64_t ref_id_at_publisher; // updatable

	struct UpdatedData
	{
		uint64_t ref_id_at_subscriber;
		uint64_t ref_id_at_publisher;
		bool update_ref_id_at_subscriber = false;
		bool update_ref_id_at_publisher = false;
	};

	template<class ParserT>
	void parse( ParserT& parser )
	{
		globalmq::marshalling::impl::parsePublishableStructBegin( parser, "hdr" );
		size_t msgType;
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &state_type_id, "state_type_id" );
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &priority, "priority" );
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			{
				type = MsgType::subscriptionRequest;
				globalmq::marshalling::impl::publishableParseString<ParserT, GMQ_COLL string>( parser, &path, "path" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &reply_back_id, "reply_back_id" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				break;
			}
			case MsgType::subscriptionResponse:
			{
				type = MsgType::subscriptionResponse;
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &reply_back_id, "reply_back_id" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_publisher, "ref_id_at_publisher" );
				break;
			}
			case MsgType::stateUpdate:
			{
				type = MsgType::stateUpdate;
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &reply_back_id, "reply_back_id" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_publisher, "ref_id_at_publisher" );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		globalmq::marshalling::impl::parseStructEnd( parser );
	}

	template<class ComposerT>
	void compose(ComposerT& composer)
	{
		globalmq::marshalling::impl::composeKey( composer, "hdr" );
		globalmq::marshalling::impl::composeStructBegin( composer );
		globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, (uint32_t)(type), "msg_type", true );
		globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, state_type_id, "state_type_id", true );
		globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, priority, "priority", true );
		switch ( type )
		{
			case MsgType::subscriptionRequest:
			{
				globalmq::marshalling::impl::publishableStructComposeString( composer, path, "path", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, reply_back_id, "reply_back_id", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, ref_id_at_subscriber, "ref_id_at_subscriber", true );
				break;
			}
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
			{
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, reply_back_id, "reply_back_id", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, ref_id_at_subscriber, "ref_id_at_subscriber", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, ref_id_at_publisher, "ref_id_at_publisher", true );
				break;
			}
		}
		globalmq::marshalling::impl::composeStructEnd( composer );
	}
};

using InterThreadMsg = Buffer;

class InProcessMessagePostmanBase
{
public:
	InProcessMessagePostmanBase() {};
	virtual void postMessage( InterThreadMsg&& ) = 0;
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
};

class AddressableLocations // one per process; provides process-unique Slot with Postman and returns its SlotIdx
{
	std::mutex mx;
	std::vector<AddressableLocation> slots; // mx-protected!
public:
	SlotIdx add( InProcessMessagePostmanBase* postman )
	{ 
		std::unique_lock<std::mutex> lock(mx);
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
		std::unique_lock<std::mutex> lock(mx);
		// find by idx.idx, check reincarnaion, set postman to null
		assert ( idx.idx < slots.size() ); 
		assert ( idx.reincarnation == slots[idx.idx].reincarnation ); 
		slots[idx.idx].postman = nullptr;
	}
	InProcessMessagePostmanBase* getPostman( SlotIdx idx )
	{
		std::unique_lock<std::mutex> lock(mx);
		// access, verify, return
		assert ( idx.idx < slots.size() ); 
		assert ( idx.reincarnation == slots[idx.idx].reincarnation ); 
		return slots[idx.idx].postman;
	}
};

extern AddressableLocations& getAddressableLocations();

template<class PlatformSupportT>
class GMQueue
{
	using InputBufferT = typename PlatformSupportT::BufferT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using ParserT = typename PlatformSupportT::ParserT;

	GMQ_COLL string myAuthority;
	bool isMyAuthority( GMQ_COLL string authority )
	{
		// TODO: regexp comparison (note: myAuthority could be '*', etc );
		return authority == myAuthority || authority.empty();
	}

	struct ReplyProcessingInfo
	{
		enum Type { undefined, local, external };
		Type type = undefined;
		uint64_t reply_back_id;
		uint64_t ref_id_at_subscriber;
		uint64_t ref_id_at_publisher;
		uint64_t concentratorID; // localConcentrator
		// TODO: for externally outgoing: generalized socket; etc
	};

	class ConcentratorWrapper
	{
		StateConcentratorBase<InputBufferT, ComposerT>* ptr = nullptr;

		struct SubscriberData
		{
			ReplyProcessingInfo info; // TODO: consider having postingInstruction itself here to avoid extra searches
		};
		GMQ_COLL vector<SubscriberData> subscribers;

	public:
		ConcentratorWrapper( StateConcentratorBase<InputBufferT, ComposerT>* ptr_ ) : ptr( ptr_ ) {}
		ConcentratorWrapper( const ConcentratorWrapper& ) = delete;
		ConcentratorWrapper& operator = ( const ConcentratorWrapper& ) = delete;
		ConcentratorWrapper( ConcentratorWrapper&& other ) { ptr = other.ptr; other.ptr = nullptr; }
		ConcentratorWrapper& operator = ( ConcentratorWrapper&& other ) { ptr = other.ptr; other.ptr = nullptr; return *this; }
		~ConcentratorWrapper() { if ( ptr ) delete ptr; }
		StateConcentratorBase<InputBufferT, ComposerT>* operator -> () { return ptr; }

		// Gmqueue part (indeed, we need it only if 'remove concentrator' event may actually happen (conditions?))
		GMQ_COLL string address;
		uint64_t id;

		void addSubscriber( ReplyProcessingInfo info )
		{
			SubscriberData sd;
			sd.info = info;
			subscribers.push_back( sd );
		}

		InterThreadMsg&& generateSubscriptionResponseMessage()
		{
			InterThreadMsg msg;
			// TODO: generate message
			return msg;
		}
	};

	std::mutex mxAddressesToStateConcentrators;
	std::unordered_map<GMQ_COLL string, ConcentratorWrapper> addressesToStateConcentrators; // address to concentrator mapping, 1 - 1, mx-protected
	uint64_t concentratorIDBase = 0;

	std::mutex mxIdToStateConcentrators;
	std::unordered_map<uint64_t, ConcentratorWrapper*> idToStateConcentrators; // id to concentrator mapping, many - 1, mx-protected

	std::mutex mxNamedRecipients;
	std::unordered_map<GMQ_COLL string, AddressableLocation> namedRecipients; // node name to location, mx-protected

	std::mutex mxSenders;
	std::unordered_map<uint64_t, SlotIdx> senders; // node name to location, mx-protected
	uint64_t senderIDBase = 0;

	StateConcentratorFactoryBase<InputBufferT, ComposerT>* stateConcentratorFactory = nullptr;

	auto createConcentrator( GMQ_COLL string path, uint64_t stateTypeID )
	{
		assert( !path.empty() );
		assert( stateConcentratorFactory != nullptr );
		auto concentrator = stateConcentratorFactory->createConcentrator( stateTypeID );
		assert( concentrator != nullptr );
		ConcentratorWrapper cwrapper( concentrator );
		ConcentratorWrapper* c = addStateConcentrator( path, std::move( concentrator ) );
		uint64_t concentratorID = ++concentratorIDBase;
		addIdToStateConcentratorsMapping( concentratorID, c );
		c->address = path;
		c->id = concentratorID;
		return std::make_pair<c, concentratorID>;
	}

	// concentrators (address2concentrators)
	ConcentratorWrapper* addStateConcentrator( GMQ_COLL string path, ConcentratorWrapper&& concentrator ) {
		assert( !path.empty() );
		std::unique_lock<std::mutex> lock(mxAddressesToStateConcentrators);
		auto ins = addressesToStateConcentrators.insert( std::make_pair( path, std::move( concentrator ) ) );
		assert( ins.second );
		return &(ins.first->second);
	}
	void removeStateConcentrator( GMQ_COLL string path ) {
		assert( !path.empty() );
		std::unique_lock<std::mutex> lock(mxAddressesToStateConcentrators);
		addressesToStateConcentrators.erase( path );
	}
	ConcentratorWrapper* findStateConcentrator( GMQ_COLL string path ) {
		assert( !path.empty() );
		std::unique_lock<std::mutex> lock(mxAddressesToStateConcentrators);
		auto f = addressesToStateConcentrators.find( path );
		if ( f != addressesToStateConcentrators.end() )
			return &(*(f->second));
		else
			return nullptr;
	}

	// id toconcentrators mapping (idToStateConcentrators)
	void addIdToStateConcentratorsMapping( uint64_t id, ConcentratorWrapper&& concentrator ) {
		assert( id != 0 );
		std::unique_lock<std::mutex> lock(mxIdToStateConcentrators);
		auto ins = idToStateConcentrators.insert( std::make_pair( id, std::move( concentrator ) ) );
		assert( ins.second );
	}
	void removeIdToStateConcentratorsMapping( uint64_t id ) {
		assert( id != 0 );
		std::unique_lock<std::mutex> lock(mxIdToStateConcentrators);
		idToStateConcentrators.erase( id );
	}
	ConcentratorWrapper* findStateConcentrator( uint64_t id ) {
		assert( id != 0 );
		std::unique_lock<std::mutex> lock(mxIdToStateConcentrators);
		auto f = idToStateConcentrators.find( id );
		if ( f != idToStateConcentrators.end() )
			return &(*(f->second));
		else
			return nullptr;
	}

	// named local objects (namedRecipients)
	void addNamedLocation( GMQ_COLL string name, SlotIdx idx ) {
		assert( !name.empty() );
		std::unique_lock<std::mutex> lock(mxNamedRecipients);
		auto ins = namedRecipients.insert( std::make_pair( name, idx ) );
		assert( ins.second );
	}
	void removeNamedLocation( GMQ_COLL string name ) {
		assert( !name.empty() );
		std::unique_lock<std::mutex> lock(mxNamedRecipients);
		namedRecipients.erase( name );
	}
	SlotIdx locationNameToSlotIdx( GMQ_COLL string name ) {
		assert( !name.empty() );
		std::unique_lock<std::mutex> lock(mxNamedRecipients);
		auto f = namedRecipients.find( name );
		if ( f != namedRecipients.end() )
			return *(f->second);
		else
			return SlotIdx();
	}

	uint64_t addSender( SlotIdx idx ) {
		std::unique_lock<std::mutex> lock(mxSenders);
		uint64_t id = ++senderIDBase;
		auto ins = senders.insert( std::make_pair( id, idx ) );
		assert( ins.second );
		return id;
	}
	void removeSender( uint64_t id, SlotIdx idx ) {
		std::unique_lock<std::mutex> lock(mxSenders);
		auto f = senders.find( id );
		assert( f != senders.end() );
		assert( f->second.idx == idx.idx );
		assert( f->second.reincarnation == idx.reincarnation );
		senders.erase( id );
	}
	SlotIdx senderIDToSlotIdx( uint64_t id ) {
		std::unique_lock<std::mutex> lock(mxSenders);
		auto f = senders.find( id );
		if ( f != senders.end() )
			return *(f->second);
		else
			return SlotIdx();
	}

public:
	GMQueue() {}
	~GMQueue() { if ( stateConcentratorFactory != nullptr ) delete stateConcentratorFactory; }

	template<class StateFactoryT>
	void initStateConcentratorFactory() // Note: potentially, temporary solution
	{
		assert( stateConcentratorFactory == nullptr ); // must be called just once
		stateConcentratorFactory = new StateFactoryT;
	}
	void setAuthority( GMQ_COLL string authority )
	{ 
		assert( myAuthority.empty ); // set just once
		myAuthority = authority;
	}

	void postMessage( InterThreadMsg&& msg, uint64_t senderID, SlotIdx senderSlotIdx ){ // called by local objects
		SlotIdx senderIdx = senderIDToSlotIdx( senderID );
		assert( senderIdx.idx == senderSlotIdx.idx );
		assert( senderIdx.reincarnation == senderSlotIdx.reincarnation );
		MessageHeader mh;
		ParserT parser( msg );
		mh.parse( parser );
		switch ( mh.type )
		{
			case MessageHeader::Type::subscriptionRequest:
			{
				GmqPathHelper::PathComponents pc;
				bool pathOK = GmqPathHelper::parse( mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)
				GMQ_COLL string addr = GmqPathHelper::localPart( pc );
				if ( pc.authority.empty() ) // local; TODO: add case when authority is present but points to local machine, too
				{
					assert( !pc.nodeName.empty() );

					ReplyProcessingInfo rpi;
					rpi.type = ReplyProcessingInfo::Type::local;
					rpi.idx = senderSlotIdx;
					rpi.reply_back_id = mh.reply_back_id;
					rpi.ref_id_at_subscriber = mh.ref_id_at_subscriber;
					//rpi.ref_id_at_publisher = mh.ref_id_at_publisher;
//					uint64_t rpiIdx = addReplyProcessingInstructions( rpi );

					ConcentratorWrapper* concentrator = findStateConcentrator( addr );
					if ( concentrator != nullptr )
					{
						concentrator->addSubscriber( rpi );
						// TODO: consider adding message header first yet here, and adding message body at concentrator
						auto msgBack = concentrator->generateSubscriptionResponseMessage();
						// add subscriber, get its subscriptionResponse message, post it back to caller
						InProcessMessagePostmanBase* postman = getAddressableLocations().getPostman( senderSlotIdx );
						postman->postMessage( std::move( msgBack ) );
					}
					else
					{
						auto cci = createConcentrator( addr, mh.state_type_id );
						concentrator->addSubscriber( rpi );
						SlotIdx targetIdx = locationNameToSlotIdx( pc.nodeName );
						assert( targetIdx.idx != SlotIdx::invalid_idx ); // TODO: post permanent error message to sender instead
						// TODO: update message fields
						InProcessMessagePostmanBase* postman = getAddressableLocations().getPostman( targetIdx );
						postman->postMessage( std::move( msg ) );
					}
				}
				else
				{
					assert( false ); // not yet implemented
					// TODO: find concentrator by path; 
					//          - if found, pass request there, get state-sync msg, post back
					//          - if not found, create new concentrator, pass request there, make it generate (or generate on behalf of it) a subscriptionRequest with the same path, send it to a next hop
				}
				break;
			}
			case MessageHeader::Type::subscriptionResponse:
			{
				ConcentratorWrapper* concentrator = findStateConcentrator( mh.ref_id_at_publisher );
				// TODO: apply to concentrator, send to all currently present subscribers
				break;
			}
			case MessageHeader::Type::stateUpdate: // so far we have the same processing
			{
				ConcentratorWrapper* concentrator = findStateConcentrator( mh.ref_id_at_publisher );
				// TODO: apply to concentrator, send to all currently present subscribers
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
	}

	void onMessage( InterThreadMsg&& msg ){ // externally coming
		MessageHeader mh;
		ParserT parser( msg );
		mh.parse( parser );
		switch ( mh.type )
		{
			case MessageHeader::Type::subscriptionRequest:
			{
				GmqPathHelper::PathComponents pc;
				bool pathOK = GmqPathHelper::parse( mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)
				if ( isMyAuthority( pc.authority ) )
				{
					ConcentratorWrapper* concentrator = findStateConcentrator( GmqPathHelper::localPart( pc ) );
					if ( concentrator )
					{
						// TODO: 
						//    - add subscriber
						//    - make Concentrator generate subscriptionResponse
						//    - post it back to Subscriber
					}
					else
					{
						// TODO: 
						//    - check that node exists, if not - post permanent error to Subscriber
						//    - create concentrator
						//    - update msg fields, forward msg to node
					}
				}
				else
				{
					ConcentratorWrapper* concentrator = findStateConcentrator( mh.path );
					if ( concentrator )
					{
						// TODO: 
						//    - add subscriber
						//    - add Subscriber
						//    - make Concentrator generate subscriptionResponse
						//    - post it back to Subscriber
					}
					else
					{
						// TODO: 
						//    - create concentrator
						//    - add Subscriber
						//    - update msg fields, forward msg to node
					}
				}
				break;
			}
			case MessageHeader::Type::stateUpdate: // so far we have the same processing
			case MessageHeader::Type::subscriptionResponse:
			{
				ConcentratorWrapper* concentrator = findStateConcentrator( mh.ref_id_at_subscriber );
				if ( concentrator == nullptr )
					throw std::exception(); // TODO: ... (unknown recipient)
				// TODO: apply message to concentrator; forward it to all concentrator's subscribers
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
	}
};

template<class PlatformSupportT>
class GMQTransportBase
{
protected:
	GMQueue<PlatformSupportT>& gmq;
	SlotIdx idx;
	GMQ_COLL string name;
	uint64_t id;

public:
	GMQTransportBase( GMQueue<PlatformSupportT>& queue, GMQ_COLL string name_, InProcessMessagePostmanBase* postman ) : gmq( queue ), name( name_ ) {
		assert( !name_.empty() );
		idx = getAddressableLocations().add( postman );
		gmq.add( name, idx );
	};
	GMQTransportBase( GMQueue<PlatformSupportT>& queue, InProcessMessagePostmanBase* postman ) : gmq( queue ) {
		idx = getAddressableLocations().add( postman );
		gmq.add( name, idx );
	};
	virtual ~GMQTransportBase() {
		getAddressableLocations().remove( idx );
		gmq.remove( name, idx );
	}

	void postMessage( InterThreadMsg&& msg, GMQ_COLL string address ){
		gmq.postMessage( std::move( msg ), address );
	}
};



} // namespace globalmq::marshalling

#endif // GMQUEUE_H
