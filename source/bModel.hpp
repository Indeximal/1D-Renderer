#pragma once

#include <vector>

#include "bVertex.hpp"

namespace b2d
{
    class Model
    {
    private:
        std::vector<b2d::Vertex> vertices;
        std::vector<int> indices;

        b2d::Matrix3 translation;
        b2d::Matrix3 rotation;
        b2d::Matrix3 scale;
        
    public:
        Model()
        {
        }

        Model(std::vector<b2d::Vector2> corners, std::vector<int> indices, b2d::Color col)
        : indices(indices)
        {
            vertices.reserve(corners.size());
            for (b2d::Vector2 corner : corners)
            {
                vertices.push_back(b2d::Vertex(corner, col, b2d::Vector2::Zero()));
            }
        }

        Model(std::vector<b2d::Vertex> vertices, std::vector<int> indices)
        : vertices(vertices), indices(indices)
        {}

        void setColor(b2d::Color col)
        {
            for (int i = 0; i < vertices.size(); i++)
            {
                vertices[i].color = col;
            }
        }

        void setColor(float r, float g, float b)
        {
            setColor(b2d::Color(r, g, b));
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
        
        b2d::Vertex getVertex(int index)
        {
            return vertices.at(indices.at(index));
        }

        std::vector<b2d::Vertex> getModelVertices()
        {
            std::vector<b2d::Vertex> v;
            for (int i = 0; i < getVertexCount(); i++) {
                v.push_back(getVertex(i));
            }
            return v;
        }

        std::vector<b2d::Vertex> getWorldVertices()
        {
            b2d::Matrix3 mat = getTransformMatrix();
            std::vector<b2d::Vertex> vs;
            for (int i = 0; i < getVertexCount(); i++) {
                b2d::Vertex v = getVertex(i);
                vs.push_back(b2d::Vertex(mat * v.position, v.color, (rotation * scale * v.normal).toNormal()));
            }
            return vs;
        }

        b2d::Matrix3 getTransformMatrix()
        {
            return translation * rotation * scale;
        }

        static Model Rectangle(float width, float height, b2d::Color color)
        {
            std::vector<b2d::Vertex> squareV = {
                b2d::Vertex(b2d::Vector2( width/2,  height/2), color, b2d::Vector2(1, 1).toNormal()),
                b2d::Vertex(b2d::Vector2(-width/2,  height/2), color, b2d::Vector2(-1, 1).toNormal()),
                b2d::Vertex(b2d::Vector2(-width/2, -height/2), color, b2d::Vector2(-1, -1).toNormal()),
                b2d::Vertex(b2d::Vector2( width/2, -height/2), color, b2d::Vector2(1, -1).toNormal())
            };
            std::vector<int> squareI = {
                0, 1,
                1, 2,
                2, 3,
                3, 0
            };
            return Model(squareV, squareI);
        }

        static Model ColoredRect(float width, float height, b2d::Color c1, b2d::Color c2, b2d::Color c3, b2d::Color c4)
        {
            std::vector<b2d::Vertex> squareV = {
                b2d::Vertex(b2d::Vector2( width/2,  height/2), c1, b2d::Vector2(0, 1).toNormal()),
                b2d::Vertex(b2d::Vector2(-width/2,  height/2), c1, b2d::Vector2(0, 1).toNormal()),
                b2d::Vertex(b2d::Vector2(-width/2,  height/2), c2, b2d::Vector2(-1, 0).toNormal()),
                b2d::Vertex(b2d::Vector2(-width/2, -height/2), c2, b2d::Vector2(-1, 0).toNormal()),
                b2d::Vertex(b2d::Vector2(-width/2, -height/2), c3, b2d::Vector2(0, -1).toNormal()),
                b2d::Vertex(b2d::Vector2( width/2, -height/2), c3, b2d::Vector2(0, -1).toNormal()),
                b2d::Vertex(b2d::Vector2( width/2, -height/2), c4, b2d::Vector2(1, 0).toNormal()),
                b2d::Vertex(b2d::Vector2( width/2,  height/2), c4, b2d::Vector2(1, 0).toNormal()),
            };
            std::vector<int> squareI = {
                0, 1,
                2, 3,
                4, 5,
                6, 7
            };
            return Model(squareV, squareI);
        }

        static Model FlatShadedRect(float width, float height, b2d::Color color){
            return ColoredRect(width, height, color, color, color, color);
        }

        static Model Cross(float size, b2d::Color color){
            std::vector<b2d::Vector2> crossV = {
                b2d::Vector2(size, size),
                b2d::Vector2(-size, -size),
                b2d::Vector2(-size, size),
                b2d::Vector2(size, -size)
            };
            std::vector<int> crossI = {
                0, 1,
                2, 3
            };
            return Model(crossV, crossI, color);
        }

        static Model Square(float size, b2d::Color color)
        {
            return Rectangle(size, size, color);
        }

        static Model Camera(float size, b2d::Color color)
        {
            std::vector<b2d::Vector2> cameraV = {
                b2d::Vector2(0, 0), b2d::Vector2(10*size, 10*size), b2d::Vector2(-10*size, 10*size),
                b2d::Vector2(size, 0), b2d::Vector2(-size, 0),
                b2d::Vector2(size, -3*size), b2d::Vector2(-size, -3*size)
            };
            std::vector<int> cameraI = {
                0, 1,  0, 2,
                3, 4,  4, 6,  6, 5,  5, 3
            };
            return Model(cameraV, cameraI, color);
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
            return Model(gridV, gridI, b2d::Color::Black());
        }
    };

}