cmake_minimum_required(VERSION 3.18)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(DCS)

find_package(Freetype REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(external/glad)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(DCS ${SOURCES})
target_link_libraries(DCS PUBLIC glad stdc++exp Freetype::Freetype glfw glm::glm nlohmann_json::nlohmann_json)
target_include_directories(DCS PUBLIC ${CMAKE_SOURCE_DIR}/external/)
target_compile_definitions(DCS PUBLIC GLFW_INCLUDE_NONE)
