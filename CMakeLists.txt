cmake_minimum_required(VERSION 3.0)
project(RRT-RRTstar LANGUAGES C CXX)

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O0 -Wall -Wextra -g")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -Wall -Wextra -g")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_subdirectory(src)