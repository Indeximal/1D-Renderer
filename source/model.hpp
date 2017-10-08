#pragma once

#include <vector>

#include "math.hpp"
#include "b2dColor.hpp"

namespace b2d
{
    class Model
    {
    private:
        std::vector<b2d::Vector2> vertices;
        std::vector<int> indices;

        b2d::Matrix3 translation;
        b2d::Matrix3 rotation;
        b2d::Matrix3 scale;

        b2d::Color color;
        
    public:
        Model()
        {
        }

        Model(std::vector<b2d::Vector2> vertices, std::vector<int> indices)
        : vertices(vertices), indices(indices)
        {
        }

        void setColor(b2d::Color col)
        {
            color = col;
        }

        void setColor(float r, float g, float b)
        {
            color = b2d::Color(r, g, b);
        }

        void setScale(float x, float y)
        {
            scale = b2d::Matrix3::scale2d(x, y);
        }

        void setScale(b2d::Vector2 s)
        {
            scale = b2d::Matrix3::scale2d(s.x, s.y);
        }

        void setScale(float s)
        {
            scale = b2d::Matrix3::scale2d(s);
        }

        void setRotation(float a)
        {
            rotation = b2d::Matrix3::rotate2d(a);
        }

        void setPosition(float x, float y)
        {
            translation = b2d::Matrix3::translate2d(x, y);
        }

        void setPosition(b2d::Vector2 pos)
        {
            translation = b2d::Matrix3::translate2d(pos.x, pos.y);
        }

        size_t getVertexCount()
        {
            return indices.size();
        }
        
        b2d::Vector2 getVertex(int index)
        {
            return vertices.at(indices.at(index));
        }

        std::vector<b2d::Vector2> getModelVertices()
        {
            std::vector<b2d::Vector2> v;
            for (int i = 0; i < getVertexCount(); i++) {
                v.push_back(getVertex(i));
            }
            return v;
        }

        std::vector<b2d::Vector2> getWorldVertices()
        {
            b2d::Matrix3 mat = getTransformMatrix();
            std::vector<b2d::Vector2> v;
            for (int i = 0; i < getVertexCount(); i++) {
                v.push_back(mat * getVertex(i));
            }
            return v;
        }

        b2d::Matrix3 getTransformMatrix()
        {
            return translation * rotation * scale;
        }

        b2d::Color getColor()
        {
            return color;
        }

        static Model Square()
        {
            std::vector<b2d::Vector2> squareV = {
                b2d::Vector2(1, 1),
                b2d::Vector2(-1, 1),
                b2d::Vector2(-1, -1),
                b2d::Vector2(1, -1)
            };
            std::vector<int> squareI = {
                0, 1,
                1, 2,
                2, 3,
                3, 0
            };
            return Model(squareV, squareI);
        }

        static Model Camera()
        {
            std::vector<b2d::Vector2> cameraV = {
                b2d::Vector2(0, 0), b2d::Vector2(1, 2), b2d::Vector2(-1, 2),
                b2d::Vector2(1, 0), b2d::Vector2(-1, 0),
                b2d::Vector2(1, -3), b2d::Vector2(-1, -3)
            };
            std::vector<int> cameraI = {
                0, 1,  0, 2,
                3, 4,  4, 6,  6, 5,  5, 3
            };
            return Model(cameraV, cameraI);
        }

        static Model Grid(float extend, float oneFlagWidth)
        {
            return Grid(extend, extend, extend, extend, oneFlagWidth);
        }

        static Model Grid(float extendPX, float extendNX, float extendPY, float extendNY, float gridOneWidth)
        {
            std::vector<b2d::Vector2> gridV = {
                b2d::Vector2(0, extendPY), b2d::Vector2(extendPX, 0), b2d::Vector2(0, -extendNY), b2d::Vector2(-extendNX, 0), // 0-3
                b2d::Vector2(1, gridOneWidth), b2d::Vector2(1, -gridOneWidth), // 4,5
                b2d::Vector2(-1, gridOneWidth), b2d::Vector2(-1, -gridOneWidth), // 6,7
                b2d::Vector2(gridOneWidth, 1), b2d::Vector2(-gridOneWidth, 1), // 8,9
                b2d::Vector2(gridOneWidth, -1), b2d::Vector2(-gridOneWidth, -1) // 10,11
            };
            std::vector<int> gridI = {
                0, 2,  8, 9,  10, 11,
                1, 3,  4, 5,  6, 7
            };
            return Model(gridV, gridI);
        }
    };

}