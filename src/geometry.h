#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <algorithm>
#include <cmath>

namespace Geometry {

    const double EPS = 1e-9;

    struct Point {
        double x, y;

        Point() = default;
        
        Point(const double x1, const double y1)
        : x(x1)
        , y(y1) {}

        bool operator== (const Geometry::Point a) const {
            return abs(x - a.x) < Geometry::EPS && (y - a.y) < Geometry::EPS;
        }
    };

    struct Rectangle {
        Point first, second;

        Rectangle() = default;

        Rectangle(const Geometry::Point first1, const Geometry::Point second1)
        : first(first1)
        , second(second1) {}

        Rectangle(double x1, double y1, double x2, double y2)
        : first(x1, y1)
        , second(x2, y2) {}
    };

    struct Segment {
        Point first, second;

        Segment() = default;

        Segment(const Geometry::Point first1, const Geometry::Point second1)
        : first(first1)
        , second(second1) {}

        Segment(double x1, double y1, double x2, double y2)
        : first(x1, y1)
        , second(x2, y2) {}
    };

    struct Line {
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
    int sgn(const T val) {
        return (T(0) < val) - (val <= T(0));
    }

    double euclidean_metric(const Geometry::Point, const Geometry::Point);
    double area_of_treangle(const Geometry::Point, const Geometry::Point, const Geometry::Point);
    int sign_area_of_treangle(const Geometry::Point, const Geometry::Point, const Geometry::Point);
    bool intersection_of_two_segments(const Geometry::Segment, const Geometry::Segment);
    bool intersection_of_segment_and_rectangle(const Geometry::Segment, const Geometry::Rectangle);
}
#endif