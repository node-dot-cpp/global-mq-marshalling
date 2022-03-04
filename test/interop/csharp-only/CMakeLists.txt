#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------


add_custom_target( test-csharp-only-idl ALL
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop2.idl ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop2.cs --csharp -m=mtest
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop2.cs
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop2.idl
)


if(MSVC)
    add_test(NAME test-csharp-only
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cs
    )
endif()
