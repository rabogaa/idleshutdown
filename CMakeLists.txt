cmake_minimum_required(VERSION 3.8)
project(idleshutdown)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(SOURCE_FILES main.cpp)
set(LIB_FILES lib/timer.cpp lib/timer.h)

add_executable(idleshutdown ${SOURCE_FILES})

include_directories(lib)

add_library(libtimer SHARED ${LIB_FILES})
target_link_libraries(idleshutdown libtimer)
SET_TARGET_PROPERTIES(libtimer PROPERTIES PREFIX "")
