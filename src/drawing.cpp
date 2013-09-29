#include "drawing.h"

void drawRectangle(Image &img, int x, int y, int w, int h, const Color &color)
{
	for(int i = y; i <= y + h; ++i)
		for(int j = x; j <= x + w; ++j)
			img.setColor(i, j, color);
}

void drawLine(Image &img, int x0, int y0, int x1, int y1, const Color &color)
{
	int dx = x1 - x0;
	int dy = y1 - y0;

	int minx = std::min(x0, x1);
	int maxx = std::max(x0, x1);
	int miny = std::min(y0, y1);
	int maxy = std::max(y0, y1);
	
	if(std::abs(dy) < std::abs(dx))
	{
		float a = dy / float(dx);
		int bx = a < 0.0f ? maxx : minx;
		for(uint32 x = minx; x <= maxx; ++x)
			img.setColor(x, uint32(miny + a * (x - bx)), color);
	}
	else
	{
		float ainv = dx / float(dy);
		int by = ainv < 0.0f ? maxy : miny;
		for(uint32 y = miny; y <= maxy; ++y)
			img.setColor(uint32(minx + ainv * (y - by)), y, color);
	}
}