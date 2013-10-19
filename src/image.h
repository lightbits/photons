#ifndef IMAGE_H
#define IMAGE_H
#include "vec.h"
#include <string>

struct HDRImage
{
	HDRImage() : width(0), height(0), pixels(nullptr) { }
	HDRImage(int width_, int height_) : width(width_), height(height_) { pixels = new vec3[width_ * height_]; }
	~HDRImage() { if(pixels != nullptr) delete[] pixels; }
	void clear(const vec3 &c) { for(int i = 0; i < width * height; ++i) pixels[i] = c; }
	vec3 getPixel(int x, int y) const { return pixels[y * width + x]; }
	void setPixel(int x, int y, const vec3 &pixel) { pixels[y * width + x] = pixel; }
	vec3 *pixels;
	int width;
	int height;
};

bool savePng(const char *filename, const HDRImage &img);

bool loadPng(const char *filename, HDRImage &img);

#endif