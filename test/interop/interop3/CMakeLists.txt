#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_subdirectory(test-interop3-cplusplus)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/test-interop3-csharp/generated_interop3.cs
    COMMAND idl_compiler interop3.idl test-interop3-csharp/generated_interop3.cs --csharp -m=mtest
    DEPENDS idl_compiler
    MAIN_DEPENDENCY interop3.idl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(test-interop3-csharp-generated ALL
    DEPENDS test-interop3-csharp/generated_interop3.cs
)


if(MSVC)
    add_test(NAME interop3-csharp
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test-interop3-csharp
    )
endif()
