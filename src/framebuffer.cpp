#include "framebuffer.h"

FrameBuffer::FrameBuffer(uint32 Width, uint32 Height)
	: width(Width), height(Height), channel(Width * Height * 3)
{ }

void FrameBuffer::clear(const Colorf &color)
{
	for(uint32 y = 0; y < height; ++y)
		for(uint32 x = 0; x < width; ++x)
			setFragment(x, y, color);
}

void FrameBuffer::setFragment(uint32 x, uint32 y, const Colorf &color)
{
	uint32 offset = 3 * (x + width * y);
	channel[offset + 0] = color.r;
	channel[offset + 1] = color.g;
	channel[offset + 2] = color.b;

	if(color.r > max.r) max.r = color.r;
	if(color.g > max.g) max.g = color.g;
	if(color.b > max.b) max.b = color.b;
}

Colorf FrameBuffer::getFragment(uint32 x, uint32 y) const
{
	uint32 offset = 3 * (x + width * y);
	return Colorf(
		channel[offset + 0],
		channel[offset + 1],
		channel[offset + 2]);
}