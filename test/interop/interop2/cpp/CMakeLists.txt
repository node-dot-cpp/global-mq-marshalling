#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop2.h
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../interop2.idl ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop2.h --cpptemplates -m=mtest
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../interop2.idl
)

add_executable(test-interop2-cpptemplates
    generated_interop2.h
    main.cpp
    test_common.h
    test_publishable_dictionary.cpp
    test_publishable_dunion.cpp
    test_simple.cpp
)

target_compile_definitions(test-interop2-cpptemplates PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(test-interop2-cpptemplates global-mq) 

add_test(NAME interop2-cpptemplates
    COMMAND test-interop2-cpptemplates
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
)

