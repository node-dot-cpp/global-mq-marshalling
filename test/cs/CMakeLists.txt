#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-1021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_subdirectory(cpp)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/IdlGenerated.cs
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl ${CMAKE_CURRENT_SOURCE_DIR}/IdlGenerated.cs --csharp -m=mtest
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl
)

add_custom_target(test-interop1-cs-idl ALL
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/IdlGenerated.cs
)


if(MSVC)
    add_test(NAME interop1-csharp
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()