#include "RRTStar.hpp"

void RRTStar::insertEdge(Tree::Node* parent, Tree::Node* son)
{
    tree.insertEdge(parent, son);
}

void RRTStar::eraseEdge(Tree::Node* parent, Tree::Node* son)
{
    tree.eraseEdge(parent, son);
}

void RRTStar::getNear(Tree::Node* x, std::vector<Tree::Node*>& res)
{
    tree.getNear(x, res);
}