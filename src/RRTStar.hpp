#ifndef RRT_RRTstar_RRT_STAR_HPP
#define RRT_RRTstar_RRT_STAR_HPP

#include "RRT.hpp"

class RRTStar: public RRT
{
public:
    RRTStar(const Map& map, const Algorithm& algo)
    : RRT(map, algo) {}

    RRTStar() = delete;
    ~RRTStar() = default;

    void insertEdge(Tree::Node*, Tree::Node*);
    void eraseEdge(Tree::Node*, Tree::Node*);
    void getNear(Tree::Node*, std::vector<Tree::Node*>&);
};

#endif