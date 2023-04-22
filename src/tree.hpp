#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <nanoflann.hpp>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <geometry.hpp>

class Tree {
 public:
  struct Node {
    std::unordered_set<Node*> childrens;
    Node* parent;
    Geometry::Point point;

    Node(const std::unordered_set<Node*>& otherChildrens, Node* otherParent,
         const Geometry::Point& otherPoint)
        : childrens(otherChildrens.begin(), otherChildrens.end()),
          parent(otherParent),
          point(otherPoint) {
    }

    Node(const Tree::Node& other)
        : childrens(other.childrens), parent(other.parent), point(other.point) {
    }

    Node(Node&& other)
        : childrens(std::move(other.childrens)),
          parent(std::move(other.parent)),
          point(std::move(other.point)) {
    }

    bool operator==(const Tree::Node& other) {
      return other.childrens == childrens && parent == other.parent &&
             point == other.point;
    }

    double getDistance() const {
      double res = 0;
      const Node* prev = this;
      const Node* cur = prev->parent;
      while (cur != nullptr) {
        res += std::sqrt(Geometry::euclideanMetric(prev->point, cur->point));
        prev = cur;
        cur = cur->parent;
      }
      return res;
    }
  };

  Tree() = delete;
  Tree(const Geometry::Point&, const double);
  ~Tree();

  Node* getNearest(const Geometry::Point&) const;
  Node* insertVertexAndEdge(Node*, const Geometry::Point&);
  void changeEdge(Node*, Node*, Node*);
  void getNear(const Geometry::Point&, const double gamma, std::vector<Node*>&);
  // void getInRadius(const Geometry::Point&, double, std::vector<Node*>&);
  // void printTree(std::ostream& = std::cout);
  void drawTree(sf::RenderWindow&);

 private:
  Node* root;
  size_t N{0};
  double stepSize{0};

  struct Point {
    double x, y;
    Node* link;
  };

  struct PointCloud {
    std::vector<Point> pts;

    inline size_t kdtree_get_point_count() const {
      return pts.size();
    }

    inline double kdtree_get_pt(const size_t idx, const size_t dim) const {
      if (dim >= 2) {
        std::cerr << "Error! dim.\n";
        exit(EXIT_FAILURE);
      }

      if (dim == 0)
        return pts[idx].x;

      return pts[idx].y;
    }

    template <class BBOX>
    bool kdtree_get_bbox(BBOX&) const {
      return false;
    }
  };

  typedef nanoflann::KDTreeSingleIndexDynamicAdaptor<
      nanoflann::L2_Simple_Adaptor<double, PointCloud>, PointCloud, 2>
      kdTree;

  kdTree* index;
  PointCloud cloud;
  std::unordered_map<double, std::unordered_set<double>> used;

  struct nodePrintStateT {
    nodePrintStateT* childState;
    bool printingLastChild;
  };
  nodePrintStateT* rootState;
  // void printNode(const Node* , std::ostream&);
  void deleteTree(Node*);
  void recDrawTree(const Node*, const Node*, sf::RenderWindow&);
};
