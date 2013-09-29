#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include "color.h"

typedef unsigned int uint32;
typedef unsigned char uint8;

class Image
{
public:
	Image(uint32 width, uint32 height);
	Image(const Image &copy);
	Image operator=(const Image &rhs);
	~Image();

	int save(const std::string &filename);
	bool load(const std::string &filename);

	void setColor(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b);
	void setColor(uint32 x, uint32 y, const Color &color);
	void setColor(uint32 x, uint32 y, const Colorf &color);
	void clear(uint8 r, uint8 g, uint8 b);

	uint32 getWidth() const { return width; }
	uint32 getHeight() const { return height; }
	Color getColor(uint32 x, uint32 y) const;
	Colorf getColorf(uint32 x, uint32 y) const;

	uint8 *getData() { return data; }
private:
	uint8 *data;
	uint32 width, height;
};

#endif