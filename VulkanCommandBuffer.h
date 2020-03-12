#pragma once
#include "Common.h"
#include "VulkanDevice.h"
#include "VulkanResourceManager.h"
#include "VulkanFramebuffer.h"
class VulkanSingleTimeCommandBuffer
{

public:
	VulkanSingleTimeCommandBuffer(VkCommandPool commandPool);
	~VulkanSingleTimeCommandBuffer();
	VkCommandBuffer GetInstance()
	{
		return commandBuffer;
	}
private:
	VkCommandBuffer commandBuffer;
	VkCommandPool commandPool;
};

class VulkanFrameRenderCommandBuffer
{

public:
	VulkanFrameRenderCommandBuffer(VkCommandPool commandPool);
	~VulkanFrameRenderCommandBuffer();

	void VulkanCommandBegin(int index);
	void VulkanCommandEnd(int index);
	
private:
	std::vector<VkCommandBuffer> commandBuffers;
	VkCommandPool commandPool;
};