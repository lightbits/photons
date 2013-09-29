#ifndef COLOR_H
#define COLOR_H
#include <iostream> // for min

struct Color
{
	Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) :
		r(R), g(G), b(B), a(A) { }
	Color() : r(0), g(0), b(0), a(0) { }

	Color operator*(const Color &rhs) const
	{
		return Color(
			unsigned char(r * rhs.r / 255.0f),
			unsigned char(g * rhs.g / 255.0f), 
			unsigned char(b * rhs.b / 255.0f), 
			unsigned char(a * rhs.a / 255.0f)); 
	}

	Color operator*(float s) const
	{
		return Color(
			unsigned char(r * s),
			unsigned char(g * s), 
			unsigned char(b * s), 
			unsigned char(a * s)); 
	}

	Color operator+(const Color &rhs) const
	{
		return Color(
			std::min(r + rhs.r, 255),
			std::min(g + rhs.g, 255),
			std::min(b + rhs.b, 255),
			std::min(a + rhs.a, 255));
	}

	unsigned char r, g, b, a;
};

struct Colorf
{
	Colorf(float R, float G, float B, float A = 0.0f) :
		r(R), g(G), b(B), a(A) { }
	Colorf() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) { }

	Colorf operator*(const Colorf &rhs) const
	{
		return Colorf(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
	}

	Colorf operator*(float s) const
	{
		return Colorf(r * s, g * s, b * s, a * s); 
	}

	Colorf operator+(const Colorf &rhs) const
	{
		return Colorf(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
	}

	float r, g, b, a;
};

#endif