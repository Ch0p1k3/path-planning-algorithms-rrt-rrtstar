#include <SFML/Graphics.hpp>
#include <cstring>
#include <iostream>

#include <rrt.hpp>
#include <algorithm.hpp>
#include <constants.hpp>
#include <geometry.hpp>
#include <log.hpp>
#include <map.hpp>
#include <obstacle.hpp>
#include <outdata.hpp>
#include <process.hpp>
#include <searchresult.hpp>
#include <tree.hpp>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "Error! Pathfinding task file (XML) is not specified!"
              << std::endl;
    return 0;
  }
  char* filePath = argv[1];
  Map m(filePath);
  Algorithm algo(filePath);
  bool hasVis = false, hasVisAfter = false, hasVisAfterWithoutTree = false,
       hasSecret = false;
  for (size_t i = 2; i < argc; ++i) {
    if (!hasVis && !std::strcmp(argv[i], CS_ARG_VIS)) {
      hasVis = true;
    }
    if (!hasVisAfter && !std::strcmp(argv[i], CS_ARG_VIS_AFTER)) {
      hasVisAfter = true;
    }
    if (!hasVisAfterWithoutTree &&
        !std::strcmp(argv[i], CS_ARG_VIS_AFTER_WITHOUT_TREE)) {
      hasVisAfterWithoutTree = true;
    }
    if (!hasSecret && !std::strcmp(argv[i], CS_ARG_SECRET)) {
      hasSecret = true;
    }
  }
  Log log(filePath);
  std::pair<SearchResult, SearchResult> res;
  if (hasSecret) {
    Secret::launch(m, algo);
    return 0;
  } else if (hasVisAfterWithoutTree) {
    if (algo.getSearchType() == CI_TAG_RRT) {
      res.second = RRTAlgorithm::launchWithVisAfterWithoutTree(m, algo);
    } else if (algo.getSearchType() == CI_TAG_RRT_STAR) {
      res = RRTStarAlgorithm::launchWithVisAfterWithoutTree(m, algo);
    }
  } else if (hasVisAfter) {
    if (algo.getSearchType() == CI_TAG_RRT) {
      res.second = RRTAlgorithm::launchWithVisAfter(m, algo);
    } else if (algo.getSearchType() == CI_TAG_RRT_STAR) {
      res = RRTStarAlgorithm::launchWithVisAfter(m, algo);
    }
  } else if (hasVis) {
    if (algo.getSearchType() == CI_TAG_RRT) {
      res.second = RRTAlgorithm::launchWithVis(m, algo);
    } else if (algo.getSearchType() == CI_TAG_RRT_STAR) {
      res = RRTStarAlgorithm::launchWithVis(m, algo);
    }
  } else {
    if (algo.getSearchType() == CI_TAG_RRT) {
      res.second = RRTAlgorithm::launch(m, algo);
    } else if (algo.getSearchType() == CI_TAG_RRT_STAR) {
      res = RRTStarAlgorithm::launch(m, algo);
    }
  }
  if (algo.getSearchType() == CI_TAG_RRT) {
    OutData::fill(log.getPath(), res.second);
  } else {
    OutData::fill(log.getPath(), res.second);
    if (res.second.pathFound) {
      OutData::fillFirst(log.getPath(), res.first);
    }
  }
}
