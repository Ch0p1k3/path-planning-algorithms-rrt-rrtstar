#ifndef RRT_RRTstar_GEOMETRY_HPP
#define RRT_RRTstar_GEOMETRY_HPP

#include <iostream>
#include <algorithm>
#include <cmath>

namespace Geometry
{
    const double EPS = 1e-9;

    struct Point
    {
        double x, y;

        Point() = default;
        
        Point(const double x1, const double y1)
        : x(x1)
        , y(y1) {}

        inline bool operator==(const Geometry::Point a) const
        {
            return abs(x - a.x) < Geometry::EPS && (y - a.y) < Geometry::EPS;
        }

        inline Geometry::Point operator-(const Geometry::Point& other) const
        {
            return Geometry::Point(x - other.x, y - other.y);
        }

        inline Geometry::Point operator+(const Geometry::Point& other) const
        {
            return Geometry::Point(x + other.x, y + other.y);
        }

        inline Geometry::Point operator/(const double lambda) const
        {
            return Geometry::Point(x / lambda, y / lambda);
        }

        inline Geometry::Point operator*(const double lambda) const
        {
            return Geometry::Point(x * lambda, y * lambda);
        }
    };

    struct Rectangle
    {
        Point first, second;

        Rectangle() = default;

        Rectangle(const Geometry::Point first1, const Geometry::Point second1)
        : first(first1)
        , second(second1) {}

        Rectangle(double x1, double y1, double x2, double y2)
        : first(x1, y1)
        , second(x2, y2) {}
    };

    struct Segment
    {
        Point first, second;

        Segment() = default;

        Segment(const Geometry::Point first1, const Geometry::Point second1)
        : first(first1)
        , second(second1) {}

        Segment(double x1, double y1, double x2, double y2)
        : first(x1, y1)
        , second(x2, y2) {}
    };

    struct Line
    {
        double a, b, c;

        Line(const Geometry::Point p, const Geometry::Point q) {
            a = p.y - q.y;
            b = q.x - p.x;
            c = -a * p.x - b * p.y;
        }

        double operator()(const Geometry::Point p){
            return p.x * a + b * p.y + c;
        }
    };

    template <typename T> 
    inline int sgn(const T val)
    {
        return (T(0) < val) - (val <= T(0));
    }

    double euclideanMetric(const Geometry::Point&, const Geometry::Point&);
    double areaOfTreangle(const Geometry::Point&, const Geometry::Point&, const Geometry::Point&);
    int signAreaOfTreangle(const Geometry::Point&, const Geometry::Point&, const Geometry::Point&);
    bool intersectionOfTwoSegments(const Geometry::Segment&, const Geometry::Segment&);
    bool intersectionOfSegmentAndRectangle(const Geometry::Segment&, const Geometry::Rectangle&);
}

std::ostream& operator<<(std::ostream& out, const Geometry::Point&);
#endif