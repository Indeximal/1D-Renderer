#include <SFML/Graphics.hpp>
#include "triangle.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(720,480), "Triangle", sf::Style::Default, settings);
    
    Triangle2D triangle(sf::Vector2f(300, 100), sf::Vector2f(100, 350), sf::Vector2f(500, 300));

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        window.draw(triangle);

        window.display();
    }
    return 0;
}