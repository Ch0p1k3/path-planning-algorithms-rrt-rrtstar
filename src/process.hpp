#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <optional>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "RRT.hpp"
#include "RRTStar.hpp"
#include "map.hpp"
#include "algorithm.hpp"
#include "geometry.hpp"
#include "searchresult.hpp"

namespace RRTAlgorithm
{
    const SearchResult launch(const Map&, const Algorithm&);

    const SearchResult launchWithVis(const Map&, const Algorithm&);

    const SearchResult launchWithVisAfter(const Map&, const Algorithm&);

    const SearchResult launchWithVisAfterWithoutTree(const Map&, const Algorithm&);
}

namespace RRTStarAlgorithm
{
    const std::pair<SearchResult, SearchResult> launch(const Map&, const Algorithm&);

    const std::pair<SearchResult, SearchResult> launchWithVis(const Map&, const Algorithm&);

    const std::pair<SearchResult, SearchResult> launchWithVisAfter(const Map&, const Algorithm&);

    const std::pair<SearchResult, SearchResult> launchWithVisAfterWithoutTree(const Map&, const Algorithm&);
}

namespace Secret
{
    void launch(const Map&, const Algorithm&);
}
