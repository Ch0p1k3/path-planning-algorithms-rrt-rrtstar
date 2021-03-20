#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <vector>
#include <cstdio>
#include <limits>
#include <sstream>
#include "lib/pugixml/src/pugixml.hpp"
#include "geometry.hpp"
#include "constants.hpp"

class Map
{
private:
    size_t height, width;
    Geometry::Point start;
    Geometry::Point finish;
    std::vector<std::vector<bool>> grid;

    bool buildMap(const char *);
public:
    explicit Map(const char *);
    Map() = delete;
    ~Map() = default;

    size_t getMapHeight() const;
    size_t getMapWidth() const;
    Geometry::Point getStart() const;
    Geometry::Point getFinish() const;

    const std::vector<bool>& operator[](const size_t) const;
};

#endif