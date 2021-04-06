#include "pch.h"

#include "Renderer.h"
#include "utils.h"

void Renderer::writePixel(const int x, const int y,  Color& cc) {
    // Write the translated [0,255] value of each color component.
    int iR = static_cast<int>(256 * clamp(cc.r(), 0.0, 0.999));
    int iG = static_cast<int>(256 * clamp(cc.g(), 0.0, 0.999));
    int iB = static_cast<int>(256 * clamp(cc.b(), 0.0, 0.999));

	SetPixel(_consoleDC, _OFFSET_X + x, _OFFSET_Y - y, RGB(iR, iG, iB));
}

Renderer::Renderer() {
    _consoleWindow = GetConsoleWindow();
    _consoleDC = GetDC(_consoleWindow);
    std::cout << "Renderer initialized" << std::endl;
}

Renderer::~Renderer() {
    ReleaseDC(_consoleWindow, _consoleDC);
    std::cout << "Renderer destroyed" << std::endl;
}