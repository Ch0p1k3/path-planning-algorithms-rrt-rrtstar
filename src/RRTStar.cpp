#include "RRTStar.hpp"

void RRTStar::changeEdge(Tree::Node* parent, Tree::Node* son, Tree::Node* newParent)
{
    tree.changeEdge(parent, son, newParent);
}

void RRTStar::getNear(const Geometry::Point& x, std::vector<Tree::Node*>& res)
{
    tree.getNear(x, gamma, res);
}