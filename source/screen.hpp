#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace b2d  // Basic 2D
{
    struct Color 
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
        Color(uint8_t r, uint8_t g, uint8_t b)
        : r(r), g(g), b(b), a(255)
        {}
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : r(r), g(g), b(b), a(a)
        {}
    }


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
        }

        void setPixel(int x, int y, Color color)
        {
            sf::Color c(color.r, color.g, color.b, color.a);
            sf::RectangleShape pixel(sf::Vector2f(1,1));
            pixel.setPosition(x, y);
            pixel.setFillColor(c);
            m_window->draw(pixel);
        }

        void clear()
        {
            m_window->clear();
        }

        void show()
        {
            m_window->display();
        }

        bool shouldClose()
        {
            sf::Event event;
            while (m_window->pollEvent(event)) {
                if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyEvent && event.key.code == sf::Keyboard::Escape)) {
                    return true;
                }
            }
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