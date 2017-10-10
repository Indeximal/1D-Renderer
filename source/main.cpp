
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "bScreen.hpp"
#include "bRenderer1D.hpp"

int main() {

    int width = 1000;

    b2d::Screen screen("2D Perspective", width, 600);

    b2d::Renderer1D renderer(&screen, 0, 573, width, 4);
    renderer.setLightPosition(b2d::Vector2(0, 0));

    b2d::Matrix3 topDownViewMat = b2d::Matrix3::translate2d(250, 250) * b2d::Matrix3::scale2d(100) * b2d::Matrix3::scale2d(1, -1);
    b2d::Matrix3 mouseMat =  b2d::Matrix3::scale2d(0.01) * b2d::Matrix3::scale2d(1, -1) * b2d::Matrix3::translate2d(-250, -250);
    b2d::Matrix3 rTDViewMat = b2d::Matrix3::translate2d(750, 400) * b2d::Matrix3::scale2d(150) * b2d::Matrix3::scale2d(1, -1);

    b2d::Model gridL = b2d::Model::Grid(2.5, 0.1);
    b2d::Model gridR = b2d::Model::Grid(2, 2, 5, 1, 0.1);

    std::vector<b2d::Model> models;
    {
        b2d::Model square = b2d::Model::Rectangle(1.3, 1.8, b2d::Color::Green());
        square.setPosition(1, 2);
        square.setRotation(2);
        models.push_back(square);
        b2d::Model square2 = b2d::Model::Rectangle(0.7, 0.9, b2d::Color::Red());
        square2.setPosition(-0.5, 0.5);
        square2.setRotation(1);
        models.push_back(square2);
        b2d::Model square3 = b2d::Model::Square(1.4, b2d::Color::Blue());
        square3.setPosition(0.6, -1.2);
        square3.setRotation(3);
        models.push_back(square3);
    }

    b2d::Model camera = b2d::Model::Camera(0.1, b2d::Color::Blue());

    float perspNearClip = 0.1;
    float perspFarClip = 3;
    float zoomFactor = 1; //std::atan(3.14 - perspFOV)
    // float perspFOV = 90 / 180 * 3.14;

    b2d::Matrix3 perspTranslation = b2d::Matrix3::translate2d(0, -perspNearClip);
    b2d::Matrix3 perspScale = b2d::Matrix3::scale2d(zoomFactor, 1 / (perspFarClip - perspNearClip)); //TODO x-axis
    b2d::Matrix3 perspMat = perspScale * perspTranslation;

    auto lastFrame = std::chrono::high_resolution_clock::now();
    auto startTime = std::chrono::high_resolution_clock::now();

    // std::cout << "started" << std::endl;
    int frameCounter = 0;

    while (!screen.shouldClose()) {
        auto thisFrame = std::chrono::high_resolution_clock::now();
        double tslf = std::chrono::duration_cast<std::chrono::microseconds>(thisFrame - lastFrame).count() / 1000000.0;
        double tss = std::chrono::duration_cast<std::chrono::microseconds>(thisFrame - startTime).count() / 1000000.0;

        screen.clear();

        float cameraRot = 3.14 / 2;
        b2d::Vector2 cameraPos = screen.getMousePosition();
        // std::cout << cameraPos.x << " " << cameraPos.y << std::endl;
        cameraPos = mouseMat * cameraPos;
        // std::cout << cameraPos.x << " " << cameraPos.y << std::endl;
        

        camera.setPosition(cameraPos);
        camera.setRotation(cameraRot);

        b2d::Matrix3 viewT = b2d::Matrix3::translate2d(-cameraPos.x, -cameraPos.y);
        b2d::Matrix3 viewR = b2d::Matrix3::rotate2d(-cameraRot);
        b2d::Matrix3 viewMat = viewR * viewT; // Inversed

        // Left: Top Down
        for (b2d::Model model : models)
        {
            screen.draw(model, topDownViewMat);
        }
        screen.draw(gridL, topDownViewMat);
        screen.draw(camera, topDownViewMat); 

        // Right: Top Down Camera 
        std::vector<b2d::Vector2> tdVerts;
        for (b2d::Model model : models)
        {
            for (b2d::Vertex vert : model.getWorldVertices()) { // Perspective
                b2d::Vector2 pos = viewMat * vert.position; // VertexShader
                b2d::Vector2 persp = perspMat * b2d::Vector2(pos.x / std::abs(pos.y), pos.y);
                tdVerts.push_back(rTDViewMat * persp);
            }
        }
        screen.drawLines(tdVerts, b2d::Color::Red());
        screen.draw(gridR, rTDViewMat);
        screen.draw(camera, rTDViewMat * viewMat);

        renderer.clear();
        for (b2d::Model model : models)
        {
            renderer.render(model, perspMat, cameraPos, cameraRot);
        }

        renderer.draw();

        screen.show();

        lastFrame = thisFrame;
    }
    renderer.deactivate();
    screen.close();

    return 0;
}