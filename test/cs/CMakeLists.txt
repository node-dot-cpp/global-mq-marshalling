#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-1021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/IdlGenerated.cs
#    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl ${IDL_OUTPUT} -csharp=${CMAKE_CURRENT_SOURCE_DIR}/../dotnet/test/IdlGenerated.cs -m=mtest -p=Nodecpp -c=GMQueueStatePublisherSubscriberTypeInfo
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.h -csharp=${CMAKE_CURRENT_SOURCE_DIR}/IdlGenerated.cs -m=mtest
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl
)

add_custom_target( test-cs DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/IdlGenerated.cs )
