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
	void createDepthResource();
	void destroyDepthResource();

	void createDeferredColorBufferResource();
	void destroyDeferredColorBufferResource();

	void createForwardColorBufferResource();
	void destroyForwardColorBufferResource();

	void createSwapChainFrameBuffers(VulkanRenderPass* vulkanRenderPass);
	void createDeferredFrameBuffer(VulkanRenderPass* vulkanRenderPass);
	void createForwardFrameBuffer(VulkanRenderPass* vulkanRenderPass);

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
	VkFramebuffer createFrameBuffer(std::vector<VkImageView> attachments,
		VkExtent2D swapChainExtent, VulkanRenderPass* vulkanRenderPass);

	std::vector<VkFramebuffer> vVulkanFrameBuffer;
	VkFramebuffer deferredFrameBuffer;
	VkFramebuffer forwardFrameBuffer;
	VulkanDevice* vulkanDevice;

	VkImage gbufferAImage;
	VkImage gbufferBImage;
	VkImage gbufferCImage;
	VkImage gbufferDImage;
	VkImage gbufferEImage;

	VkImageView gbufferAImageView;
	VkImageView gbufferBImageView;
	VkImageView gbufferCImageView;
	VkImageView gbufferDImageView;
	VkImageView gbufferEImageView;

	VkDeviceMemory gbufferAImageMemory;
	VkDeviceMemory gbufferBImageMemory;
	VkDeviceMemory gbufferCImageMemory;
	VkDeviceMemory gbufferDImageMemory;
	VkDeviceMemory gbufferEImageMemory;

	VkImage colorBufferImage;
	VkImage vectorBufferImage;

	VkImageView colorBufferImageView;
	VkImageView vectorBufferImageView;

	VkDeviceMemory colorBufferImageMemory;
	VkDeviceMemory vectorBufferImageMemory;

	VkImage depthImage;
	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;

};

