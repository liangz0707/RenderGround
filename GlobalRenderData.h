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

struct BatchBufferObject {
	glm::mat4 model;
};

struct GlobalBufferObject {
	glm::mat4 view;
	glm::mat4 proj;
};

struct PostBufferObject {
	glm::vec4 postColor;
};

struct ForwardLigtingBufferObject {
	glm::vec4 lightPosition;
	glm::vec4 lightColor;
};

struct DeferredLigtingBufferObject {
	glm::vec4 lightPosition;
	glm::vec4 lightColor;
};


class GlobalRenderData
{
public:
	GlobalRenderData();
	//void createGlobalBuffer();
	//void createGbufferLightingBuffer();
	//void createForwardLightingBuffer();
	//void createPostBuffer();


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

