cmake_minimum_required(VERSION 3.27)

project(opengl-learning)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#     message(${CMAKE_BUILD_TYPE})
#     add_compile_definitions(SOURCE_ROOT=${PROJECT_SOURCE_DIR})
# else()
#     add_compile_definitions(SOURCE_ROOT=.)
# endif()

add_library(glad ${PROJECT_SOURCE_DIR}/third_party/glad/src/glad.c)
target_include_directories(glad PRIVATE ${PROJECT_SOURCE_DIR}/third_party/glad/include/)

file(GLOB_RECURSE TASK_CPP_SRC ${PROJECT_SOURCE_DIR}/src/task/src/*.cpp)

add_executable(gl_test ${TASK_CPP_SRC} ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_include_directories(gl_test PUBLIC ${PROJECT_SOURCE_DIR}/src/task/include/)
target_include_directories(gl_test PUBLIC ${PROJECT_SOURCE_DIR}/third_party/glad/include/)
target_include_directories(gl_test PRIVATE ${PROJECT_SOURCE_DIR}/third_party/stb/include/)
target_include_directories(gl_test PUBLIC ${PROJECT_SOURCE_DIR}/third_party/glm/include/)
target_link_libraries(gl_test PUBLIC glad)

set(GLFW_DIR ${PROJECT_SOURCE_DIR}/third_party/glfw-3.3.8)
target_include_directories(gl_test PRIVATE ${GLFW_DIR}/include/)
add_subdirectory(${GLFW_DIR})
target_link_libraries(gl_test PRIVATE glfw)

# add_custom_target(res_target ALL DEPENDS ${PROJECT_BINARY_DIR}/resources)
# add_custom_command(
#     OUTPUT ${PROJECT_BINARY_DIR}/resources
#     COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources
#                                                ${PROJECT_BINARY_DIR}/resources
#     DEPENDS ${PROJECT_SOURCE_DIR}/resources)

# add_custom_command(
#     TARGET gl_test POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources
#                                                ${PROJECT_BINARY_DIR}/resources)

# set_target_properties(gl_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR})
# set_target_properties(gl_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})
