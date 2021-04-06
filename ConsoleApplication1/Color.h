#ifndef _COLOR_H_
#define _COLOR_H_

#include "Vector3.h"

using Color = Vector3;
Color* computeColor(const int x, const int y, Color& pixel_color, const double scale);

typedef struct Pixel {
	int r, g, b;
} Pixel;

#endif