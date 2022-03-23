#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_subdirectory(test-interop3-cplusplus)

add_custom_command(
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop3.idl ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop3.cs --csharp -m=mtest
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop3.cs
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop3.idl
)

add_custom_target(test-interop3-csharp ALL
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop3.cs
)


if(MSVC)
    add_test(NAME interop3-csharp
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cs
    )
endif()
