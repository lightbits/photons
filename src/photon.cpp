#include "photon.h"

Photon::Photon(const vec2 &position_, const vec2 &velocity_, const Color &color_) :
	position(position_), velocity(velocity_), color(color_), active(true)
{ }