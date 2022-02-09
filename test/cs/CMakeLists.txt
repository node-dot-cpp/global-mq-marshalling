#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-1021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_subdirectory(cpp)

if(MSVC)
    add_test(NAME interop1-csharp
        COMMAND dotnet test
            --output ${CMAKE_CURRENT_BINARY_DIR}/Release
            --configuration Release
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()