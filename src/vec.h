#ifndef VEC_H
#define VEC_H

struct vec2
{
	vec2(float X, float Y) : x(X), y(Y) { }
	vec2() : x(0.0f), y(0.0f) { }

	vec2 operator+(const vec2 &rhs) const { return vec2(x + rhs.x, y + rhs.y); }
	vec2 operator-(const vec2 &rhs) const { return vec2(x - rhs.x, y - rhs.y); }
	vec2 operator*(float s) const { return vec2(x * s, y * s); }

	vec2 &operator+=(const vec2 &rhs) { *this = *this + rhs; return *this; }

	float dot(const vec2 &rhs) const { return x * rhs.x + y * rhs.y; }
	float length() const { return dot(*this); }
	float x, y;
};

#endif