#ifndef _RENDERER_H_
#define _RENDERER_H_
class Renderer
{
protected:
	int _width;
	int _height;

	virtual void writePixel(const int x, const int y, int r, int g, int b) const = 0;

public:
	virtual ~Renderer(){}

	virtual void writePixel(const int x, const int y, Color& cc) const = 0;
	
	void clear(int r, int g, int b) const {
		for (int y = 0; y < _height; ++y) {
			for (int x = 0; x < _width; ++x) {
				this->writePixel(x, y, r, g, b);
			}
		}
	}
	void clear() const {
		this->clear(255, 255, 255);
	}
};

#endif