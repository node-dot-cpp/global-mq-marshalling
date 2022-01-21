#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-1021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/IdlGenerated.cs ${CMAKE_CURRENT_SOURCE_DIR}/cpp/idl_generated.h
# #    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl ${IDL_OUTPUT} -csharp=${CMAKE_CURRENT_SOURCE_DIR}/../dotnet/test/IdlGenerated.cs -m=mtest -p=Nodecpp -c=GMQueueStatePublisherSubscriberTypeInfo
#     COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl ${CMAKE_CURRENT_SOURCE_DIR}/cpp/idl_generated.h -csharp=${CMAKE_CURRENT_SOURCE_DIR}/cs/IdlGenerated.cs -m=mtest
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl
# )

# # custom target to run idl_compiler
# add_custom_target( test-set1-cs DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/IdlGenerated.cs )

add_subdirectory(cpp)

