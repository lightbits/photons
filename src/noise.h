#ifndef NOISE_H
#define NOISE_H

static unsigned int xor128()
{
	static unsigned int x = 123456789;
	static unsigned int y = 362436069;
	static unsigned int z = 521288629;
	static unsigned int w = 88675123;
	unsigned int t;

	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >>8));
}

// Returns a uniformly distributed single precision float between -1 and 1
static float frand()
{
	return 2.0f * (xor128() / float(4294967295.0f)) - 1.0f;
}

#endif