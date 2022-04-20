#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-1021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_subdirectory(test-interop1-cplusplus)
if(MSVC)
    add_subdirectory(test-interop1-cpptemplates)
endif()


add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/test-interop1-csharp/generated_interop1.cs
    COMMAND idl_compiler interop1.idl test-interop1-csharp/generated_interop1.cs --csharp -m=mtest
    DEPENDS idl_compiler
    MAIN_DEPENDENCY interop1.idl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(test-interop1-csharp-generated ALL
    DEPENDS test-interop1-csharp/generated_interop1.cs
)


if(MSVC)
    add_test(NAME interop1-csharp
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test-interop1-csharp
    )
endif()