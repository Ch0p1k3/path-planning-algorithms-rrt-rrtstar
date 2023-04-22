#include "geometry.hpp"

double Geometry::euclideanMetric(const Geometry::Point& a,
                                 const Geometry::Point& b) {
  return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

double Geometry::areaOfTreangle(const Geometry::Point& a,
                                const Geometry::Point& b,
                                const Geometry::Point& c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

int Geometry::signAreaOfTreangle(const Geometry::Point& a,
                                 const Geometry::Point& b,
                                 const Geometry::Point& c) {
  return Geometry::sgn<double>((b.x - a.x) * (c.y - a.y) -
                               (b.y - a.y) * (c.x - a.x));
}

bool Geometry::intersectionOfTwoSegments(const Geometry::Segment& l,
                                         const Geometry::Segment& k) {
  // auto help = [](double a, double b, double c, double d) {
  //     if (a > b) std::swap(a, b);
  //     if (c > d) std::swap(c, d);
  //     return std::max(a,c) <= std::min(b,d) + Geometry::EPS;
  // };

  // if (!help(a.first.x, a.second.x, b.first.x, b.second.x) || !help(a.first.y,
  // a.second.y, b.first.y, b.second.y)) {
  //     return false;
  // }

  // Geometry::Line line1(a.first, a.second);
  // Geometry::Line line2(b.first, b.second);
  // double n = line1.a * line2.b - line1.b * line2.a;
  // if (abs(n) < Geometry::EPS) {
  //     if (abs(line1(b.first)) > Geometry::EPS || abs(line2(a.first)) >
  //     Geometry::EPS) {
  //         return false;
  //     }
  //     return true;
  // }
  // auto check = [](const Geometry::Point pt1, const Geometry::Point pt2, const
  // Geometry::Point pt_check) {
  //     return (std::min(pt1.x, pt2.x) <= pt_check.x + Geometry::EPS &&
  //     pt_check.x <= std::max(pt1.x, pt2.x) + Geometry::EPS)
  //         && (std::min(pt1.y, pt2.y) <= pt_check.y + Geometry::EPS &&
  //         pt_check.y <= std::max(pt1.y, pt2.y) + Geometry::EPS);
  // };
  // Geometry::Point left, right;
  // left.x = right.x = -(line1.c * line2.b - line1.b * line2.c) / (line1.a *
  // line2.b - line1.b * line2.a); left.y = right.y = -(line1.a * line2.c -
  // line1.c * line2.a) / (line1.a * line2.b - line1.b * line2.a); return
  // check(a.first, a.second, left) && check(b.first, b.second, left);
  Geometry::Point a = l.first, b = l.second, c = k.first, d = k.second;
  double denominator =
      ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
  double numerator1 = ((a.y - c.y) * (d.x - c.x)) - ((a.x - c.x) * (d.y - c.y));
  double numerator2 = ((a.y - c.y) * (b.x - a.x)) - ((a.x - c.x) * (b.y - a.y));
  if (denominator == 0)
    return numerator1 == 0 && numerator2 == 0;
  double r = numerator1 / denominator;
  double s = numerator2 / denominator;
  return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

bool Geometry::intersectionOfSegmentAndRectangle(const Geometry::Segment& a,
                                                 const Geometry::Rectangle& b) {
  Geometry::Segment first(b.first.x, b.first.y, b.first.x, b.second.y);
  Geometry::Segment second(b.first.x, b.first.y, b.second.x, b.first.y);
  Geometry::Segment third(b.first.x, b.second.y, b.second.x, b.second.y);
  Geometry::Segment forth(b.second.x, b.first.y, b.second.x, b.second.y);
  return Geometry::intersectionOfTwoSegments(a, first) ||
         Geometry::intersectionOfTwoSegments(a, second) ||
         Geometry::intersectionOfTwoSegments(a, third) ||
         Geometry::intersectionOfTwoSegments(a, forth);
}

std::ostream& operator<<(std::ostream& out, const Geometry::Point& a) {
  out << "(" << a.x << ", " << a.y << ")";
  return out;
}