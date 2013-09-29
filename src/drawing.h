#ifndef DRAWING_H
#define DRAWING_H
#include "image.h"

void drawRectangle(Image &img, int x, int y, int w, int h, const Color &color);

void drawLine(Image &img, int x0, int y0, int x1, int y1, const Color &color);

#endif