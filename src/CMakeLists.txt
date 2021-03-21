set(HEADERS     geometry.hpp
                map.hpp
                constants.hpp
                algorithm.hpp)

set(SOURCES     geometry.cpp
                map.cpp
                algorithm.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} main.cpp)