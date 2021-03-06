cmake_minimum_required(VERSION 3.17.3)
project(RRT-RRTstar)

if (${DCMAKE})

if (${MINGW})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -Wall -Wextra")
else ()
endif()

add_subdirectory(src)