add_library(
    lib
    rrt.cpp
    rrtstar.cpp
    algorithm.cpp
    geometry.cpp
    log.cpp
    map.cpp
    obstacle.cpp
    outdata.cpp
    process.cpp
    tree.cpp
)
target_link_libraries(
    lib
    sfml-graphics
    pugixml
    nanoflann
)
