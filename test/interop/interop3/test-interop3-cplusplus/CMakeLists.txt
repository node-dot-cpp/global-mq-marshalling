#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop3.h
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../interop3.idl ${CMAKE_CURRENT_SOURCE_DIR}/generated_interop3.h --cplusplus -m=mtest
    DEPENDS idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../interop3.idl
)

add_executable(test-interop3-cplusplus
    generated_interop3.h
    main.cpp
#    test_gmqueue.cpp
    test_common.h
    test_publishable_dictionary.cpp
    test_publishable_dunion.cpp
    test_simple.cpp
)

target_compile_definitions(test-interop3-cplusplus PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(test-interop3-cplusplus global-mq) 

# add_test(NAME interop3-cplusplus
#     COMMAND test-interop3-cplusplus
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
# )

