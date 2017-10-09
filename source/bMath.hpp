#pragma once

#include <cmath>

namespace b2d 
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
        : x(1), y(1), z(1)
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
        : x(1), y(1)
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

        void setCell(float val, int col, int row)
        {
            if (row == 0){
                columns[col].x = val;
            } else if (row == 1){
                columns[col].y = val;
            } else if (row == 2) {
                columns[col].z = val;
            }
        }

        static Matrix3 scale2d(float s)
        {
            Matrix3 mat;
            mat.setCell(s, 0, 0);
            mat.setCell(s, 1, 1);
            return mat;
        }

        static Matrix3 scale2d(float x, float y)
        {
            Matrix3 mat;
            mat.setCell(x, 0, 0);
            mat.setCell(y, 1, 1);
            return mat;
        }

        static Matrix3 translate2d(float x, float y)
        {
            Matrix3 mat;
            mat.setCell(x, 2, 0);
            mat.setCell(y, 2, 1);
            return mat;
        }

        static Matrix3 rotate2d(float angle)
        {
            Matrix3 mat;
            if (angle == 0) return mat;
            mat.setCell(std::cos(angle), 0, 0);
            mat.setCell(std::sin(angle), 0, 1);
            mat.setCell(std::cos(angle + 3.14159 / 2), 1, 0);
            mat.setCell(std::sin(angle + 3.14159 / 2), 1, 1);
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

        Matrix3 operator*(Matrix3 other)
        {
            Matrix3 mat;
            mat.columns[0] = *this * (other.columns[0]);
            mat.columns[1] = *this * (other.columns[1]);
            mat.columns[2] = *this * (other.columns[2]);
            return mat;
        }

    };
}