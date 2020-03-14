#pragma once
#include "Common.h"
#include "VulkanResourceManager.h"

class VulkanModel
{
private:
	const std::string MODEL_PATH = "models/chalet.obj";
	const std::string TEXTURE_PATH = "textures/chalet.jpg";
	int texWidth;
	int texHeight;
	int texChannel;
	Vertex* vertex;
	//std::vector<Vertex> vertices;
	//std::vector<uint32_t> indices;
	const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f,0}, {0.0f, 1.0f, 0.0f},{1,0}},
	{{0.5f, -0.5f, 0}, {0.0f, 0.0f, 1.0f},{1,0}},
	{{0.5f, 0.5f,0}, {1.0f, 0.0f, 0.0f},{0,0}},
	{{-0.5f, 0.5f,0}, {1.0f, 1.0f, 1.0f},{0,0}}
	};
	//std::vector<uint32_t> indices;
	const std::vector<uint32_t> indices = {
		0, 2, 1, 3, 2, 0
	};


	unsigned char* pixels;
public:
	VulkanModel();

	std::vector<Vertex> GetVertex()
	{
		return vertices;
	}

	std::vector<uint32_t> GetIndex()
	{
		return indices;
	}
};

