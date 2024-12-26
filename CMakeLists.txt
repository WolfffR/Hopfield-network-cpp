cmake_minimum_required(VERSION 3.29)
project(Hopfield-network-cpp)

set(CMAKE_CXX_STANDARD 20)
file(COPY ${CMAKE_SOURCE_DIR}/Tests DESTINATION ${CMAKE_BINARY_DIR})

#set(CMAKE_EXE_LINKER_FLAGS "-static")
# Укажи пути к исходникам и заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR}/include)
# Укажи исходники
set(SOURCES
        src/main.cpp
)

add_executable(run src/main.cpp)


