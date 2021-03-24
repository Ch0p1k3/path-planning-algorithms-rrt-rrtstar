#ifndef RRT_RRTstar_PROCESS_HPP
#define RRT_RRTstar_PROCESS_HPP

#include <algorithm>
#include <optional>
#include <vector>
#include "RRT.hpp"
#include "map.hpp"
#include "algorithm.hpp"
#include "geometry.hpp"
namespace RRTAlgorithm
{
    void launch(const Map&, const Algorithm&);
}

#endif