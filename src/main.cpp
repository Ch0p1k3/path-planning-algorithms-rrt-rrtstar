#include <iostream>
#include <cstring>
#include "algorithm.hpp"
#include "geometry.hpp"
#include "obstacle.hpp"
#include "map.hpp"
#include "tree.hpp"
#include "RRT.hpp"
#include "process.hpp"
#include "constants.hpp"
#include "lib/SFML/include/SFML/Graphics.hpp"

int main(int argc, char *argv[])
{
    if(argc < 2) {
        std::cout<<"Error! Pathfinding task file (XML) is not specified!"<<std::endl;
        return 0;
    }
    char *filePath = argv[1];
    Map m(argv[1]);
    Algorithm algo(argv[1]);
    bool hasVis = false, hasVisAfter = false, hasVisAfterWithoutTree = false;
    for (size_t i = 2; i < argc; ++i) {
        if (!hasVis && !std::strcmp(argv[i], CS_ARG_VIS)) {
            hasVis = true;
        }
        if (!hasVisAfter && !std::strcmp(argv[i], CS_ARG_VIS_AFTER)) {
            hasVisAfter = true;
        }
        if (!hasVisAfterWithoutTree && !std::strcmp(argv[i], CS_ARG_VIS_AFTER_WITHOUT_TREE)) {
            hasVisAfterWithoutTree = true;
        }
    }
    if (hasVisAfterWithoutTree) {
        RRTAlgorithm::launchWithVisAfterWithoutTree(m, algo);
    } else if (hasVisAfter) {
        RRTAlgorithm::launchWithVisAfter(m, algo);
    } else if (hasVis) {
        RRTAlgorithm::launchWithVis(m, algo);
    } else {
        RRTAlgorithm::launch(m, algo);
    }
}