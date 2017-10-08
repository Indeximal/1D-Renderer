
#include <vector>
#include <chrono>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "math.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1000, 500), "2D Perspective", sf::Style::Default, settings);
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

    std::vector<b2d::Vector2> cameraVerts = {
        b2d::Vector2(0, 0), b2d::Vector2(1, 2), b2d::Vector2(-1, 2),
        b2d::Vector2(1, 0), b2d::Vector2(-1, 0),
        b2d::Vector2(1, -3), b2d::Vector2(-1, -3)
    };

    std::vector<size_t> cameraLines = {
        0, 1,  0, 2,
        3, 4,  4, 6,  6, 5,  5, 3
    };

    float gridExtend = 2.5;
    float gridOneWidth = 0.1;
    std::vector<b2d::Vector2> verticesGrid = {
        b2d::Vector2(0, gridExtend*3), b2d::Vector2(gridExtend, 0), b2d::Vector2(0, -gridExtend), b2d::Vector2(-gridExtend, 0), // 0-3
        b2d::Vector2(1, gridOneWidth), b2d::Vector2(1, -gridOneWidth), // 4,5
        b2d::Vector2(-1, gridOneWidth), b2d::Vector2(-1, -gridOneWidth), // 6,7
        b2d::Vector2(gridOneWidth, 1), b2d::Vector2(-gridOneWidth, 1), // 8,9
        b2d::Vector2(gridOneWidth, -1), b2d::Vector2(-gridOneWidth, -1) // 10,11
    };

    std::vector<size_t> indicesGrid = {
        0, 2,  8, 9,  10, 11,
        1, 3,  4, 5,  6, 7
    };

    b2d::Matrix3 topDownViewMat = b2d::Matrix3::translate2d(250, 250) * b2d::Matrix3::scale2d(100) * b2d::Matrix3::scale2d(1, -1);
    b2d::Matrix3 rTDViewMat = b2d::Matrix3::translate2d(750, 400) * b2d::Matrix3::scale2d(100) * b2d::Matrix3::scale2d(1, -1);

    b2d::Matrix3 modelTranslation = b2d::Matrix3::translate2d(0, 0);
    b2d::Matrix3 modelScale = b2d::Matrix3::scale2d(0.8);
    b2d::Matrix3 modelRotation = b2d::Matrix3::rotate2d(0);
    b2d::Matrix3 modelMat = modelTranslation * modelRotation * modelScale;

    b2d::Vector2 cameraPos(2, 0);
    float cameraRot = 3.14 / 2;

    b2d::Matrix3 cameraTranslation = b2d::Matrix3::translate2d(cameraPos.x, cameraPos.y);
    b2d::Matrix3 cameraScale = b2d::Matrix3::scale2d(0.1);
    b2d::Matrix3 cameraRotation = b2d::Matrix3::rotate2d(cameraRot);
    b2d::Matrix3 cameraMat = cameraTranslation * cameraRotation * cameraScale;

    b2d::Matrix3 viewT = b2d::Matrix3::translate2d(-cameraPos.x, -cameraPos.y);
    b2d::Matrix3 viewR = b2d::Matrix3::rotate2d(-cameraRot);
    b2d::Matrix3 viewMat = viewR * viewT; // Inversed

    float perspNearClip = 0.1;
    float perspFarClip = 3;
    float perspFOV = 90 / 180 * 3.14;
    float zoomFactor = 0.8; //std::atan(3.14 - perspFOV)

    b2d::Matrix3 perspTranslation = b2d::Matrix3::translate2d(0, -perspNearClip);
    b2d::Matrix3 perspScale = b2d::Matrix3::scale2d(zoomFactor, 1 / (perspFarClip - perspNearClip)); //TODO x-axis
    b2d::Matrix3 perspMat = perspScale * perspTranslation;

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

        window.clear(sf::Color::White);

        { // Left: Top Down
            std::vector<sf::Vertex> sfVerts;

            for (size_t index : indices) {
                b2d::Vector2 pos = vertices.at(index);
                pos = topDownViewMat * modelMat * pos; // VertexShader
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Green));
            }
            for (size_t index : indicesGrid) {
                b2d::Vector2 pos = verticesGrid.at(index);
                pos = topDownViewMat * pos; // VertexShader
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Black));
            }
            for (size_t index : cameraLines) {
                b2d::Vector2 pos = cameraVerts.at(index);
                pos = topDownViewMat * cameraMat * pos; // VertexShader
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Blue));
            }

            window.draw(sfVerts.data(), sfVerts.size(), sf::PrimitiveType::Lines);
        }

        sf::RectangleShape rRect(sf::Vector2f(500, 500));
        rRect.setPosition(sf::Vector2f(500, 0));
        rRect.setFillColor(sf::Color::White);
        window.draw(rRect);

        { // Right: Top Down Camera
            std::vector<sf::Vertex> sfVerts;
        
            for (size_t index : indices) { // Perspective
                b2d::Vector2 pos = vertices.at(index);
                pos = viewMat * modelMat * pos; // VertexShader
                b2d::Vector2 persp = rTDViewMat * perspMat * b2d::Vector2(pos.x / pos.y, pos.y);
                sfVerts.push_back(sf::Vertex(sf::Vector2f(persp.x, persp.y), sf::Color::Green));
            }
            for (size_t index : indices) {
                b2d::Vector2 pos = vertices.at(index);
                pos = rTDViewMat * perspMat * viewMat * modelMat * pos; // VertexShader
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Cyan));
            }
            for (size_t index : indicesGrid) {
                b2d::Vector2 pos = verticesGrid.at(index);
                pos = rTDViewMat * pos; // VertexShader
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Black));
            }
            for (size_t index : cameraLines) {
                b2d::Vector2 pos = cameraVerts.at(index);
                pos = rTDViewMat * viewMat * cameraMat * pos; // VertexShader
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Blue));
            }
        
            window.draw(sfVerts.data(), sfVerts.size(), sf::PrimitiveType::Lines);
        }

        window.display();
        lastFrame = thisFrame;
    }
    return 0;
}