#ifndef RRT_RRTstar_RRT_HPP
#define RRT_RRTstar_RRT_HPP

#include "algorithm.hpp"
#include "obstacle.hpp"
#include "tree.hpp"
#include "geometry.hpp"
#include "map.hpp"
#include "constants.hpp"
#include <random>
#include <iostream>

class RRT
{
private:
    size_t maxIter;
    size_t height;
    size_t width;
    Geometry::Point start;
    Geometry::Point finish;
    double stepSize;
    Tree tree;
    Obstacle obstacles;
public:
    RRT(const Map&, const Algorithm&);
    RRT() = delete;
    ~RRT() = default;

    Geometry::Point getRandomPoint() const;    
    Tree::Node *insertEdge(Tree::Node *, const Geometry::Point&);
    Tree::Node *getNearest(const Geometry::Point&);
    Geometry::Point steer(const Geometry::Point&, const Geometry::Point&) const;
    bool obstacleFree(const Geometry::Point&, const Geometry::Point&) const;
    void printTree(std::ostream& = std::cout);

    inline Geometry::Point getStart() const
    {
        return start;
    }

    inline Geometry::Point getFinish() const
    {
        return finish;
    }

    inline size_t getNumberOfIterations() const
    {
        return maxIter;
    }
};

#endif