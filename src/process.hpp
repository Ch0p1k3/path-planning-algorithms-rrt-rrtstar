#pragma once

#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <optional>
#include <vector>

#include <rrt.hpp>
#include <rrtstar.hpp>
#include <algorithm.hpp>
#include <geometry.hpp>
#include <map.hpp>
#include <searchresult.hpp>

namespace RRTAlgorithm {
const SearchResult launch(const Map&, const Algorithm&);

const SearchResult launchWithVis(const Map&, const Algorithm&);

const SearchResult launchWithVisAfter(const Map&, const Algorithm&);

const SearchResult launchWithVisAfterWithoutTree(const Map&, const Algorithm&);
}  // namespace RRTAlgorithm

namespace RRTStarAlgorithm {
const std::pair<SearchResult, SearchResult> launch(const Map&,
                                                   const Algorithm&);

const std::pair<SearchResult, SearchResult> launchWithVis(const Map&,
                                                          const Algorithm&);

const std::pair<SearchResult, SearchResult> launchWithVisAfter(
    const Map&, const Algorithm&);

const std::pair<SearchResult, SearchResult> launchWithVisAfterWithoutTree(
    const Map&, const Algorithm&);
}  // namespace RRTStarAlgorithm

namespace Secret {
void launch(const Map&, const Algorithm&);
}
