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

        Color operator*(float s)
        {
            float r2 = (float) r * s;
            float g2 = (float) g * s;
            float b2 = (float) b * s;
            return Color((uint8_t) r2, (uint8_t) g2, (uint8_t) b2);
        }
        
        Color operator+(Color other)
        {
            int r2 = r + other.r;
            int g2 = g + other.g;
            int b2 = b + other.b;
            return Color((uint8_t) r2, (uint8_t) g2, (uint8_t) b2);
        }

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