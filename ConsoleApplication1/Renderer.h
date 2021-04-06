#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Color.h"

/* Renderer class, for pixel writing operation,
*  And any method related to displaying a window.
*/

class Renderer
{
private:
	static const int _OFFSET_X = 100;
	static const int _OFFSET_Y = 500;

	//Windows console related handles
	HWND _consoleWindow;
	HDC _consoleDC;

public:
	Renderer();
	~Renderer();

	void writePixel(const int x, const int y , Color& cc );
};

#endif