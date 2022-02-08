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
using System.Diagnostics;
using System.Globalization;
using System.Text;

namespace globalmq.marshalling
{
	enum Proto { GMQ, JSON };

	public interface ComposerBase {	}
	public class JsonComposer : ComposerBase
	{
		BufferT buff;

		// mb: this is needed to make float format independant of locale
		NumberFormatInfo nfi = CultureInfo.InvariantCulture.NumberFormat;
		Char[] escapeChars = new Char[] { '\\', '\n', '\r', '\t', '\"' };
		public JsonComposer(BufferT buff_)
		{
			buff = buff_;
		}
		public BufferT getBuffer()
		{
			return buff;
		}
		public void composeSignedInteger(long num)
		{
			buff.appendAscii(num.ToString());
		}

		public void composeUnsignedInteger(ulong num)
		{
			buff.appendAscii(num.ToString());
		}

		public void composeReal(double num)
		{
			buff.appendAscii(num.ToString(nfi));
		}
		public void composeString(string str)
		{
			buff.appendUint8('\"');
			if(str.IndexOfAny(escapeChars) == -1)
				buff.appendAscii(str);
            else
            {
				// go char by char
				foreach(Char ch in str)
                {
					switch (ch)
					{
						case '\\':
							buff.appendUint8('\\');
							buff.appendUint8('\\');
							break;
						case '\n':
							buff.appendUint8('\\');
							buff.appendUint8('n');
							break;
						case '\r':
							buff.appendUint8('\\');
							buff.appendUint8('r');
							break;
						case '\t':
							buff.appendUint8('\\');
							buff.appendUint8('t');
							break;
						case '\"':
							buff.appendUint8('\\');
							buff.appendUint8('\"');
							break;
						default:
							buff.appendUint8(ch);
							break;
					}

				}
			}
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
		public BufferT getBuffer()
		{
			return buff;
		}

		public void composeSignedInteger(long num)
		{
			buff.append(BitConverter.GetBytes(num));
		}

		public void composeUnsignedInteger(ulong num)
		{
			buff.append(BitConverter.GetBytes(num));
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

    public interface ParserBase { }
    public class GmqParser : ParserBase
	{
		ReadIteratorT riter;

		public GmqParser(ReadIteratorT riter_) { riter = riter_; }

		public ReadIteratorT getIterator()
        {
			return riter;
        }
		public void parseSignedInteger(out long num)
		{
			num = BitConverter.ToInt64(riter.read(8));
		}
		public void parseUnsignedInteger(out ulong num)
		{
			num = BitConverter.ToUInt64(riter.read(8));
		}
		public void parseUnsignedInteger(out int num)
		{
			num = (int)BitConverter.ToUInt64(riter.read(8));
		}
		public void parseReal(out double num)
		{
			num = BitConverter.ToDouble(riter.read(8));
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
	};
	public class JsonParser : ParserBase
	{
		ReadIteratorT riter;

		// mb: this is needed to make float format independant of locale
		NumberFormatInfo nfi = new NumberFormatInfo();

		public JsonParser(ReadIteratorT riter_) { riter = riter_; nfi.NumberDecimalSeparator = "."; }

		public ReadIteratorT getIterator()
		{
			return riter;
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

			bool done = false;
			while ((!done) && riter.isData())
			{
				switch (riter.getChar())
				{
                case '\"':
                    done = true;
                    break;
                case '\\':
                    riter.inc();
                    if (!riter.isData())
                        throw new Exception(); ; // TODO
                    switch (riter.getChar())
                    {
                    case '\\':
                        sb.Append('\\');
                        break;
                    case 't':
                        sb.Append('\t');
                        break;
                    case 'r':
                        sb.Append('\r');
                        break;
                    case 'n':
                        sb.Append('\n');
                        break;
                    case '\"':
                        sb.Append('\"');
                        break;
                    default:
                        throw new Exception(); // TODO (unexpected)
                    }
                    riter.inc();
                    break;
                default:
                    sb.Append(riter.getChar());
                    riter.inc();
                    break;
                }
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
		StringBuilder impl_readIntegerFromJson(bool allowNegative)
		{
			skipSpacesEtc();
			StringBuilder sb = new StringBuilder();
			if (riter.getChar() == '-')
			{
				if (allowNegative)
				{
					sb.Append(riter.getChar());
					riter.inc();
				}
				else
					throw new Exception(); // negative is unexpected
			}
			else if (riter.getChar() == '+')
			{
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
		public void parseUnsignedInteger(out ulong num)
		{
			StringBuilder sb = impl_readIntegerFromJson(false);
			num = ulong.Parse(sb.ToString());
		}
		public void parseUnsignedInteger(out int num)
		{
			StringBuilder sb = impl_readIntegerFromJson(false);
			num = int.Parse(sb.ToString());
		}
		public void parseSignedInteger(out long num)
		{
			StringBuilder sb = impl_readIntegerFromJson(true);
			num = long.Parse(sb.ToString());
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
		public void parseReal(out double num)
		{
			StringBuilder sb = impl_readRealFromJson();
			num = double.Parse(sb.ToString(), NumberStyles.Float, nfi);
		}
		public void readKeyFromJson(out string s)
		{
			skipSpacesEtc();
			parseString(out s);
			skipSpacesEtc();
			if (riter.getChar() != ':')
				throw new Exception(); // TODO (expected ':')
			riter.inc();
		}
	};

} // namespace globalmq::marshalling
