#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------


add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../test-csharp-exe/generated_interop2.cs ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop2.h
#    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl ${IDL_OUTPUT} -csharp=${CMAKE_CURRENT_SOURCE_DIR}/../dotnet/test/IdlGenerated.cs -m=mtest -p=Nodecpp -c=GMQueueStatePublisherSubscriberTypeInfo
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../interop2.idl ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop2.h -csharp=${CMAKE_CURRENT_SOURCE_DIR}/../test-csharp-exe/generated_interop2.cs -m=mtest -p=Nodecpp -c=GMQueueStatePublisherSubscriberTypeInfo
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../interop2.idl
)


add_library(test-cpp-dll SHARED
    generated_interop2.h
    dllmain.cpp
)


target_link_libraries(test-cpp-dll global-mq)

