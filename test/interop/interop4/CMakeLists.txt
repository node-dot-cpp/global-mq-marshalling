#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_subdirectory(test-interop4-cplusplus)

add_custom_command(
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop4.idl ${CMAKE_CURRENT_SOURCE_DIR}/test-interop4-csharp/generated_interop4.cs --csharp -m=mtest
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/test-interop4-csharp/generated_interop4.cs
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop4.idl
)

add_custom_target(test-interop4-csharp-idl ALL
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test-interop4-csharp/generated_interop4.cs
)


if(MSVC)
    add_test(NAME interop4-csharp
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test-interop4-csharp
    )
endif()
