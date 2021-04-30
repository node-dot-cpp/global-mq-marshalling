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
		uint64_t ref_id_at_subscriber;
		uint64_t ref_id_at_publisher;
		// TODO: for externally outgoing: generalized socket; etc
	};

	class ConcentratorWrapper
	{
		template<class PlatformSupportT>
		friend class GMQueue;

		StateConcentratorBase<InputBufferT, ComposerT>* ptr = nullptr;
		bool subscriptionResponseReceived = false;
		uint64_t idAtPublisher;

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

		// Gmqueue part (indeed, we need it only if 'remove concentrator' event may actually happen (conditions?))
		GMQ_COLL string address;
		uint64_t id;

	public:
		void addSubscriber( ReplyProcessingInfo info )
		{
			SubscriberData sd;
			sd.info = info;
			subscribers.push_back( sd );
		}

		bool isSsubscriptionResponseReceived() { return subscriptionResponseReceived; }

		void generateStateSyncMessage( ComposerT& composer )
		{
			assert( ptr != nullptr );
			assert( subscriptionResponseReceived );
			ptr->generateStateSyncMessage( composer );
		}

		void onSubscriptionResponseMessage( ParserT& parser, uint64_t idAtPublisher_ ) 
		{
			assert( ptr != nullptr );
			assert( subscribers.size() != 0 ); // current implementation does not practically assume removing subscribers 
			if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
				ptr->applyJsonStateSyncMessage( parser );
			else 
			{
				static_assert( ParserT::proto == globalmq::marshalling::Proto::GMQ );
				ptr->applyGmqStateSyncMessage( parser );
			}
			subscriptionResponseReceived = true;
			idAtPublisher = idAtPublisher_;
		}

		void onStateUpdateMessage( ParserT& parser ) 
		{
			assert( ptr != nullptr );
			if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
				ptr->applyJsonMessageWithUpdates( parser );
			else 
			{
				static_assert( ParserT::proto == globalmq::marshalling::Proto::GMQ );
				ptr->applyGmqMessageWithUpdates( parser );
			}
		}
	};

	std::mutex mx;

	std::unordered_map<GMQ_COLL string, ConcentratorWrapper> addressesToStateConcentrators; // address to concentrator mapping, 1 - 1, mx-protected
	std::unordered_map<uint64_t, ConcentratorWrapper*> idToStateConcentrators; // id to concentrator mapping, many - 1, mx-protected
	uint64_t concentratorIDBase = 0;

	std::unordered_map<GMQ_COLL string, AddressableLocation> namedRecipients; // node name to location, mx-protected

	std::unordered_map<uint64_t, SlotIdx> senders; // node name to location, mx-protected
	uint64_t senderIDBase = 0;

	StateConcentratorFactoryBase<InputBufferT, ComposerT>* stateConcentratorFactory = nullptr;

	// concentrators (address2concentrators)
	std::pair<ConcentratorWrapper*, bool> findOrAddStateConcentrator( GMQ_COLL string path, uint64_t stateTypeID ) {
		assert( !path.empty() );
		auto f = addressesToStateConcentrators.find( path );
		if ( f != addressesToStateConcentrators.end() )
			return std::make_pair(&(*(f->second)), true);
		else
		{
			assert( !path.empty() );
			assert( stateConcentratorFactory != nullptr );
			auto concentrator = stateConcentratorFactory->createConcentrator( stateTypeID );
			assert( concentrator != nullptr );
			ConcentratorWrapper cwrapper( concentrator );
			auto ins = addressesToStateConcentrators.insert( std::make_pair( path, std::move( concentrator ) ) );
			assert( ins.second );
			ConcentratorWrapper* c = &(ins.first->second);
			uint64_t concentratorID = ++concentratorIDBase;
			auto ins = idToStateConcentrators.insert( std::make_pair( concentratorID, std::move( concentrator ) ) );
			assert( ins.second );
			c->address = path;
			c->id = concentratorID;
			return std::make_pair<c, concentratorID>;
		}
	}
	ConcentratorWrapper* findStateConcentrator( uint64_t id ) {
		assert( id != 0 );
		auto f = idToStateConcentrators.find( id );
		if ( f != idToStateConcentrators.end() )
			return &(*(f->second));
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
	SlotIdx locationNameToSlotIdx( GMQ_COLL string name ) {
		assert( !name.empty() );
		auto f = namedRecipients.find( name );
		if ( f != namedRecipients.end() )
			return *(f->second);
		else
			return SlotIdx();
	}

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
	SlotIdx senderIDToSlotIdx( uint64_t id ) {
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
		assert( myAuthority.empty() ); // set just once
		myAuthority = authority;
	}

	void postMessage( InterThreadMsg&& msg, uint64_t senderID, SlotIdx senderSlotIdx )
	{
		std::unique_lock<std::mutex> lock(mx);

		SlotIdx senderIdx = senderIDToSlotIdx( senderID );
		assert( senderIdx.idx == senderSlotIdx.idx );
		assert( senderIdx.reincarnation == senderSlotIdx.reincarnation );
		PublishableStateMessageHeader mh;
		ParserT parser( msg );
		helperParsePublishableStateMessageBegin( parser, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::Type::subscriptionRequest:
			{
				GmqPathHelper::PathComponents pc;
				bool pathOK = GmqPathHelper::parse( mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)

				GMQ_COLL string addr = GmqPathHelper::localPart( pc );
				if ( isMyAuthority( pc.authority ) ) // local
				{
					assert( !pc.nodeName.empty() );

					ReplyProcessingInfo rpi;
					rpi.type = ReplyProcessingInfo::Type::local;
					rpi.idx = senderSlotIdx;
					rpi.ref_id_at_subscriber = mh.ref_id_at_subscriber;

					auto findCr = findOrAddStateConcentrator( addr, mh.state_type_id );
					ConcentratorWrapper* concentrator = findCr.first;
					assert( concentrator != nullptr );
					if ( findCr.second )
					{
						concentrator->addSubscriber( rpi );
						if ( concentrator->isSsubscriptionResponseReceived() )
						{
							PublishableStateMessageHeader hdrBack;
							hdrBack.type = PublishableStateMessageHeader::MsgType::subscriptionResponse;
							hdrBack.priority = mh.priority;
							hdrBack.ref_id_at_subscriber = mh.ref_id_at_subscriber;
							hdrBack.ref_id_at_publisher = concentrator->id;

							typename ComposerT::BufferType msgBack;
							ComposerT composer( msgBack );
							globalmq::marshalling::impl::composeStructBegin( composer );
							hdrBack.compose( composer, true );
							globalmq::marshalling::impl::composeKey( composer, "data" );
							concentrator->generateStateSyncMessage( composer );
							globalmq::marshalling::impl::composeStructEnd( composer );

							InProcessMessagePostmanBase* postman = getAddressableLocations().getPostman( senderSlotIdx );
							postman->postMessage( std::move( msgBack ) );
						}
					}
					else
					{
						concentrator->addSubscriber( rpi );
						SlotIdx targetIdx = locationNameToSlotIdx( pc.nodeName );
						if ( targetIdx.idx != SlotIdx::invalid_idx )
							throw std::exception(); // TODO: post permanent error message to sender instead or in addition

						globalmq::marshalling::PublishableStateMessageHeader::UpdatedData ud;
						ud.ref_id_at_subscriber = concentrator->id;
						ud.update_ref_id_at_subscriber = true;

						typename ComposerT::BufferType msgForward;
						ComposerT composer( msgForward );
						ParserT parser( msg );
						PublishableStateMessageHeader::parseAndUpdate<ParserT, ComposerT>( parser, msgForward, ud );

						InProcessMessagePostmanBase* postman = getAddressableLocations().getPostman( targetIdx );
						postman->postMessage( std::move( msgForward ) );
					}
				}
				else
				{
					assert( false ); // not yet implemented (but up to Postman should go along the same lines)
				}
				break;
			}
			case PublishableStateMessageHeader::Type::subscriptionResponse:
			{
				ConcentratorWrapper* concentrator = findStateConcentrator( mh.ref_id_at_publisher );
				concentrator->onSubscriptionResponseMessage( parser, mh.ref_id_at_publisher );

				// forward message to all concentrator's subscribers
				PublishableStateMessageHeader::UpdatedData ud;
				ud.ref_id_at_publisher = concentrator->id; // TODO!!! revise, this is insufficient!
				ud.update_ref_id_at_publisher = true;
				ud.update_ref_id_at_subscriber = true;
				for ( auto& subscriber : concentrator->subscribers )
				{
					ud.ref_id_at_subscriber = subscriber.info.ref_id_at_subscriber;
					typename ComposerT::BufferType msgForward;
					ComposerT composer( msgForward );
					ParserT parser( msg );
					PublishableStateMessageHeader::parseAndUpdate<ParserT, ComposerT>( parser, msgForward, ud );

					InProcessMessagePostmanBase* postman = getAddressableLocations().getPostman( subscriber.info.ref_id_at_subscriber );
					postman->postMessage( std::move( msgForward ) );
				}

				break;
			}
			case PublishableStateMessageHeader::Type::stateUpdate: // so far we have the same processing
			{
				ConcentratorWrapper* concentrator = findStateConcentrator( mh.ref_id_at_publisher );
				concentrator->onStateUpdateMessage( parser );

				// forward message to all concentrator's subscribers
				PublishableStateMessageHeader::UpdatedData ud;
				ud.ref_id_at_publisher = concentrator->id; // TODO!!! revise, this is insufficient!
				ud.update_ref_id_at_publisher = true;
				ud.update_ref_id_at_subscriber = true;
				for ( auto& subscriber : concentrator->subscribers )
				{
					ud.ref_id_at_subscriber = subscriber.info.ref_id_at_subscriber;
					typename ComposerT::BufferType msgForward;
					ComposerT composer( msgForward );
					ParserT parser( msg );
					PublishableStateMessageHeader::parseAndUpdate<ParserT, ComposerT>( parser, msgForward, ud );

					InProcessMessagePostmanBase* postman = getAddressableLocations().getPostman( subscriber.info.ref_id_at_subscriber );
					postman->postMessage( std::move( msgForward ) );
				}

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

	void postMessage( InterThreadMsg&& msg ){
		gmq.postMessage( std::move( msg ), id, idx );
	}
};



} // namespace globalmq::marshalling

#endif // GMQUEUE_H
