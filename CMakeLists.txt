cmake_minimum_required(VERSION 3.14)
project(rrt-rrtstar LANGUAGES CXX)

include(cmake/CCache.cmake)
include(cmake/CheckCompiler.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/Development.cmake)
include(cmake/Sanitize.cmake)
include(cmake/ThirdParty.cmake)

add_subdirectory(bin)
add_subdirectory(src)
add_subdirectory(third_party)
