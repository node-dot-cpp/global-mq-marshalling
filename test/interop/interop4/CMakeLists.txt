#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_subdirectory(test-interop4-cplusplus)

add_custom_command(
    COMMAND idl_compiler interop4.idl test-interop4-csharp/generated_interop4.cs --csharp -m=mtest
    OUTPUT test-interop4-csharp/generated_interop4.cs
    DEPENDS idl_compiler
    MAIN_DEPENDENCY interop4.idl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(test-interop4-csharp-generated ALL
    DEPENDS test-interop4-csharp/generated_interop4.cs
)


if(MSVC)
    add_test(NAME interop4-csharp
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test-interop4-csharp
    )
endif()
