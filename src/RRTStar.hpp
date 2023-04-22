#pragma once

#include "RRT.hpp"

class RRTStar: public RRT
{
public:
    RRTStar(const Map& map, const Algorithm& algo)
    : RRT(map, algo), gamma(algo.getGamma()) {}

    RRTStar() = delete;
    ~RRTStar() = default;

    void changeEdge(Tree::Node*, Tree::Node*, Tree::Node*);
    void getNear(const Geometry::Point&, std::vector<Tree::Node*>&);

private:
    double gamma{stepSize};
};
