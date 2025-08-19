cmake_minimum_required(VERSION 3.10)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
project(OpenGLApp)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(main src/main.cpp)

# Add this line to include your header directory
target_include_directories(main PRIVATE include)

target_link_libraries(main ${GLFW_LIBRARIES} GL GLEW)
target_compile_options(main PRIVATE ${GLFW_CFLAGS_OTHER})