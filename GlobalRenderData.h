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
	void createGbufferDescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout layout);
	void createUniformDescriptorSets(VkDescriptorPool pool, VkDescriptorSetLayout layout);
	void destroyUniformBuffers();
	void updateUniformBuffer(int swapChainImageIndex);
	VkDescriptorSet getUniformDescriptorSet(int swapChainImageIndex)
	{
		return uniformDescriptorSets[swapChainImageIndex];
	}
	VkDescriptorSet getGbufferDescriptorSet()
	{
		return gbufferDescriptorSet;
	}
private:

	VkDescriptorSet gbufferDescriptorSet;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<VkDescriptorSet> uniformDescriptorSets;
};

