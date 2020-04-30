#pragma once
#include "Common.h"
#include "VulkanCommandBuffer.h"
#include "VulkanResourceManager.h"
#include "VulkanSceneManager.h"

class VulkanResourceManager;
class VulkanFrameRenderCommandBuffer;
class VulkanSwapChain;
class VulkanFramebuffer;
class VulkanApplication;

class GlobalRenderData
{
public:
	GlobalRenderData();
	void createUniformBuffers(VkDeviceSize bufferSize);
	void createUniformDescriptorSets(VkDescriptorPool pool, VkDescriptorSetLayout layout);
	void destroyUniformBuffers();
	void updateUniformBuffer(int swapChainImageIndex);
	VkDescriptorSet getUniformDescriptorSet(int swapChainImageIndex)
	{
		return uniformDescriptorSets[swapChainImageIndex];
	}
private:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<VkDescriptorSet> uniformDescriptorSets;
};

