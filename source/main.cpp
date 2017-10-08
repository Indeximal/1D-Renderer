
#include <vector>
#include <chrono>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "math.hpp"
#include "model.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1000, 500), "2D Perspective", sf::Style::Default, settings);
    window.setFramerateLimit(60);
 

    b2d::Matrix3 topDownViewMat = b2d::Matrix3::translate2d(250, 250) * b2d::Matrix3::scale2d(100) * b2d::Matrix3::scale2d(1, -1);
    b2d::Matrix3 rTDViewMat = b2d::Matrix3::translate2d(750, 400) * b2d::Matrix3::scale2d(100) * b2d::Matrix3::scale2d(1, -1);


    b2d::Model grid = b2d::Model::Grid(2.5, 2.5, 5, 2, 0.1);
    grid.setColor(0, 0, 0);

    b2d::Model square = b2d::Model::Square();
    square.setColor(0, 1, 0);
    square.setPosition(0, 0);
    square.setScale(0.8);
    square.setRotation(0);

    b2d::Vector2 cameraPos(2, 0);
    float cameraRot = 3.14 / 2;

    b2d::Model camera = b2d::Model::Camera();
    camera.setColor(0, 0, 1);
    camera.setPosition(cameraPos);
    camera.setScale(0.1);
    camera.setRotation(cameraRot);

    b2d::Matrix3 viewT = b2d::Matrix3::translate2d(-cameraPos.x, -cameraPos.y);
    b2d::Matrix3 viewR = b2d::Matrix3::rotate2d(-cameraRot);
    b2d::Matrix3 viewMat = viewR * viewT; // Inversed

    float perspNearClip = 0.1;
    float perspFarClip = 3;
    float zoomFactor = 0.8; //std::atan(3.14 - perspFOV)
    // float perspFOV = 90 / 180 * 3.14;

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

            for (b2d::Vector2 vert : square.getWorldVertices()) {
                b2d::Vector2 pos = topDownViewMat * vert;
                b2d::Vector3 col = square.getColor();
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color(col.x, col.y, col.z)));
            }
            for (b2d::Vector2 vert : grid.getWorldVertices()) {
                b2d::Vector2 pos = topDownViewMat * vert;
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Black));
            }
            for (b2d::Vector2 vert : camera.getWorldVertices()) {
                b2d::Vector2 pos = topDownViewMat * vert;
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

            for (b2d::Vector2 vert : square.getWorldVertices()) { // Perspective
                b2d::Vector2 pos = viewMat * vert; // VertexShader
                b2d::Vector2 persp = rTDViewMat * perspMat * b2d::Vector2(pos.x / pos.y, pos.y);
                sfVerts.push_back(sf::Vertex(sf::Vector2f(persp.x, persp.y), sf::Color::Green));
            }
            for (b2d::Vector2 vert : square.getWorldVertices()) {
                b2d::Vector2 pos = rTDViewMat * perspMat * viewMat * vert; // VertexShader
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Cyan));
            }
            for (b2d::Vector2 vert : grid.getWorldVertices()) {
                b2d::Vector2 pos = rTDViewMat * vert; // VertexShader
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Black));
            }
            for (b2d::Vector2 vert : camera.getWorldVertices()) {
                b2d::Vector2 pos = rTDViewMat * viewMat * vert; // VertexShader
                sfVerts.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Blue));
            }
        
            window.draw(sfVerts.data(), sfVerts.size(), sf::PrimitiveType::Lines);
        }

        window.display();
        lastFrame = thisFrame;
    }
    return 0;
}