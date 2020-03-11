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
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;

	unsigned char* pixels;
public:
	VulkanModel();
	void createTextureImageView();

};

