#ifndef SCENE_H
#define SCENE_H
#include "image.h"
#include "photon.h"

//struct CollisionGrid
//{
//	CollisionGrid(uint32 Width, uint32 Height)
//		: width(Width), height(Height), data(Width * Height)
//	{ }
//
//	void set(uint32 x, uint32 y) { data[x + y * width] = true; }
//	void reset(uint32 x, uint32 y) { data[x + y * width] = false; }
//	bool get(uint32 x, uint32 y) const { return data[x + y * width]; }
//
//	std::vector<bool> data;
//	uint32 width;
//	uint32 height;
//};

class Scene
{
public:
	Scene(const Image &collision_, const Image &texture_, const Image &lightbuffer_);
	void simulate(int photoncount);
	void save(const std::string &filename);
	//void applyTonemap();
private:
	Image collision;
	Image texture;
	Image lightbuffer;

	// Continually updated during simulation
	float logSumLuminance;
	float sceneMaxLuminance;
};

#endif