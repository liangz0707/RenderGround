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
	void destroyDepthResource();
	void createSwapChainFrameBuffers(VulkanSwapChain * vulkanSwapChain, VulkanRenderPass* vulkanRenderPass);
	void destroySwapChainFrameBuffers();

	size_t GetFrameBufferSize()
	{
		return vVulkanFrameBuffer.size();
	}

	VkFramebuffer GetFrameBufferByIndex(int index)
	{
		return vVulkanFrameBuffer[index];
	}


private:
	VkFramebuffer createFrameBuffer(std::array<VkImageView, 2> attachments,
		VkExtent2D swapChainExtent, VulkanRenderPass* vulkanRenderPass);
	std::vector<VkFramebuffer> vVulkanFrameBuffer;
	VulkanDevice* vulkanDevice;

	VkImage depthImage;

	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;

};

