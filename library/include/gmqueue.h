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
		ret += '/';
		ret += nodeName;
		assert( !statePublisherName.empty() );
		ret += "?sp=";
		ret += statePublisherName;
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
		ret += '/';
		ret += components.nodeName;
		assert( !components.statePublisherName.empty() );
		ret += "?sp=";
		ret += components.statePublisherName;
		return ret;
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

class InterThreadMsg;

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

	class ConcentratorWrapper
	{
		StateConcentratorBase<InputBufferT, ComposerT>* ptr = nullptr;
	public:
		ConcentratorWrapper( StateConcentratorBase<InputBufferT, ComposerT>* ptr_ ) : ptr( ptr_ ) {}
		ConcentratorWrapper( const ConcentratorWrapper& ) = delete;
		ConcentratorWrapper& operator = ( const ConcentratorWrapper& ) = delete;
		ConcentratorWrapper( ConcentratorWrapper&& other ) { ptr = other.ptr; other.ptr = nullptr; }
		ConcentratorWrapper& operator = ( ConcentratorWrapper&& other ) { ptr = other.ptr; other.ptr = nullptr; return *this; }
		~ConcentratorWrapper() { if ( ptr ) delete ptr; }
		StateConcentratorBase<InputBufferT, ComposerT>* operator -> () { return ptr; }
	};

	struct ReplyProcessingInstructions
	{
		enum Type { undefined, local, localConcentrator };
		Type type = undefined;
		SlotIdx idx; // local
		uint64_t concentratorID; // localConcentrator
		// TODO: for externally outgoing: generalized socket; etc
	};

	std::mutex mx;
	std::unordered_map<GMQ_COLL string, ConcentratorWrapper> concentrators; // address to concentrator, mx-protected
	std::unordered_map<GMQ_COLL string, AddressableLocation> namedRecipients; // node name to location, mx-protected
	std::unordered_map<uint64_t, SlotIdx> senders; // node name to location, mx-protected
	uint64_t senderIDBase = 0;

public:
	GMQueue() {}

	void addNamedLocation( GMQ_COLL string name, SlotIdx idx ) {
		assert( !name.empty() );
		std::unique_lock<std::mutex> lock(mx);
		auto ins = namedRecipients.insert( std::make_pair( name, idx ) );
		assert( ins.second );
	}
	void removeNamedLocation( GMQ_COLL string name ) {
		assert( !name.empty() );
		std::unique_lock<std::mutex> lock(mx);
		namedRecipients.erase( name );
	}
	SlotIdx locationNameToSlotIdx( GMQ_COLL string name ) {
		assert( !name.empty() );
		std::unique_lock<std::mutex> lock(mx);
		auto f = namedRecipients.find( name );
		if ( f != namedRecipients.end() )
			return f->second;
		else
			return SlotIdx();
	}

	uint64_t addSender( SlotIdx idx ) {
		std::unique_lock<std::mutex> lock(mx);
		uint64_t id = ++senderIDBase;
		auto ins = senders.insert( std::make_pair( id, idx ) );
		assert( ins.second );
		return id;
	}
	void removeSender( uint64_t id, SlotIdx idx ) {
		std::unique_lock<std::mutex> lock(mx);
		auto f = senders.find( id );
		assert( f != senders.end() );
		assert( f->second.idx == idx.idx );
		assert( f->second.reincarnation == idx.reincarnation );
		senders.erase( id );
	}
	SlotIdx senderIDToSlotIdx( uint64_t id ) {
		std::unique_lock<std::mutex> lock(mx);
		auto f = senders.find( id );
		if ( f != senders.end() )
			return f->second;
		else
			return SlotIdx();
	}

	void postMessage( InterThreadMsg&& msg, GMQ_COLL string address, uint64_t senderID, SlotIdx senderSlotdx ){
		SlotIdx senderIdx = senderIDToSlotIdx( senderID );
		assert( senderIdx.idx == senderSlotdx.idx );
		assert( senderIdx.reincarnation == senderSlotdx.reincarnation );
		GmqPathHelper::PathComponents pc;
		bool pathOK = GmqPathHelper::parse( address, pc );
		assert( pathOK );
		if ( pc.authority.empty() )
		{
			assert( !pc.nodeName.empty() );
			SlotIdx targetIdx = locationNameToSlotIdx( pc.nodeName );
			assert( targetIdx.idx != SlotIdx::invalid_idx ); // TODO: post permanent error message to sender instead
			InProcessMessagePostmanBase* postman = getAddressableLocations(). getPostman( targetIdx );
			// TODO: add senderID to message
			postman->postMessage( std::move( msg ) );
		}
		else
		{
			assert( false ); // not yet implemented
		}
	}

	void postMessage( InterThreadMsg&& msg, uint64_t recipientID, uint64_t senderID, SlotIdx senderSlotdx ){
		SlotIdx senderIdx = senderIDToSlotIdx( senderID );
		assert( senderIdx.idx == senderSlotdx.idx );
		assert( senderIdx.reincarnation == senderSlotdx.reincarnation );
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
