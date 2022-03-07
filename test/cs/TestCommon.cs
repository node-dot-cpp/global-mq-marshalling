using globalmq.marshalling;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xunit;

namespace TestProject1
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
        public const string DataPathPrefix = "../../../";
    }
}
