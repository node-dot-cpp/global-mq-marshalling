#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop4.h
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../interop4.idl ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop4.h --cplusplus -m=mtest
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../interop4.idl
)

add_executable(test-interop4-cplusplus
    generated_interop4.h
    main.cpp
    test_common.h
    test_publishable_cycle.cpp
    test_publishable_html_tag.cpp
    test_simple.cpp
)

target_compile_definitions(test-interop4-cplusplus PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(test-interop4-cplusplus global-mq) 

add_test(NAME interop4-cplusplus
    COMMAND test-interop4-cplusplus
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
)

