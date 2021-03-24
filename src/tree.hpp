#ifndef RRT_RRTstar_TREE_HPP
#define RRT_RRTstar_TREE_HPP

#include <iostream>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "geometry.hpp"
#include "lib/nanoflann/include/nanoflann.hpp"

class Tree
{
public:
    struct Node
    {
        std::vector<std::shared_ptr<Node>> childrens;
        std::shared_ptr<Node> parent;
        Geometry::Point point;
        double distance;

        Node(std::vector<std::shared_ptr<Node>> otherChildrens,
            std::shared_ptr<Node> otherParent,
            Geometry::Point otherPoint,
            double otherDistance)
            : childrens(otherChildrens)
            , parent(otherParent)
            , point(otherPoint)
            , distance(otherDistance) {}

        // Node(const Tree::Node& other)
        // : childrens(other.childrens)
        // , parent(other.parent)
        // , point(other.point)
        // , distance(other.distance) {}

        // Node(Node&& other)
        // : childrens(std::move(other.childrens))
        // , parent(std::move(other.parent))
        // , point(std::move(other.point))
        // , distance(other.distance) {}
    };

private:
    std::shared_ptr<Node> root;
    
    struct Point
    {
        double  x, y;
        std::shared_ptr<Node> link;
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
    void printNode(const std::shared_ptr<Node>, std::ostream&);

public:
    Tree() = delete;
    Tree(const Geometry::Point&);
    ~Tree();

    std::shared_ptr<Node> getNearest(const Geometry::Point&) const;
    std::shared_ptr<Node> insert(std::shared_ptr<Node>, const Geometry::Point&);
    void printTree(std::ostream& = std::cout);
};

#endif