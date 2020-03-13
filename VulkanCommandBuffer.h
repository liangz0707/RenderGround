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
	VulkanFrameRenderCommandBuffer(VkCommandPool commandPool, size_t commandBufferSize);
	~VulkanFrameRenderCommandBuffer();

	VkCommandBuffer VulkanCommandBegin(int index);
	void VulkanCommandEnd(int index);
	int GetCommandBufferSize() {
		return commandBuffers.size();
	}
private:
	std::vector<VkCommandBuffer> commandBuffers;
	VkCommandPool commandPool;
};