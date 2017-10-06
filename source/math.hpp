#pragma once

namespace idx 
{
    class Vector3 
    {
    public:
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z)
        : x(x), y(y), z(z)
        {
        }

        Vector3()
        : x(0), y(0), z(0)
        {
        }

        Vector3 operator*(float s)
        {
            return Vector3(x * s, y * s, z * s);
        }

        Vector3 operator+(Vector3 other)
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        bool operator==(Vector3 other)
        {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    class Vector2
    {
    public:
        float x;
        float y;

        Vector2(float x, float y)
        : x(x), y(y)
        {
        }
        
        Vector2()
        : x(0), y(0)
        {
        }

        Vector2 operator*(float s)
        {
            return Vector2(x * s, y * s);
        }

        Vector2 operator+(Vector2 other)
        {
            return Vector2(x + other.x, y + other.y);
        }

        bool operator==(Vector2 other)
        {
            return x == other.x && y == other.y;
        }

    };

    class Matrix3 
    {
    public:
        Vector3 columns[3];
        
        Matrix3()
        {
            columns[0] = Vector3(1, 0, 0);
            columns[1] = Vector3(0, 1, 0);
            columns[2] = Vector3(0, 0, 1);
        }

        static Matrix3 scale(float s)
        {
            Matrix3 mat;
            mat.columns[0] = Vector3(s, 0, 0);
            mat.columns[1] = Vector3(0, s, 0);
            mat.columns[2] = Vector3(0, 0, 1);
            return mat;
        }

        Vector3 operator*(Vector3 v)
        {
            return (columns[0] * v.x) + (columns[1] * v.y) + (columns[2] * v.z);
        }

        Vector2 operator*(Vector2 v)
        {
            Vector3 u = operator*(Vector3(v.x, v.y, 1));
            return Vector2(u.x / u.z, u.y / u.z);
        }

    };
}