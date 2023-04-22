#include "rrt.hpp"

RRT::RRT(const Map& map, const Algorithm& algo)
    : maxIter(algo.getNumberOfIterations()),
      height(map.getMapHeight()),
      width(map.getMapWidth()),
      start(map.getStart()),
      finish(map.getFinish()),
      stepSize(algo.getStepSize()),
      eps(algo.getEps()),
      tree(map.getStart(), algo.getStepSize()),
      obstacles(map, algo),
      gen((std::random_device())()) {
}

Geometry::Point RRT::getRandomPoint() {
  // std::random_device rd;
  // std::mt19937 gen1(rd());
  std::uniform_real_distribution<> xs(0, width);
  // std::mt19937 gen2(rd());
  std::uniform_real_distribution<> ys(0, height);
  return Geometry::Point{xs(gen), ys(gen)};
}

Tree::Node* RRT::insertVertexAndEdge(Tree::Node* x, const Geometry::Point& y) {
  return tree.insertVertexAndEdge(x, y);
}

Tree::Node* RRT::getNearest(const Geometry::Point& p) {
  return tree.getNearest(p);
}

Geometry::Point RRT::steer(const Geometry::Point& x,
                           const Geometry::Point& y) const {
  double norm = std::sqrt(Geometry::euclideanMetric(x, y));
  if (norm <= stepSize) {
    return y;
  }
  return x + (((y - x) / norm) * stepSize);
}

bool RRT::obstacleFree(const Geometry::Point& x,
                       const Geometry::Point& y) const {
  return obstacles.obstacleFree(x, y);
}

// void RRT::printTree(std::ostream& out)
// {
//     tree.printTree(out);
// }

void RRT::drawTree(sf::RenderWindow& window) {
  tree.drawTree(window);
}
