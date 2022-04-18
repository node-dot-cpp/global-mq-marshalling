#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_custom_command(
    OUTPUT generated_interop3.h
    COMMAND idl_compiler ../interop3.idl generated_interop3.h --cplusplus -m=mtest
    DEPENDS idl_compiler
    MAIN_DEPENDENCY ../interop3.idl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(test-interop3-cplusplus
    generated_interop3.h
    main.cpp
    test_common.h
    test_publishable_dictionary.cpp
    test_simple.cpp
    test_struct_dictionary.cpp
    test_struct_du.cpp
)

target_compile_definitions(test-interop3-cplusplus PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(test-interop3-cplusplus global-mq) 

add_test(NAME interop3-cplusplus
    COMMAND test-interop3-cplusplus
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
)

