#ifndef RRT_RRTstar_PROCESS_HPP
#define RRT_RRTstar_PROCESS_HPP

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <optional>
#include <vector>
#include <chrono>
#include "RRT.hpp"
#include "map.hpp"
#include "algorithm.hpp"
#include "geometry.hpp"
#include "lib/SFML/include/SFML/Graphics.hpp"

namespace RRTAlgorithm
{
    void launch(const Map&, const Algorithm&);

    void launchWithVis(const Map&, const Algorithm&);

    void launchWithVisAfter(const Map&, const Algorithm&);

    void launchWithVisAfterWithoutTree(const Map&, const Algorithm&);
}

#endif