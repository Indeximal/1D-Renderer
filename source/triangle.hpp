#pragma once

#include <SFML/Graphics.hpp>
#include "math.hpp"

class Triangle2D : public sf::Shape 
{
public:
    Triangle2D (idx::Vector2 p1, idx::Vector2 p2, idx::Vector2 p3)
    {
        vertices[0] = p1;
        vertices[1] = p2;
        vertices[2] = p3;
        update();
    }

    virtual std::size_t getPointCount() const
    {
        return 3;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const
    {
        return sf::Vector2f(vertices[index].x, vertices[index].y);
    }
private:
    idx::Vector2 vertices[3];

} ;