set(HEADERS     geometry.hpp
                map.hpp
                constants.hpp
                algorithm.hpp
                lib/pugixml/src/pugiconfig.hpp
                lib/nanoflann/include/nanoflann.hpp
                obstacle.hpp
                tree.hpp
                RRT.hpp
                process.hpp)

set(SOURCES     geometry.cpp
                map.cpp
                algorithm.cpp
                lib/pugixml/src/pugixml.cpp
                obstacle.cpp
                tree.cpp
                RRT.cpp
                process.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} main.cpp)