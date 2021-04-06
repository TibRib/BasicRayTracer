#include "pch.h"
#include "Color.h"

Color* computeColor(const int x, const int y, Color& pixel_color, const double scale) {
    Color* c;
    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    double r = sqrt(scale * pixel_color.r());
    double g = sqrt(scale * pixel_color.g());
    double b = sqrt(scale * pixel_color.b());
    c = new Color(r, g, b);

    return c;
}