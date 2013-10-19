/* Photon simulator
*/

#include <iostream>
#include <string>
#include "vec.h"
#include "image.h"
#include "noise.h"

typedef unsigned int uint32;

float toLuminance(float R, float G, float B) 
{
	return 0.2126f * R + 0.7152f * G + 0.0722 * B; 
}

// Computes the (log) average of the luminance of the image by eq. 1 in [Reinhard]
float logAverageLuminance(const HDRImage &img)
{
	const float delta = 0.001f; // Small value to avoid singularity if black pixels are present
	float sum = 0.0f;

	// Sum up the log of the luminance value of each pixel
	for(uint32 y = 0; y < img.height; ++y)
	{
		for(uint32 x = 0; x < img.width; ++x)
		{
			vec3 pixel = img.getPixel(x, y);
			sum += logf(delta + toLuminance(pixel.x, pixel.y, pixel.z));
		}
	}

	int n = img.width * img.height; // Number of pixels in image
	return expf(sum / float(n));
}

// http://imdoingitwrong.wordpress.com/tag/hdr/
// Blend the color of the lightbuffer and the texture image by using the tonemap operator
// defined in (Eq. 4) of http://www.cs.utah.edu/~reinhard/cdrom/tonemap.pdf
void applyTonemap(HDRImage &img)
{
	const float exposure = 0.3f; // Key (image specific)
	const float white = 128.0f; // The smallest luminance that will be mapped to pure white

	float lumAvg = logAverageLuminance(img);

	const uint32 width = img.width;
	const uint32 height = img.height;
	for(uint32 y = 0; y < height; ++y)
	{
		for(uint32 x = 0; x < width; ++x)
		{
			vec3 pixel = img.getPixel(x, y);
			float R = pixel.x;
			float G = pixel.y;
			float B = pixel.z;

			// Calculate pixel luminance
			float Lp = toLuminance(R, G, B);

			// Scale the pixel luminance to a middle gray zone
			float L = Lp * exposure / lumAvg;

			// Apply modified tonemapping operator and compress luminance to displayable range
			float Ld = L * (1.0f + (L / (white * white))) / (1.0f + L);

			float scale = Ld / Lp;

			// Scale and clamp all colors by the relative luminance gain
			R = std::min(R * scale, 1.0f);
			G = std::min(G * scale, 1.0f);
			B = std::min(B * scale, 1.0f);
			img.setPixel(x, y, vec3(R, G, B));
		}
	}
}

void trace(const HDRImage &texture, const HDRImage &collision, HDRImage &lightbuffer, 
const vec3 &photonColor, const vec2 &ro, const vec2 &d, int bounces = 0)
{
	if(bounces > 4)
		return;

	float t = 0.0f;
	float dt = 2.0f;
	const int steps = 256;
	for(int i = 0; i < steps; ++i)
	{
		vec2 p = ro + d * t;
		t += dt;

		int xi = int(p.x);
		int yi = int(p.y);
		if(xi < 0 || xi >= collision.width || yi < 0 || yi >= collision.height)
			continue;

		// Add photon color to lightbuffer
		lightbuffer.setPixel(xi, yi, photonColor + lightbuffer.getPixel(xi, yi));

		// We hit something (red value is 0)
		if(collision.getPixel(xi, yi).x == 0)
		{
			// Multiply photon color with texture color and emit photon in new direction
			vec2 ndir = vec2(frand(), frand());
			vec2 nro = ro + d * (t - 2.0f * dt);
			vec3 newPhotonColor = photonColor * texture.getPixel(xi, yi);
			trace(texture, collision, lightbuffer, newPhotonColor, nro, ndir, bounces + 1);
			return;
		}
	}
}

void simulate(const HDRImage &texture, const HDRImage &collision, HDRImage &lightbuffer, 
int photoncount, const vec3 &light)
{
	for(uint32 i = 0; i < photoncount; ++i)
	{
		// Emit photons in a circular distribution, with a slight random offset
		vec2 ro = vec2(270.0f, 20.0f) + vec2(frand(), frand()) * 4.0f;
		vec2 dir = vec2(frand(), frand());
		trace(texture, collision, lightbuffer, light, ro, dir);
	}
}

int main(int argc, char *argv[])
{
	HDRImage texture;
	HDRImage collision;
	std::cout<<"Loading images...";
	if(!loadPng("data/mariotexture.png", texture) ||
	   !loadPng("data/mariocollision.png", collision))
	{
		std::cerr<<"failed :("<<std::endl;
		return -1;
	}
	std::cout<<"done!"<<std::endl;

	// Create a lightbuffer and add an ambient light
	HDRImage lightbuffer(texture.width, texture.height);
	lightbuffer.clear(vec3(32.0f));

	// Simulate photon distribution into a lightbuffer
	int photonCount = 1000000;
	vec3 lightColor = vec3(1.0f, 0.9f, 0.9f);
	simulate(texture, collision, lightbuffer, photonCount, lightColor);

	// Multiplicatively blend texture with lightbuffer
	HDRImage result(texture.width, texture.height);
	for(int i = 0; i < texture.width * texture.height; ++i)
	{
		vec3 tex = texture.pixels[i];
		vec3 light = lightbuffer.pixels[i];
		result.pixels[i] = tex * light;
	}

	applyTonemap(result);
	//applyTonemap(lightbuffer);

	std::cout<<"Saving image...";
	savePng("data/texturepp.png", result);
	std::cout<<"done!\n";

	std::cin.get();
	return 0;
}