using globalmq.marshalling;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_interop3_csharp
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
