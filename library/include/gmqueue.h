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

#ifndef GMQUEUE_CUSTOMIZED_Q_TYPES
class GMQueueStatePublisherSubscriberTypeInfo
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
#else
#include GMQUEUE_CUSTOMIZED_Q_TYPES
#endif


namespace globalmq::marshalling {

using MessageBufferT = typename GMQueueStatePublisherSubscriberTypeInfo::BufferT;

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
};

class AddressableLocations // one per process; provides process-unique Slot with Postman and returns its SlotIdx
{
	std::mutex mx;
	GMQ_COLL vector<AddressableLocation> slots; // mx-protected!
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

	class ConcentratorWrapper
	{
		template<class PlatformSupportT>
		friend class GMQueue;

		StateConcentratorBase<InputBufferT, ComposerT>* ptr = nullptr;
		bool subscriptionResponseReceived = false;
		uint64_t idAtPublisher;

	public:
		struct SubscriberData
		{
			uint64_t ref_id_at_subscriber;
			uint64_t ref_id_at_publisher;
			SlotIdx senderSlotIdx;
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
			return *(f->second);
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
			auto ins = addressesToStateConcentrators.insert( std::make_pair( path, std::move( concentrator ) ) );
			assert( ins.second );
			ConcentratorWrapper* c = &(ins.first->second);
			uint64_t concentratorID = ++concentratorIDBase;
			auto ins1 = idToStateConcentrators.insert( std::make_pair( concentratorID, c ) );
			assert( ins1.second );
			c->address = path;
			c->id = concentratorID;
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
	SlotIdx locationNameToSlotIdx( GMQ_COLL string name ) {
		assert( !name.empty() );
		auto f = namedRecipients.find( name );
		if ( f != namedRecipients.end() )
			return f->second;
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
		std::unique_lock<std::mutex> lock(mx);

		SlotIdx senderIdx = senderIDToSlotIdx( senderID );
		assert( senderIdx.idx == senderSlotIdx.idx );
		assert( senderIdx.reincarnation == senderSlotIdx.reincarnation );
		PublishableStateMessageHeader mh;
		ParserT parser( msg );
		helperParsePublishableStateMessageBegin( parser, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::subscriptionRequest:
			{
				GmqPathHelper::PathComponents pc;
				bool pathOK = GmqPathHelper::parse( mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)

				GMQ_COLL string addr = GmqPathHelper::localPart( pc );
				if ( isMyAuthority( pc.authority ) ) // local
				{
					assert( !pc.nodeName.empty() );

					auto findCr = findOrAddStateConcentrator( addr, mh.state_type_id );
					ConcentratorWrapper* concentrator = findCr.first;
					assert( concentrator != nullptr );

					typename ConcentratorWrapper::SubscriberData sd;
					sd.ref_id_at_subscriber = mh.ref_id_at_subscriber;
					sd.ref_id_at_publisher = ++publisherAndItsConcentratorBase;
					sd.senderSlotIdx = senderSlotIdx;
					uint64_t sid = concentrator->addSubscriber( sd );
					addConcentratorSubscriberPair( sd.ref_id_at_publisher, sid, concentrator->id );

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

							InProcessMessagePostmanBase* postman = getAddressableLocations().getPostman( senderSlotIdx );
							postman->postMessage( std::move( msgBack ) );
						}
					}
					else
					{
						// TODO: revise!
//						concentrator->addSubscriber( rpi );
						SlotIdx targetIdx = locationNameToSlotIdx( pc.nodeName );
						if ( targetIdx.idx != SlotIdx::invalid_idx )
							throw std::exception(); // TODO: post permanent error message to sender instead or in addition

						globalmq::marshalling::PublishableStateMessageHeader::UpdatedData ud;
						ud.ref_id_at_subscriber = concentrator->id;
						ud.update_ref_id_at_subscriber = true;

						typename ComposerT::BufferType msgForward;
						helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( msg, msgForward, ud );

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

					InProcessMessagePostmanBase* postman = getAddressableLocations().getPostman( subscriber.senderSlotIdx );
					postman->postMessage( std::move( msgForward ) );
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

					InProcessMessagePostmanBase* postman = getAddressableLocations().getPostman( subscriber.senderSlotIdx );
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

	void postMessage( MessageBufferT&& msg ){
		gmq.postMessage( std::move( msg ), id, idx );
	}
};



} // namespace globalmq::marshalling

#endif // GMQUEUE_H
