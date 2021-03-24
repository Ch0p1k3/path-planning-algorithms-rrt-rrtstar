#include "RRT.hpp"

RRT::RRT(const Map& map, const Algorithm& algo)
: maxIter(algo.getNumberOfIterations())
, height(map.getMapHeight())
, width(map.getMapWidth())
, start(map.getStart())
, finish(map.getFinish())
, stepSize(CI_STEP_SIZE)
, tree(map.getStart())
, obstacles(map) {}

Geometry::Point RRT::getRandomPoint() const
{
    std::random_device rd;
    std::mt19937 gen1(rd());
    std::uniform_real_distribution<> xs(0, width + 1);
    std::mt19937 gen2(rd());
    std::uniform_real_distribution<> ys(0, height + 1);
    return Geometry::Point{xs(gen1), ys(gen2)};
}

std::shared_ptr<Tree::Node> RRT::insertEdge(std::shared_ptr<Tree::Node> x, const Geometry::Point& y)
{
    return tree.insert(x, y);
}

std::shared_ptr<Tree::Node> RRT::getNearest(const Geometry::Point& p)
{
    return tree.getNearest(p);
}

Geometry::Point RRT::steer(const Geometry::Point& x, const Geometry::Point& y) const
{
    double norm = Geometry::euclideanMetric(x, y);

    return x + (((y - x) / norm) * stepSize);
}

bool RRT::obstacleFree(const Geometry::Point& x, const Geometry::Point& y) const
{
    return obstacles.obstacleFree(x, y);
}

void RRT::printTree(std::ostream& out)
{
    tree.printTree(out);
}