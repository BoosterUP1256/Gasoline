cmake_minimum_required(VERSION 3.21)

project("GasEngine")

add_library(${PROJECT_NAME} SHARED)
add_subdirectory("src")

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
)

target_compile_definitions(${PROJECT_NAME} PRIVATE FOO_LIBRARY_BUILD)
target_include_directories(${PROJECT_NAME} INTERFACE "include")

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME}.dll ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()