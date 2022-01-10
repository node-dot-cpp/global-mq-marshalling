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

using System;
using System.Globalization;
using System.Text;

namespace globalmq.marshalling
{
	enum Proto { GMQ, JSON };

	public interface ComposerBase
	{
		public void composeSignedInteger(sbyte num);
		public void composeSignedInteger(short num);
		public void composeSignedInteger(int num);
		public void composeSignedInteger(long num);
		public void composeUnsignedInteger(byte num);
		public void composeUnsignedInteger(ushort num);
		public void composeUnsignedInteger(uint num);
		public void composeUnsignedInteger(ulong num);
		public void composeReal(float num);
		public void composeReal(double num);
		public void composeString(string str);
	}
	public class JsonComposer : ComposerBase
	{
		BufferT buff;

		// mb: this is needed to make float format independant of locale
		NumberFormatInfo nfi = new NumberFormatInfo();
		public JsonComposer(BufferT buff_) { buff = buff_; nfi.NumberDecimalSeparator = "."; }
		public void composeSignedInteger(sbyte num)
		{
			buff.appendAscii(num.ToString());
		}
		public void composeSignedInteger(short num)
		{
			buff.appendAscii(num.ToString());
		}
		public void composeSignedInteger(int num)
		{
			buff.appendAscii(num.ToString());
		}
		public void composeSignedInteger(long num)
		{
			buff.appendAscii(num.ToString());
		}

		public void composeUnsignedInteger(byte num)
		{
			buff.appendAscii(num.ToString());
		}
		public void composeUnsignedInteger(ushort num)
		{
			buff.appendAscii(num.ToString());
		}
		public void composeUnsignedInteger(uint num)
		{
			buff.appendAscii(num.ToString());
		}
		public void composeUnsignedInteger(ulong num)
		{
			buff.appendAscii(num.ToString());
		}

		public void composeReal(float num)
		{
			buff.appendAscii(num.ToString(nfi));
		}
		public void composeReal(double num)
		{
			buff.appendAscii(num.ToString(nfi));
		}
		public void composeString(string str)
		{
			buff.appendUint8('\"');
			buff.appendAscii(str);
			buff.appendUint8('\"');

		}

		public void addNamePart(string name)
		{
			buff.appendUint8('\"');
			buff.appendAscii(name);
			buff.appendUint8('\"');
			buff.appendUint8(':');
		}

		public void append(string str)
        {
			buff.appendAscii(str);
		}
		public void append(char ch)
		{
			buff.appendUint8(ch);
		}
	};

	public class GmqComposer : ComposerBase
	{
		BufferT buff;

		public GmqComposer(BufferT buff_) { buff = buff_; }

		public void composeSignedInteger(sbyte num)
		{
			composeSignedInteger((long)num);
		}
		public void composeSignedInteger(short num)
		{
			composeSignedInteger((long)num);
		}
		public void composeSignedInteger(int num)
		{
			composeSignedInteger((long)num);
		}
		public void composeSignedInteger(long num)
		{
			buff.append(BitConverter.GetBytes(num));
		}

		public void composeUnsignedInteger(byte num)
		{
			composeUnsignedInteger((ulong)num);
		}
		public void composeUnsignedInteger(ushort num)
		{
			composeUnsignedInteger((ulong)num);
		}
		public void composeUnsignedInteger(uint num)
		{
			composeUnsignedInteger((ulong)num);
		}
		public void composeUnsignedInteger(ulong num)
		{
			buff.append(BitConverter.GetBytes(num));
		}

		public void composeReal(float num)
		{
			composeReal((double)num);
		}
		public void composeReal(double num)
		{
			buff.append(BitConverter.GetBytes(num));
		}
		public void composeString(string str)
		{
			buff.appendAscii(str);
			buff.appendUint8(0);
		}
	};

} // namespace globalmq::marshalling


// parsing

namespace globalmq.marshalling
{

    public interface ParserBase
    {
        public void parseSignedInteger(out sbyte num);
        public void parseSignedInteger(out short num);
        public void parseSignedInteger(out int num);
        public void parseSignedInteger(out long num);
        public void skipSignedInteger();

        public void parseUnsignedInteger(out byte num);
        public void parseUnsignedInteger(out ushort num);
        public void parseUnsignedInteger(out uint num);
        public void parseUnsignedInteger(out ulong num);
        public void parseUnsignedInteger(out int num);
        public void skipUnsignedInteger();
        public void parseReal(out float num);
        public void parseReal(out double num);
		public void skipReal();
        public void parseString(out string s);
        public void skipString();
    }
    public class GmqParser : ParserBase
	{
		ReadIteratorT riter;

		public GmqParser(ReadIteratorT riter_) { riter = riter_; }

		public void parseSignedInteger(out sbyte num)
		{
			num = (sbyte)BitConverter.ToInt64(riter.read(8));
		}
		public void parseSignedInteger(out short num)
		{
			num = (short)BitConverter.ToInt64(riter.read(8));
		}
		public void parseSignedInteger(out int num)
		{
			num = (int)BitConverter.ToInt64(riter.read(8));
		}
		public void parseSignedInteger(out long num)
		{
			num = BitConverter.ToInt64(riter.read(8));
		}
		public void skipSignedInteger()
		{
			riter.skip(8);
		}

		public void parseUnsignedInteger(out byte num)
		{
			num = (byte)BitConverter.ToUInt64(riter.read(8));
		}
		public void parseUnsignedInteger(out ushort num)
		{
			num = (ushort)BitConverter.ToUInt64(riter.read(8));
		}
		public void parseUnsignedInteger(out uint num)
		{
			num = (uint)BitConverter.ToUInt64(riter.read(8));
		}
		public void parseUnsignedInteger(out ulong num)
		{
			num = BitConverter.ToUInt64(riter.read(8));
		}
		public void parseUnsignedInteger(out int num)
		{
			num = (int)BitConverter.ToUInt64(riter.read(8));
		}
		public void skipUnsignedInteger()
		{
			riter.skip(8);
		}

		public void parseReal(out float num)
		{
			num = (float)BitConverter.ToDouble(riter.read(8));
		}
		public void parseReal(out double num)
		{
			num = BitConverter.ToDouble(riter.read(8));
		}
		public void skipReal()
		{
			riter.skip(8);
		}
		public void parseString(out string s)
		{
			StringBuilder sb = new StringBuilder();
			while (riter.isData() && riter.getChar() != '\0')
			{
				sb.Append(riter.getChar());
				riter.inc();
			}
			if (!riter.isData())
				throw new Exception(); // TODO
			riter.inc();

			s = sb.ToString();
		}
		public void skipString()
		{
			while (riter.isData() && riter.getChar() != '\0')
			{
				riter.inc();
			}
			if (!riter.isData())
				throw new Exception(); // TODO
			riter.inc();
		}
	};
	public class JsonParser : ParserBase
	{
		ReadIteratorT riter;

		// mb: this is needed to make float format independant of locale
		NumberFormatInfo nfi = new NumberFormatInfo();

		public JsonParser(ReadIteratorT riter_) { riter = riter_; nfi.NumberDecimalSeparator = "."; }
		void impl_skipBlockFromJson(char left, char right)
		{
			// mb: TODO this needs more work
			 
			skipSpacesEtc();
			if (riter.getChar() != left)
				throw new Exception(); // TODO
			riter.inc();
			ulong ctr = 1;
			while (riter.isData())
			{
				if (riter.getChar() == left)
					++ctr;
				else if (riter.getChar() == right)
				{
					--ctr;
					if (ctr == 0)
					{
						riter.inc();
						return;
					}
				}
				riter.inc();
			}
		}

		public void skipSpacesEtc()
		{
			while (riter.isData() && (riter.getChar() == ' ' || riter.getChar() == '\t' || riter.getChar() == '\r' || riter.getChar() == '\n'))
				riter.inc();
		}
		public bool isDelimiter(char delim)
		{
			skipSpacesEtc();
			return riter.getChar() == delim;
		}
		public void skipDelimiter(char delim)
		{
			skipSpacesEtc();
			if (riter.getChar() != delim)
				throw new Exception(); // TODO
			riter.inc();
		}
		StringBuilder impl_readStringFromJson()
		{
			skipSpacesEtc();
			if (riter.getChar() != '\"')
				throw new Exception(); // TODO
			riter.inc();
			StringBuilder sb = new StringBuilder();
			while (riter.isData() && riter.getChar() != '\"')
			{
				sb.Append(riter.getChar());
				riter.inc();
			}
			if (!riter.isData())
				throw new Exception(); // TODO
			riter.inc();
			return sb;
		}

		public void parseString(out string s)
		{
			StringBuilder sb = impl_readStringFromJson();
			s = sb.ToString();
		}
		public void skipString()
		{
			impl_readStringFromJson();
		}

		public void skipVectorFromJson()
		{
			impl_skipBlockFromJson('[', ']');
		}
		public void skipMessageFromJson()
		{
			impl_skipBlockFromJson('{', '}');
		}

		StringBuilder impl_readUnsignedIntegerFromJson()
		{
			skipSpacesEtc();
			if (riter.getChar() == '-')
				throw new Exception(); // TODO: (negative is unexpected)
			StringBuilder sb = new StringBuilder();
			while (riter.isData() && (riter.getChar() >= '0' && riter.getChar() <= '9'))
			{
				sb.Append(riter.getChar());
				riter.inc();
			}
			if (!riter.isData())
				throw new Exception(); // TODO

			return sb;
		}

		public void parseUnsignedInteger(out byte num)
		{
			StringBuilder sb = impl_readUnsignedIntegerFromJson();
			num = byte.Parse(sb.ToString());
		}
		public void parseUnsignedInteger(out ushort num)
		{
			StringBuilder sb = impl_readUnsignedIntegerFromJson();
			num = ushort.Parse(sb.ToString());
		}
		public void parseUnsignedInteger(out uint num)
		{
			StringBuilder sb = impl_readUnsignedIntegerFromJson();
			num = uint.Parse(sb.ToString());
		}
		public void parseUnsignedInteger(out ulong num)
		{
			StringBuilder sb = impl_readUnsignedIntegerFromJson();
			num = ulong.Parse(sb.ToString());
		}
		public void parseUnsignedInteger(out int num)
		{
			StringBuilder sb = impl_readUnsignedIntegerFromJson();
			num = int.Parse(sb.ToString());
		}

		public void skipUnsignedInteger()
		{
			impl_readUnsignedIntegerFromJson();
		}

		StringBuilder impl_readSignedIntegerFromJson()
		{
			skipSpacesEtc();
			StringBuilder sb = new StringBuilder();
			if (riter.getChar() == '-')
			{
				sb.Append(riter.getChar());
				riter.inc();
			}
			while (riter.isData() && (riter.getChar() >= '0' && riter.getChar() <= '9'))
			{
				sb.Append(riter.getChar());
				riter.inc();
			}
			if (!riter.isData())
				throw new Exception(); // TODO

			return sb;
		}
		public void parseSignedInteger(out sbyte num)
		{
			StringBuilder sb = impl_readSignedIntegerFromJson();
			num = sbyte.Parse(sb.ToString());
		}
		public void parseSignedInteger(out short num)
		{
			StringBuilder sb = impl_readSignedIntegerFromJson();
			num = short.Parse(sb.ToString());
		}
		public void parseSignedInteger(out int num)
		{
			StringBuilder sb = impl_readSignedIntegerFromJson();
			num = int.Parse(sb.ToString());
		}
		public void parseSignedInteger(out long num)
		{
			StringBuilder sb = impl_readSignedIntegerFromJson();
			num = long.Parse(sb.ToString());
		}
		public void skipSignedInteger()
		{
			impl_readSignedIntegerFromJson();
		}
		StringBuilder impl_readRealFromJson()
		{
			skipSpacesEtc();
			StringBuilder sb = new StringBuilder();
			//		while ( riter.isData() && ((*riter >= '0' && *riter <= '9') || *riter == '.') ) // TODO: more than one '.' in json with errors
			while (riter.isData() && !(riter.getChar() == ' ' || riter.getChar() == '\t' || riter.getChar() == '\r' ||
				riter.getChar() == '\n' || riter.getChar() == ',' || riter.getChar() == ']' || riter.getChar() == '}')) // expected terminators
			{
				sb.Append(riter.getChar());
				riter.inc();
			}
			if (!riter.isData())
				throw new Exception(); // TODO

			return sb;
		}

		public void parseReal(out float num)
		{
			StringBuilder sb = impl_readRealFromJson();
			num = float.Parse(sb.ToString(),NumberStyles.Float, nfi);
		}
		public void parseReal(out double num)
		{
			StringBuilder sb = impl_readRealFromJson();
			num = double.Parse(sb.ToString(), NumberStyles.Float, nfi);
		}
		public void skipReal()
		{
			impl_readRealFromJson();
		}

		public void readKeyFromJson(out string s)
		{
			//s->clear();
			skipSpacesEtc();
			parseString(out s);
			skipSpacesEtc();
			if (riter.getChar() != ':')
				throw new Exception(); // TODO (expected ':')
			riter.inc();
		}
		/*void readKey(GMQ_COLL string_view* s)
		{
			skipSpacesEtc();
			readStringFromJson(s);
			skipSpacesEtc();
			if ( *riter != ':' )
				throw std::exception(); // TODO (expected ':')
			++riter;
		}*/

	};

} // namespace globalmq::marshalling

//#endif // COMPOSE_AND_PARSE_IMPL_H
