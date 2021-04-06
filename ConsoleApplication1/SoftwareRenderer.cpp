#include "pch.h"
#include "SoftwareRenderer.h"

#include "utils.h"

SoftwareRenderer::SoftwareRenderer(int width, int height) {
    _width = width;
    _height = height;
    //TODO : implement
    std::cout << "Renderer initialized" << std::endl;
}

SoftwareRenderer::SoftwareRenderer() : SoftwareRenderer(0, 0) {}

SoftwareRenderer::~SoftwareRenderer() {
  
    std::cout << "Renderer destroyed" << std::endl;
}

void SoftwareRenderer::writePixel(const int x, const int y, Color& cc) const {
    // Write the translated [0,255] value of each color component.
    int iR = static_cast<int>(256 * clamp(cc.r(), 0.0, 0.999));
    int iG = static_cast<int>(256 * clamp(cc.g(), 0.0, 0.999));
    int iB = static_cast<int>(256 * clamp(cc.b(), 0.0, 0.999));

    writePixel(x, y, iR, iG, iB);
}

void SoftwareRenderer::writePixel(const int x, const int y, int r, int g, int b) const {
    //TODO : write
}