#pragma once

#include <iostream>
#include <vector>
#include "geometry.hpp"

struct SearchResult
{
    double time{0};
    size_t countOfEdges{0};
    bool pathFound{false};
    double distance{0};
    std::vector<Geometry::Point> path{};

    SearchResult() = default;
    ~SearchResult() = default;

    SearchResult(
        const double time_,
        const size_t countOfEdges_,
        const bool pathFound_,
        const double distance_,
        const std::vector<Geometry::Point>& path_
    )
    : time(time_)
    , countOfEdges(countOfEdges_)
    , pathFound(pathFound_)
    , distance(distance_)
    , path(path_) {}

    SearchResult(const SearchResult& other)
    {
        SearchResult(other.time, other.countOfEdges, other.pathFound, other.distance, other.path);
    }

    SearchResult& operator=(const SearchResult& other) {
        time = other.time;
        countOfEdges = other.countOfEdges;
        pathFound = other.pathFound;
        distance = other.distance;
        path = other.path;
        return *this;
    }
};

