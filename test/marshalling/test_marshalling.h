#ifndef _test_marshalling_h_guard
#define _test_marshalling_h_guard

#include <marshalling.h>

namespace m {

using LineMap_Type = NamedParameter<struct LineMap_Struct>;
using ObstacleMap_Type = NamedParameter<struct ObstacleMap_Struct>;
using PolygonMap_Type = NamedParameter<struct PolygonMap_Struct>;
using a_Type = NamedParameter<struct a_Struct>;
using b_Type = NamedParameter<struct b_Struct>;
using concaveMap_Type = NamedParameter<struct concaveMap_Struct>;
using jumpMap_Type = NamedParameter<struct jumpMap_Struct>;
using obstacleMap_Type = NamedParameter<struct obstacleMap_Struct>;
using polygonMap_Type = NamedParameter<struct polygonMap_Struct>;
using polygonSpeed_Type = NamedParameter<struct polygonSpeed_Struct>;
using portalMap_Type = NamedParameter<struct portalMap_Struct>;
using x_Type = NamedParameter<struct x_Struct>;
using y_Type = NamedParameter<struct y_Struct>;
using z_Type = NamedParameter<struct z_Struct>;

constexpr LineMap_Type::TypeConverter LineMap;
constexpr ObstacleMap_Type::TypeConverter ObstacleMap;
constexpr PolygonMap_Type::TypeConverter PolygonMap;
constexpr a_Type::TypeConverter a;
constexpr b_Type::TypeConverter b;
constexpr concaveMap_Type::TypeConverter concaveMap;
constexpr jumpMap_Type::TypeConverter jumpMap;
constexpr obstacleMap_Type::TypeConverter obstacleMap;
constexpr polygonMap_Type::TypeConverter polygonMap;
constexpr polygonSpeed_Type::TypeConverter polygonSpeed;
constexpr portalMap_Type::TypeConverter portalMap;
constexpr x_Type::TypeConverter x;
constexpr y_Type::TypeConverter y;
constexpr z_Type::TypeConverter z;

//**********************************************************************
// CompositeType "PolygonSt" Targets: JSON GMQ (6 parameters)
// 1. VECTOR< MESSAGE PolygonMap> polygonMap (REQUIRED)
// 2. VECTOR< MESSAGE PolygonMap> concaveMap (REQUIRED)
// 3. VECTOR< MESSAGE ObstacleMap> obstacleMap (REQUIRED)
// 4. VECTOR< MESSAGE LineMap> portalMap (REQUIRED)
// 5. VECTOR< MESSAGE LineMap> jumpMap (REQUIRED)
// 6. REAL polygonSpeed (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void PolygonSt_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfMessageType, polygonMap_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::VectorOfMessageType, concaveMap_Type::Name>;
	using arg_3_type = NamedParameterWithType<impl::VectorOfMessageType, obstacleMap_Type::Name>;
	using arg_4_type = NamedParameterWithType<impl::VectorOfMessageType, portalMap_Type::Name>;
	using arg_5_type = NamedParameterWithType<impl::VectorOfMessageType, jumpMap_Type::Name>;
	using arg_6_type = NamedParameterWithType<impl::RealType, polygonSpeed_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_4_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_5_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_6_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_3_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_4_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_4_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_5_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_5_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_6_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, arg_6_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "polygonMap", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "concaveMap", arg_2_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_3_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "obstacleMap", arg_3_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_4_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "portalMap", arg_4_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_5_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "jumpMap", arg_5_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_6_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, "polygonSpeed", arg_6_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );	}
}

template<class ParserT, typename ... Args>
void PolygonSt_parse(ParserT& p, Args&& ... args)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfMessageType, polygonMap_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::VectorOfMessageType, concaveMap_Type::Name>;
	using arg_3_type = NamedParameterWithType<impl::VectorOfMessageType, obstacleMap_Type::Name>;
	using arg_4_type = NamedParameterWithType<impl::VectorOfMessageType, portalMap_Type::Name>;
	using arg_5_type = NamedParameterWithType<impl::VectorOfMessageType, jumpMap_Type::Name>;
	using arg_6_type = NamedParameterWithType<impl::RealType, polygonSpeed_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_4_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_5_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_6_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ParserT::proto == Proto::GMQ )
	{
		impl::gmq::parseGmqParam<ParserT, arg_1_type, false>(p, arg_1_type::nameAndTypeID, args...);
		impl::gmq::parseGmqParam<ParserT, arg_2_type, false>(p, arg_2_type::nameAndTypeID, args...);
		impl::gmq::parseGmqParam<ParserT, arg_3_type, false>(p, arg_3_type::nameAndTypeID, args...);
		impl::gmq::parseGmqParam<ParserT, arg_4_type, false>(p, arg_4_type::nameAndTypeID, args...);
		impl::gmq::parseGmqParam<ParserT, arg_5_type, false>(p, arg_5_type::nameAndTypeID, args...);
		impl::gmq::parseGmqParam<ParserT, arg_6_type, false>(p, arg_6_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ParserT::proto == Proto::JSON );
		p.skipDelimiter( '{' );
		for ( ;; )
		{
			std::string key;
			p.readKey( &key );
			if ( key == "polygonMap" )
				impl::json::parseJsonParam<ParserT, arg_1_type, false>(arg_1_type::nameAndTypeID, p, args...);
			else if ( key == "concaveMap" )
				impl::json::parseJsonParam<ParserT, arg_2_type, false>(arg_2_type::nameAndTypeID, p, args...);
			else if ( key == "obstacleMap" )
				impl::json::parseJsonParam<ParserT, arg_3_type, false>(arg_3_type::nameAndTypeID, p, args...);
			else if ( key == "portalMap" )
				impl::json::parseJsonParam<ParserT, arg_4_type, false>(arg_4_type::nameAndTypeID, p, args...);
			else if ( key == "jumpMap" )
				impl::json::parseJsonParam<ParserT, arg_5_type, false>(arg_5_type::nameAndTypeID, p, args...);
			else if ( key == "polygonSpeed" )
				impl::json::parseJsonParam<ParserT, arg_6_type, false>(arg_6_type::nameAndTypeID, p, args...);
			p.skipSpacesEtc();
			if ( p.isDelimiter( ',' ) )
			{
				p.skipDelimiter( ',' );
				continue;
			}
			if ( p.isDelimiter( '}' ) )
			{
				p.skipDelimiter( '}' );
				break;
			}
			throw std::exception(); // bad format
		}
	}
}

//**********************************************************************
// CompositeType "LineMap" Targets: JSON GMQ (1 parameters)
// 1. VECTOR< MESSAGE Line> LineMap (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void LineMap_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfMessageType, LineMap_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "LineMap", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );	}
}

template<class ParserT, typename ... Args>
void LineMap_parse(ParserT& p, Args&& ... args)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfMessageType, LineMap_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ParserT::proto == Proto::GMQ )
	{
		impl::gmq::parseGmqParam<ParserT, arg_1_type, false>(p, arg_1_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ParserT::proto == Proto::JSON );
		p.skipDelimiter( '{' );
		for ( ;; )
		{
			std::string key;
			p.readKey( &key );
			if ( key == "LineMap" )
				impl::json::parseJsonParam<ParserT, arg_1_type, false>(arg_1_type::nameAndTypeID, p, args...);
			p.skipSpacesEtc();
			if ( p.isDelimiter( ',' ) )
			{
				p.skipDelimiter( ',' );
				continue;
			}
			if ( p.isDelimiter( '}' ) )
			{
				p.skipDelimiter( '}' );
				break;
			}
			throw std::exception(); // bad format
		}
	}
}

//**********************************************************************
// CompositeType "Line" Targets: JSON GMQ (2 parameters)
// 1. VECTOR<NONEXTENDABLE MESSAGE Vertex> a (REQUIRED)
// 2. VECTOR<NONEXTENDABLE MESSAGE Vertex> b (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void Line_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, a_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, b_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "a", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "b", arg_2_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );	}
}

template<class ParserT, typename ... Args>
void Line_parse(ParserT& p, Args&& ... args)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, a_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, b_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ParserT::proto == Proto::GMQ )
	{
		impl::gmq::parseGmqParam<ParserT, arg_1_type, false>(p, arg_1_type::nameAndTypeID, args...);
		impl::gmq::parseGmqParam<ParserT, arg_2_type, false>(p, arg_2_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ParserT::proto == Proto::JSON );
		p.skipDelimiter( '{' );
		for ( ;; )
		{
			std::string key;
			p.readKey( &key );
			if ( key == "a" )
				impl::json::parseJsonParam<ParserT, arg_1_type, false>(arg_1_type::nameAndTypeID, p, args...);
			else if ( key == "b" )
				impl::json::parseJsonParam<ParserT, arg_2_type, false>(arg_2_type::nameAndTypeID, p, args...);
			p.skipSpacesEtc();
			if ( p.isDelimiter( ',' ) )
			{
				p.skipDelimiter( ',' );
				continue;
			}
			if ( p.isDelimiter( '}' ) )
			{
				p.skipDelimiter( '}' );
				break;
			}
			throw std::exception(); // bad format
		}
	}
}

//**********************************************************************
// CompositeType "ObstacleMap" Targets: JSON GMQ (1 parameters)
// 1. VECTOR< MESSAGE PolygonMap> ObstacleMap (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void ObstacleMap_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfMessageType, ObstacleMap_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "ObstacleMap", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );	}
}

template<class ParserT, typename ... Args>
void ObstacleMap_parse(ParserT& p, Args&& ... args)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfMessageType, ObstacleMap_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ParserT::proto == Proto::GMQ )
	{
		impl::gmq::parseGmqParam<ParserT, arg_1_type, false>(p, arg_1_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ParserT::proto == Proto::JSON );
		p.skipDelimiter( '{' );
		for ( ;; )
		{
			std::string key;
			p.readKey( &key );
			if ( key == "ObstacleMap" )
				impl::json::parseJsonParam<ParserT, arg_1_type, false>(arg_1_type::nameAndTypeID, p, args...);
			p.skipSpacesEtc();
			if ( p.isDelimiter( ',' ) )
			{
				p.skipDelimiter( ',' );
				continue;
			}
			if ( p.isDelimiter( '}' ) )
			{
				p.skipDelimiter( '}' );
				break;
			}
			throw std::exception(); // bad format
		}
	}
}

//**********************************************************************
// CompositeType "PolygonMap" Targets: JSON GMQ (1 parameters)
// 1. VECTOR<NONEXTENDABLE MESSAGE Vertex> PolygonMap (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void PolygonMap_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, PolygonMap_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "PolygonMap", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );	}
}

template<class ParserT, typename ... Args>
void PolygonMap_parse(ParserT& p, Args&& ... args)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using arg_1_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, PolygonMap_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ParserT::proto == Proto::GMQ )
	{
		impl::gmq::parseGmqParam<ParserT, arg_1_type, false>(p, arg_1_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ParserT::proto == Proto::JSON );
		p.skipDelimiter( '{' );
		for ( ;; )
		{
			std::string key;
			p.readKey( &key );
			if ( key == "PolygonMap" )
				impl::json::parseJsonParam<ParserT, arg_1_type, false>(arg_1_type::nameAndTypeID, p, args...);
			p.skipSpacesEtc();
			if ( p.isDelimiter( ',' ) )
			{
				p.skipDelimiter( ',' );
				continue;
			}
			if ( p.isDelimiter( '}' ) )
			{
				p.skipDelimiter( '}' );
				break;
			}
			throw std::exception(); // bad format
		}
	}
}

//**********************************************************************
// CompositeType "Vertex" NONEXTENDABLE Targets: JSON GMQ (3 parameters)
// 1. INTEGER x (REQUIRED)
// 2. INTEGER y (REQUIRED)
// 3. INTEGER z (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void Vertex_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<impl::SignedIntegralType, x_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::SignedIntegralType, y_Type::Name>;
	using arg_3_type = NamedParameterWithType<impl::SignedIntegralType, z_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_3_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "x", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "y", arg_2_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_3_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "z", arg_3_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );	}
}

template<class ParserT, typename ... Args>
void Vertex_parse(ParserT& p, Args&& ... args)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using arg_1_type = NamedParameterWithType<impl::SignedIntegralType, x_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::SignedIntegralType, y_Type::Name>;
	using arg_3_type = NamedParameterWithType<impl::SignedIntegralType, z_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ParserT::proto == Proto::GMQ )
	{
		impl::gmq::parseGmqParam<ParserT, arg_1_type, false>(p, arg_1_type::nameAndTypeID, args...);
		impl::gmq::parseGmqParam<ParserT, arg_2_type, false>(p, arg_2_type::nameAndTypeID, args...);
		impl::gmq::parseGmqParam<ParserT, arg_3_type, false>(p, arg_3_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ParserT::proto == Proto::JSON );
		p.skipDelimiter( '{' );
		for ( ;; )
		{
			std::string key;
			p.readKey( &key );
			if ( key == "x" )
				impl::json::parseJsonParam<ParserT, arg_1_type, false>(arg_1_type::nameAndTypeID, p, args...);
			else if ( key == "y" )
				impl::json::parseJsonParam<ParserT, arg_2_type, false>(arg_2_type::nameAndTypeID, p, args...);
			else if ( key == "z" )
				impl::json::parseJsonParam<ParserT, arg_3_type, false>(arg_3_type::nameAndTypeID, p, args...);
			p.skipSpacesEtc();
			if ( p.isDelimiter( ',' ) )
			{
				p.skipDelimiter( ',' );
				continue;
			}
			if ( p.isDelimiter( '}' ) )
			{
				p.skipDelimiter( '}' );
				break;
			}
			throw std::exception(); // bad format
		}
	}
}


} // namespace m

#endif // _test_marshalling_h_guard
