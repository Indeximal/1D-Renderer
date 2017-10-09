#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "bModel.hpp"
#include "bMath.hpp"
#include "bColor.hpp"

namespace b2d  // Basic 2D
{
    class Screen
    {
    private:
        sf::ContextSettings m_settings;
        sf::VideoMode m_screenSize;
        sf::RenderWindow* m_window;
    public:
        Screen(std::string name, int width, int height)
        : m_screenSize(sf::VideoMode(width, height))
        {
            m_settings.antialiasingLevel = 4;
            m_window = new sf::RenderWindow(m_screenSize, name, sf::Style::Default, m_settings);
            m_window->setFramerateLimit(60);
        }

        void setPixel(int x, int y, Color color)
        {
            sf::Color c(color.r, color.g, color.b, color.a);
            sf::RectangleShape pixel(sf::Vector2f(1,1));
            pixel.setPosition(x, y);
            pixel.setFillColor(c);
            m_window->draw(pixel);
        }

        void drawLines(std::vector<b2d::Vector2> verts, b2d::Color col)
        {
            std::vector<sf::Vertex> sfVerts;
            for (b2d::Vector2 vert : verts) {
                sfVerts.push_back(sf::Vertex(sf::Vector2f(vert.x, vert.y), sf::Color(col.r, col.g, col.b, col.a)));
            }
            m_window->draw(sfVerts.data(), sfVerts.size(), sf::PrimitiveType::Lines);
        }

        void draw(b2d::Model model, b2d::Matrix3 view, b2d::Color col)
        {
            std::vector<sf::Vertex> sfVerts;
            for (b2d::Vector2 vert : model.getWorldVertices()) {
                b2d::Vector2 pos = view * vert;
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color(col.r, col.g, col.b, col.a)));
            }
            m_window->draw(sfVerts.data(), sfVerts.size(), sf::PrimitiveType::Lines);
        }

        void draw(b2d::Model model, b2d::Matrix3 view)
        {
            draw(model, view, model.getColor());
        }

        void clear()
        {
            m_window->clear(sf::Color::White);
        }

        void clear(b2d::Color col)
        {
            m_window->clear(sf::Color(col.r, col.g, col.b, col.a));
        }

        void show()
        {
            m_window->display();
        }

        bool shouldClose()
        {
            sf::Event event;
            while (m_window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    return true;
                }
            }
            return false;
        }

        void close()
        {
            m_window->close();
        }

        ~Screen()
        {
            if (m_window->isOpen())
            {
                this->close();
            }
            delete m_window;
        }

    };
}