#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "bModel.hpp"

namespace b2d  // Basic 2D
{
    class Screen
    {
    private:
        sf::ContextSettings m_settings;
        sf::VideoMode m_screenSize;
        sf::RenderWindow* m_window;

        bool m_shouldClose = false;

        void updateEvents()
        {
            sf::Event event;
            while (m_window->pollEvent(event))
            {
                switch (event.type){
                    case sf::Event::Closed :
                        m_shouldClose = true;
                        break;
                    default:
                        break;

                }
            }

        }

    public:
        Screen(std::string name, int width, int height)
        : m_screenSize(sf::VideoMode(width, height))
        {
            m_settings.antialiasingLevel = 1;
            m_window = new sf::RenderWindow(m_screenSize, name, sf::Style::Default, m_settings);
            m_window->setFramerateLimit(60);
        }

        void setPixels(int x, int y, int w, int h, b2d::Color pixels[], int pixelCount)
        {
            static_assert((sizeof(b2d::Color) == sizeof(uint8_t) * 4) && (sizeof(sf::Uint8) == sizeof(uint8_t)), "Cant convert b2d::Color into a image");
            if (w * h != pixelCount)
            {
                throw std::logic_error("Pixel count doesn't match w * h");
            }
            sf::Uint8* data = reinterpret_cast<sf::Uint8*>(pixels);

            sf::Image image; // sfml cpu
            image.create(w, h, data);
            sf::Texture tex; // gpu
            tex.loadFromImage(image);
            sf::Sprite sprite(tex); // position data
            sprite.setPosition(x, y);
            m_window->draw(sprite); // draw
        }

        //TODO remove
        /*
        void setPixelsDeprecated(int x, int y, int w, int h, b2d::Color color)
        {
            throw std::logic_error("deprecated setPixels using RectangleShape");
            sf::Color c(color.r, color.g, color.b, color.a);
            sf::RectangleShape pixel(sf::Vector2f(w, h));
            pixel.setPosition(x, y);
            pixel.setFillColor(c);
            m_window->draw(pixel);
        }

        void setPixelDeprecated(int x, int y, b2d::Color color)
        {
            setPixels(x, y, 1, 1, color);
        }*/

        void drawLines(std::vector<b2d::Vector2> verts, b2d::Color col)
        {
            std::vector<sf::Vertex> sfVerts;
            for (b2d::Vector2 vert : verts) {
                sfVerts.push_back(sf::Vertex(sf::Vector2f(vert.x, vert.y), sf::Color(col.r, col.g, col.b, col.a)));
            }
            m_window->draw(sfVerts.data(), sfVerts.size(), sf::PrimitiveType::Lines);
        }

        void draw(b2d::Model model, b2d::Matrix3 view)
        {
            std::vector<sf::Vertex> sfVerts;
            for (b2d::Vertex vert : model.getWorldVertices()) {
                b2d::Vector2 pos = view * vert.position;
                b2d::Color col = vert.color;
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color(col.r, col.g, col.b, col.a)));
            }
            m_window->draw(sfVerts.data(), sfVerts.size(), sf::PrimitiveType::Lines);
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

        b2d::Vector2 getMousePosition()
        {
            sf::Vector2i pos = sf::Mouse::getPosition(*m_window);
            return b2d::Vector2(pos.x, pos.y);
        }

        bool shouldClose()
        {
            updateEvents();
            return m_shouldClose;
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