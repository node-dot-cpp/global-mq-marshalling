#-------------------------------------------------------------------------------------------
# Copyright (c) 2020, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

set(IDL_CPLUSPLUS ${CMAKE_CURRENT_SOURCE_DIR}/gen_message_one.h)
set(IDL_CSHARP ${CMAKE_CURRENT_SOURCE_DIR}/gen_message_one.cs)

add_custom_command(
    OUTPUT ${IDL_CPLUSPLUS} ${IDL_CSHARP}
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/message_one.idl ${IDL_CPLUSPLUS} -csharp=${IDL_CSHARP} -m=mtest
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/message_one.idl
)


add_executable(cpp-2-csharp
    cpp_message.cpp
    ${IDL_CPLUSPLUS}
)

target_link_libraries(cpp-2-csharp global-mq)

#add_executable(csharp-message
#    csharp_message.cs
#    ${IDL_CSHARP}
#)
