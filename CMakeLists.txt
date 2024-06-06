cmake_minimum_required (VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(FnF LANGUAGES C CXX)

set (BUILD_SHARED_LIBS OFF)

add_subdirectory("${CMAKE_SOURCE_DIR}/extern/fox-engine")

file(GLOB_RECURSE GAME_SOURCE src/*.cpp) 
file(GLOB_RECURSE GAME_HEADERS src/*.hpp) 

set(GAME_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    ${GAME_SOURCE}
    ${GAME_HEADERS}
)

add_executable(FnF "${GAME_SOURCES}")

target_include_directories(FnF PUBLIC "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/src")

set(DEPEND_LIBRARIES
    FOX_ENGINE_LIB
)

target_precompile_headers(FnF PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp")

target_link_libraries(FnF PRIVATE ${DEPEND_LIBRARIES})
