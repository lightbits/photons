#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "color.h"
#include <vector>

typedef unsigned int uint32;

// Three-channel framebuffer
class FrameBuffer
{
public:
	FrameBuffer(uint32 Width, uint32 Height);
	void clear(const Colorf &color);
	void setFragment(uint32 x, uint32 y, const Colorf &color);
	Colorf getFragment(uint32 x, uint32 y) const;

	uint32 getWidth() const { return width; }
	uint32 getHeight() const { return height; }
private:
	std::vector<float> channel;
	Colorf max;
	uint32 width;
	uint32 height;
};

#endif