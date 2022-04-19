/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
* Copyright (c) 2022, OLogN Technologies AG
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

using globalmq.marshalling;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_interop4_csharp
{
    public interface ITestPlatformSupport : IPlatformSupport
    {
        bool AreEqual(BufferT left, BufferT right);
    }

    public class TestJsonPlatform : DefaultJsonPlatformSupport, ITestPlatformSupport
    {
        public bool AreEqual(BufferT left, BufferT right)
        {
            return SimpleBuffer.AreEqualIgnoreEol(left, right);
        }

    }

    public class TestGmqPlatform : DefaultGmqPlatformSupport, ITestPlatformSupport
    {
        public bool AreEqual(BufferT left, BufferT right)
        {
            return SimpleBuffer.AreEqual(left, right);
        }
    }
    class TestCommon
    {
        public const string DataPathPrefix = "../../../../data/";
        public enum Protocol { Json, Gmq };
        public static BufferT makeBuffer() { return new SimpleBuffer(); }
        public static IPublishableComposer makePublishableComposer(Protocol proto, BufferT buffer)
        {
            if (proto == Protocol.Json)
                return new JsonPublishableComposer(buffer);
            else if (proto == Protocol.Gmq)
                return new GmqPublishableComposer(buffer);
            else
                throw new Exception();
        }
        public static IPublishableParser makePublishableParser(Protocol proto, ReadIteratorT readIter)
        {
            if (proto == Protocol.Json)
                return new JsonPublishableParser(readIter);
            else if (proto == Protocol.Gmq)
                return new GmqPublishableParser(readIter);
            else
                throw new Exception();
        }

    }
}
