#ifndef RRT_RRTstar_MAP_HPP
#define RRT_RRTstar_MAP_HPP

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

    size_t getMapHeight() const
    {
        return height;
    }

    size_t getMapWidth() const
    {
        return width;
    }

    Geometry::Point getStart() const
    {
        return start;
    }

    Geometry::Point getFinish() const
    {
        return finish;
    }

    const std::vector<bool>& operator[](const size_t) const;
};

#endif