set(HEADERS     geometry.hpp
                map.hpp
                constants.hpp)

set(SOURCES     geometry.cpp
                map.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} main.cpp)