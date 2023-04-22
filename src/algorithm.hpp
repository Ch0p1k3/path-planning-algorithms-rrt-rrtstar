#pragma once

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <pugixml.hpp>
#include <sstream>
#include <vector>

#include <constants.hpp>

class Algorithm {
 public:
  Algorithm() = delete;
  explicit Algorithm(const char*);
  ~Algorithm() = default;

  inline size_t getSearchType() const {
    // CI_TAG_RRT                      0
    // CI_TAG_RRT_STAR                 1
    return searchType;
  }

  inline size_t getNumberOfIterations() const {
    return numberOfIterations;
  }

  inline double getStepSize() const {
    return stepSize;
  }

  inline double getEps() const {
    return eps;
  }

  inline double getGamma() const {
    return gamma;
  }

 private:
  size_t searchType;
  size_t numberOfIterations;
  double stepSize;
  double eps;
  double gamma;

  bool buildAlgorithm(const char*);
};
