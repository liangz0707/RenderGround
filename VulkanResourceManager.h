#pragma once
#include "Common.h"
#include "VulkanDevice.h"
#include "VulkanApplication.h"

class VulkanResourceManager
{
public:
	static VulkanResourceManager* GetResourceManager() {
		if (vulkanResourceManager == nullptr)
			throw std::runtime_error("resource manager are not inited!");
		return vulkanResourceManager;
	}

	static void SetResourceManager(VulkanDevice* vulkanDevice, VulkanApplication* vulkanInstance) {
		if (vulkanResourceManager == nullptr)
			vulkanResourceManager = new VulkanResourceManager(vulkanDevice, vulkanInstance);

	}

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags flags);
	
	void createImage(uint32_t width, uint32_t height, 
		VkFormat format, 
		VkImageTiling tiling, 
		VkImageUsageFlags usage, 
		VkMemoryPropertyFlags properties, 
		VkImage& image, 
		VkDeviceMemory& imageMemory);

	void destroyImage(VkImage);
	void destroyBuffer(VkBuffer);
	void freeMemory(VkDeviceMemory memory);
	void createBuffer(VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);

	void createCommandPool();
	VkFramebuffer createFramebuffer(VkFramebufferCreateInfo *framebufferInfo);

	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	QueueFamilyIndices findQueueFamilies();

	void mapMemory(VkDeviceMemory, VkDeviceSize, void** data);
	void unMapMemory(VkDeviceMemory);

private:
	VulkanResourceManager(VulkanDevice* vulkanDevice, VulkanApplication* vulkanInstance);

	static VulkanResourceManager* vulkanResourceManager;

	VkCommandPool commandPool;
	VulkanDevice* vulkanDevice;
	VulkanApplication* vulkanInstance;
	VkFramebuffer vkFramebuffer;

};

