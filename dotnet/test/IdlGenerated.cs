using globalmq.marshalling;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace mtest
{


//**********************************************************************
// STRUCT "SIZE" Targets: JSON (3 parameters)
// 1. REAL X (REQUIRED)
// 2. REAL Y (REQUIRED)
// 3. REAL Z (REQUIRED)
//**********************************************************************

public class SIZE : IEquatable<SIZE>
{
	public Double X;
	public Double Y;
	public Double Z;

	public override bool Equals(object obj)
	{
		return obj is SIZE d && Equals(d);
	}
	public static bool operator ==(SIZE left, SIZE right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(SIZE left, SIZE right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(SIZE other)
	{
		return
			this.X == other.X &&
			this.Y == other.Y &&
			this.Z == other.Z;
	}
	public static void compose(ComposerBase composer, Double X, Double Y, Double Z)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, X, Y, Z);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, X, Y, Z);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, Double X, Double Y, Double Z)
	{
		composer.append( "{\n  ");
		composer.addNamePart("X");
		composer.composeReal(X);
		composer.append( ",\n  " );
		composer.addNamePart("Y");
		composer.composeReal(Y);
		composer.append( ",\n  " );
		composer.addNamePart("Z");
		composer.composeReal(Z);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, Double X, Double Y, Double Z)
	{
		composer.composeReal(X);
		composer.composeReal(Y);
		composer.composeReal(Z);
	}
	public static void parse(ParserBase parser, ref Double X, ref Double Y, ref Double Z)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, ref X, ref Y, ref Z);
		else if (parser is JsonParser jsonP)
			parse(jsonP, ref X, ref Y, ref Z);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, ref Double X, ref Double Y, ref Double Z)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "X" )
				parser.parseReal(out X);
			else if ( key == "Y" )
				parser.parseReal(out Y);
			else if ( key == "Z" )
				parser.parseReal(out Z);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, ref Double X, ref Double Y, ref Double Z)
	{
		parser.parseReal(out X);
		parser.parseReal(out Y);
		parser.parseReal(out Z);
	}
	public static void compose(ComposerBase composer, SIZE msg)
	{
		compose(composer,
			X: msg.X,
			Y: msg.Y,
			Z: msg.Z
		);
	}
	public static void parse(ParserBase parser, out SIZE msg)
	{
		SIZE tmp = new SIZE();
		parse(parser,
			X: ref tmp.X,
			Y: ref tmp.Y,
			Z: ref tmp.Z
		);
	msg = tmp;
	}
} // class SIZE

//**********************************************************************
// STRUCT "point3D" Targets: JSON GMQ (3 parameters)
// 1. INTEGER x (REQUIRED)
// 2. INTEGER y (REQUIRED)
// 3. INTEGER z (REQUIRED)
//**********************************************************************

public class point3D : IEquatable<point3D>
{
	public Int64 x;
	public Int64 y;
	public Int64 z;

	public override bool Equals(object obj)
	{
		return obj is point3D d && Equals(d);
	}
	public static bool operator ==(point3D left, point3D right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(point3D left, point3D right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(point3D other)
	{
		return
			this.x == other.x &&
			this.y == other.y &&
			this.z == other.z;
	}
	public static void compose(ComposerBase composer, Int64 x, Int64 y, Int64 z)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, x, y, z);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, x, y, z);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, Int64 x, Int64 y, Int64 z)
	{
		composer.append( "{\n  ");
		composer.addNamePart("x");
		composer.composeSignedInteger(x);
		composer.append( ",\n  " );
		composer.addNamePart("y");
		composer.composeSignedInteger(y);
		composer.append( ",\n  " );
		composer.addNamePart("z");
		composer.composeSignedInteger(z);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, Int64 x, Int64 y, Int64 z)
	{
		composer.composeSignedInteger(x);
		composer.composeSignedInteger(y);
		composer.composeSignedInteger(z);
	}
	public static void parse(ParserBase parser, ref Int64 x, ref Int64 y, ref Int64 z)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, ref x, ref y, ref z);
		else if (parser is JsonParser jsonP)
			parse(jsonP, ref x, ref y, ref z);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, ref Int64 x, ref Int64 y, ref Int64 z)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "x" )
				parser.parseSignedInteger(out x);
			else if ( key == "y" )
				parser.parseSignedInteger(out y);
			else if ( key == "z" )
				parser.parseSignedInteger(out z);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, ref Int64 x, ref Int64 y, ref Int64 z)
	{
		parser.parseSignedInteger(out x);
		parser.parseSignedInteger(out y);
		parser.parseSignedInteger(out z);
	}
	public static void compose(ComposerBase composer, point3D msg)
	{
		compose(composer,
			x: msg.x,
			y: msg.y,
			z: msg.z
		);
	}
	public static void parse(ParserBase parser, out point3D msg)
	{
		point3D tmp = new point3D();
		parse(parser,
			x: ref tmp.x,
			y: ref tmp.y,
			z: ref tmp.z
		);
	msg = tmp;
	}
} // class point3D

//**********************************************************************
// STRUCT "point" NONEXTENDABLE Targets: JSON GMQ (2 parameters)
// 1. INTEGER x (REQUIRED)
// 2. INTEGER y (REQUIRED)
//**********************************************************************

public class point : IEquatable<point>
{
	public Int64 x;
	public Int64 y;

	public override bool Equals(object obj)
	{
		return obj is point d && Equals(d);
	}
	public static bool operator ==(point left, point right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(point left, point right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(point other)
	{
		return
			this.x == other.x &&
			this.y == other.y;
	}
	public static void compose(ComposerBase composer, Int64 x, Int64 y)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, x, y);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, x, y);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, Int64 x, Int64 y)
	{
		composer.append( "{\n  ");
		composer.addNamePart("x");
		composer.composeSignedInteger(x);
		composer.append( ",\n  " );
		composer.addNamePart("y");
		composer.composeSignedInteger(y);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, Int64 x, Int64 y)
	{
		composer.composeSignedInteger(x);
		composer.composeSignedInteger(y);
	}
	public static void parse(ParserBase parser, ref Int64 x, ref Int64 y)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, ref x, ref y);
		else if (parser is JsonParser jsonP)
			parse(jsonP, ref x, ref y);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, ref Int64 x, ref Int64 y)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "x" )
				parser.parseSignedInteger(out x);
			else if ( key == "y" )
				parser.parseSignedInteger(out y);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, ref Int64 x, ref Int64 y)
	{
		parser.parseSignedInteger(out x);
		parser.parseSignedInteger(out y);
	}
	public static void compose(ComposerBase composer, point msg)
	{
		compose(composer,
			x: msg.x,
			y: msg.y
		);
	}
	public static void parse(ParserBase parser, out point msg)
	{
		point tmp = new point();
		parse(parser,
			x: ref tmp.x,
			y: ref tmp.y
		);
	msg = tmp;
	}
} // class point

//**********************************************************************
// STRUCT "Vertex" NONEXTENDABLE Targets: GMQ (3 parameters)
// 1. INTEGER x (REQUIRED)
// 2. INTEGER y (REQUIRED)
// 3. INTEGER z (REQUIRED)
//**********************************************************************

public class Vertex : IEquatable<Vertex>
{
	public Int64 x;
	public Int64 y;
	public Int64 z;

	public override bool Equals(object obj)
	{
		return obj is Vertex d && Equals(d);
	}
	public static bool operator ==(Vertex left, Vertex right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(Vertex left, Vertex right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(Vertex other)
	{
		return
			this.x == other.x &&
			this.y == other.y &&
			this.z == other.z;
	}
	public static void compose(ComposerBase composer, Int64 x, Int64 y, Int64 z)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, x, y, z);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, x, y, z);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, Int64 x, Int64 y, Int64 z)
	{
		composer.append( "{\n  ");
		composer.addNamePart("x");
		composer.composeSignedInteger(x);
		composer.append( ",\n  " );
		composer.addNamePart("y");
		composer.composeSignedInteger(y);
		composer.append( ",\n  " );
		composer.addNamePart("z");
		composer.composeSignedInteger(z);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, Int64 x, Int64 y, Int64 z)
	{
		composer.composeSignedInteger(x);
		composer.composeSignedInteger(y);
		composer.composeSignedInteger(z);
	}
	public static void parse(ParserBase parser, ref Int64 x, ref Int64 y, ref Int64 z)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, ref x, ref y, ref z);
		else if (parser is JsonParser jsonP)
			parse(jsonP, ref x, ref y, ref z);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, ref Int64 x, ref Int64 y, ref Int64 z)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "x" )
				parser.parseSignedInteger(out x);
			else if ( key == "y" )
				parser.parseSignedInteger(out y);
			else if ( key == "z" )
				parser.parseSignedInteger(out z);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, ref Int64 x, ref Int64 y, ref Int64 z)
	{
		parser.parseSignedInteger(out x);
		parser.parseSignedInteger(out y);
		parser.parseSignedInteger(out z);
	}
	public static void compose(ComposerBase composer, Vertex msg)
	{
		compose(composer,
			x: msg.x,
			y: msg.y,
			z: msg.z
		);
	}
	public static void parse(ParserBase parser, out Vertex msg)
	{
		Vertex tmp = new Vertex();
		parse(parser,
			x: ref tmp.x,
			y: ref tmp.y,
			z: ref tmp.z
		);
	msg = tmp;
	}
} // class Vertex

//**********************************************************************
// STRUCT "PolygonMap" Targets: GMQ (1 parameters)
// 1. VECTOR<NONEXTENDABLE STRUCT Vertex> polygonMap (REQUIRED)
//**********************************************************************

public class PolygonMap : IEquatable<PolygonMap>
{
	public Vertex[] polygonMap;

	public override bool Equals(object obj)
	{
		return obj is PolygonMap d && Equals(d);
	}
	public static bool operator ==(PolygonMap left, PolygonMap right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(PolygonMap left, PolygonMap right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(PolygonMap other)
	{
		return
			Enumerable.SequenceEqual(this.polygonMap, other.polygonMap);
	}
	public static void compose(ComposerBase composer, ICompose polygonMap)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, polygonMap);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, polygonMap);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, ICompose polygonMap)
	{
		composer.append( "{\n  ");
		composer.addNamePart("polygonMap");
		globalmq.marshalling.impl.json.composeParamToJson(composer, polygonMap);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, ICompose polygonMap)
	{
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, polygonMap);
	}
	public static void parse(ParserBase parser, IParse polygonMap)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, polygonMap);
		else if (parser is JsonParser jsonP)
			parse(jsonP, polygonMap);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, IParse polygonMap)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "polygonMap" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, polygonMap);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, IParse polygonMap)
	{
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, polygonMap);
	}
	public static void compose(ComposerBase composer, PolygonMap msg)
	{
		compose(composer,
			polygonMap: new CollectionWrapperForComposing(
				() => { return msg.polygonMap.Length; },
				(ComposerBase composer, int ordinal) => { Vertex.compose(composer, msg.polygonMap[ordinal]); })
		);
	}
	public static void parse(ParserBase parser, out PolygonMap msg)
	{
		PolygonMap tmp = new PolygonMap();
		parse(parser,
			polygonMap: new CollectionWrapperForParsing(
				(int size) => { tmp.polygonMap = new Vertex[size]; },
				(ParserBase parser, int ordinal) => { Vertex.parse(parser, out tmp.polygonMap[ordinal]); })
		);
	msg = tmp;
	}
} // class PolygonMap

//**********************************************************************
// STRUCT "ObstacleMap" Targets: GMQ (1 parameters)
// 1. VECTOR< STRUCT PolygonMap> obstacleMap (REQUIRED)
//**********************************************************************

public class ObstacleMap : IEquatable<ObstacleMap>
{
	public PolygonMap[] obstacleMap;

	public override bool Equals(object obj)
	{
		return obj is ObstacleMap d && Equals(d);
	}
	public static bool operator ==(ObstacleMap left, ObstacleMap right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(ObstacleMap left, ObstacleMap right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(ObstacleMap other)
	{
		return
			Enumerable.SequenceEqual(this.obstacleMap, other.obstacleMap);
	}
	public static void compose(ComposerBase composer, ICompose obstacleMap)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, obstacleMap);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, obstacleMap);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, ICompose obstacleMap)
	{
		composer.append( "{\n  ");
		composer.addNamePart("obstacleMap");
		globalmq.marshalling.impl.json.composeParamToJson(composer, obstacleMap);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, ICompose obstacleMap)
	{
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, obstacleMap);
	}
	public static void parse(ParserBase parser, IParse obstacleMap)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, obstacleMap);
		else if (parser is JsonParser jsonP)
			parse(jsonP, obstacleMap);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, IParse obstacleMap)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "obstacleMap" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, obstacleMap);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, IParse obstacleMap)
	{
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, obstacleMap);
	}
	public static void compose(ComposerBase composer, ObstacleMap msg)
	{
		compose(composer,
			obstacleMap: new CollectionWrapperForComposing(
				() => { return msg.obstacleMap.Length; },
				(ComposerBase composer, int ordinal) => { PolygonMap.compose(composer, msg.obstacleMap[ordinal]); })
		);
	}
	public static void parse(ParserBase parser, out ObstacleMap msg)
	{
		ObstacleMap tmp = new ObstacleMap();
		parse(parser,
			obstacleMap: new CollectionWrapperForParsing(
				(int size) => { tmp.obstacleMap = new PolygonMap[size]; },
				(ParserBase parser, int ordinal) => { PolygonMap.parse(parser, out tmp.obstacleMap[ordinal]); })
		);
	msg = tmp;
	}
} // class ObstacleMap

//**********************************************************************
// STRUCT "Line" Targets: GMQ (2 parameters)
// 1. VECTOR<NONEXTENDABLE STRUCT Vertex> a (REQUIRED)
// 2. VECTOR<NONEXTENDABLE STRUCT Vertex> b (REQUIRED)
//**********************************************************************

public class Line : IEquatable<Line>
{
	public Vertex[] a;
	public Vertex[] b;

	public override bool Equals(object obj)
	{
		return obj is Line d && Equals(d);
	}
	public static bool operator ==(Line left, Line right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(Line left, Line right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(Line other)
	{
		return
			Enumerable.SequenceEqual(this.a, other.a) &&
			Enumerable.SequenceEqual(this.b, other.b);
	}
	public static void compose(ComposerBase composer, ICompose a, ICompose b)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, a, b);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, a, b);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, ICompose a, ICompose b)
	{
		composer.append( "{\n  ");
		composer.addNamePart("a");
		globalmq.marshalling.impl.json.composeParamToJson(composer, a);
		composer.append( ",\n  " );
		composer.addNamePart("b");
		globalmq.marshalling.impl.json.composeParamToJson(composer, b);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, ICompose a, ICompose b)
	{
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, a);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, b);
	}
	public static void parse(ParserBase parser, IParse a, IParse b)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, a, b);
		else if (parser is JsonParser jsonP)
			parse(jsonP, a, b);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, IParse a, IParse b)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "a" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, a);
			else if ( key == "b" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, b);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, IParse a, IParse b)
	{
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, a);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, b);
	}
	public static void compose(ComposerBase composer, Line msg)
	{
		compose(composer,
			a: new CollectionWrapperForComposing(
				() => { return msg.a.Length; },
				(ComposerBase composer, int ordinal) => { Vertex.compose(composer, msg.a[ordinal]); }),
			b: new CollectionWrapperForComposing(
				() => { return msg.b.Length; },
				(ComposerBase composer, int ordinal) => { Vertex.compose(composer, msg.b[ordinal]); })
		);
	}
	public static void parse(ParserBase parser, out Line msg)
	{
		Line tmp = new Line();
		parse(parser,
			a: new CollectionWrapperForParsing(
				(int size) => { tmp.a = new Vertex[size]; },
				(ParserBase parser, int ordinal) => { Vertex.parse(parser, out tmp.a[ordinal]); }),
			b: new CollectionWrapperForParsing(
				(int size) => { tmp.b = new Vertex[size]; },
				(ParserBase parser, int ordinal) => { Vertex.parse(parser, out tmp.b[ordinal]); })
		);
	msg = tmp;
	}
} // class Line

//**********************************************************************
// STRUCT "LineMap" Targets: GMQ (1 parameters)
// 1. VECTOR< STRUCT Line> lineMap (REQUIRED)
//**********************************************************************

public class LineMap : IEquatable<LineMap>
{
	public Line[] lineMap;

	public override bool Equals(object obj)
	{
		return obj is LineMap d && Equals(d);
	}
	public static bool operator ==(LineMap left, LineMap right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(LineMap left, LineMap right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(LineMap other)
	{
		return
			Enumerable.SequenceEqual(this.lineMap, other.lineMap);
	}
	public static void compose(ComposerBase composer, ICompose lineMap)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, lineMap);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, lineMap);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, ICompose lineMap)
	{
		composer.append( "{\n  ");
		composer.addNamePart("lineMap");
		globalmq.marshalling.impl.json.composeParamToJson(composer, lineMap);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, ICompose lineMap)
	{
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, lineMap);
	}
	public static void parse(ParserBase parser, IParse lineMap)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, lineMap);
		else if (parser is JsonParser jsonP)
			parse(jsonP, lineMap);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, IParse lineMap)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "lineMap" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, lineMap);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, IParse lineMap)
	{
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, lineMap);
	}
	public static void compose(ComposerBase composer, LineMap msg)
	{
		compose(composer,
			lineMap: new CollectionWrapperForComposing(
				() => { return msg.lineMap.Length; },
				(ComposerBase composer, int ordinal) => { Line.compose(composer, msg.lineMap[ordinal]); })
		);
	}
	public static void parse(ParserBase parser, out LineMap msg)
	{
		LineMap tmp = new LineMap();
		parse(parser,
			lineMap: new CollectionWrapperForParsing(
				(int size) => { tmp.lineMap = new Line[size]; },
				(ParserBase parser, int ordinal) => { Line.parse(parser, out tmp.lineMap[ordinal]); })
		);
	msg = tmp;
	}
} // class LineMap

//**********************************************************************
// STRUCT "POINT3DREAL" Targets: JSON (3 parameters)
// 1. REAL X (REQUIRED)
// 2. REAL Y (REQUIRED)
// 3. REAL Z (REQUIRED)
//**********************************************************************

public class POINT3DREAL : IEquatable<POINT3DREAL>
{
	public Double X;
	public Double Y;
	public Double Z;

	public override bool Equals(object obj)
	{
		return obj is POINT3DREAL d && Equals(d);
	}
	public static bool operator ==(POINT3DREAL left, POINT3DREAL right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(POINT3DREAL left, POINT3DREAL right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(POINT3DREAL other)
	{
		return
			this.X == other.X &&
			this.Y == other.Y &&
			this.Z == other.Z;
	}
	public static void compose(ComposerBase composer, Double X, Double Y, Double Z)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, X, Y, Z);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, X, Y, Z);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, Double X, Double Y, Double Z)
	{
		composer.append( "{\n  ");
		composer.addNamePart("X");
		composer.composeReal(X);
		composer.append( ",\n  " );
		composer.addNamePart("Y");
		composer.composeReal(Y);
		composer.append( ",\n  " );
		composer.addNamePart("Z");
		composer.composeReal(Z);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, Double X, Double Y, Double Z)
	{
		composer.composeReal(X);
		composer.composeReal(Y);
		composer.composeReal(Z);
	}
	public static void parse(ParserBase parser, ref Double X, ref Double Y, ref Double Z)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, ref X, ref Y, ref Z);
		else if (parser is JsonParser jsonP)
			parse(jsonP, ref X, ref Y, ref Z);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, ref Double X, ref Double Y, ref Double Z)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "X" )
				parser.parseReal(out X);
			else if ( key == "Y" )
				parser.parseReal(out Y);
			else if ( key == "Z" )
				parser.parseReal(out Z);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, ref Double X, ref Double Y, ref Double Z)
	{
		parser.parseReal(out X);
		parser.parseReal(out Y);
		parser.parseReal(out Z);
	}
	public static void compose(ComposerBase composer, POINT3DREAL msg)
	{
		compose(composer,
			X: msg.X,
			Y: msg.Y,
			Z: msg.Z
		);
	}
	public static void parse(ParserBase parser, out POINT3DREAL msg)
	{
		POINT3DREAL tmp = new POINT3DREAL();
		parse(parser,
			X: ref tmp.X,
			Y: ref tmp.Y,
			Z: ref tmp.Z
		);
	msg = tmp;
	}
} // class POINT3DREAL

//**********************************************************************
// STRUCT "CharacterParamStruct" Targets: JSON (2 parameters)
// 1. INTEGER ID (REQUIRED)
// 2. STRUCT SIZE Size (REQUIRED)
//**********************************************************************

public class CharacterParamStruct : IEquatable<CharacterParamStruct>
{
	public Int64 ID;
	public SIZE Size;

	public override bool Equals(object obj)
	{
		return obj is CharacterParamStruct d && Equals(d);
	}
	public static bool operator ==(CharacterParamStruct left, CharacterParamStruct right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(CharacterParamStruct left, CharacterParamStruct right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(CharacterParamStruct other)
	{
		return
			this.ID == other.ID &&
			this.Size.Equals(other.Size);
	}
	public static void compose(ComposerBase composer, Int64 ID, ICompose Size)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, ID, Size);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, ID, Size);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, Int64 ID, ICompose Size)
	{
		composer.append( "{\n  ");
		composer.addNamePart("ID");
		composer.composeSignedInteger(ID);
		composer.append( ",\n  " );
		composer.addNamePart("Size");
		globalmq.marshalling.impl.json.composeParamToJson(composer, Size);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, Int64 ID, ICompose Size)
	{
		composer.composeSignedInteger(ID);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, Size);
	}
	public static void parse(ParserBase parser, ref Int64 ID, IParse Size)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, ref ID, Size);
		else if (parser is JsonParser jsonP)
			parse(jsonP, ref ID, Size);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, ref Int64 ID, IParse Size)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "ID" )
				parser.parseSignedInteger(out ID);
			else if ( key == "Size" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, Size);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, ref Int64 ID, IParse Size)
	{
		parser.parseSignedInteger(out ID);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, Size);
	}
	public static void compose(ComposerBase composer, CharacterParamStruct msg)
	{
		compose(composer,
			ID: msg.ID,
			Size: new MessageWrapperForComposing(
				(ComposerBase composer) => { SIZE.compose(composer, msg.Size); })
		);
	}
	public static void parse(ParserBase parser, out CharacterParamStruct msg)
	{
		CharacterParamStruct tmp = new CharacterParamStruct();
		parse(parser,
			ID: ref tmp.ID,
			Size: new MessageWrapperForParsing(
				(ParserBase parser) => { SIZE.parse(parser, out tmp.Size); })
		);
	msg = tmp;
	}
} // class CharacterParamStruct

//////////////////////////////////////////////////////////////
//
//  Scopes:
//
//  scope_one
//  {
//    point3D_alias
//    point_alias
//  }
//
//  level_trace
//  {
//    LevelTraceData
//  }
//
//  infrastructural
//  {
//    PolygonSt
//    point
//    point3D
//  }
//
//  test_gmq
//  {
//    message_one
//  }
//
//  test_json
//  {
//    message_one
//  }
//
//////////////////////////////////////////////////////////////

public class scope_one
{
	public static void handleMessage( BufferT buffer, MessageHandlerArray handlers )
	{
		ReadIteratorT riter = buffer.getReadIterator();
		handleMessage(riter, handlers);
	}
public static void handleMessage( ReadIteratorT riter, MessageHandlerArray handlers )
{
	JsonParser parser = new JsonParser( riter );
	globalmq.marshalling.impl.json.handleMessage(parser, handlers);
}
//**********************************************************************
// MESSAGE "point3D_alias" Targets: JSON (Alias of point3D)
//**********************************************************************

public class point3D_alias : point3D
{
public static UInt64 MsgId = 1;
	public new static void compose(ComposerBase composer, Int64 x, Int64 y, Int64 z)
	{
		point3D.compose(composer, x, y, z);
	}
	public new static void parse(ParserBase parser, ref Int64 x, ref Int64 y, ref Int64 z)
	{
		point3D.parse(parser, ref x, ref y, ref z);
	}
	public new static void compose(GmqComposer composer, Int64 x, Int64 y, Int64 z)
	{
		point3D.compose(composer, x, y, z);
	}
	public new static void parse(GmqParser parser, ref Int64 x, ref Int64 y, ref Int64 z)
	{
		point3D.parse(parser, ref x, ref y, ref z);
	}
	public new static void compose(JsonComposer composer, Int64 x, Int64 y, Int64 z)
	{
		point3D.compose(composer, x, y, z);
	}
	public new static void parse(JsonParser parser, ref Int64 x, ref Int64 y, ref Int64 z)
	{
		point3D.parse(parser, ref x, ref y, ref z);
	}
	public static void composeMessage(ComposerBase composer, Int64 x, Int64 y, Int64 z)
	{
		if (composer is GmqComposer gmqC)
		{
			gmqC.composeUnsignedInteger(MsgId);
			compose(gmqC, x, y, z);
		}
		else if (composer is JsonComposer jsonC)
		{
			jsonC.append("{\n  ");
			jsonC.addNamePart("msgid");
			jsonC.composeUnsignedInteger(MsgId);
			jsonC.append(",\n  ");
			jsonC.addNamePart("msgbody");
			compose(jsonC, x, y, z);
			jsonC.append("\n}");
		}
		else
			throw new ArgumentException();
	}
} // class point3D_alias

//**********************************************************************
// MESSAGE "point_alias" Targets: JSON (Alias of point)
//**********************************************************************

public class point_alias : point
{
public static UInt64 MsgId = 2;
	public new static void compose(ComposerBase composer, Int64 x, Int64 y)
	{
		point.compose(composer, x, y);
	}
	public new static void parse(ParserBase parser, ref Int64 x, ref Int64 y)
	{
		point.parse(parser, ref x, ref y);
	}
	public new static void compose(GmqComposer composer, Int64 x, Int64 y)
	{
		point.compose(composer, x, y);
	}
	public new static void parse(GmqParser parser, ref Int64 x, ref Int64 y)
	{
		point.parse(parser, ref x, ref y);
	}
	public new static void compose(JsonComposer composer, Int64 x, Int64 y)
	{
		point.compose(composer, x, y);
	}
	public new static void parse(JsonParser parser, ref Int64 x, ref Int64 y)
	{
		point.parse(parser, ref x, ref y);
	}
	public static void composeMessage(ComposerBase composer, Int64 x, Int64 y)
	{
		if (composer is GmqComposer gmqC)
		{
			gmqC.composeUnsignedInteger(MsgId);
			compose(gmqC, x, y);
		}
		else if (composer is JsonComposer jsonC)
		{
			jsonC.append("{\n  ");
			jsonC.addNamePart("msgid");
			jsonC.composeUnsignedInteger(MsgId);
			jsonC.append(",\n  ");
			jsonC.addNamePart("msgbody");
			compose(jsonC, x, y);
			jsonC.append("\n}");
		}
		else
			throw new ArgumentException();
	}
} // class point_alias

} // class scope_one

public class level_trace
{
	public static void handleMessage( BufferT buffer, MessageHandlerArray handlers )
	{
		ReadIteratorT riter = buffer.getReadIterator();
		handleMessage(riter, handlers);
	}
public static void handleMessage( ReadIteratorT riter, MessageHandlerArray handlers )
{
	JsonParser parser = new JsonParser( riter );
	globalmq.marshalling.impl.json.handleMessage(parser, handlers);
}
//**********************************************************************
// MESSAGE "LevelTraceData" Targets: JSON (2 parameters)
// 1. STRUCT CharacterParamStruct CharacterParam (REQUIRED)
// 2. VECTOR< STRUCT POINT3DREAL> Points (REQUIRED)
//**********************************************************************

public class LevelTraceData : IEquatable<LevelTraceData>
{
public static UInt64 MsgId = 1;
	public CharacterParamStruct CharacterParam;
	public POINT3DREAL[] Points;

	public override bool Equals(object obj)
	{
		return obj is LevelTraceData d && Equals(d);
	}
	public static bool operator ==(LevelTraceData left, LevelTraceData right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(LevelTraceData left, LevelTraceData right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(LevelTraceData other)
	{
		return
			this.CharacterParam.Equals(other.CharacterParam) &&
			Enumerable.SequenceEqual(this.Points, other.Points);
	}
	public static void compose(ComposerBase composer, ICompose CharacterParam, ICompose Points)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, CharacterParam, Points);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, CharacterParam, Points);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, ICompose CharacterParam, ICompose Points)
	{
		composer.append( "{\n  ");
		composer.addNamePart("CharacterParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, CharacterParam);
		composer.append( ",\n  " );
		composer.addNamePart("Points");
		globalmq.marshalling.impl.json.composeParamToJson(composer, Points);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, ICompose CharacterParam, ICompose Points)
	{
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, CharacterParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, Points);
	}
	public static void parse(ParserBase parser, IParse CharacterParam, IParse Points)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, CharacterParam, Points);
		else if (parser is JsonParser jsonP)
			parse(jsonP, CharacterParam, Points);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, IParse CharacterParam, IParse Points)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "CharacterParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, CharacterParam);
			else if ( key == "Points" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, Points);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, IParse CharacterParam, IParse Points)
	{
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, CharacterParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, Points);
	}
	public static void compose(ComposerBase composer, LevelTraceData msg)
	{
		compose(composer,
			CharacterParam: new MessageWrapperForComposing(
				(ComposerBase composer) => { CharacterParamStruct.compose(composer, msg.CharacterParam); }),
			Points: new CollectionWrapperForComposing(
				() => { return msg.Points.Length; },
				(ComposerBase composer, int ordinal) => { POINT3DREAL.compose(composer, msg.Points[ordinal]); })
		);
	}
	public static void parse(ParserBase parser, out LevelTraceData msg)
	{
		LevelTraceData tmp = new LevelTraceData();
		parse(parser,
			CharacterParam: new MessageWrapperForParsing(
				(ParserBase parser) => { CharacterParamStruct.parse(parser, out tmp.CharacterParam); }),
			Points: new CollectionWrapperForParsing(
				(int size) => { tmp.Points = new POINT3DREAL[size]; },
				(ParserBase parser, int ordinal) => { POINT3DREAL.parse(parser, out tmp.Points[ordinal]); })
		);
	msg = tmp;
	}
	public static void composeMessage(ComposerBase composer, ICompose CharacterParam, ICompose Points)
	{
		if (composer is GmqComposer gmqC)
		{
			gmqC.composeUnsignedInteger(MsgId);
			compose(gmqC, CharacterParam, Points);
		}
		else if (composer is JsonComposer jsonC)
		{
			jsonC.append("{\n  ");
			jsonC.addNamePart("msgid");
			jsonC.composeUnsignedInteger(MsgId);
			jsonC.append(",\n  ");
			jsonC.addNamePart("msgbody");
			compose(jsonC, CharacterParam, Points);
			jsonC.append("\n}");
		}
		else
			throw new ArgumentException();
	}
} // class LevelTraceData

} // class level_trace

public class infrastructural
{
	public static void handleMessage( BufferT buffer, MessageHandlerArray handlers )
	{
		ReadIteratorT riter = buffer.getReadIterator();
		handleMessage(riter, handlers);
	}
public static void handleMessage( ReadIteratorT riter, MessageHandlerArray handlers )
{
	GmqParser parser = new GmqParser( riter );
	globalmq.marshalling.impl.gmq.handleMessage(parser, handlers);
}
//**********************************************************************
// MESSAGE "PolygonSt" Targets: GMQ (6 parameters)
// 1. VECTOR< STRUCT PolygonMap> polygonMap (REQUIRED)
// 2. VECTOR< STRUCT PolygonMap> concaveMap (REQUIRED)
// 3. VECTOR< STRUCT ObstacleMap> obstacleMap (REQUIRED)
// 4. VECTOR< STRUCT LineMap> portalMap (REQUIRED)
// 5. VECTOR< STRUCT LineMap> jumpMap (REQUIRED)
// 6. REAL polygonSpeed (REQUIRED)
//**********************************************************************

public class PolygonSt : IEquatable<PolygonSt>
{
public static UInt64 MsgId = 2;
	public PolygonMap[] polygonMap;
	public PolygonMap[] concaveMap;
	public ObstacleMap[] obstacleMap;
	public LineMap[] portalMap;
	public LineMap[] jumpMap;
	public Double polygonSpeed;

	public override bool Equals(object obj)
	{
		return obj is PolygonSt d && Equals(d);
	}
	public static bool operator ==(PolygonSt left, PolygonSt right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(PolygonSt left, PolygonSt right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(PolygonSt other)
	{
		return
			Enumerable.SequenceEqual(this.polygonMap, other.polygonMap) &&
			Enumerable.SequenceEqual(this.concaveMap, other.concaveMap) &&
			Enumerable.SequenceEqual(this.obstacleMap, other.obstacleMap) &&
			Enumerable.SequenceEqual(this.portalMap, other.portalMap) &&
			Enumerable.SequenceEqual(this.jumpMap, other.jumpMap) &&
			this.polygonSpeed == other.polygonSpeed;
	}
	public static void compose(ComposerBase composer, ICompose polygonMap, ICompose concaveMap, ICompose obstacleMap, ICompose portalMap, ICompose jumpMap, Double polygonSpeed)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, polygonMap, concaveMap, obstacleMap, portalMap, jumpMap, polygonSpeed);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, polygonMap, concaveMap, obstacleMap, portalMap, jumpMap, polygonSpeed);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, ICompose polygonMap, ICompose concaveMap, ICompose obstacleMap, ICompose portalMap, ICompose jumpMap, Double polygonSpeed)
	{
		composer.append( "{\n  ");
		composer.addNamePart("polygonMap");
		globalmq.marshalling.impl.json.composeParamToJson(composer, polygonMap);
		composer.append( ",\n  " );
		composer.addNamePart("concaveMap");
		globalmq.marshalling.impl.json.composeParamToJson(composer, concaveMap);
		composer.append( ",\n  " );
		composer.addNamePart("obstacleMap");
		globalmq.marshalling.impl.json.composeParamToJson(composer, obstacleMap);
		composer.append( ",\n  " );
		composer.addNamePart("portalMap");
		globalmq.marshalling.impl.json.composeParamToJson(composer, portalMap);
		composer.append( ",\n  " );
		composer.addNamePart("jumpMap");
		globalmq.marshalling.impl.json.composeParamToJson(composer, jumpMap);
		composer.append( ",\n  " );
		composer.addNamePart("polygonSpeed");
		composer.composeReal(polygonSpeed);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, ICompose polygonMap, ICompose concaveMap, ICompose obstacleMap, ICompose portalMap, ICompose jumpMap, Double polygonSpeed)
	{
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, polygonMap);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, concaveMap);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, obstacleMap);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, portalMap);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, jumpMap);
		composer.composeReal(polygonSpeed);
	}
	public static void parse(ParserBase parser, IParse polygonMap, IParse concaveMap, IParse obstacleMap, IParse portalMap, IParse jumpMap, ref Double polygonSpeed)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, polygonMap, concaveMap, obstacleMap, portalMap, jumpMap, ref polygonSpeed);
		else if (parser is JsonParser jsonP)
			parse(jsonP, polygonMap, concaveMap, obstacleMap, portalMap, jumpMap, ref polygonSpeed);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, IParse polygonMap, IParse concaveMap, IParse obstacleMap, IParse portalMap, IParse jumpMap, ref Double polygonSpeed)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "polygonMap" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, polygonMap);
			else if ( key == "concaveMap" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, concaveMap);
			else if ( key == "obstacleMap" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, obstacleMap);
			else if ( key == "portalMap" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, portalMap);
			else if ( key == "jumpMap" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, jumpMap);
			else if ( key == "polygonSpeed" )
				parser.parseReal(out polygonSpeed);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, IParse polygonMap, IParse concaveMap, IParse obstacleMap, IParse portalMap, IParse jumpMap, ref Double polygonSpeed)
	{
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, polygonMap);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, concaveMap);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, obstacleMap);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, portalMap);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, jumpMap);
		parser.parseReal(out polygonSpeed);
	}
	public static void compose(ComposerBase composer, PolygonSt msg)
	{
		compose(composer,
			polygonMap: new CollectionWrapperForComposing(
				() => { return msg.polygonMap.Length; },
				(ComposerBase composer, int ordinal) => { PolygonMap.compose(composer, msg.polygonMap[ordinal]); }),
			concaveMap: new CollectionWrapperForComposing(
				() => { return msg.concaveMap.Length; },
				(ComposerBase composer, int ordinal) => { PolygonMap.compose(composer, msg.concaveMap[ordinal]); }),
			obstacleMap: new CollectionWrapperForComposing(
				() => { return msg.obstacleMap.Length; },
				(ComposerBase composer, int ordinal) => { ObstacleMap.compose(composer, msg.obstacleMap[ordinal]); }),
			portalMap: new CollectionWrapperForComposing(
				() => { return msg.portalMap.Length; },
				(ComposerBase composer, int ordinal) => { LineMap.compose(composer, msg.portalMap[ordinal]); }),
			jumpMap: new CollectionWrapperForComposing(
				() => { return msg.jumpMap.Length; },
				(ComposerBase composer, int ordinal) => { LineMap.compose(composer, msg.jumpMap[ordinal]); }),
			polygonSpeed: msg.polygonSpeed
		);
	}
	public static void parse(ParserBase parser, out PolygonSt msg)
	{
		PolygonSt tmp = new PolygonSt();
		parse(parser,
			polygonMap: new CollectionWrapperForParsing(
				(int size) => { tmp.polygonMap = new PolygonMap[size]; },
				(ParserBase parser, int ordinal) => { PolygonMap.parse(parser, out tmp.polygonMap[ordinal]); }),
			concaveMap: new CollectionWrapperForParsing(
				(int size) => { tmp.concaveMap = new PolygonMap[size]; },
				(ParserBase parser, int ordinal) => { PolygonMap.parse(parser, out tmp.concaveMap[ordinal]); }),
			obstacleMap: new CollectionWrapperForParsing(
				(int size) => { tmp.obstacleMap = new ObstacleMap[size]; },
				(ParserBase parser, int ordinal) => { ObstacleMap.parse(parser, out tmp.obstacleMap[ordinal]); }),
			portalMap: new CollectionWrapperForParsing(
				(int size) => { tmp.portalMap = new LineMap[size]; },
				(ParserBase parser, int ordinal) => { LineMap.parse(parser, out tmp.portalMap[ordinal]); }),
			jumpMap: new CollectionWrapperForParsing(
				(int size) => { tmp.jumpMap = new LineMap[size]; },
				(ParserBase parser, int ordinal) => { LineMap.parse(parser, out tmp.jumpMap[ordinal]); }),
			polygonSpeed: ref tmp.polygonSpeed
		);
	msg = tmp;
	}
	public static void composeMessage(ComposerBase composer, ICompose polygonMap, ICompose concaveMap, ICompose obstacleMap, ICompose portalMap, ICompose jumpMap, Double polygonSpeed)
	{
		if (composer is GmqComposer gmqC)
		{
			gmqC.composeUnsignedInteger(MsgId);
			compose(gmqC, polygonMap, concaveMap, obstacleMap, portalMap, jumpMap, polygonSpeed);
		}
		else if (composer is JsonComposer jsonC)
		{
			jsonC.append("{\n  ");
			jsonC.addNamePart("msgid");
			jsonC.composeUnsignedInteger(MsgId);
			jsonC.append(",\n  ");
			jsonC.addNamePart("msgbody");
			compose(jsonC, polygonMap, concaveMap, obstacleMap, portalMap, jumpMap, polygonSpeed);
			jsonC.append("\n}");
		}
		else
			throw new ArgumentException();
	}
} // class PolygonSt

//**********************************************************************
// MESSAGE "point" NONEXTENDABLE Targets: GMQ (1 parameters)
// 1. STRUCT point pt (REQUIRED)
//**********************************************************************

public class point : IEquatable<point>
{
public static UInt64 MsgId = 4;
	public point pt;

	public override bool Equals(object obj)
	{
		return obj is point d && Equals(d);
	}
	public static bool operator ==(point left, point right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(point left, point right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(point other)
	{
		return
			this.pt.Equals(other.pt);
	}
	public static void compose(ComposerBase composer, ICompose pt)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, pt);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, pt);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, ICompose pt)
	{
		composer.append( "{\n  ");
		composer.addNamePart("pt");
		globalmq.marshalling.impl.json.composeParamToJson(composer, pt);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, ICompose pt)
	{
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, pt);
	}
	public static void parse(ParserBase parser, IParse pt)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, pt);
		else if (parser is JsonParser jsonP)
			parse(jsonP, pt);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, IParse pt)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "pt" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, pt);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, IParse pt)
	{
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, pt);
	}
	public static void compose(ComposerBase composer, point msg)
	{
		compose(composer,
			pt: new MessageWrapperForComposing(
				(ComposerBase composer) => { point.compose(composer, msg.pt); })
		);
	}
	public static void parse(ParserBase parser, out point msg)
	{
		point tmp = new point();
		parse(parser,
			pt: new MessageWrapperForParsing(
				(ParserBase parser) => { point.parse(parser, out tmp.pt); })
		);
	msg = tmp;
	}
	public static void composeMessage(ComposerBase composer, ICompose pt)
	{
		if (composer is GmqComposer gmqC)
		{
			gmqC.composeUnsignedInteger(MsgId);
			compose(gmqC, pt);
		}
		else if (composer is JsonComposer jsonC)
		{
			jsonC.append("{\n  ");
			jsonC.addNamePart("msgid");
			jsonC.composeUnsignedInteger(MsgId);
			jsonC.append(",\n  ");
			jsonC.addNamePart("msgbody");
			compose(jsonC, pt);
			jsonC.append("\n}");
		}
		else
			throw new ArgumentException();
	}
} // class point

//**********************************************************************
// MESSAGE "point3D" NONEXTENDABLE Targets: GMQ (1 parameters)
// 1. STRUCT point3D pt (REQUIRED)
//**********************************************************************

public class point3D : IEquatable<point3D>
{
public static UInt64 MsgId = 5;
	public point3D pt;

	public override bool Equals(object obj)
	{
		return obj is point3D d && Equals(d);
	}
	public static bool operator ==(point3D left, point3D right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(point3D left, point3D right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(point3D other)
	{
		return
			this.pt.Equals(other.pt);
	}
	public static void compose(ComposerBase composer, ICompose pt)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, pt);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, pt);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, ICompose pt)
	{
		composer.append( "{\n  ");
		composer.addNamePart("pt");
		globalmq.marshalling.impl.json.composeParamToJson(composer, pt);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, ICompose pt)
	{
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, pt);
	}
	public static void parse(ParserBase parser, IParse pt)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, pt);
		else if (parser is JsonParser jsonP)
			parse(jsonP, pt);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, IParse pt)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "pt" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, pt);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, IParse pt)
	{
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, pt);
	}
	public static void compose(ComposerBase composer, point3D msg)
	{
		compose(composer,
			pt: new MessageWrapperForComposing(
				(ComposerBase composer) => { point3D.compose(composer, msg.pt); })
		);
	}
	public static void parse(ParserBase parser, out point3D msg)
	{
		point3D tmp = new point3D();
		parse(parser,
			pt: new MessageWrapperForParsing(
				(ParserBase parser) => { point3D.parse(parser, out tmp.pt); })
		);
	msg = tmp;
	}
	public static void composeMessage(ComposerBase composer, ICompose pt)
	{
		if (composer is GmqComposer gmqC)
		{
			gmqC.composeUnsignedInteger(MsgId);
			compose(gmqC, pt);
		}
		else if (composer is JsonComposer jsonC)
		{
			jsonC.append("{\n  ");
			jsonC.addNamePart("msgid");
			jsonC.composeUnsignedInteger(MsgId);
			jsonC.append(",\n  ");
			jsonC.addNamePart("msgbody");
			compose(jsonC, pt);
			jsonC.append("\n}");
		}
		else
			throw new ArgumentException();
	}
} // class point3D

} // class infrastructural

public class test_gmq
{
	public static void handleMessage( BufferT buffer, MessageHandlerArray handlers )
	{
		ReadIteratorT riter = buffer.getReadIterator();
		handleMessage(riter, handlers);
	}
public static void handleMessage( ReadIteratorT riter, MessageHandlerArray handlers )
{
	GmqParser parser = new GmqParser( riter );
	globalmq.marshalling.impl.gmq.handleMessage(parser, handlers);
}
//**********************************************************************
// MESSAGE "message_one" Targets: GMQ (10 parameters)
// 1. INTEGER firstParam (REQUIRED)
// 2. VECTOR<INTEGER> secondParam (REQUIRED)
// 3. VECTOR< STRUCT point3D> thirdParam (REQUIRED)
// 4. UINTEGER forthParam (REQUIRED)
// 5. CHARACTER_STRING fifthParam (REQUIRED)
// 6. VECTOR<NONEXTENDABLE STRUCT point> sixthParam (REQUIRED)
// 7. REAL seventhParam (REQUIRED)
// 8. STRUCT NONEXTENDABLE point eighthParam (REQUIRED)
// 9. STRUCT point3D ninethParam (REQUIRED)
// 10. VECTOR<REAL> tenthParam (REQUIRED)
//**********************************************************************

public class message_one : IEquatable<message_one>
{
public static UInt64 MsgId = 3;
	public Int64 firstParam;
	public Int64[] secondParam;
	public point3D[] thirdParam;
	public UInt64 forthParam;
	public String fifthParam;
	public point[] sixthParam;
	public Double seventhParam;
	public point eighthParam;
	public point3D ninethParam;
	public Double[] tenthParam;

	public override bool Equals(object obj)
	{
		return obj is message_one d && Equals(d);
	}
	public static bool operator ==(message_one left, message_one right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(message_one left, message_one right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(message_one other)
	{
		return
			this.firstParam == other.firstParam &&
			Enumerable.SequenceEqual(this.secondParam, other.secondParam) &&
			Enumerable.SequenceEqual(this.thirdParam, other.thirdParam) &&
			this.forthParam == other.forthParam &&
			this.fifthParam == other.fifthParam &&
			Enumerable.SequenceEqual(this.sixthParam, other.sixthParam) &&
			this.seventhParam == other.seventhParam &&
			this.eighthParam.Equals(other.eighthParam) &&
			this.ninethParam.Equals(other.ninethParam) &&
			Enumerable.SequenceEqual(this.tenthParam, other.tenthParam);
	}
	public static void compose(ComposerBase composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam, UInt64 forthParam, String fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam, UInt64 forthParam, String fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
	{
		composer.append( "{\n  ");
		composer.addNamePart("firstParam");
		composer.composeSignedInteger(firstParam);
		composer.append( ",\n  " );
		composer.addNamePart("secondParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, secondParam);
		composer.append( ",\n  " );
		composer.addNamePart("thirdParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, thirdParam);
		composer.append( ",\n  " );
		composer.addNamePart("forthParam");
		composer.composeUnsignedInteger(forthParam);
		composer.append( ",\n  " );
		composer.addNamePart("fifthParam");
		composer.composeString(fifthParam);
		composer.append( ",\n  " );
		composer.addNamePart("sixthParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, sixthParam);
		composer.append( ",\n  " );
		composer.addNamePart("seventhParam");
		composer.composeReal(seventhParam);
		composer.append( ",\n  " );
		composer.addNamePart("eighthParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, eighthParam);
		composer.append( ",\n  " );
		composer.addNamePart("ninethParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, ninethParam);
		composer.append( ",\n  " );
		composer.addNamePart("tenthParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, tenthParam);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam, UInt64 forthParam, String fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
	{
		composer.composeSignedInteger(firstParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, secondParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, thirdParam);
		composer.composeUnsignedInteger(forthParam);
		composer.composeString(fifthParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, sixthParam);
		composer.composeReal(seventhParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, eighthParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, ninethParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, tenthParam);
	}
	public static void parse(ParserBase parser, ref Int64 firstParam, IParse secondParam, IParse thirdParam, ref UInt64 forthParam, ref String fifthParam, IParse sixthParam, ref Double seventhParam, IParse eighthParam, IParse ninethParam, IParse tenthParam)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, ref firstParam, secondParam, thirdParam, ref forthParam, ref fifthParam, sixthParam, ref seventhParam, eighthParam, ninethParam, tenthParam);
		else if (parser is JsonParser jsonP)
			parse(jsonP, ref firstParam, secondParam, thirdParam, ref forthParam, ref fifthParam, sixthParam, ref seventhParam, eighthParam, ninethParam, tenthParam);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, ref Int64 firstParam, IParse secondParam, IParse thirdParam, ref UInt64 forthParam, ref String fifthParam, IParse sixthParam, ref Double seventhParam, IParse eighthParam, IParse ninethParam, IParse tenthParam)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "firstParam" )
				parser.parseSignedInteger(out firstParam);
			else if ( key == "secondParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, secondParam);
			else if ( key == "thirdParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, thirdParam);
			else if ( key == "forthParam" )
				parser.parseUnsignedInteger(out forthParam);
			else if ( key == "fifthParam" )
				parser.parseString(out fifthParam);
			else if ( key == "sixthParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, sixthParam);
			else if ( key == "seventhParam" )
				parser.parseReal(out seventhParam);
			else if ( key == "eighthParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, eighthParam);
			else if ( key == "ninethParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, ninethParam);
			else if ( key == "tenthParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, tenthParam);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, ref Int64 firstParam, IParse secondParam, IParse thirdParam, ref UInt64 forthParam, ref String fifthParam, IParse sixthParam, ref Double seventhParam, IParse eighthParam, IParse ninethParam, IParse tenthParam)
	{
		parser.parseSignedInteger(out firstParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, secondParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, thirdParam);
		parser.parseUnsignedInteger(out forthParam);
		parser.parseString(out fifthParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, sixthParam);
		parser.parseReal(out seventhParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, eighthParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, ninethParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, tenthParam);
	}
	public static void compose(ComposerBase composer, message_one msg)
	{
		compose(composer,
			firstParam: msg.firstParam,
			secondParam: SimpleTypeCollection.makeComposer(msg.secondParam),
			thirdParam: new CollectionWrapperForComposing(
				() => { return msg.thirdParam.Length; },
				(ComposerBase composer, int ordinal) => { point3D.compose(composer, msg.thirdParam[ordinal]); }),
			forthParam: msg.forthParam,
			fifthParam: msg.fifthParam,
			sixthParam: new CollectionWrapperForComposing(
				() => { return msg.sixthParam.Length; },
				(ComposerBase composer, int ordinal) => { point.compose(composer, msg.sixthParam[ordinal]); }),
			seventhParam: msg.seventhParam,
			eighthParam: new MessageWrapperForComposing(
				(ComposerBase composer) => { point.compose(composer, msg.eighthParam); }),
			ninethParam: new MessageWrapperForComposing(
				(ComposerBase composer) => { point3D.compose(composer, msg.ninethParam); }),
			tenthParam: SimpleTypeCollection.makeComposer(msg.tenthParam)
		);
	}
	public static void parse(ParserBase parser, out message_one msg)
	{
		message_one tmp = new message_one();
		parse(parser,
			firstParam: ref tmp.firstParam,
			secondParam: new CollectionWrapperForParsing(
				(int size) => { tmp.secondParam = new Int64[size]; },
				(ParserBase parser, int ordinal) => { parser.parseSignedInteger(out tmp.secondParam[ordinal]); }),
			thirdParam: new CollectionWrapperForParsing(
				(int size) => { tmp.thirdParam = new point3D[size]; },
				(ParserBase parser, int ordinal) => { point3D.parse(parser, out tmp.thirdParam[ordinal]); }),
			forthParam: ref tmp.forthParam,
			fifthParam: ref tmp.fifthParam,
			sixthParam: new CollectionWrapperForParsing(
				(int size) => { tmp.sixthParam = new point[size]; },
				(ParserBase parser, int ordinal) => { point.parse(parser, out tmp.sixthParam[ordinal]); }),
			seventhParam: ref tmp.seventhParam,
			eighthParam: new MessageWrapperForParsing(
				(ParserBase parser) => { point.parse(parser, out tmp.eighthParam); }),
			ninethParam: new MessageWrapperForParsing(
				(ParserBase parser) => { point3D.parse(parser, out tmp.ninethParam); }),
			tenthParam: new CollectionWrapperForParsing(
				(int size) => { tmp.tenthParam = new Double[size]; },
				(ParserBase parser, int ordinal) => { parser.parseReal(out tmp.tenthParam[ordinal]); })
		);
	msg = tmp;
	}
	public static void composeMessage(ComposerBase composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam, UInt64 forthParam, String fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
	{
		if (composer is GmqComposer gmqC)
		{
			gmqC.composeUnsignedInteger(MsgId);
			compose(gmqC, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
		}
		else if (composer is JsonComposer jsonC)
		{
			jsonC.append("{\n  ");
			jsonC.addNamePart("msgid");
			jsonC.composeUnsignedInteger(MsgId);
			jsonC.append(",\n  ");
			jsonC.addNamePart("msgbody");
			compose(jsonC, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
			jsonC.append("\n}");
		}
		else
			throw new ArgumentException();
	}
} // class message_one

} // class test_gmq

public class test_json
{
	public static void handleMessage( BufferT buffer, MessageHandlerArray handlers )
	{
		ReadIteratorT riter = buffer.getReadIterator();
		handleMessage(riter, handlers);
	}
public static void handleMessage( ReadIteratorT riter, MessageHandlerArray handlers )
{
	JsonParser parser = new JsonParser( riter );
	globalmq.marshalling.impl.json.handleMessage(parser, handlers);
}
//**********************************************************************
// MESSAGE "message_one" Targets: JSON (10 parameters)
// 1. INTEGER firstParam (REQUIRED)
// 2. VECTOR<INTEGER> secondParam (REQUIRED)
// 3. VECTOR< STRUCT point3D> thirdParam (REQUIRED)
// 4. UINTEGER forthParam (REQUIRED)
// 5. CHARACTER_STRING fifthParam (REQUIRED)
// 6. VECTOR<NONEXTENDABLE STRUCT point> sixthParam (REQUIRED)
// 7. REAL seventhParam (REQUIRED)
// 8. STRUCT NONEXTENDABLE point eighthParam (REQUIRED)
// 9. STRUCT point3D ninethParam (REQUIRED)
// 10. VECTOR<REAL> tenthParam (REQUIRED)
//**********************************************************************

public class message_one : IEquatable<message_one>
{
public static UInt64 MsgId = 3;
	public Int64 firstParam;
	public Int64[] secondParam;
	public point3D[] thirdParam;
	public UInt64 forthParam;
	public String fifthParam;
	public point[] sixthParam;
	public Double seventhParam;
	public point eighthParam;
	public point3D ninethParam;
	public Double[] tenthParam;

	public override bool Equals(object obj)
	{
		return obj is message_one d && Equals(d);
	}
	public static bool operator ==(message_one left, message_one right)
	{
		return left.Equals(right);
	}
	public static bool operator !=(message_one left, message_one right)
	{
		return !(left == right);
	}
	public override int GetHashCode()
	{
		// TODO
		throw new InvalidOperationException();
	}
	public bool Equals(message_one other)
	{
		return
			this.firstParam == other.firstParam &&
			Enumerable.SequenceEqual(this.secondParam, other.secondParam) &&
			Enumerable.SequenceEqual(this.thirdParam, other.thirdParam) &&
			this.forthParam == other.forthParam &&
			this.fifthParam == other.fifthParam &&
			Enumerable.SequenceEqual(this.sixthParam, other.sixthParam) &&
			this.seventhParam == other.seventhParam &&
			this.eighthParam.Equals(other.eighthParam) &&
			this.ninethParam.Equals(other.ninethParam) &&
			Enumerable.SequenceEqual(this.tenthParam, other.tenthParam);
	}
	public static void compose(ComposerBase composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam, UInt64 forthParam, String fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
	{
		if (composer is GmqComposer gmqC)
			compose(gmqC, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
		else if (composer is JsonComposer jsonC)
			compose(jsonC, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
		else
			throw new ArgumentException();
	}
	public static void compose(JsonComposer composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam, UInt64 forthParam, String fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
	{
		composer.append( "{\n  ");
		composer.addNamePart("firstParam");
		composer.composeSignedInteger(firstParam);
		composer.append( ",\n  " );
		composer.addNamePart("secondParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, secondParam);
		composer.append( ",\n  " );
		composer.addNamePart("thirdParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, thirdParam);
		composer.append( ",\n  " );
		composer.addNamePart("forthParam");
		composer.composeUnsignedInteger(forthParam);
		composer.append( ",\n  " );
		composer.addNamePart("fifthParam");
		composer.composeString(fifthParam);
		composer.append( ",\n  " );
		composer.addNamePart("sixthParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, sixthParam);
		composer.append( ",\n  " );
		composer.addNamePart("seventhParam");
		composer.composeReal(seventhParam);
		composer.append( ",\n  " );
		composer.addNamePart("eighthParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, eighthParam);
		composer.append( ",\n  " );
		composer.addNamePart("ninethParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, ninethParam);
		composer.append( ",\n  " );
		composer.addNamePart("tenthParam");
		globalmq.marshalling.impl.json.composeParamToJson(composer, tenthParam);
		composer.append( "\n}" );
	}
	public static void compose(GmqComposer composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam, UInt64 forthParam, String fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
	{
		composer.composeSignedInteger(firstParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, secondParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, thirdParam);
		composer.composeUnsignedInteger(forthParam);
		composer.composeString(fifthParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, sixthParam);
		composer.composeReal(seventhParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, eighthParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, ninethParam);
		globalmq.marshalling.impl.gmq.composeParamToGmq(composer, tenthParam);
	}
	public static void parse(ParserBase parser, ref Int64 firstParam, IParse secondParam, IParse thirdParam, ref UInt64 forthParam, ref String fifthParam, IParse sixthParam, ref Double seventhParam, IParse eighthParam, IParse ninethParam, IParse tenthParam)
	{
		if (parser is GmqParser gmqP)
			parse(gmqP, ref firstParam, secondParam, thirdParam, ref forthParam, ref fifthParam, sixthParam, ref seventhParam, eighthParam, ninethParam, tenthParam);
		else if (parser is JsonParser jsonP)
			parse(jsonP, ref firstParam, secondParam, thirdParam, ref forthParam, ref fifthParam, sixthParam, ref seventhParam, eighthParam, ninethParam, tenthParam);
		else
			throw new ArgumentException();
	}
	public static void parse(JsonParser parser, ref Int64 firstParam, IParse secondParam, IParse thirdParam, ref UInt64 forthParam, ref String fifthParam, IParse sixthParam, ref Double seventhParam, IParse eighthParam, IParse ninethParam, IParse tenthParam)
	{
		parser.skipDelimiter( '{' );
		while (true)
		{
			string key;
			parser.readKeyFromJson( out key );
			if ( key == "firstParam" )
				parser.parseSignedInteger(out firstParam);
			else if ( key == "secondParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, secondParam);
			else if ( key == "thirdParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, thirdParam);
			else if ( key == "forthParam" )
				parser.parseUnsignedInteger(out forthParam);
			else if ( key == "fifthParam" )
				parser.parseString(out fifthParam);
			else if ( key == "sixthParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, sixthParam);
			else if ( key == "seventhParam" )
				parser.parseReal(out seventhParam);
			else if ( key == "eighthParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, eighthParam);
			else if ( key == "ninethParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, ninethParam);
			else if ( key == "tenthParam" )
				globalmq.marshalling.impl.json.parseJsonEntry(parser, tenthParam);

			parser.skipSpacesEtc();
			if ( parser.isDelimiter( ',' ) )
			{
				parser.skipDelimiter( ',' );
				continue;
			}
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				break;
			}
			throw new FormatException(); // bad format
		}
	}
	public static void parse(GmqParser parser, ref Int64 firstParam, IParse secondParam, IParse thirdParam, ref UInt64 forthParam, ref String fifthParam, IParse sixthParam, ref Double seventhParam, IParse eighthParam, IParse ninethParam, IParse tenthParam)
	{
		parser.parseSignedInteger(out firstParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, secondParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, thirdParam);
		parser.parseUnsignedInteger(out forthParam);
		parser.parseString(out fifthParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, sixthParam);
		parser.parseReal(out seventhParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, eighthParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, ninethParam);
		globalmq.marshalling.impl.gmq.parseGmqParam(parser, tenthParam);
	}
	public static void compose(ComposerBase composer, message_one msg)
	{
		compose(composer,
			firstParam: msg.firstParam,
			secondParam: SimpleTypeCollection.makeComposer(msg.secondParam),
			thirdParam: new CollectionWrapperForComposing(
				() => { return msg.thirdParam.Length; },
				(ComposerBase composer, int ordinal) => { point3D.compose(composer, msg.thirdParam[ordinal]); }),
			forthParam: msg.forthParam,
			fifthParam: msg.fifthParam,
			sixthParam: new CollectionWrapperForComposing(
				() => { return msg.sixthParam.Length; },
				(ComposerBase composer, int ordinal) => { point.compose(composer, msg.sixthParam[ordinal]); }),
			seventhParam: msg.seventhParam,
			eighthParam: new MessageWrapperForComposing(
				(ComposerBase composer) => { point.compose(composer, msg.eighthParam); }),
			ninethParam: new MessageWrapperForComposing(
				(ComposerBase composer) => { point3D.compose(composer, msg.ninethParam); }),
			tenthParam: SimpleTypeCollection.makeComposer(msg.tenthParam)
		);
	}
	public static void parse(ParserBase parser, out message_one msg)
	{
		message_one tmp = new message_one();
		parse(parser,
			firstParam: ref tmp.firstParam,
			secondParam: new CollectionWrapperForParsing(
				(int size) => { tmp.secondParam = new Int64[size]; },
				(ParserBase parser, int ordinal) => { parser.parseSignedInteger(out tmp.secondParam[ordinal]); }),
			thirdParam: new CollectionWrapperForParsing(
				(int size) => { tmp.thirdParam = new point3D[size]; },
				(ParserBase parser, int ordinal) => { point3D.parse(parser, out tmp.thirdParam[ordinal]); }),
			forthParam: ref tmp.forthParam,
			fifthParam: ref tmp.fifthParam,
			sixthParam: new CollectionWrapperForParsing(
				(int size) => { tmp.sixthParam = new point[size]; },
				(ParserBase parser, int ordinal) => { point.parse(parser, out tmp.sixthParam[ordinal]); }),
			seventhParam: ref tmp.seventhParam,
			eighthParam: new MessageWrapperForParsing(
				(ParserBase parser) => { point.parse(parser, out tmp.eighthParam); }),
			ninethParam: new MessageWrapperForParsing(
				(ParserBase parser) => { point3D.parse(parser, out tmp.ninethParam); }),
			tenthParam: new CollectionWrapperForParsing(
				(int size) => { tmp.tenthParam = new Double[size]; },
				(ParserBase parser, int ordinal) => { parser.parseReal(out tmp.tenthParam[ordinal]); })
		);
	msg = tmp;
	}
	public static void composeMessage(ComposerBase composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam, UInt64 forthParam, String fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
	{
		if (composer is GmqComposer gmqC)
		{
			gmqC.composeUnsignedInteger(MsgId);
			compose(gmqC, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
		}
		else if (composer is JsonComposer jsonC)
		{
			jsonC.append("{\n  ");
			jsonC.addNamePart("msgid");
			jsonC.composeUnsignedInteger(MsgId);
			jsonC.append(",\n  ");
			jsonC.addNamePart("msgbody");
			compose(jsonC, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
			jsonC.append("\n}");
		}
		else
			throw new ArgumentException();
	}
} // class message_one

} // class test_json


} // namespace mtest

