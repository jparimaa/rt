cmake_minimum_required(VERSION 3.5)
project(rt-weekend VERSION 0.0.1 LANGUAGES CXX)

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "64bit is required")
endif()

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

set(EXE_NAME rt-weekend1)
add_executable(${EXE_NAME} ${SOURCES})

target_compile_features(${EXE_NAME} PUBLIC cxx_std_11)

target_include_directories(${EXE_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/external/stb"
)

if(MSVC)
    target_compile_options(${EXE_NAME} PRIVATE /W3 /WX /MP)
else()
    target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
