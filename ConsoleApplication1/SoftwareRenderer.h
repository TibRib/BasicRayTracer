#ifndef _SOFTWARE_RENDERER_H_
#define _SOFTWARE_RENDERER_H_

#include "Color.h"
#include "Renderer.h"

class SoftwareRenderer : public Renderer
{

protected:
	virtual void writePixel(const int x, const int y, int r, int g, int b) const;

public:
	SoftwareRenderer();
	SoftwareRenderer(int width, int height);
	virtual ~SoftwareRenderer();
	//Overrides
	virtual void writePixel(const int x, const int y, Color& cc) const;

};

#endif