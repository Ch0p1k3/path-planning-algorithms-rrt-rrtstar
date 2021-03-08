#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <vector>
#include <cstdio>
#include "lib/pugixml/src/pugixml.hpp"
#include "geometry.hpp"
#include "constants.hpp"

class Map {
private:
    ssize_t height, width;
    Geometry::Point start;
    Geometry::Point finish;
    std::vector<std::vector<bool>> grid;

public:
    Map() = default;
    ~Map() = default;

    bool buildMap(const char *);
    ssize_t getMapHeight() const;
    ssize_t getMapWidth() const;
    Geometry::Point getStart() const;
    Geometry::Point getFinish() const;
};

#endif