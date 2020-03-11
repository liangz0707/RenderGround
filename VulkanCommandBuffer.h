#pragma once
#include "Common.h"
#include "VulkanDevice.h"
#include "VulkanResourceManager.h"
class VulkanCommandBuffer
{
private:
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VulkanDevice* device;

public:
	VulkanCommandBuffer(VulkanDevice* device);
	void createCommandBuffers();
	bool hasStencilComponent(VkFormat format);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void createTextureImage(unsigned char* pixels,
		int texWidth,
		int texHeight,
		int texChannel,
		VkImage textureImage,
		VkDeviceMemory textureImageMemory);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
};