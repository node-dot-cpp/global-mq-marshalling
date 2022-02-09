#-------------------------------------------------------------------------------------------
# Copyright (c) 2022, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../IdlGenerated.cs ${CMAKE_CURRENT_SOURCE_DIR}/idl_generated.h
#    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test_marshalling.idl ${IDL_OUTPUT} -csharp=${CMAKE_CURRENT_SOURCE_DIR}/../dotnet/test/IdlGenerated.cs -m=mtest -p=Nodecpp -c=GMQueueStatePublisherSubscriberTypeInfo
    COMMAND idl_compiler ${CMAKE_CURRENT_SOURCE_DIR}/../test_marshalling.idl ${CMAKE_CURRENT_SOURCE_DIR}/idl_generated.h -csharp=${CMAKE_CURRENT_SOURCE_DIR}/../IdlGenerated.cs -m=mtest
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../test_marshalling.idl
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

add_test(NAME interop1-cpp COMMAND test-interop1-cpp --pass-zen)

file(GLOB JSON_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../*.json")
file(GLOB GMQ_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../*.gmq")

file(COPY ${JSON_TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${GMQ_TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_struct_one.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_struct_one.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_message_two.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_message_three.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six_update1.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six_update2.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six_update3.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six2.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six2_update1.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six2_update2.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_six2_update3.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u1.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u2.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u3.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u4.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u5.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u1.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u2.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u3.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u4.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../test_publishable_seven_u5.gmq DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
