#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_executable(test-interop1-cpp
    main.cpp
    test_struct_one.cpp
    test_message_two.cpp
    test_message_three.cpp
)

target_link_libraries(test-interop1-cpp global-mq)

add_test(NAME interop1-cpp COMMAND test-interop1-cpp --pass-zen)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_struct_one.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_struct_one.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_message_two.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_message_three.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six_update1.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six_update2.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six_update3.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six2.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six2_update1.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six2_update2.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six2_update3.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
