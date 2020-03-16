#pragma once
#include "Common.h"
#include "VulkanTexture.h"
class VulkanResource
{
public:
	VulkanResource();


private:
	std::vector<VulkanTexture> textures;
};

