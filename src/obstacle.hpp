#ifndef RRT_RRTstar_OBSTACLE_HPP
#define RRT_RRTstar_OBSTACLE_HPP

#include <iostream>
#include <vector>
#include "lib/nanoflann/include/nanoflann.hpp"
#include "geometry.hpp"
#include "constants.hpp"
#include "map.hpp"

class Obstacle
{
private:
    struct Point
    {
        double  x, y;
        double  x1, y1, x2, y2;
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

    typedef nanoflann::KDTreeSingleIndexAdaptor<
                nanoflann::L2_Simple_Adaptor<double, PointCloud>,
                PointCloud,
                2> kdTree;

    kdTree *index;
    PointCloud cloud;
public:
    Obstacle(const Map& map);
    Obstacle() = delete;
    ~Obstacle();

    bool obstacleFree(const Geometry::Point&, const Geometry::Point&) const;
};

#endif