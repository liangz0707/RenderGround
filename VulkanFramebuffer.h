#pragma once
#include "Common.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanResourceManager.h"

class VulkanSwapChain;
class VulkanRenderPass;

class VulkanFramebuffer
{
public:
	void createDepthResource(VulkanSwapChain* vulkanSwapChain);
	
	void createSwapChainFrameBuffers(VulkanSwapChain * vulkanSwapChain);
	size_t GetFrameBufferSize()
	{
		return vVulkanFrameBuffer.size();
	}

	VkFramebuffer GetFrameBufferByIndex(int index)
	{
		return vVulkanFrameBuffer[index];
	}


private:
	VkFramebuffer createFrameBuffer(std::array<VkImageView, 2> attachments, VkExtent2D swapChainExtent);
	std::vector<VkFramebuffer> vVulkanFrameBuffer;
	VulkanRenderPass* vulkanRenderPass;
	VulkanDevice* vulkanDevice;

	VkImage depthImage;

	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;

};

