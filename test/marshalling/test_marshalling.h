#ifndef _test_marshalling_h_guard
#define _test_marshalling_h_guard

#include <marshalling.h>

namespace m {

using fifthParam_Type = NamedParameter<struct fifthParam_Struct>;
using firstParam_Type = NamedParameter<struct firstParam_Struct>;
using forthParam_Type = NamedParameter<struct forthParam_Struct>;
using justPointOne_Type = NamedParameter<struct justPointOne_Struct>;
using justPointTwo_Type = NamedParameter<struct justPointTwo_Struct>;
using secondParam_Type = NamedParameter<struct secondParam_Struct>;
using seventhParam_Type = NamedParameter<struct seventhParam_Struct>;
using sixthParam_Type = NamedParameter<struct sixthParam_Struct>;
using thirdParam_Type = NamedParameter<struct thirdParam_Struct>;
using x_Type = NamedParameter<struct x_Struct>;
using y_Type = NamedParameter<struct y_Struct>;
using z_Type = NamedParameter<struct z_Struct>;

constexpr fifthParam_Type::TypeConverter fifthParam;
constexpr firstParam_Type::TypeConverter firstParam;
constexpr forthParam_Type::TypeConverter forthParam;
constexpr justPointOne_Type::TypeConverter justPointOne;
constexpr justPointTwo_Type::TypeConverter justPointTwo;
constexpr secondParam_Type::TypeConverter secondParam;
constexpr seventhParam_Type::TypeConverter seventhParam;
constexpr sixthParam_Type::TypeConverter sixthParam;
constexpr thirdParam_Type::TypeConverter thirdParam;
constexpr x_Type::TypeConverter x;
constexpr y_Type::TypeConverter y;
constexpr z_Type::TypeConverter z;

//**********************************************************************
// CompositeType "message_one" Targets: JSON GMQ (9 parameters)
// 1. INTEGER firstParam (REQUIRED)
// 2. VECTOR<INTEGER> secondParam (REQUIRED)
// 3. VECTOR< MESSAGE point3D> thirdParam (REQUIRED)
// 4. UINTEGER forthParam (REQUIRED)
// 5. CHARACTER_STRING fifthParam (REQUIRED)
// 6. VECTOR<NONEXTENDABLE MESSAGE point> sixthParam (REQUIRED)
// 7. REAL seventhParam (REQUIRED)
// 8. MESSAGE justPointOne (REQUIRED)
// 9. MESSAGE justPointTwo (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void message_one_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<impl::SignedIntegralType, firstParam_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::SignedIntegralType>, secondParam_Type::Name>;
	using arg_3_type = NamedParameterWithType<impl::VectorOfMessageType, thirdParam_Type::Name>;
	using arg_4_type = NamedParameterWithType<impl::UnsignedIntegralType, forthParam_Type::Name>;
	using arg_5_type = NamedParameterWithType<impl::StringType, fifthParam_Type::Name>;
	using arg_6_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, sixthParam_Type::Name>;
	using arg_7_type = NamedParameterWithType<impl::RealType, seventhParam_Type::Name>;
//       MessageParameterType::KIND::MESSAGE !!!!!!!!!!!!!
//       MessageParameterType::KIND::MESSAGE !!!!!!!!!!!!!

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_4_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_5_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_6_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_7_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_8_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_9_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_3_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_4_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_4_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_5_type, true, uint64_t, uint64_t, (uint64_t)0>(composer, arg_5_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_6_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_6_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_7_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, arg_7_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "firstParam", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "secondParam", arg_2_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_3_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "thirdParam", arg_3_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_4_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "forthParam", arg_4_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_5_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "fifthParam", arg_5_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_6_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "sixthParam", arg_6_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_7_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, "seventhParam", arg_7_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		composer.buff.append( ",\n  ", 4 );
		composer.buff.append( "\n}", 2 );	}
}

template<class ParserT, typename ... Args>
void message_one_parse(ParserT& p, Args&& ... args)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using arg_1_type = NamedParameterWithType<impl::SignedIntegralType, firstParam_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::VectorOfSympleTypes<impl::SignedIntegralType>, secondParam_Type::Name>;
	using arg_3_type = NamedParameterWithType<impl::VectorOfMessageType, thirdParam_Type::Name>;
	using arg_4_type = NamedParameterWithType<impl::UnsignedIntegralType, forthParam_Type::Name>;
	using arg_5_type = NamedParameterWithType<impl::StringType, fifthParam_Type::Name>;
	using arg_6_type = NamedParameterWithType<impl::VectorOfNonextMessageTypes, sixthParam_Type::Name>;
	using arg_7_type = NamedParameterWithType<impl::RealType, seventhParam_Type::Name>;
//       MessageParameterType::KIND::MESSAGE !!!!!!!!!!!!!
//       MessageParameterType::KIND::MESSAGE !!!!!!!!!!!!!

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_4_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_5_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_6_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_7_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_8_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_9_type::nameAndTypeID, Args::nameAndTypeID...);
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
		impl::gmq::parseGmqParam<ParserT, arg_7_type, false>(p, arg_7_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ParserT::proto == Proto::JSON );
		p.skipDelimiter( '{' );
		for ( ;; )
		{
			std::string key;
			p.readKey( &key );
			if ( key == "firstParam" )
				impl::json::parseJsonParam<ParserT, arg_1_type, false>(arg_1_type::nameAndTypeID, p, args...);
			else if ( key == "secondParam" )
				impl::json::parseJsonParam<ParserT, arg_2_type, false>(arg_2_type::nameAndTypeID, p, args...);
			else if ( key == "thirdParam" )
				impl::json::parseJsonParam<ParserT, arg_3_type, false>(arg_3_type::nameAndTypeID, p, args...);
			else if ( key == "forthParam" )
				impl::json::parseJsonParam<ParserT, arg_4_type, false>(arg_4_type::nameAndTypeID, p, args...);
			else if ( key == "fifthParam" )
				impl::json::parseJsonParam<ParserT, arg_5_type, false>(arg_5_type::nameAndTypeID, p, args...);
			else if ( key == "sixthParam" )
				impl::json::parseJsonParam<ParserT, arg_6_type, false>(arg_6_type::nameAndTypeID, p, args...);
			else if ( key == "seventhParam" )
				impl::json::parseJsonParam<ParserT, arg_7_type, false>(arg_7_type::nameAndTypeID, p, args...);
			else if ( key == "justPointOne" )
				impl::json::parseJsonParam<ParserT, arg_8_type, false>(arg_8_type::nameAndTypeID, p, args...);
			else if ( key == "justPointTwo" )
				impl::json::parseJsonParam<ParserT, arg_9_type, false>(arg_9_type::nameAndTypeID, p, args...);
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
// CompositeType "point" NONEXTENDABLE Targets: JSON GMQ (2 parameters)
// 1. INTEGER x (REQUIRED)
// 2. INTEGER y (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void point_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<impl::SignedIntegralType, x_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::SignedIntegralType, y_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
		impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "x", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "y", arg_2_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );	}
}

template<class ParserT, typename ... Args>
void point_parse(ParserT& p, Args&& ... args)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using arg_1_type = NamedParameterWithType<impl::SignedIntegralType, x_Type::Name>;
	using arg_2_type = NamedParameterWithType<impl::SignedIntegralType, y_Type::Name>;

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
			if ( key == "x" )
				impl::json::parseJsonParam<ParserT, arg_1_type, false>(arg_1_type::nameAndTypeID, p, args...);
			else if ( key == "y" )
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
// CompositeType "point3D" NONEXTENDABLE Targets: JSON GMQ (3 parameters)
// 1. INTEGER x (REQUIRED)
// 2. INTEGER y (REQUIRED)
// 3. INTEGER z (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void point3D_compose(ComposerT& composer, Args&& ... args)
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
void point3D_parse(ParserT& p, Args&& ... args)
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
