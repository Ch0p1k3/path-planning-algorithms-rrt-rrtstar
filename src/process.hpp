#ifndef RRT_RRTstar_PROCESS_HPP
#define RRT_RRTstar_PROCESS_HPP

#include <algorithm>
#include <optional>
#include <vector>
#include "RRT.hpp"
#include "map.hpp"
#include "algorithm.hpp"
#include "geometry.hpp"
#include "lib/SFML/include/SFML/Graphics.hpp"

namespace RRTAlgorithm
{
    void launch(const Map&, const Algorithm&);

    void launchWithVirt(const Map&, const Algorithm&);

    void launchWithVirtAfter(const Map&, const Algorithm&);

    void launchWithVirtAfterWithoutTree(const Map&, const Algorithm&);
}

#endif