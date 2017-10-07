
#include <vector>
#include <chrono>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "math.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(500, 500), "2D Perspective", sf::Style::Default, settings);
    window.setFramerateLimit(60);
 
    std::vector<b2d::Vector2> vertices = {
        b2d::Vector2(1, 1),
        b2d::Vector2(-1, 1),
        b2d::Vector2(-1, -1),
        b2d::Vector2(1, -1)
    };

    std::vector<size_t> indices = {
        0, 1,
        1, 2,
        2, 3,
        3, 0
    };

    float gridExtend = 3;
    float gridOneWidth = 0.1;
    std::vector<b2d::Vector2> verticesGrid = {
        b2d::Vector2(0, gridExtend), b2d::Vector2(gridExtend, 0), b2d::Vector2(0, -gridExtend), b2d::Vector2(-gridExtend, 0), // 0-3
        b2d::Vector2(1, gridOneWidth), b2d::Vector2(1, -gridOneWidth), // 4,5
        b2d::Vector2(-1, gridOneWidth), b2d::Vector2(-1, -gridOneWidth), // 6,7
        b2d::Vector2(gridOneWidth, 1), b2d::Vector2(-gridOneWidth, 1), // 8,9
        b2d::Vector2(gridOneWidth, -1), b2d::Vector2(-gridOneWidth, -1) // 10,11
    };

    std::vector<size_t> indicesGrid = {
        0, 2,  8, 9,  10, 11,
        1, 3,  4, 5,  6, 7
    };

    b2d::Matrix3 viewMat = b2d::Matrix3::translate2d(250, 250) * b2d::Matrix3::scale2d(100) * b2d::Matrix3::scale2d(1, -1);

    auto lastFrame = std::chrono::high_resolution_clock::now();
    auto startTime = std::chrono::high_resolution_clock::now();

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        auto thisFrame = std::chrono::high_resolution_clock::now();
        double tslf = std::chrono::duration_cast<std::chrono::microseconds>(thisFrame - lastFrame).count() / 1000000.0;
        double tss = std::chrono::duration_cast<std::chrono::microseconds>(thisFrame - startTime).count() / 1000000.0;

        window.clear();

        b2d::Matrix3 modelTranslation = b2d::Matrix3::translate2d(1, 0);
        b2d::Matrix3 modelScale = b2d::Matrix3::scale2d(0.5);
        b2d::Matrix3 modelRotation = b2d::Matrix3::rotate2d(tss);
        b2d::Matrix3 mat1 = viewMat * modelTranslation * modelRotation * modelScale;

        std::vector<sf::Vertex> sfVerts;

        for (size_t index : indices) {
            b2d::Vector2 pos = vertices.at(index);
            pos = mat1 * pos; // VertexShader
            sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Green));
        }
        for (size_t index : indicesGrid) {
            b2d::Vector2 pos = verticesGrid.at(index);
            pos = viewMat * pos; // VertexShader
            sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::White));
        }

        window.draw(sfVerts.data(), sfVerts.size(), sf::PrimitiveType::Lines);

        window.display();
        lastFrame = thisFrame;
    }
    return 0;
}