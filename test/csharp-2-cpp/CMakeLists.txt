#-------------------------------------------------------------------------------------------
# Copyright (c) 2020, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

set(IDL_CPLUSPLUS ${CMAKE_CURRENT_SOURCE_DIR}/gen_message_one.h)
set(IDL_CSHARP ${CMAKE_CURRENT_SOURCE_DIR}/gen_message_one.cs)

add_custom_command(
    OUTPUT ${IDL_CPLUSPLUS}
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/message_one.idl ${IDL_CPLUSPLUS} -m=mtest
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/message_one.idl
)

add_custom_command(
    OUTPUT ${IDL_CSHARP}
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/message_one.idl ${IDL_CSHARP} --csharp -m=mtest
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/message_one.idl
)

add_executable(cpp-2-csharp
    cpp_message.cpp
    ${IDL_CPLUSPLUS}
)

target_compile_definitions(cpp-2-csharp PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(cpp-2-csharp global-mq)

if(MSVC)
    add_custom_target(cshrp-2-cpp ALL dotnet build
                        --output ${CMAKE_CURRENT_BINARY_DIR}/Release
                        --configuration Release
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    DEPENDS ${IDL_CSHARP}
    )
endif()
