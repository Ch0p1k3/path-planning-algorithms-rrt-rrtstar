#ifndef ROUNDENDEDLINE_H
#define ROUNDENDEDLINE_H

// Class written by Foaly
// https://github.com/SFML/SFML/wiki/Source%3A-Round-Ended-Lines

#include "lib/SFML/include/SFML/Graphics/Shape.hpp"
#include <cmath>

const double M_PI = std::acos(-1);

class CRoundendedLine : public sf::Shape
{
public:

    CRoundendedLine(const sf::Vector2f& endPoint = sf::Vector2f(0, 0), const float width = 1.0);

    void setEndPoint(const sf::Vector2f& endPoint);

    void setWidth(const float width);

    virtual size_t getPointCount() const;

    virtual sf::Vector2f getPoint(size_t index) const;

private :

    sf::Vector2f m_endPoint;
    float m_Width;
};
#endif //ROUNDENDEDLINE_H