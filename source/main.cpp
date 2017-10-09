
#include <vector>
#include <chrono>
#include <cmath>

#include "bMath.hpp"
#include "bModel.hpp"
#include "bScreen.hpp"
#include "bVertex.hpp"

int main() {
    int width = 1000;
    int posYframe = 575;

    b2d::Screen screen("2D Perspective", width, 600);

    b2d::Matrix3 topDownViewMat = b2d::Matrix3::translate2d(250, 250) * b2d::Matrix3::scale2d(100) * b2d::Matrix3::scale2d(1, -1);
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

    b2d::Vector2 cameraPos(2, 0);
    float cameraRot = 3.14 / 2;

    b2d::Model camera = b2d::Model::Camera(0.1, b2d::Color::Blue());
    camera.setPosition(cameraPos);
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

    b2d::Color frameBuffer[width];
    float depthBuffer[width];

    auto lastFrame = std::chrono::high_resolution_clock::now();
    auto startTime = std::chrono::high_resolution_clock::now();

    while (!screen.shouldClose()) {
        auto thisFrame = std::chrono::high_resolution_clock::now();
        double tslf = std::chrono::duration_cast<std::chrono::microseconds>(thisFrame - lastFrame).count() / 1000000.0;
        double tss = std::chrono::duration_cast<std::chrono::microseconds>(thisFrame - startTime).count() / 1000000.0;

        screen.clear();

        // Left: Top Down
        for (b2d::Model model : models)
        {
            screen.draw(model, topDownViewMat);
        }
        screen.draw(gridL, topDownViewMat);
        screen.draw(camera, topDownViewMat); 

        // Right: Top Down Camera 
        std::vector<b2d::Vector2> tdVerts;
        std::vector<b2d::Vertex> verts;
        for (b2d::Model model : models)
        {
            for (b2d::Vertex vert : model.getWorldVertices()) { // Perspective
                b2d::Vector2 pos = viewMat * vert.position; // VertexShader
                b2d::Vector2 persp = perspMat * b2d::Vector2(pos.x / std::abs(pos.y), pos.y);
                verts.push_back(b2d::Vertex(persp, vert.color, vert.normal));
                tdVerts.push_back(rTDViewMat * persp);
            }
        }
        screen.drawLines(tdVerts, b2d::Color::Red());
        screen.draw(gridR, rTDViewMat);
        screen.draw(camera, rTDViewMat * viewMat);

        for (int i = 0; i < width; i++) // clear1dFrame
        {
            frameBuffer[i] = b2d::Color::White();
            depthBuffer[i] = 1;
        }

        for (int i = 0; i < verts.size(); i += 2)
        {
            b2d::Vertex v1 = verts[i];
            b2d::Vertex v2 = verts[i+1];
            if (v1.position.x > v2.position.x){
                b2d::Vertex tmp = v2;
                v2 = v1;
                v1 = tmp;
            }
            int leftEdge = (v1.position.x + 1) / 2 * width;
            int rightEdge = (v2.position.x + 1) / 2 * width;
            int diff = rightEdge - leftEdge;
            
            for (int pixel = leftEdge > 0 ? leftEdge : 0; pixel < (rightEdge < width ? rightEdge : width); pixel++)
            {
                float t = (pixel - leftEdge) / (float)diff;
                if (t < 0 || t > 1) {
                    continue;
                }
                float depth = v1.position.y * (1-t) + v2.position.y * t;
                if (depth < 0 || depth > 1) {
                    continue;
                }
                if (depth < depthBuffer[pixel])
                {
                    depthBuffer[pixel] = depth;
                    frameBuffer[pixel] = v1.color * (1-t) + v2.color * t;
                }
            }
        }

        for (int i = 0; i < width; i++) // present
        {
            screen.setPixel(i, posYframe, frameBuffer[i]);
        }

        screen.show();

        lastFrame = thisFrame;
    }
    screen.close();

    return 0;
}