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
	void createDepthResource(VkFormat format, VkExtent2D extent2D);
	VkFramebuffer createFrameBuffer(std::array<VkImageView, 2> attachments, VkExtent2D swapChainExtent);
	void createSwapChainFrameBuffers();
	size_t GetFrameBufferSize()
	{
		return vVulkanFrameBuffer.size();
	}
	VkExtent2D GetFrameBufferExtent();
	VkFramebuffer GetFrameBufferByIndex(int index)
	{
		return vVulkanFrameBuffer[index];
	}

	VulkanSwapChain* GetSwapChain()
	{
		return vulkanSwapChain;
	}

private:

	VulkanSwapChain* vulkanSwapChain;
	std::vector<VkFramebuffer> vVulkanFrameBuffer;
	VulkanRenderPass* vulkanRenderPass;
	VulkanDevice* vulkanDevice;

	VkImage depthImage;

	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;
};

