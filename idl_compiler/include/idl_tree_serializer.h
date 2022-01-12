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

#ifndef IDL_TREE_SERIALIZER_H
#define IDL_TREE_SERIALIZER_H

#include "idl_tree.h"

struct FloatingParts
{
	int64_t fraction;
	int64_t exponent;
	FloatingParts( double d ) { fromFloating( d ); }
	void fromFloating( double d ) { 
		uint64_t fraction_ = *(uint64_t*)(&d) & 0x800fffffffffffffULL; 
		fraction = *(int64_t*)(&fraction_);
		uint64_t exponent_ = ( *(uint64_t*)(&d) << 1 ) >> 53;
		exponent = *(uint64_t*)(&exponent_) - 1023;
	}
	double value() { 
		int64_t exp_ = exponent + 1023;
		uint64_t res = (*(uint64_t*)(&exp_) << 52) | *(uint64_t*)(&fraction);
		assert( ( *(uint64_t*)(&fraction) & 0x7ff0000000000000 ) == 0 );
		assert( ( exp_ & ~0x7ffLLU ) == 0 );
		return *(double*)(&res);
	}
};

const char* impl_kindToString( MessageParameterType::KIND kind );
bool impl_checkParamNameUniqueness(CompositeType& s);
bool impl_checkFollowingExtensionRules(CompositeType& s);
void impl_CollectParamNamesFromMessage( std::set<string>& params, std::set<string>& caseParams, CompositeType& s );
bool impl_processScopes( Root& r );
void impl_generateScopeHandler( FILE* header, Scope& scope );
void impl_generateScopeComposerForwardDeclaration( FILE* header, Scope& scope );
void impl_generateScopeComposer( FILE* header, Scope& scope );
bool impl_processCompositeTypeNamesInMessagesAndPublishables(Root& s, CompositeType& ct, std::vector<CompositeType*>& stack, bool isCollectionElementType = false );
string paramNameToNameTagType( string name );
void impl_generatePublishableStructForwardDeclaration( FILE* header, Root& root, CompositeType& obj );
void impl_generatePublishableStruct( FILE* header, Root& root, CompositeType& obj );
void impl_GeneratePublishableStructWrapperForwardDeclaration( FILE* header, Root& root, CompositeType& s );
void impl_GeneratePublishableStructWrapper( FILE* header, Root& root, CompositeType& s );
void impl_GeneratePublishableStructWrapper4Set( FILE* header, Root& root, CompositeType& s );
void impl_GeneratePublishableStructWrapper4SetForwardDeclaration( FILE* header, Root& root, CompositeType& s );
void generateNotifierPresenceTesterBlock( FILE* header, Root& root );

//mb exported to be used from C#
void impl_insertScopeList(FILE* header, Root& r);
void impl_generateMessageCommentBlock(FILE* header, CompositeType& s);
void orderStructsByDependency(std::vector<unique_ptr<CompositeType>>& structs, std::vector<CompositeType*>& out);
void impl_generatePublishableCommentBlock(FILE* header, CompositeType& s);
//

inline
std::string impl_discriminatedUnionCaseMemberType( MessageParameter member ) { return fmt::format( "Case_{}_{}_T", member.caseName, member.name ); }

inline
std::string impl_memberOrAccessFunctionName( const MessageParameter& member ) { return fmt::format( member.duCaseParam ? "{}()" : "{}", member.name ); }

inline
std::string impl_templateMemberTypeName( std::string templateParentName, const MessageParameter& member, bool noTypeNameAppending = false ) {
	if ( member.duCaseParam )
		return fmt::format( "{}{}::{}", noTypeNameAppending ? "" : "typename ", templateParentName, impl_discriminatedUnionCaseMemberType( member ) );
	else
		return fmt::format( "decltype({}::{})", templateParentName, member.name );
}

void impl_generateParseFunctionBodyForPublishableStructStateSyncOrMessageInDepth( FILE* header, Root& root, CompositeType& obj );


// printing global_mq tree
void printRoot( Root& s );

void printScope( Scope& s, size_t offset );
void printMessage( CompositeType& s, size_t offset );
void printPublishable( CompositeType& s, size_t offset );
void printStruct( CompositeType& s, size_t offset );
void printDiscriminatedUnionCase( CompositeType& s, size_t offset );

void printMessageParameter( MessageParameter& s, size_t offset );
void printMessageMembers( CompositeType& s, size_t offset );
void printPublishableMembers( CompositeType& s, size_t offset );
void printStructMembers( CompositeType& s, size_t offset );
void printDiscriminatedUnionCases( CompositeType& s, size_t offset );
void printLimit( Limit& s, size_t offset );
void printLocation( Location& s, size_t offset );
void printDataType( MessageParameterType& s, size_t offset );
void print__unique_ptr_DataType( unique_ptr<MessageParameterType>& s, size_t offset );
void print__unique_ptr_Message( unique_ptr<CompositeType>& s, size_t offset );
void print__unique_ptr_Publishable( unique_ptr<CompositeType>& s, size_t offset );
void print__unique_ptr_Struct( unique_ptr<CompositeType>& s, size_t offset );
void print__unique_ptr_MessageParameter( unique_ptr<MessageParameter>& s, size_t offset );
void print__unique_ptr_DiscriminatedUnionCase( unique_ptr<CompositeType>& s, size_t offset );

void printDataType( MessageParameterType& s );
void printLimit( Limit& s );
void printVariant( Variant& s );

// code generation
void preprocessRoot(Root& s);
void generateRoot( const char* fileName, uint32_t fileChecksum, FILE* header, const char* metascope, std::string platformPrefix, std::string classNotifierName, Root& s );
void generateMessage( FILE* header, Root& root, CompositeType& s );
void generatePublishable( FILE* header, Root& root, CompositeType& s, std::string platformPrefix, std::string classNotifierName );
void generateMessageAlias( FILE* header, Root& root, CompositeType& s );
void generateMessageParameter( FILE* header, MessageParameter& s );
void generateMessageMembers( FILE* header, CompositeType& s );
void generateLimit( FILE* header, Limit& s );
void generateLocation( FILE* header, Location& s );
void generateDataType( FILE* header, MessageParameterType& s );
void generate__unique_ptr_DataType( FILE* header, unique_ptr<MessageParameterType>& s );
void generate__unique_ptr_MessageParameter( FILE* header, unique_ptr<MessageParameter>& s );

void generateDataType( FILE* header, MessageParameterType& s );
void generateLimit( FILE* header, Limit& s );
void generateVariant( FILE* header, Variant& s );

uint32_t idlFileChecksum( std::string path );

#endif // IDL_TREE_SERIALIZER_H