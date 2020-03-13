#pragma once
#include "Common.h"
#include "VulkanModel.h"
class VulkanRModel
{
public:
	VulkanRModel(VulkanModel* vulkanModel);
private:
	VkImage textureImage;
	VkImageView textureImageView;

	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
};

