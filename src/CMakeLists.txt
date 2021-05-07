set(HEADERS     geometry.hpp
                map.hpp
                constants.hpp
                algorithm.hpp
                lib/pugixml/src/pugiconfig.hpp
                lib/nanoflann/include/nanoflann.hpp
                obstacle.hpp
                tree.hpp
                RRT.hpp
                process.hpp
                searchresult.hpp
                outdata.hpp
                log.hpp
                RRTStar.hpp)

set(SOURCES     geometry.cpp
                map.cpp
                algorithm.cpp
                lib/pugixml/src/pugixml.cpp
                obstacle.cpp
                tree.cpp
                RRT.cpp
                process.cpp
                outdata.cpp
                log.cpp
                RRTStar.cpp)


set(SFML_DIR "${PROJECT_SOURCE_DIR}/src/lib/SFML/SFML")
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} main.cpp)
target_link_libraries(${PROJECT_NAME} sfml-graphics)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/debug)
install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/release)