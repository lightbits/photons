#ifndef PHOTON_H
#define PHOTON_H
#include "image.h"
#include "vec.h"

struct Photon
{
	Photon(const vec2 &position_, const vec2 &velocity_, const Color &color_);
	vec2 position;
	vec2 velocity;
	Color color;
	bool active;
};

#endif