#include <iostream>
#include <cstring>
#include "lib/SFML/include/SFML/Graphics.hpp"
#include "algorithm.hpp"
#include "geometry.hpp"
#include "obstacle.hpp"
#include "map.hpp"
#include "tree.hpp"
#include "RRT.hpp"
#include "process.hpp"
#include "constants.hpp"
#include "searchresult.hpp"
#include "log.hpp"
#include "outdata.hpp"

int main(int argc, char* argv[])
{
    if(argc < 2) {
        std::cout<<"Error! Pathfinding task file (XML) is not specified!"<<std::endl;
        return 0;
    }
    char* filePath = argv[1];
    Map m(filePath);
    Algorithm algo(filePath);
    bool hasVis = false, hasVisAfter = false, hasVisAfterWithoutTree = false, hasSecret = false;
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
        if (!hasSecret && !std::strcmp(argv[i], CS_ARG_SECRET)) {
            hasSecret = true;
        }
    }
    Log log(filePath);
    SearchResult res;
    if (hasSecret) {
        Secret::launch(m, algo);
        return 0;
    } else if (hasVisAfterWithoutTree) {
        if (algo.getSearchType() == CI_TAG_RRT) {
            res = RRTAlgorithm::launchWithVisAfterWithoutTree(m, algo);
        } else if (algo.getSearchType() == CI_TAG_RRT_STAR) {
            res = RRTStarAlgorithm::launchWithVisAfterWithoutTree(m, algo);
        }
    } else if (hasVisAfter) {
        if (algo.getSearchType() == CI_TAG_RRT) {
            res = RRTAlgorithm::launchWithVisAfter(m, algo);
        } else if (algo.getSearchType() == CI_TAG_RRT_STAR) {
            res = RRTStarAlgorithm::launchWithVisAfter(m, algo);
        }
    } else if (hasVis) {
        if (algo.getSearchType() == CI_TAG_RRT) {
            res = RRTAlgorithm::launchWithVis(m, algo);
        } else if (algo.getSearchType() == CI_TAG_RRT_STAR) {
            res = RRTStarAlgorithm::launchWithVis(m, algo);
        }
    } else {
        if (algo.getSearchType() == CI_TAG_RRT) {
            res = RRTAlgorithm::launch(m, algo);
        } else if (algo.getSearchType() == CI_TAG_RRT_STAR) {
            res = RRTStarAlgorithm::launch(m, algo);
        }
    }
    OutData::fill(log.getPath(), res);
}