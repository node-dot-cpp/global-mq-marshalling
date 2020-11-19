#-------------------------------------------------------------------------------------------
# Copyright (c) 2020, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(IDL_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/marshalling/test_marshalling.h)

add_custom_command(
    OUTPUT ${IDL_OUTPUT}
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl ${IDL_OUTPUT}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl
)


add_executable(test_marshalling
    main.cpp
    test_marshalling.cpp
    ${IDL_OUTPUT}
)

target_link_libraries(test_marshalling global-mq)

add_test(marshalling test_marshalling)
