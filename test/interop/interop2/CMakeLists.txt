#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_subdirectory(test-interop2-cplusplus)
if(MSVC)
    add_subdirectory(cpp)
endif()

add_custom_command(
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop2.idl ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop2.cs --csharp -m=mtest
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop2.cs
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop2.idl
)

add_custom_target(test-interop2-cs-idl ALL
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop2.cs
)


if(MSVC)
    add_test(NAME interop2-csharp
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cs
    )
endif()
