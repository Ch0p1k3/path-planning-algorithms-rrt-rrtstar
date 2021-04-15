#ifndef RRT_RRTstar_PROCESS_HPP
#define RRT_RRTstar_PROCESS_HPP

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <optional>
#include <vector>
#include <chrono>
#include "lib/SFML/include/SFML/Graphics.hpp"
#include "RRT.hpp"
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

namespace Secret
{
    void launch(const Map&, const Algorithm&);
}

#endif