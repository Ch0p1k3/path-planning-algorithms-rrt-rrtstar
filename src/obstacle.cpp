#include "obstacle.hpp"

Obstacle::Obstacle(const Map& map, const Algorithm& algo) {
  stepSize = algo.getStepSize();
  size_t n = map.getMapHeight();
  size_t m = map.getMapWidth();
  cloud.pts.clear();
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      if (map[i][j]) {
        double l = static_cast<double>(i);
        double k = static_cast<double>(j);
        cloud.pts.push_back(Point{k, l, k, l, k + 1, l + 1});
        cloud.pts.push_back(Point{k + 1, l + 1, k, l, k + 1, l + 1});
        cloud.pts.push_back(Point{k + 1, l, k, l, k + 1, l + 1});
        cloud.pts.push_back(Point{k, l + 1, k, l, k + 1, l + 1});
      }
    }
  }
  index = new kdTree(2, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(20));
  index->buildIndex();
  Geometry::Point start = map.getStart();
  Geometry::Point finish = map.getFinish();
  const size_t numResults = 4;
  size_t resIndex[4];
  double outDistSqr[4];
  double queryPt[2] = {start.x, start.y};
  nanoflann::KNNResultSet<double> resultSet(numResults);
  resultSet.init(resIndex, outDistSqr);
  index->findNeighbors(resultSet, queryPt, nanoflann::SearchParams());
  if (cloud.pts.size() >= 1) {
    for (size_t i = 0; i < resultSet.size(); ++i) {
      if (start.x >= cloud.pts[resIndex[i]].x1 &&
          start.x <= cloud.pts[resIndex[i]].x2 &&
          start.y >= cloud.pts[resIndex[i]].y1 &&
          start.y <= cloud.pts[resIndex[i]].y2) {
        std::cerr << "Start point on obstacle!\n";
        exit(EXIT_FAILURE);
      }
    }
  }
  queryPt[0] = finish.x;
  queryPt[1] = finish.y;
  nanoflann::KNNResultSet<double> resultSet2(numResults);
  resultSet2.init(resIndex, outDistSqr);
  index->findNeighbors(resultSet2, queryPt, nanoflann::SearchParams());
  if (cloud.pts.size() >= 1) {
    for (size_t i = 0; i < resultSet2.size(); ++i) {
      if (finish.x >= cloud.pts[resIndex[i]].x1 &&
          finish.x <= cloud.pts[resIndex[i]].x2 &&
          finish.y >= cloud.pts[resIndex[i]].y1 &&
          finish.y <= cloud.pts[resIndex[i]].y2) {
        std::cerr << "Finish point on obstacle!\n";
        exit(EXIT_FAILURE);
      }
    }
  }
}

Obstacle::~Obstacle() {
  if (index) {
    delete index;
  }
}

bool Obstacle::obstacleFree(const Geometry::Point& x,
                            const Geometry::Point& y) const {
  // Geometry::Segment seg(x, y);
  // if (cloud.pts.size() > 25) {
  //     const size_t numResults = 25;
  //     size_t resIndex[numResults];
  //     double outDistSqr[numResults];
  //     double queryPt[2] = {x.x, x.y};
  //     nanoflann::KNNResultSet<double> resultSet(numResults);
  //     resultSet.init(resIndex, outDistSqr);
  //     index->findNeighbors(resultSet, queryPt, nanoflann::SearchParams());
  //     for (size_t i = 0; i < resultSet.size(); ++i) {
  //         Geometry::Rectangle r(cloud.pts[resIndex[i]].x1,
  //         cloud.pts[resIndex[i]].y1, cloud.pts[resIndex[i]].x2,
  //         cloud.pts[resIndex[i]].y2); if
  //         (Geometry::intersectionOfSegmentAndRectangle(seg, r)) {
  //             return false;
  //         }
  //     }
  // } else {
  //     for (size_t i = 0; i < cloud.pts.size(); ++i) {
  //         Geometry::Rectangle r(cloud.pts[i].x1, cloud.pts[i].y1,
  //         cloud.pts[i].x2, cloud.pts[i].y2); if
  //         (Geometry::intersectionOfSegmentAndRectangle(seg, r)) {
  //             return false;
  //         }
  //     }
  // }
  // return true;
  const double searchRadius = Geometry::euclideanMetric(x, y) + 1;
  std::vector<std::pair<unsigned int, double>> resMatches;
  nanoflann::SearchParams params;
  const double query_point[2] = {x.x, x.y};
  const size_t nMatches =
      index->radiusSearch(&query_point[0], searchRadius, resMatches, params);
  Geometry::Segment seg(x, y);
  // for (size_t i = 0; i < nMatches; ++i) {
  //     std::cout << cloud.pts[resMatches[i].first].x1 << ' ' <<
  //      cloud.pts[resMatches[i].first].y1 << ' ' <<
  //       cloud.pts[resMatches[i].first].x2 << ' ' <<
  //       cloud.pts[resMatches[i].first].y2 << '\n';
  // }
  for (size_t i = 0; i < nMatches; ++i) {
    Geometry::Rectangle r(
        cloud.pts[resMatches[i].first].x1, cloud.pts[resMatches[i].first].y1,
        cloud.pts[resMatches[i].first].x2, cloud.pts[resMatches[i].first].y2);
    if (Geometry::intersectionOfSegmentAndRectangle(seg, r)) {
      return false;
    }
  }
  return true;
}
