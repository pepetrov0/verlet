cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(verlet)

## INCLUDE MODULES

include(FetchContent)

## FLAGS

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(FETCHCONTENT_QUIET NO)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

## DEPS

FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/4.5.0.tar.gz
)

FetchContent_MakeAvailable(raylib)

## EXECUTABLE

file(
  GLOB_RECURSE PLAYGROUND_SOURCES 
  RELATIVE "${PROJECT_SOURCE_DIR}" 
  "src/*.cpp" "include/*.h"
)

add_executable(${PROJECT_NAME} ${PLAYGROUND_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/assets
  COMMENT "Copying assets"
  VERBATIM
)