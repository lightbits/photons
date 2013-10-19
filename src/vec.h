#ifndef VEC_H
#define VEC_H
#include <algorithm>

struct vec2
{
	float x;
	float y;

	inline vec2() : x(0.0f), y(0.0f) { }
	inline vec2(float X, float Y) : x(X), y(Y){ }
	inline explicit vec2(float S) : x(S), y(S) { }
	inline vec2 operator + (const vec2 &rhs) const { return vec2(x + rhs.x, y + rhs.y); }
	inline vec2 operator * (const vec2 &rhs) const { return vec2(x * rhs.x, y * rhs.y); }
	inline vec2 operator - (const vec2 &rhs) const { return vec2(x - rhs.x, y - rhs.y); }
	inline vec2 operator * (const float s)	 const { return vec2(x * s, y * s); }
	inline vec2 operator / (const float s)	 const { return vec2(x / s, y / s); }

	inline vec2 &operator += (const vec2 &rhs) { *this = *this + rhs; return *this; }
	inline vec2 &operator *= (const vec2 &rhs) { *this = *this * rhs; return *this; }
	inline vec2 &operator -= (const vec2 &rhs) { *this = *this - rhs; return *this; }
};

struct vec3
{
	float x;
	float y;
	float z;

	inline vec3() : x(0.0f), y(0.0f), z(0.0f) { }
	inline vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) { }
	inline explicit vec3(float S) : x(S), y(S), z(S) { }
	inline vec3 operator + (const vec3 &rhs) const { return vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
	inline vec3 operator * (const vec3 &rhs) const { return vec3(x * rhs.x, y * rhs.y, z * rhs.z); }
	inline vec3 operator - (const vec3 &rhs) const { return vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
	inline vec3 operator * (const float s)	 const { return vec3(x * s, y * s, z * s); }
	inline vec3 operator / (const float s)	 const { return vec3(x / s, y / s, z / s); }

	inline vec3 &operator += (const vec3 &rhs) { *this = *this + rhs; return *this; }
	inline vec3 &operator *= (const vec3 &rhs) { *this = *this * rhs; return *this; }
	inline vec3 &operator -= (const vec3 &rhs) { *this = *this - rhs; return *this; }
};

static vec3 cross(const vec3 &lhs, const vec3 &rhs) 
{
	return vec3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

static float dot(const vec3 &lhs, const vec3 &rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
static float dot(const vec2 &lhs, const vec2 &rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
static float length(const vec3 &v) { return sqrtf(dot(v, v)); }
static float length(const vec2 &v) { return sqrtf(dot(v, v)); }
static vec3 max(const vec3 &lhs, const vec3 &rhs) { return vec3(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z)); }
static vec3 min(const vec3 &lhs, const vec3 &rhs) { return vec3(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z)); }
static vec3 abs(const vec3 &v) { return vec3(std::abs(v.x), std::abs(v.y), std::abs(v.z)); }
static vec3 normalize(const vec3 &v) { float l = length(v); return v / l; }
static vec2 normalize(const vec2 &v) { float l = length(v); return v / l; }

// Rotate t radians around y-axis
static vec3 rotateY(const vec3 &v, float t)
{
	float cost = cosf(t); float sint = sinf(t);
	return vec3(v.x * cost + v.z * sint, v.y, -v.x * sint + v.z * cost);
}

// Rotate t radians around x-axis
static vec3 rotateX(const vec3 &v, float t)
{
	float cost = cosf(t); float sint = sinf(t);
	return vec3(v.x, v.y * cost - v.z * sint, v.y * sint + v.z * cost);
}

// Rotate t radians around z-axis
static vec3 rotateZ(const vec3 &v, float t)
{
	float cost = cosf(t); float sint = sinf(t);
	return vec3(v.x * cost - v.y * sint, v.x * sint + v.y * cost ,v.z);
}

#endif