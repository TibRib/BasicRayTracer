#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Color.h"
#include "Renderer.h"

/* Renderer class, for pixel writing operation,
*  And any method related to displaying a window.
*/

class ConsoleRenderer : public Renderer
{
private:
	static const int _OFFSET_X = 100;
	static const int _OFFSET_Y = 500;

	//Windows console related handles
	HWND _consoleWindow;
	HDC _consoleDC;

protected:
	virtual void writePixel(const int x, const int y, int r, int g, int b) const;

public:
	ConsoleRenderer();
	ConsoleRenderer(int width, int height);
	virtual ~ConsoleRenderer();
	//Overrides
	virtual void writePixel(const int x, const int y, Color& cc) const;
	virtual void clear() const;
	virtual void clear(int r, int g, int b) const;
};

#endif