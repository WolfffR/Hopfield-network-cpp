cmake_minimum_required(VERSION 3.29)
project(Hopfield)

set(CMAKE_CXX_STANDARD 20)



# Укажи пути к исходникам и заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR}/include)
# Укажи исходники
set(SOURCES
        src/run_net.cpp
)

add_executable(old src/main.cpp)
add_executable(run src/run_net.cpp)


