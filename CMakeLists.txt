cmake_minimum_required(VERSION 3.29)
project(Hopfield-network-cpp)

set(CMAKE_CXX_STANDARD 20)



# Укажи пути к исходникам и заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR}/include)
# Укажи исходники
set(SOURCES
        src/main.cpp
)

add_executable(run src/main.cpp)


