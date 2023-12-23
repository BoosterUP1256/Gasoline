cmake_minimum_required(VERSION 3.16)

project("Testbed")

add_executable(${PROJECT_NAME})
add_subdirectory("src")

add_subdirectory("../GasEngine" GasEngine)
target_link_libraries(${PROJECT_NAME} PRIVATE GasEngine)