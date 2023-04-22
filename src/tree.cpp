#include "tree.hpp"

Tree::Tree(const Geometry::Point& p, const double step) {
  N = 1;
  stepSize = step;
  root = new Node(std::unordered_set<Node*>(), nullptr, p);
  cloud.pts.clear();
  cloud.pts.push_back(Tree::Point{p.x, p.y, root});
  index = new kdTree(2, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(20));
  used.clear();
  rootState = nullptr;
}

void Tree::deleteTree(Node* root) {
  if (!root)
    return;
  for (Node* n : root->childrens) {
    deleteTree(n);
  }
  if (root) {
    delete root;
  }
}

Tree::~Tree() {
  if (index) {
    delete index;
  }
  deleteTree(root);
}

Tree::Node* Tree::getNearest(const Geometry::Point& p) const {
  const size_t numResults = 1;
  size_t resIndex;
  double outDistSqr;
  double queryPt[2] = {p.x, p.y};
  nanoflann::KNNResultSet<double> resultSet(numResults);
  resultSet.init(&resIndex, &outDistSqr);
  index->findNeighbors(resultSet, queryPt, nanoflann::SearchParams());
  return cloud.pts[resIndex].link;
}

void Tree::getNear(const Geometry::Point& x, const double gamma,
                   std::vector<Tree::Node*>& result) {
  result.clear();
  double distance =
      std::min(stepSize, gamma * std::sqrt(log((double)N) / (double)(N)));
  distance *= distance;
  std::vector<std::pair<size_t, double>> resMatches;
  double queryPt[2] = {x.x, x.y};
  nanoflann::RadiusResultSet<double, size_t> resultSet(distance, resMatches);
  index->findNeighbors(resultSet, queryPt, nanoflann::SearchParams());
  for (const auto& [index, _] : resMatches) {
    result.push_back(cloud.pts[index].link);
  }
  // const size_t numResults = std::ceil(8 * std::exp(1) * 1.5 * log((double)N +
  // 1)); size_t resIndex[numResults]; double outDistSqr[numResults];
  // nanoflann::KNNResultSet<double> resultSet(numResults);
  // resultSet.init(resIndex, outDistSqr);
  // index->findNeighbors(resultSet, queryPt, nanoflann::SearchParams(10));
  // for (size_t i = 0; i < resultSet.size(); ++i) {
  //     result.push_back(cloud.pts[resIndex[i]].link);
  // }
}

// void Tree::getInRadius(const Geometry::Point& x, double distance,
// std::vector<Tree::Node* >& result)
// {
//     result.clear();
//     distance *= distance;
//     std::vector<std::pair<size_t, double>> resMatches;
//     double queryPt[2] = {x.x, x.y};
//     nanoflann::RadiusResultSet<double, size_t> resultSet(distance,
//     resMatches); index->findNeighbors(resultSet, queryPt,
//     nanoflann::SearchParams()); for (const auto& [index, _]: resMatches) {
//         result.push_back(cloud.pts[index].link);
//     }
// }

void Tree::changeEdge(Tree::Node* parent, Tree::Node* son,
                      Tree::Node* newParent) {
  parent->childrens.erase(son);
  son->parent = newParent;
  newParent->childrens.insert(son);
}

Tree::Node* Tree::insertVertexAndEdge(Tree::Node* node,
                                      const Geometry::Point& p) {
  if (used.find(p.x) != used.end() && used[p.x].find(p.y) != used[p.x].end()) {
    return nullptr;
  }
  Node* newNode = new Node(std::unordered_set<Node*>(), node, p);
  node->childrens.insert(newNode);
  cloud.pts.push_back(Tree::Point{p.x, p.y, newNode});
  index->addPoints(cloud.pts.size() - 1, cloud.pts.size() - 1);
  ++N;
  return newNode;
}

// void Tree::printNode(const Tree::Node* node, std::ostream& out)
// {
//     nodePrintStateT* parentState;
//     if (rootState != nullptr) {
//         out << " ";
//         nodePrintStateT* s = rootState;
//         while (s->childState != nullptr) {
//             out << (s->printingLastChild ? "  " : "| ");
//             s = s->childState;
//         }
//         parentState = s;
//         out << (parentState->printingLastChild ? "L" : "+");
//     } else {
//         parentState = nullptr;
//     }
//     out << '>' << node->point << '\n';
//     if (!node->childrens.empty()) {
//         nodePrintStateT s;
//         if (parentState != nullptr) {
//             parentState->childState = &s;
//         } else {
//             rootState = &s;
//         }
//         s.childState = nullptr;
//         for (size_t i = 0; i < node->childrens.size(); ++i) {
//             if (i == node->childrens.size() - 1) {
//                 s.printingLastChild = true;
//                 printNode(node->childrens[i], out);
//             } else {
//                 s.printingLastChild = false;
//                 printNode(node->childrens[i], out);
//             }
//         }
//         if (parentState != nullptr) {
//            parentState->childState = nullptr;
//         } else {
//             rootState = nullptr;
//         }
//     }
// }

// void Tree::printTree(std::ostream& out)
// {
//     printNode(root, out);
// }

void Tree::recDrawTree(const Node* root, const Node* par,
                       sf::RenderWindow& window) {
  sf::Event event;

  if (!window.isOpen())
    return;

  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }
  }
  auto height = window.getView().getSize().x;
  if (!root)
    return;
  sf::CircleShape nodeCircle((height / 100) * 0.1);
  nodeCircle.setFillColor(sf::Color::Black);
  nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
  nodeCircle.setPosition(sf::Vector2f(root->point.x, root->point.y));
  window.draw(nodeCircle);
  if (par) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(root->point.x, root->point.y)),
        sf::Vertex(sf::Vector2f(par->point.x, par->point.y)),
    };
    line[0].color = sf::Color::Black;
    line[1].color = sf::Color::Black;
    window.draw(line, 2, sf::Lines);
  }

  for (const Node* child : root->childrens) {
    if (!window.isOpen())
      return;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    recDrawTree(child, root, window);
  }
}

void Tree::drawTree(sf::RenderWindow& window) {
  recDrawTree(root, nullptr, window);
}