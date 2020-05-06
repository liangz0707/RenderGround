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
	void CreateGlobalBuffer();



	void createGbufferDescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout layout);
	void createUniformDescriptorSets(VkDescriptorPool pool, VkDescriptorSetLayout layout);


	void createUniformBuffers(VkDeviceSize bufferSize);
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

	VkBuffer globalBuffer;
	VkDeviceMemory globalBufferMemory;
	VkDescriptorSet globalDescriptorSet;

	VkDescriptorSet gbufferDescriptorSet;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<VkDescriptorSet> uniformDescriptorSets;
};

