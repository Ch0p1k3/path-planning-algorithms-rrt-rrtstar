#include "geometry.h"

double Geometry::euclidean_metric(const Geometry::Point a, const Geometry::Point b) {
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

double Geometry::area_of_treangle(const Geometry::Point a, const Geometry::Point b, const Geometry::Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

int Geometry::sign_area_of_treangle(const Geometry::Point a, const Geometry::Point b, const Geometry::Point c) {
    return Geometry::sgn<double>((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}

bool Geometry::intersection_of_two_segments(const Geometry::Segment a, const Geometry::Segment b) {
    auto help = [](double a, double b, double c, double d) {
        if (a > b) std::swap(a, b);
        if (c > d) std::swap(c, d);
        return std::max(a,c) <= std::min(b,d) + Geometry::EPS;
    };

    if (!help(a.first.x, a.second.x, b.first.x, b.second.x) || !help(a.first.y, a.second.y, b.first.y, b.second.y)) {
        return false;
    }
    
    Geometry::Line line1(a.first, a.second);
    Geometry::Line line2(b.first, b.second);
    double n = line1.a * line2.b - line1.b * line2.a;
    if (abs(n) < Geometry::EPS) {
        if (abs(line1(b.first)) > Geometry::EPS || abs(line2(a.first)) > Geometry::EPS) {
            return false;
        }
        return true;
    }
    auto check = [](const Geometry::Point pt1, const Geometry::Point pt2, const Geometry::Point pt_check) {
        return (std::min(pt1.x, pt2.x) <= pt_check.x + Geometry::EPS && pt_check.x <= std::max(pt1.x, pt2.x) + Geometry::EPS)
            && (std::min(pt1.y, pt2.y) <= pt_check.y + Geometry::EPS && pt_check.y <= std::max(pt1.y, pt2.y) + Geometry::EPS);
    };
    Geometry::Point left, right;
    left.x = right.x = -(line1.c * line2.b - line1.b * line2.c) / (line1.a * line2.b - line1.b * line2.a);
    left.y = right.y = -(line1.a * line2.c - line1.c * line2.a) / (line1.a * line2.b - line1.b * line2.a);
    return check(a.first, a.second, left) && check(b.first, b.second, left);
}

bool Geometry::intersection_of_segment_and_rectangle(const Geometry::Segment a, const Geometry::Rectangle b) {
    Geometry::Segment first(b.first.x, b.first.y, b.first.x, b.second.y);
    Geometry::Segment second(b.first.x, b.first.y, b.second.x, b.first.y);
    Geometry::Segment third(b.first.x, b.second.y, b.second.x, b.second.y);
    Geometry::Segment forth(b.second.x, b.first.y, b.second.x, b.second.y);
    return Geometry::intersection_of_two_segments(a, first)
        || Geometry::intersection_of_two_segments(a, second)
        || Geometry::intersection_of_two_segments(a, third)
        || Geometry::intersection_of_two_segments(a, forth);
}