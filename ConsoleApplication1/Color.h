#pragma once

#ifndef COLOR_H
#define COLOR_H

#include "Vector3.h"
#include <iostream>
using Color = Vector3;

#define OFFSET_X 100
#define OFFSET_Y 500

void write_color(int x, int y,HDC* output, Color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.r();
    auto g = pixel_color.g();
    auto b = pixel_color.b();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    int iR = static_cast<int>(256 * clamp(r, 0.0, 0.999));
    int iG = static_cast<int>(256 * clamp(g, 0.0, 0.999));
    int iB = static_cast<int>(256 * clamp(b, 0.0, 0.999));
    
    SetPixel(*output, OFFSET_X + x, OFFSET_Y - y, RGB(iR, iG, iB));
}

#endif