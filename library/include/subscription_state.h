/* -------------------------------------------------------------------------------
* Copyright (c) 2020, OLogN Technologies AG
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

#ifndef SUBSCRIPTION_STATE_H
#define SUBSCRIPTION_STATE_H

#include "common.h"

class GmqSubscriptionState
{
public:
	using PositionVectorT = GMQ_COLL vector<size_t>;

private:
	struct SSNode
	{
		using VectorOfNodesT = GMQ_COLL vector<SSNode>;
		using NodeIterT = typename VectorOfNodesT::iterator;
		m::Buffer msgBuff;
		VectorOfNodesT nodes;
		uint64_t lastUpdate;

		SSNode() {}
		SSNode( m::Buffer&& msg, size_t lastUpdate_ ) : msgBuff( std::move( msg ) ), lastUpdate( lastUpdate_ ) {}
		SSNode( const SSNode& ) = delete;
		SSNode& operator = ( const SSNode& ) = delete;
		SSNode( SSNode&& other ) = default;
		SSNode& operator = ( SSNode&& other ) = default;
	};

	SSNode root; 
	uint64_t updateCount = 0;

	SSNode* nodePosToIterator( PositionVectorT nodePos, size_t cnt )
	{
		GMQ_ASSERT( cnt <= nodePos.size() );
		SSNode* pnode = &root;
		size_t i=0;
		for ( ; i<cnt; ++i )
		{
			size_t idx = nodePos[i];
			GMQ_ASSERT( idx < pnode->nodes.size() );
			pnode = &(pnode->nodes[i]);
		}
		return pnode;
	}

public:
	void insertNode( PositionVectorT parentPos, size_t insertBefore, m::Buffer&& msg )
	{
		auto resolved = nodePosToIterator( parentPos, parentPos.size() );
		GMQ_ASSERT( insertBefore <= resolved->nodes.size() );
		resolved->nodes.insert( resolved->nodes.begin() + insertBefore, SSNode( std::move( msg ), updateCount ) );
		++updateCount;
	}
	void removeNode( PositionVectorT nodePos )
	{
		GMQ_ASSERT( nodePos.size() );
		auto resolved = nodePosToIterator( nodePos, nodePos.size() - 1 );
		size_t idx = nodePos.back();
		resolved->nodes.erase( resolved->nodes.begin() + idx );
		resolved->lastUpdate = updateCount;
		++updateCount;
	}
	void updateNode( PositionVectorT nodePos, m::Buffer&& msg )
	{
		auto resolved = nodePosToIterator( nodePos, nodePos.size() );
		resolved->msgBuff = std::move( msg );
		resolved->lastUpdate = updateCount;
		++updateCount;
	}

	void dbgPrintNodeRecursively( SSNode& node, GMQ_COLL string offset, size_t depth, size_t currDepth )
	{
		fmt::print( "{}[{}] \"{}\"\n", offset, node.lastUpdate, GMQ_COLL string_view( reinterpret_cast<char*>(node.msgBuff.begin()), node.msgBuff.size() ) );
		if ( depth == currDepth )
		{
			fmt::print( "{} <more nodes in depth>\n", offset );
			return;
		}
		for ( size_t i=0; i<node.nodes.size(); ++i )
			dbgPrintNodeRecursively( node.nodes[i], offset + "  ", depth, currDepth + 1 );
	}

	void dbgPrintTree()
	{
		dbgPrintNodeRecursively( root, "", 1000, 0 );
	}
};

#endif // SUBSCRIPTION_STATE_H
