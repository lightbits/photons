#include "image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "stb_image.h"

bool savePng(const char *filename, const HDRImage &img)
{
	// Convert floating point rgb data to uint8 buffer
	const unsigned int stride = img.width * 3;
	unsigned char *pixels = new unsigned char[img.width * img.height * 3];

	// Transfer pixel data (no clamping is done!)
	const unsigned int numpixels = img.width * img.height;
	for(unsigned int i = 0; i < numpixels; ++i)
	{
		vec3 color = img.pixels[i];
		unsigned int byteOffset = i * 3;
		pixels[byteOffset + 0] = (unsigned char)(color.x * 255.0f);
		pixels[byteOffset + 1] = (unsigned char)(color.y * 255.0f);
		pixels[byteOffset + 2] = (unsigned char)(color.z * 255.0f);
	}

	bool status = stbi_write_png(filename, img.width, img.height, 3, pixels, stride) != 0;
	delete[] pixels;
	return status;
}

bool loadPng(const char *filename, HDRImage &img)
{
	const int forceChannels = 3;
	int width, height, channels;
	unsigned char *pixels = stbi_load(filename, &width, &height, &channels, forceChannels);
	if(pixels == NULL)
	{
		stbi_image_free(pixels);
		return false;
	}

	// Create new image
	img.width = width;
	img.height = height;
	if(img.pixels != nullptr)
		delete[] img.pixels;

	const int numpixels = width * height;
	img.pixels = new vec3[numpixels];

	// Copy 256-bit RGB data to floating point data
	for(int i = 0; i < numpixels; ++i)
	{
		int offset = i * channels;
		img.pixels[i] = vec3(
			float(pixels[offset + 0] / 255.0f),
			float(pixels[offset + 1] / 255.0f),
			float(pixels[offset + 2] / 255.0f)
			);
	}

	stbi_image_free(pixels);
	return true;
}