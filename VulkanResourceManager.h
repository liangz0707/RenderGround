#pragma once
#include "Common.h"
#include "VulkanDevice.h"
#include "VulkanApplication.h"
#include "VulkanFramebuffer.h"
#include "VulkanSwapChain.h"
class VulkanSwapChain;
class VulkanFramebuffer;

class VulkanResourceManager
{
public:
	static VulkanResourceManager* GetResourceManager() {
		if (vulkanResourceManager == nullptr)
			throw std::runtime_error("resource manager are not inited!");
		return vulkanResourceManager;
	}

	static void SetResourceManager(VulkanDevice* vulkanDevice, VulkanApplication* vulkanInstance,VulkanSwapChain* vulkanSwapChain) {
		if (vulkanResourceManager == nullptr)
			vulkanResourceManager = new VulkanResourceManager(vulkanDevice, vulkanInstance, vulkanSwapChain);
	}
	VkFramebuffer createFramebuffer(VkFramebufferCreateInfo* framebufferInfo);

	void SetFramebuffer(VulkanFramebuffer*);
	VulkanFramebuffer* GetFramebuffer();
	void SetSwapChain(VulkanSwapChain* vulkanSwapChain);
	VulkanSwapChain* GetSwapChain();

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags flags);
	
	void createPipelineLayout(VkPipelineLayoutCreateInfo* pipelineLayoutInfo, VkPipelineLayout* pipelineLayout);
	void createGraphicsPipeline(VkGraphicsPipelineCreateInfo* pipelineLayoutInfo, VkPipeline* pipeline);
	
	void createImage(uint32_t width, uint32_t height, 
		VkFormat format, 
		VkImageTiling tiling, 
		VkImageUsageFlags usage, 
		VkMemoryPropertyFlags properties, 
		VkImage& image, 
		VkDeviceMemory& imageMemory);

	void createSampler(VkSamplerCreateInfo* samplerInfo, VkSampler* sampler);

	void destroyImage(VkImage);
	void allocCommandBuffer(VkCommandBufferAllocateInfo* allocInfo, VkCommandBuffer* commandBuffer);
	void destroyBuffer(VkBuffer);
	void freeMemory(VkDeviceMemory memory);
	void createBuffer(VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);

	void createCommandPool();
	VkCommandPool GetCommandPool();

	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	QueueFamilyIndices findQueueFamilies();

	void mapMemory(VkDeviceMemory, VkDeviceSize, void** data);
	void unMapMemory(VkDeviceMemory);

	void createSyncObjects();
	VulkanDevice * GetDevice()
	{
		return vulkanDevice;
	}

	VkExtent2D GetExtent();


	// 同步互斥操作：



private:
	VulkanResourceManager(
		VulkanDevice* vulkanDevice, 
		VulkanApplication* vulkanInstance,
		VulkanSwapChain* vulkanSwapChain);

	static VulkanResourceManager* vulkanResourceManager;
	// 同时运作的渲染的图像
	const int MAX_FRAMES_IN_FLIGHT = 2;
	// 下面的两种信号量是用来做GPU-GPU同步的
	// 图像已经从SwapChain获取可用于渲染
	std::vector<VkSemaphore> imageAvailableSemaphores;
	// 图像渲染完成可用于显示
	std::vector<VkSemaphore> renderFinishedSemaphores;
	// 下面的一个信号量是进行CPU-GPU同步的
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	VulkanSwapChain* vulkanSwapChain;
	VulkanFramebuffer* vulkanFramebuffer;
	VkCommandPool commandPool;
	VulkanDevice* vulkanDevice;
	VulkanApplication* vulkanInstance;

};

