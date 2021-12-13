﻿/*
 * Copyright (c) 2021, OLogN Technologies AG
 * All rights reserved.
 * 
 */

// mb: all code in this file should be automatically generated by 
// idl compiler. We are currently hand writting it.





//**********************************************************************
// STRUCT "point" NONEXTENDABLE Targets: JSON GMQ (2 parameters)
// 1. INTEGER x (REQUIRED)
// 2. INTEGER y (REQUIRED)

//**********************************************************************

using globalmq.marshalling;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

public class mtest
{
    public static bool DebugWithJson { get; set; }
    public static ComposerBase makeComposer(BufferT buff)
    {
        if (DebugWithJson)
            return new JsonComposer(buff);
        else
            return new GmqComposer(buff);
    }
    public static ParserBase makeParser(ReadIterT it)
    {
        if (DebugWithJson)
            return new JsonParser(it);
        else
            return new GmqParser(it);
    }
    public enum MessageName  { message_one = 1, message_two = 2 }
    public static void STRUCT_point_compose(ComposerBase composer, Int64 x, Int64 y)
    {
        if (composer is GmqComposer gmq)
        {
            gmq.composeSignedInteger(x);
            gmq.composeSignedInteger(y);
        }
        else if (composer is JsonComposer json)
        {
            json.append("{\n  ");
            json.addNamePart("x");
            json.composeSignedInteger(x);
            json.append(",\n  ");
            json.addNamePart("y");
            json.composeSignedInteger(y);
            json.append("\n}");
        }
        else
        {
            throw new Exception();//TODO unknown composer
        }
    }

    public static void STRUCT_point_parse(ParserBase parser, out Int64 x, out Int64 y)
    {
        if (parser is GmqParser gmq)
        {
            gmq.parseSignedInteger(out x);
            gmq.parseSignedInteger(out y);
        }
        else if (parser is JsonParser json)
        {
            x = 0;
            y = 0;
            json.skipDelimiter('{');
            while (true)
            {
                string key;
                json.readKeyFromJson(out key);
                if (key == "x")
                    json.parseSignedInteger(out x);
                else if (key == "y")
                    json.parseSignedInteger(out y);

                json.skipSpacesEtc();
                if (json.isDelimiter(','))
                {
                    json.skipDelimiter(',');
                    continue;
                }
                if (json.isDelimiter('}'))
                {
                    json.skipDelimiter('}');
                    break;
                }
                throw new Exception(); // bad format
            }
        }
        else
        {
            throw new Exception();//TODO unknown parser
        }
    }

    //**********************************************************************
    // STRUCT "point3D" Targets: JSON GMQ (3 parameters)
    // 1. INTEGER x (REQUIRED)
    // 2. INTEGER y (REQUIRED)
    // 3. INTEGER z (REQUIRED)

    //**********************************************************************

    public static void STRUCT_point3D_compose(ComposerBase composer, Int64 x, Int64 y, Int64 z)
    {
        if (composer is GmqComposer gmq)
        {
            gmq.composeSignedInteger(x);
            gmq.composeSignedInteger(y);
            gmq.composeSignedInteger(z);
        }
        else if (composer is JsonComposer json)
        {
            json.append("{\n  ");
            json.addNamePart("x");
            json.composeSignedInteger(x);
            json.append(",\n  ");
            json.addNamePart("y");
            json.composeSignedInteger(y);
            json.append(",\n  ");
            json.addNamePart("z");
            json.composeSignedInteger(z);
            json.append("\n}");
        }
        else
        {
            throw new Exception();//TODO unknown composer
        }
    }

    public static void STRUCT_point3D_parse(ParserBase parser, out Int64 x, out Int64 y, out Int64 z)
    {
        if (parser is GmqParser gmq)
        {
            gmq.parseSignedInteger(out x);
            gmq.parseSignedInteger(out y);
            gmq.parseSignedInteger(out z);
        }
        else if (parser is JsonParser json)
        {
            x = 0;
            y = 0;
            z = 0;
            json.skipDelimiter('{');
            while (true)
            {
                string key;
                json.readKeyFromJson(out key);
                if (key == "x")
                    json.parseSignedInteger(out x);
                else if (key == "y")
                    json.parseSignedInteger(out y);
                else if (key == "z")
                    json.parseSignedInteger(out z);

                json.skipSpacesEtc();
                if (json.isDelimiter(','))
                {
                    json.skipDelimiter(',');
                    continue;
                }
                if (json.isDelimiter('}'))
                {
                    json.skipDelimiter('}');
                    break;
                }
                throw new Exception(); // bad format
            }
        }
        else
        {
            throw new Exception();//TODO unknown parser
        }

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
    public struct Point : IEquatable<Point>
    {
        public Int64 x;
        public Int64 y;

        public override bool Equals(object obj)
        {
            return obj is Point point && Equals(point);
        }

        public bool Equals(Point other)
        {
            return x == other.x &&
                   y == other.y;
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(x, y);
        }

        public static bool operator ==(Point left, Point right)
        {
            return left.Equals(right);
        }

        public static bool operator !=(Point left, Point right)
        {
            return !(left == right);
        }
    }
    public struct Point3D : IEquatable<Point3D>
    {
        public Int64 x;
        public Int64 y;
        public Int64 z;

        public override bool Equals(object obj)
        {
            return obj is Point3D d && Equals(d);
        }

        public bool Equals(Point3D other)
        {
            return x == other.x &&
                   y == other.y &&
                   z == other.z;
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(x, y, z);
        }

        public static bool operator ==(Point3D left, Point3D right)
        {
            return left.Equals(right);
        }

        public static bool operator !=(Point3D left, Point3D right)
        {
            return !(left == right);
        }
    }
    public class message_one
    {
        public Int64 firstParam;
        public Int64[] secondParam;
        public Point3D[] thirdParam;
        public UInt64 forthParam;
        public string fifthParam;
        public Point[] sixthParam;
        public Double seventhParam;
        public Point eighthParam;
        public Point3D ninethParam;
        public Double[] tenthParam;

        public override bool Equals(object obj)
        {
            return obj is message_one one &&
                   firstParam == one.firstParam &&
                   Enumerable.SequenceEqual(secondParam, one.secondParam) &&
                   Enumerable.SequenceEqual(thirdParam, one.thirdParam) &&
                   forthParam == one.forthParam &&
                   fifthParam == one.fifthParam &&
                   Enumerable.SequenceEqual(sixthParam, one.sixthParam) &&
                   seventhParam == one.seventhParam &&
                   eighthParam.Equals(one.eighthParam) &&
                   ninethParam.Equals(one.ninethParam) &&
                   Enumerable.SequenceEqual(tenthParam, one.tenthParam);
        }

        public override int GetHashCode()
        {
            HashCode hash = new HashCode();
            hash.Add(firstParam);
            hash.Add(secondParam);
            hash.Add(thirdParam);
            hash.Add(forthParam);
            hash.Add(fifthParam);
            hash.Add(sixthParam);
            hash.Add(seventhParam);
            hash.Add(eighthParam);
            hash.Add(ninethParam);
            hash.Add(tenthParam);
            return hash.ToHashCode();
        }
    }

    public static void MESSAGE_message_one_compose(ComposerBase composer, message_one msg)
    {
        MESSAGE_message_one_compose(composer,
            firstParam: msg.firstParam,
            secondParam: SimpleTypeCollection.makeComposer(msg.secondParam),
            thirdParam: new CollectionWrapperForComposing(() => { return msg.thirdParam.Length; }, (ComposerBase composer, int ordinal) => { mtest.STRUCT_point3D_compose(composer, x: msg.thirdParam[ordinal].x, y: msg.thirdParam[ordinal].y, z: msg.thirdParam[ordinal].z); }),
            forthParam: msg.forthParam,
            fifthParam: msg.fifthParam,
            sixthParam: new CollectionWrapperForComposing(() => { return msg.sixthParam.Length; }, (ComposerBase composer, int ordinal) => { mtest.STRUCT_point_compose(composer, x: msg.sixthParam[ordinal].x, y: msg.sixthParam[ordinal].y); }),
            seventhParam: msg.seventhParam,
            eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.STRUCT_point_compose(composer, x: msg.eighthParam.x, y: msg.eighthParam.y); }),
            ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.STRUCT_point3D_compose(composer, x: msg.ninethParam.x, y: msg.ninethParam.y, z: msg.ninethParam.z); }),
            tenthParam: SimpleTypeCollection.makeComposer( msg.tenthParam )
            );
    }
    public static void MESSAGE_message_one_compose(ComposerBase composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam,
            UInt64 forthParam, string fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
    {
        if (composer is GmqComposer gmq)
            MESSAGE_message_one_compose(gmq, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
        else if (composer is JsonComposer json)
            MESSAGE_message_one_compose(json, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam, seventhParam, eighthParam, ninethParam, tenthParam);
        else
            throw new Exception(); //TODO
    }

    public static void MESSAGE_message_one_compose(JsonComposer composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam,
            UInt64 forthParam, string fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
    {

	    composer.append( "{\n  " );
        composer.addNamePart("firstParam");
	    composer.composeSignedInteger(firstParam);
	    composer.append( ",\n  ");
        composer.addNamePart("secondParam");
        json.composeParamToJson(composer, secondParam);
	    composer.append( ",\n  ");
        composer.addNamePart("thirdParam");
        json.composeParamToJson(composer, thirdParam);
        composer.append( ",\n  ");
        composer.addNamePart("forthParam");
        composer.composeUnsignedInteger(forthParam);
	    composer.append( ",\n  " );
        composer.addNamePart("fifthParam");
        composer.composeString(fifthParam);
	    composer.append( ",\n  " );
        composer.addNamePart("sixthParam");
        json.composeParamToJson(composer, sixthParam);
	    composer.append( ",\n  " );
        composer.addNamePart("seventhParam");
        composer.composeReal(seventhParam);
        composer.append( ",\n  " );
        composer.addNamePart("eighthParam");
        json.composeParamToJson(composer, eighthParam);
	    composer.append( ",\n  " );
        composer.addNamePart("ninethParam");
        json.composeParamToJson(composer, ninethParam);
        composer.append( ",\n  " );
        composer.addNamePart("tenthParam");
        json.composeParamToJson(composer, tenthParam);
        composer.append( "\n}" );
}

    public static void MESSAGE_message_one_compose(GmqComposer composer, Int64 firstParam, ICompose secondParam, ICompose thirdParam,
        UInt64 forthParam, string fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam, ICompose ninethParam, ICompose tenthParam)
    {
        composer.composeSignedInteger(firstParam);
        gmq.composeParamToGmq(composer, secondParam);
        gmq.composeParamToGmq(composer, thirdParam);
        composer.composeUnsignedInteger(forthParam);
        composer.composeString(fifthParam);
        gmq.composeParamToGmq(composer, sixthParam);
        composer.composeReal(seventhParam);
        gmq.composeParamToGmq(composer, eighthParam);
        gmq.composeParamToGmq(composer, ninethParam);
        gmq.composeParamToGmq(composer, tenthParam);
    }

    public static void MESSAGE_message_one_parse(ParserBase parser, message_one msg)
    {
        mtest.MESSAGE_message_one_parse(parser,
            firstParam: ref msg.firstParam,
            secondParam: new CollectionWrapperForParsing((int size) => { msg.secondParam = new Int64[size]; }, (ParserBase parser, int ordinal) => { parser.parseSignedInteger(out msg.secondParam[ordinal]); }),
            thirdParam: new CollectionWrapperForParsing((int size) => { msg.thirdParam = new Point3D[size]; }, (ParserBase parser, int ordinal) => { Point3D value; mtest.STRUCT_point3D_parse(parser, x: out value.x, y: out value.y, z: out value.z); msg.thirdParam[ordinal] = value; }),
            forthParam: ref msg.forthParam,
            fifthParam: ref msg.fifthParam,
            sixthParam: new CollectionWrapperForParsing((int size) => { msg.sixthParam = new Point[size]; }, (ParserBase parser, int ordinal) => { Point value; mtest.STRUCT_point_parse(parser, x: out value.x, y: out value.y); msg.sixthParam[ordinal] = value; }),
            seventhParam: ref msg.seventhParam,
            eighthParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.STRUCT_point_parse(parser, x: out msg.eighthParam.x, y: out msg.eighthParam.y); }),
            ninethParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.STRUCT_point3D_parse(parser, x: out msg.ninethParam.x, y: out msg.ninethParam.y, z: out msg.ninethParam.z); }),
            tenthParam: new CollectionWrapperForParsing((int size) => { msg.tenthParam = new Double[size]; }, (ParserBase parser, int ordinal) => { parser.parseReal(out msg.tenthParam[ordinal]); })
           );
    }

    public static void MESSAGE_message_one_parse(ParserBase parser, ref Int64 firstParam, IParse secondParam, IParse thirdParam,
            ref UInt64 forthParam, ref string fifthParam, IParse sixthParam, ref Double seventhParam, IParse eighthParam, IParse ninethParam, IParse tenthParam)
    {
        if (parser is GmqParser gmq)
            MESSAGE_message_one_parse(gmq, ref firstParam, secondParam, thirdParam, ref forthParam, ref fifthParam, sixthParam, ref seventhParam, eighthParam, ninethParam, tenthParam);
        else if (parser is JsonParser json)
            MESSAGE_message_one_parse(json, ref firstParam, secondParam, thirdParam, ref forthParam, ref fifthParam, sixthParam, ref seventhParam, eighthParam, ninethParam, tenthParam);
        else
            throw new Exception(); //TODO
    }

    public static void MESSAGE_message_one_parse(JsonParser parser, ref Int64 firstParam, IParse secondParam, IParse thirdParam,
        ref UInt64 forthParam, ref string fifthParam, IParse sixthParam, ref Double seventhParam, IParse eighthParam, IParse ninethParam, IParse tenthParam)
    {
        parser.skipDelimiter('{');
        while (true)
        {
            string key;
            parser.readKeyFromJson(out key);
            if (key == "firstParam")
                parser.parseSignedInteger(out firstParam);
            else if (key == "secondParam")
                json.parseJsonEntry(parser, secondParam);
            else if (key == "thirdParam")
                json.parseJsonEntry(parser, thirdParam);
            else if (key == "forthParam")
                parser.parseUnsignedInteger(out forthParam);
            else if (key == "fifthParam")
                parser.parseString(out fifthParam);
            else if (key == "sixthParam")
                json.parseJsonEntry(parser, sixthParam);
            else if (key == "seventhParam")
                parser.parseReal(out seventhParam);
            else if (key == "eighthParam")
                json.parseJsonEntry(parser, eighthParam);
            else if (key == "ninethParam")
                json.parseJsonEntry(parser, ninethParam);
            else if (key == "tenthParam")
                json.parseJsonEntry(parser, tenthParam);

            parser.skipSpacesEtc();
            if (parser.isDelimiter(','))
            {
                parser.skipDelimiter(',');
                continue;
            }
            if (parser.isDelimiter('}'))
            {
                parser.skipDelimiter('}');
                break;
            }
            throw new Exception();// bad format
        }

    }
    public static void MESSAGE_message_one_parse(GmqParser parser, ref Int64 firstParam, IParse secondParam, IParse thirdParam,
    ref UInt64 forthParam, ref string fifthParam, IParse sixthParam, ref Double seventhParam, IParse eighthParam, IParse ninethParam, IParse tenthParam)
    {
        parser.parseSignedInteger(out firstParam);
        gmq.parseGmqParam(parser, secondParam);
        gmq.parseGmqParam(parser, thirdParam);
        parser.parseUnsignedInteger(out forthParam);
        parser.parseString(out fifthParam);
        gmq.parseGmqParam(parser, sixthParam);
        parser.parseReal(out seventhParam);
        gmq.parseGmqParam(parser, eighthParam);
        gmq.parseGmqParam(parser, ninethParam);
        gmq.parseGmqParam(parser, tenthParam);
    }

    public static void handleGmqMessage(ReadIterT it, MessageHandlerArray handlers)
    {
        GmqParser parser = new GmqParser(it);
        gmq.handleMessage(parser, handlers);
    }
    public static void handleJsonMessage(ReadIterT it, MessageHandlerArray handlers)
    {
        JsonParser parser = new JsonParser(it);
        json.handleMessage(parser, handlers);
    }

    //TODO Message handler should be constructible only from this methods
    public MessageHandler makeMessageHandler(MessageName msgID, MessageHandler.HandlerDelegate handler)
    {
        return new MessageHandler((UInt64)msgID, handler);
    }
    public MessageHandler defaultMessageHandler(MessageHandler.HandlerDelegate handler)
    {
        return new MessageHandler(MessageHandler.DefaultHandler, handler);
    }

    public MessageHandlerArray makeHandlerArray(params MessageHandler[] handlers)
    {
        return MessageHandlerArray.make(handlers);
    }

    public void composeGmqMessage_message_one(BufferT buffer, Int64 firstParam, ICompose secondParam, ICompose thirdParam,
            UInt64 forthParam, string fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam,
            ICompose ninethParam, ICompose tenthParam)
    {
        JsonComposer composer = new JsonComposer(buffer);
        composer.append("{\n  ");

        composer.addNamePart("msgid");

        composer.composeUnsignedInteger((UInt64) MessageName.message_one);
        composer.append(",\n  ");

        composer.addNamePart("msgbody");

        MESSAGE_message_one_compose(composer, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam,
            seventhParam, eighthParam, ninethParam, tenthParam);
        composer.append("\n}");
    }

    public void composeJsonMessage_message_one(BufferT buffer, Int64 firstParam, ICompose secondParam, ICompose thirdParam,
            UInt64 forthParam, string fifthParam, ICompose sixthParam, Double seventhParam, ICompose eighthParam,
            ICompose ninethParam, ICompose tenthParam)
    {
        GmqComposer composer = new GmqComposer(buffer);
        composer.composeUnsignedInteger((UInt64)MessageName.message_one);
        MESSAGE_message_one_compose(composer, firstParam, secondParam, thirdParam, forthParam, fifthParam, sixthParam,
            seventhParam, eighthParam, ninethParam, tenthParam);
    }


}
