#ifndef RRT_RRTstar_TREE_HPP
#define RRT_RRTstar_TREE_HPP

#include <iostream>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "geometry.hpp"
#include "lib/nanoflann/include/nanoflann.hpp"
#include "lib/SFML/include/SFML/Graphics.hpp"

class Tree
{
public:
    struct Node
    {
        std::vector<Node *> childrens;
        Node *parent;
        Geometry::Point point;
        double distance;

        Node(const std::vector<Node *>& otherChildrens,
            Node *otherParent,
            const Geometry::Point& otherPoint,
            double otherDistance)
            : childrens(otherChildrens.begin(), otherChildrens.end())
            , parent(otherParent)
            , point(otherPoint)
            , distance(otherDistance) {}

        Node(const Tree::Node& other)
        : childrens(other.childrens)
        , parent(other.parent)
        , point(other.point)
        , distance(other.distance) {}

        Node(Node&& other)
        : childrens(std::move(other.childrens))
        , parent(std::move(other.parent))
        , point(std::move(other.point))
        , distance(other.distance) {}

        bool operator==(const Tree::Node& other) {
            return other.childrens == childrens 
            && parent == other.parent
            && point == other.point
            && distance == other.distance;
        }
    };

private:
    Node *root;
    
    struct Point
    {
        double  x, y;
        Node *link;
    };

    struct PointCloud
    {
        std::vector<Point> pts;

        inline size_t kdtree_get_point_count() const { return pts.size(); }

        inline double kdtree_get_pt(const size_t idx, const size_t dim) const
        {
            if (dim >= 2) {
                std::cerr << "Error! dim.\n";
                exit(EXIT_FAILURE);
            }

            if (dim == 0) return pts[idx].x;
            
            return pts[idx].y;
        }

        template <class BBOX>
	    bool kdtree_get_bbox(BBOX&) const { return false; }
    };

    typedef nanoflann::KDTreeSingleIndexDynamicAdaptor<
		nanoflann::L2_Simple_Adaptor<double, PointCloud> ,
	    PointCloud,
		2> kdTree;

    kdTree *index;
    PointCloud cloud;
    std::unordered_map<double, std::unordered_set<double>> used;

    struct nodePrintStateT
    {
        nodePrintStateT* childState;
        bool printingLastChild;
    };
    nodePrintStateT* rootState;
    void printNode(const Node *, std::ostream&);
    void deleteTree(Node *);
    void recDrawTree(const Node *, const Node *, sf::RenderWindow&);
public:
    Tree() = delete;
    Tree(const Geometry::Point&);
    ~Tree();

    Node *getNearest(const Geometry::Point&) const;
    Node *insert(Node *, const Geometry::Point&);
    void printTree(std::ostream& = std::cout);
    void drawTree(sf::RenderWindow&);
};

#endif