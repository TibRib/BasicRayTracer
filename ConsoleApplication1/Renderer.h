#pragma once
class Renderer
{
protected:
	int _width;
	int _height;

	virtual void writePixel(const int x, const int y, int r, int g, int b) const = 0;

public:
	virtual ~Renderer(){}

	virtual void writePixel(const int x, const int y, Color& cc) const = 0;
	virtual void clear() const = 0;
};

