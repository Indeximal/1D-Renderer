#pragma once

namespace b2d  // Basic 2D
{
    struct Color 
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        Color()
        : r(0), g(0), b(0), a(255)
        {}

        Color(uint8_t r, uint8_t g, uint8_t b)
        : r(r), g(g), b(b), a(255)
        {}

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : r(r), g(g), b(b), a(a)
        {}

        static Color Red()
        {
            static Color col(255, 0, 0);
            return col;
        }
        static Color Green()
        {
            static Color col(0, 255, 0);
            return col;
        }
        static Color Blue()
        {
            static Color col(0, 0, 255);
            return col;
        }
        static Color White()
        {
            static Color col(255, 255, 255);
            return col;
        }
        static Color Black()
        {
            static Color col(0, 0, 0);
            return col;
        }
    };
    
}