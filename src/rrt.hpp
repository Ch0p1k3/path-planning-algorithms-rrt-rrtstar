#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include <algorithm.hpp>
#include <constants.hpp>
#include <geometry.hpp>
#include <map.hpp>
#include <obstacle.hpp>
#include <tree.hpp>

class RRT {
 public:
  RRT(const Map&, const Algorithm&);
  RRT() = delete;
  ~RRT() = default;

  Geometry::Point getRandomPoint();
  Tree::Node* insertVertexAndEdge(Tree::Node*, const Geometry::Point&);
  Tree::Node* getNearest(const Geometry::Point&);
  Geometry::Point steer(const Geometry::Point&, const Geometry::Point&) const;
  bool obstacleFree(const Geometry::Point&, const Geometry::Point&) const;
  // void printTree(std::ostream& = std::cout);
  void drawTree(sf::RenderWindow& window);

  inline Geometry::Point getStart() const {
    return start;
  }

  inline Geometry::Point getFinish() const {
    return finish;
  }

  inline size_t getNumberOfIterations() const {
    return maxIter;
  }

  inline double getStepSize() const {
    return stepSize;
  }

  inline double getEps() const {
    return eps;
  }

 protected:
  size_t maxIter;
  size_t height;
  size_t width;
  Geometry::Point start;
  Geometry::Point finish;
  double stepSize;
  double eps;
  Tree tree;
  Obstacle obstacles;

 private:
  std::mt19937 gen;
};
