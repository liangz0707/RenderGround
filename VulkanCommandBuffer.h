#pragma once
#include "Common.h"

class VulkanCommandBuffer
{
private:
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VkDevice device;

public:
	VulkanCommandBuffer(VkDevice* device);
	void createCommandBuffers();


	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
};