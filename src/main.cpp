/* Photon simulator
Idea
====
http://forums.tigsource.com/index.php?topic=31378.0

Tonemapping and color correction
====
* http://mynameismjp.wordpress.com/2010/04/30/a-closer-look-at-tone-mapping/
* http://www.xnainfo.com/content.php?content=28
* Real-Time Rendering Third Edition
* http://www.cs.utah.edu/~reinhard/cdrom/tonemap.pdf
* http://www.cs.utah.edu/~reinhard/cdrom/
*/

#include "image.h"
#include "scene.h"
#include "noise.h"
#include "drawing.h"
#include "framebuffer.h"
#include <iostream>
#include <thread>
#include <vector>

void trace(const Image &texture, const Image &collision, FrameBuffer &lightbuffer, const Colorf &frag0, 
const vec2 &ro, const vec2 &d, int bounces = 0)
{
	if(bounces > 4)
		return;

	float t = 0.0f;
	float dt = 2.0f;
	for(int i = 0; i < 128; ++i)
	{
		vec2 p = ro + d * t;
		t += dt;

		int xi = int(p.x);
		int yi = int(p.y);
		if(xi < 0 || xi >= collision.getWidth() || yi < 0 || yi >= collision.getHeight())
			return;

		// Add photon color to lightbuffer
		Colorf fragDest = lightbuffer.getFragment(xi, yi);
		lightbuffer.setFragment(xi, yi, (frag0 + fragDest) * 0.75f);

		// We hit something
		if(collision.getColor(xi, yi).r == 0)
		{
			// Multiply photon color with texture color and emit photon in new direction
			vec2 ndir = vec2(frand(), frand());
			vec2 nro = ro + d * (t - dt);
			Colorf nfrag = frag0 * texture.getColorf(xi, yi);
			trace(texture, collision, lightbuffer, nfrag, nro, ndir, bounces + 1);
			return;
		}
	}
}

void simulate(const Image &texture, const Image &collision, FrameBuffer &lightbuffer)
{
	const uint32 photoncount = 10000;
	Colorf light(1.0f, 1.0f, 1.0f);

	for(uint32 i = 0; i < photoncount; ++i)
	{
		vec2 ro = vec2(40.0f, 180.0f) + vec2(frand(), frand()) * 4.0f;
		vec2 dir = vec2(frand(), frand());
		trace(texture, collision, lightbuffer, light, ro, dir);
	}
}

// See http://www.xnainfo.com/content.php?content=28
static float toLuminance(const Colorf &c) { return 0.299f * c.r + 0.587f * c.g + 0.114f * c.b; }

// Computes the (log) average of the luminance of the image by eq. 1 in [Reinhard]
float logAverageLuminance(const FrameBuffer &fb)
{
	const float delta = 0.001f; // Small value to avoid singularity if black pixels are present
	float sum = 0.0f;
	float asum = 0.0f;

	// Sum up the log of the luminance value of each pixel
	for(uint32 y = 0; y < fb.getHeight(); ++y)
	{
		for(uint32 x = 0; x < fb.getWidth(); ++x)
		{
			sum += logf(delta + toLuminance(fb.getFragment(x, y)));
			asum += toLuminance(fb.getFragment(x, y));
		}
	}

	int n = fb.getWidth() * fb.getHeight(); // Number of pixels in image
	return expf(sum / float(n));
}

// Blend the color of the lightbuffer and the texture image by using the tonemap operator
// defined in (Eq. 4) of http://www.cs.utah.edu/~reinhard/cdrom/tonemap.pdf
void applyTonemap(const FrameBuffer &lightbuffer, const Image &texture, Image &result)
{
	const float middleGray = 0.6f; // Key (image specific)
	const float whiteLuminance = 16.0f; // The smallest luminance that will be mapped to pure white

	float lumAvg = logAverageLuminance(lightbuffer);

	const uint32 width = lightbuffer.getWidth();
	const uint32 height = lightbuffer.getHeight();
	for(uint32 y = 0; y < width; ++y)
	{
		for(uint32 x = 0; x < height; ++x)
		{
			Colorf light = lightbuffer.getFragment(x, y);
			Colorf pixel = light;

			float lumPixel = toLuminance(pixel);

			// Apply modified tonemapping operator
			float lumScaled = lumPixel * middleGray / lumAvg;
			float lumCompressed = lumScaled * (1.0f + (lumScaled / (whiteLuminance * whiteLuminance))) / (1.0f + lumScaled);

			Colorf tex = texture.getColorf(x, y);
			result.setColor(x, y, tex * lumCompressed);
		}
	}
}

int main()
{
	const uint32 width = 256;
	const uint32 height = 256;
	const Colorf ambient = Colorf(0.5f, 0.34f, 0.33f);

	Image texture(width, height);
	texture.load("data/texture.bmp");
	Image collision(width, height);
	collision.load("data/collision.bmp");
	FrameBuffer lightbuffer(width, height);
	lightbuffer.clear(ambient);

	simulate(texture, collision, lightbuffer);
	
	Image result(width, height);
	applyTonemap(lightbuffer, texture, result);
	result.save("result_color.bmp");

	return 0;
}