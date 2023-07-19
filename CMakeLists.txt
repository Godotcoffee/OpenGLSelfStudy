cmake_minimum_required(VERSION 3.27)

project(opengl-learning)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

add_compile_definitions(SOURCE_ROOT=${PROJECT_SOURCE_DIR})

add_library(glad ${PROJECT_SOURCE_DIR}/third_party/glad/src/glad.c)
target_include_directories(glad PRIVATE ${PROJECT_SOURCE_DIR}/third_party/glad/include/)

file(GLOB_RECURSE TASK_CPP_SRC ${PROJECT_SOURCE_DIR}/src/task/src/*.cpp)

add_executable(gl_test ${TASK_CPP_SRC} ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_include_directories(gl_test PUBLIC ${PROJECT_SOURCE_DIR}/src/task/include/)
target_include_directories(gl_test PUBLIC ${PROJECT_SOURCE_DIR}/third_party/glad/include/)
target_include_directories(gl_test PUBLIC ${PROJECT_SOURCE_DIR}/third_party/stb/include/)
target_include_directories(gl_test PUBLIC ${PROJECT_SOURCE_DIR}/third_party/glm/include/)
target_link_libraries(gl_test PUBLIC glad)

target_include_directories(gl_test PUBLIC ${PROJECT_SOURCE_DIR}/third_party/glfw/include/)
target_link_libraries(gl_test PUBLIC ${PROJECT_SOURCE_DIR}/third_party/glfw/bin/libglfw3.a)