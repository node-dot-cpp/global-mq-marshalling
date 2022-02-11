// dllmain.cpp : Defines the entry point for the DLL application.

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX

// Windows Header Files
#include <windows.h>

#include <cstdint>

#include "dllmain.h"
#include "generated_interop2.h"
#include <gmqueue.h>
#include <publishable_impl.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

globalmq::marshalling::GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;
globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo> mp;
InteropTransport<GMQueueStatePublisherSubscriberTypeInfo>* transport;
InteropTransport<GMQueueStatePublisherSubscriberTypeInfo>* transport2;
mtest::TestInterop2_NodecppWrapperForPublisher<mtest::structures::TestInterop2, decltype(mp)>* publ;
mtest::TestInterop2_NodecppWrapperForSubscriber<mtest::structures::TestInterop2, decltype(mp)>* subs;

__declspec(dllexport) void GmqInit(uint64_t id, uint64_t slotId, uint64_t reincarnation, GmqPostMessage postFunc)
{
    printf("Enter GmqInit\n");
    		printf("GMQueue 0x%x\n", &gmqueue);
    		printf("MetaPool 0x%x\n", &mp);


		using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
		using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
		using ParserT = GMQueueStatePublisherSubscriberTypeInfo::ParserT;

		// globalmq::marshalling::GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;
		gmqueue.template initStateConcentratorFactory<mtest::StateConcentratorFactory<BufferT, ComposerT>>();
		gmqueue.setAuthority("");

        SlotIdx idx {slotId, reincarnation};
		// transport = new InteropTransport<GMQueueStatePublisherSubscriberTypeInfo>(gmqueue, "node_cpp", 3, postFunc);
		transport2 = new InteropTransport<GMQueueStatePublisherSubscriberTypeInfo>(gmqueue, "node_cs", 2, postFunc, id, idx);

		// using MetaPoolT = globalmq::marshalling::MetaPool<GMQueueStatePublisherSubscriberTypeInfo>;
		// MetaPoolT mp;
		mp.setTransport(transport2);

		// Mock_WrapperForPublisher<ComposerT> publ;
		publ = new mtest::TestInterop2_NodecppWrapperForPublisher<mtest::structures::TestInterop2, decltype(mp)>(mp);
		// mp.add(&publ);

        globalmq::marshalling::GmqPathHelper::PathComponents pc;
        pc.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
        pc.authority = "";
        pc.nodeName = "node_cs";
        pc.statePublisherOrConnectionType = "TestInterop2";

	    GMQ_COLL string path = globalmq::marshalling::GmqPathHelper::compose(pc);


		subs = new mtest::TestInterop2_NodecppWrapperForSubscriber<mtest::structures::TestInterop2, decltype(mp)>(mp);
		// sub.subscribe(path);
        printf("Exit GmqInit\n");

}
__declspec(dllexport) void GmqHandleMessage(uint64_t size, uint64_t capacity, uint8_t* data)
{
    printf("GmqHandleMessage Enter\n");
    globalmq::marshalling::BufferMemento memento{size, capacity, data};
    globalmq::marshalling::Buffer buff = globalmq::marshalling::Buffer::makeFromMemento(memento);

    printf("GmqHandleMessage Buffer recontructed\n");
    mp.onMessage( buff );
    printf("GmqHandleMessage Exit\n");
}


__declspec(dllexport) void TestUpdatePublisher(int64_t number)
{
    publ->set_number(number);
}

__declspec(dllexport) void EndTick()
{
    mp.postAllUpdates();
}
