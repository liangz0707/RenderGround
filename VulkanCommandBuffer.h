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
	VulkanFrameRenderCommandBuffer();
	~VulkanFrameRenderCommandBuffer();

	void createCommandBuffer(VkCommandPool commandPool, size_t commandBufferSize);
	void destroyCommandBuffer();

	VkCommandBuffer GetCommandBufferByIndex(int index)
	{
		return commandBuffers[index];
	}

	VkCommandBuffer VulkanCommandBegin(int index);
	void VulkanCommandEnd(int index);
	size_t GetCommandBufferSize() {
		return commandBuffers.size();
	}
private:
	std::vector<VkCommandBuffer> commandBuffers;
};