#include "obstacle.hpp"

Obstacle::Obstacle(const Map& map)
{
    size_t n = map.getMapHeight();
    size_t m = map.getMapWidth();
    cloud.pts.clear();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (map[i][j]) {
                double l = static_cast<double>(i);
                double k = static_cast<double>(j);
                cloud.pts.push_back(Point{l, k, l, k, l + 1, k + 1});
                cloud.pts.push_back(Point{l + 1, k + 1, l, k, l + 1, k + 1});
                cloud.pts.push_back(Point{l + 1, k, l, k, l + 1, k + 1});
                cloud.pts.push_back(Point{l, k + 1, l, k, l + 1, k + 1});
            }
        }
    }
    index = new kdTree(2, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10));
    index->buildIndex();
}

Obstacle::~Obstacle()
{
    delete index;
}

bool Obstacle::freeObstacle(const Geometry::Point& x, const Geometry::Point& y)
{
    const double searchRadius = static_cast<double>(CI_STEP_SIZE * CI_STEP_SIZE);
    std::vector<std::pair<size_t, double>> resMatches;
    nanoflann::SearchParams params;
    const double query_point[2] = {x.x, x.y};
    const size_t nMatches = index->radiusSearch(&query_point[0], searchRadius, resMatches, params);
    Geometry::Segment seg(x, y);
    for (size_t i = 0; i < nMatches; ++i) {
        std::cout << cloud.pts[resMatches[i].first].x1 << ' ' <<
         cloud.pts[resMatches[i].first].y1 << ' ' <<
          cloud.pts[resMatches[i].first].x2 << ' ' << cloud.pts[resMatches[i].first].y2 << '\n';
    }
    for (size_t i = 0; i < nMatches; ++i) {
        Geometry::Rectangle r(cloud.pts[resMatches[i].first].x1, cloud.pts[resMatches[i].first].y1, cloud.pts[resMatches[i].first].x2, cloud.pts[resMatches[i].first].y2);
        if (Geometry::intersectionOfSegmentAndRectangle(seg, r)) {
            return false;
        }
    }
    return true;
}