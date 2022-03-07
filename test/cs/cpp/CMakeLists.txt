#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/idl_generated.h
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../test_marshalling.idl ${CMAKE_CURRENT_SOURCE_DIR}/idl_generated.h -m=mtest
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../test_marshalling.idl
)

add_executable(test-interop1-cpp
    main.cpp
    idl_generated.h
    test_gmqueue.cpp
    test_message_three.cpp
    test_message_two.cpp
    test_publishable_seven.cpp
    test_publishable_six.cpp
    test_publishable_six2.cpp
    test_struct_one.cpp
)

target_compile_definitions(test-interop1-cpp PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(test-interop1-cpp global-mq) 

add_test(NAME interop1-cpp
    COMMAND test-interop1-cpp --pass-zen
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
)

