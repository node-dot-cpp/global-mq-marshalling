#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop1.h
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../interop1.idl ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop1.h --cplusplus -m=mtest
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../interop1.idl
)

add_executable(test-interop1-cplusplus
    generated_interop1.h
    main.cpp
    # test_gmqueue.cpp
    test_common.h
    test_message_three.cpp
    test_message_two.cpp
    test_publishable_seven.cpp
    test_publishable_six.cpp
    test_struct_one.cpp
)

target_compile_definitions(test-interop1-cplusplus PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(test-interop1-cplusplus global-mq) 

add_test(NAME interop1-cplusplus
    COMMAND test-interop1-cplusplus
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
)

