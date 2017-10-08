
#include <vector>
#include <chrono>
#include <cmath>

#include "math.hpp"
#include "model.hpp"
#include "screen.hpp"

int main() {
    b2d::Screen screen("2D Perspective", 1000, 500);
 

    b2d::Matrix3 topDownViewMat = b2d::Matrix3::translate2d(250, 250) * b2d::Matrix3::scale2d(100) * b2d::Matrix3::scale2d(1, -1);
    b2d::Matrix3 rTDViewMat = b2d::Matrix3::translate2d(750, 400) * b2d::Matrix3::scale2d(100) * b2d::Matrix3::scale2d(1, -1);


    b2d::Model grid = b2d::Model::Grid(2.5, 2.5, 5, 2.5, 0.1);
    grid.setColor(0, 0, 0);

    b2d::Model square = b2d::Model::Square();
    square.setColor(0, 255, 0);
    square.setPosition(0, 0);
    square.setScale(0.8);
    square.setRotation(0);

    b2d::Vector2 cameraPos(2, 0);
    float cameraRot = 3.14 / 2;

    b2d::Model camera = b2d::Model::Camera();
    camera.setColor(0, 0, 255);
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

    while (!screen.shouldClose()) {
        auto thisFrame = std::chrono::high_resolution_clock::now();
        double tslf = std::chrono::duration_cast<std::chrono::microseconds>(thisFrame - lastFrame).count() / 1000000.0;
        double tss = std::chrono::duration_cast<std::chrono::microseconds>(thisFrame - startTime).count() / 1000000.0;

        screen.clear();

        // Left: Top Down
        screen.draw(square, topDownViewMat);
        screen.draw(grid, topDownViewMat);
        screen.draw(camera, topDownViewMat); 

        // Right: Top Down Camera
        std::vector<b2d::Vector2> verts;
        for (b2d::Vector2 vert : square.getWorldVertices()) { // Perspective
            b2d::Vector2 pos = viewMat * vert; // VertexShader
            b2d::Vector2 persp = rTDViewMat * perspMat * b2d::Vector2(pos.x / pos.y, pos.y);
            verts.push_back(persp);
        }
        screen.drawLines(verts, square.getColor());

        screen.draw(square, rTDViewMat * perspMat * viewMat, b2d::Color(0, 255, 255));
        screen.draw(grid, rTDViewMat);
        screen.draw(camera, rTDViewMat * viewMat);

        screen.show();

        lastFrame = thisFrame;
    }
    screen.close();

    return 0;
}