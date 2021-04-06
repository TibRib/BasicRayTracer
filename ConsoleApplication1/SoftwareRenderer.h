#ifndef _SOFTWARE_RENDERER_H_
#define _SOFTWARE_RENDERER_H_

#include "Color.h"
#include "Renderer.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>


class SoftwareRenderer : public Renderer
{

private:
	SDL_Window* _pWindow;
	SDL_Renderer* _pRenderer;
	SDL_Texture* _pTexture;

	int32_t _renderWidth;
	int32_t _renderHeight;
	
	Pixel** _pPixelGrid;

protected:
	virtual void writePixel(const int x, const int y, int r, int g, int b) const;

public:
	SoftwareRenderer();
	SoftwareRenderer(int width, int height);
	virtual ~SoftwareRenderer();
	//Overrides
	virtual void writePixel(const int x, const int y, Color& cc) const;

	void writeRowOfPixels(const int col, Color** row) const;
	void writeGridOfPixels(Color** grid, uint32_t width, uint32_t height) const;

	bool ProcessInput();
};

#endif