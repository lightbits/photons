#include "scene.h"

Scene::Scene(const Image &collision_, const Image &texture_, const Image &lightbuffer_) :
	collision(collision_), texture(texture_), lightbuffer(lightbuffer_)
{ }

void Scene::simulate(int photoncount)
{

}

void Scene::save(const std::string &filename)
{
	uint32 width = texture.getWidth();
	uint32 height = texture.getHeight();
	Image result(width, height);
	for(uint32 y = 0; y < height; ++y)
	{
		for(uint32 x = 0; x < width; ++x)
		{
			Color dest = texture.getColor(x, y);
			Color src = lightbuffer.getColor(x, y);
			result.setColor(x, y, dest * src);
		}
	}
	result.save(filename);
}