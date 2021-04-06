#include "pch.h"

#include "ConsoleRenderer.h"
#include "utils.h"

ConsoleRenderer::ConsoleRenderer(int width, int height) {
    _width = width;
    _height = height;
    _consoleWindow = GetConsoleWindow();
    _consoleDC = GetDC(_consoleWindow);
    std::cout << "Renderer initialized" << std::endl;
}

ConsoleRenderer::ConsoleRenderer() : ConsoleRenderer(0,0){}

ConsoleRenderer::~ConsoleRenderer() {
    ReleaseDC(_consoleWindow, _consoleDC);
    std::cout << "Renderer destroyed" << std::endl;
}

void ConsoleRenderer::writePixel(const int x, const int y, Color& cc) const {
    // Write the translated [0,255] value of each color component.
    int iR = static_cast<int>(256 * clamp(cc.r(), 0.0, 0.999));
    int iG = static_cast<int>(256 * clamp(cc.g(), 0.0, 0.999));
    int iB = static_cast<int>(256 * clamp(cc.b(), 0.0, 0.999));

    writePixel(x, y, iR, iG, iB);
}

void ConsoleRenderer::writePixel(const int x, const int y, int r, int g, int b) const {
    SetPixel(_consoleDC, _OFFSET_X + x, _OFFSET_Y - y, RGB(r, g, b));
}

void ConsoleRenderer::clear(int r, int g, int b) const {
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            this->writePixel(x, y, r, g, b);
        }
    }
}

void ConsoleRenderer::clear() const {
    this->clear(255, 255, 255);
}