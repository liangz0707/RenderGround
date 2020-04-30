#pragma once
#include "Common.h"
#include "VulkanDevice.h"
#include "VulkanApplication.h"
#include "VulkanFramebuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderState.h"
#include "VulkanCommandBuffer.h"

class VulkanFrameRenderCommandBuffer;
class VulkanSwapChain;
class VulkanFramebuffer;
class VulkanApplication;
class VulkanDevice;

class VulkanResourceManager
{
public:
	static VulkanResourceManager* GetResourceManager() {
		if (vulkanResourceManager == nullptr)
			throw std::runtime_error("resource manager are not inited!");
		return vulkanResourceManager;
	}

	static void SetResourceManager(VulkanDevice* svulkanDevice, VulkanApplication* svulkanInstance) {
		if (vulkanResourceManager == nullptr)
			vulkanResourceManager = new VulkanResourceManager(svulkanDevice, svulkanInstance);
	}
	VkFramebuffer createFramebuffer(VkFramebufferCreateInfo* framebufferInfo);

	void createTextureSampler(VkSamplerCreateInfo* samplerInfo, VkSampler &sampler);

	void SetFramebuffer(VulkanFramebuffer*);
	void SetSwapChain(VulkanSwapChain*);
	VulkanFramebuffer* GetFramebuffer();
	VulkanSwapChain* GetSwapChain();

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags flags);
	void destroyImageView(VkImageView imageView);

	void createPipelineLayout(VkPipelineLayoutCreateInfo* pipelineLayoutInfo, VkPipelineLayout* pipelineLayout);
	void createGraphicsPipeline(VkGraphicsPipelineCreateInfo* pipelineLayoutInfo, VkPipeline* pipeline);
	
	void createImage(uint32_t width, uint32_t height, 
		VkFormat format, 
		VkImageTiling tiling, 
		VkImageUsageFlags usage, 
		VkMemoryPropertyFlags properties, 
		VkImage& image, 
		VkDeviceMemory& imageMemory);

	void createSampler(VkSamplerCreateInfo* samplerInfo, VkSampler& sampler);
	VkShaderModule createShaderModule(const std::vector<char>& code);
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
	void DestroyCommandPool();

	VkCommandPool GetCommandPool();

	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	QueueFamilyIndices findQueueFamilies();

	void mapMemory(VkDeviceMemory, VkDeviceSize, void** data);
	void unMapMemory(VkDeviceMemory);

	VkSemaphore CreateSemaphore(VkSemaphoreCreateInfo* vkSemaphoreCreateInfo);
	VkFence CreateFence(VkFenceCreateInfo* vkFenceCreateInfo);

	VulkanDevice* GetDevice()
	{
		return vulkanDevice;
	}

	VkExtent2D GetExtent();


	void CreateRenderState() {
		vulkanRenderState = new VulkanRenderState();
	}

	// 同步互斥操作：
	void SyncWaitForFences(VkFence vkFence);

	VulkanRenderState* GetRenderState()
	{
		return vulkanRenderState;
	}

	void CreateSync();
	void DestroySync();
	void WaitForFences();
	void ResetFence();

	VkResult AcquireNextImageKHR(uint32_t&);
	void CheckPrivousFrameFinishend(uint32_t imageIndex);

	//
	VkResult PresentQueueSubmit(uint32_t imageIndex);
	void GraphicQueueSubmit(VkCommandBuffer);

	void UpdateRenderState();
		
	VulkanApplication *GetApplication()
	{
		return vulkanInstance;
	}
private:
	VulkanResourceManager(
		VulkanDevice* vulkanDevice, 
		VulkanApplication* vulkanInstance);

	static VulkanResourceManager* vulkanResourceManager;
	// 同时运作的渲染的图像

	VulkanRenderState* vulkanRenderState;
	VulkanSwapChain* vulkanSwapChain;
	VulkanFramebuffer* vulkanFramebuffer;
	VkCommandPool commandPool;
	VulkanDevice* vulkanDevice;
	VulkanApplication* vulkanInstance;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

};

