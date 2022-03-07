#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------


add_custom_command(
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop2.idl ${CMAKE_CURRENT_SOURCE_DIR}/cpp/generated_interop2.h -m=mtest
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/cpp/generated_interop2.h
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop2.idl
)

add_custom_command(
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop2.idl ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop2.cs --csharp -m=mtest
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop2.cs
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/interop2.idl
)

add_custom_target(test-interop2-cs-idl ALL
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cs/generated_interop2.cs
)

add_executable(test-interop2-cpp
    cpp/generated_interop2.h
    cpp/main.cpp
    cpp/test_common.h
    cpp/test_publishable_dunion.cpp
    cpp/test_simple.cpp
)

target_compile_definitions(test-interop2-cpp PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(test-interop2-cpp global-mq) 

add_test(NAME interop2-cpp
    COMMAND test-interop2-cpp --pass-zen
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)



if(MSVC)
    add_test(NAME interop2-csharp
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cs
    )
endif()
