#pragma once

#include "bScreen.hpp"

namespace b2d
{
    class Renderer1D
    {
        bool m_isActive = false;
        b2d::Screen* m_screen;
        int m_x;
        int m_y;
        int m_w;
        int m_h;

        b2d::Color* m_frameBuffer;
        float* m_depthBuffer;

        b2d::Color m_clearColor = b2d::Color::White();

        
    public:
        Renderer1D(b2d::Screen* screen, int x, int y, int width, int height)
        : m_screen(screen), m_x(x), m_y(y), m_w(width), m_h(height)
        {
            m_frameBuffer = new b2d::Color[m_w];
            m_depthBuffer = new float[m_w];
            m_isActive = true;
        }

        void setClearColor(b2d::Color cc)
        {
            m_clearColor = cc;
        }

        void clear()
        {
            for (int i = 0; i < m_w; i++)
            {
                m_frameBuffer[i] = m_clearColor;
                m_depthBuffer[i] = 1;
            }
        }

        void render(b2d::Model model, b2d::Matrix3 projMat, b2d::Matrix3 viewMat){

            std::vector<b2d::Vertex> wVerts = model.getWorldVertices();

            for (int i = 0; i < wVerts.size(); i += 2)
            {
                b2d::Vertex v1 = wVerts[i];
                b2d::Vertex v2 = wVerts[i+1];

                b2d::Vector2 pos1 = viewMat * v1.position;
                b2d::Vector2 pos2 = viewMat * v2.position;

                b2d::Vector2 proj1 = projMat * b2d::Vector2(pos1.x / std::abs(pos1.y), pos1.y);
                b2d::Vector2 proj2 = projMat * b2d::Vector2(pos2.x / std::abs(pos2.y), pos2.y);


                if (proj1.x > proj2.x){
                    std::swap(v1, v2);
                    std::swap(pos1, pos2);
                    std::swap(proj1, proj2);
                }
                int leftEdge = (proj1.x + 1) / 2 * m_w;
                int rightEdge = (proj2.x + 1) / 2 * m_w;
                int diff = rightEdge - leftEdge;
                
                for (int pixel = std::max(0, leftEdge); pixel < std::min(m_w, rightEdge); pixel++)
                {
                    float t = (pixel - leftEdge) / (float)diff;
                    if (t < 0 || t > 1) {
                        continue;
                    }
                    float depth = proj1.y * (1-t) + proj2.y * t;
                    if (depth < 0 || depth > 1) {
                        continue;
                    }
                    if (depth < m_depthBuffer[pixel])
                    {
                        m_depthBuffer[pixel] = depth;
                        m_frameBuffer[pixel] = v1.color * (1-t) + v2.color * t;
                    }
                }
            }
        }

        void draw()
        {
            if (!m_isActive) {
                throw std::logic_error("The renderer has been deactivated");
            }

            m_screen->setPixels(m_x, m_y, m_w, 1, m_frameBuffer, m_w);
        }

        void deactivate()
        {
            m_isActive = false;
        }

        ~Renderer1D()
        {
            delete[] m_frameBuffer;
            delete[] m_depthBuffer;
        }
    };
}