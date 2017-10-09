#pragma once

#include "bColor.hpp"
#include "bMath.hpp"

namespace b2d
{
    struct Vertex
    {
        b2d::Vector2 position;
        b2d::Vector2 normal;
        b2d::Color color;
        
        Vertex(b2d::Vector2 pos, b2d::Color col, b2d::Vector2 normal)
        : position(pos), color(col), normal(normal)
        {}
    };
}