#pragma once
#include "Common.h"
#include "VulkanCommandBuffer.h"
#include "VulkanResourceManager.h"
#include "VulkanSceneManager.h"
class VulkanSceneManager;
class VulkanModel;

class VulkanPipelineResource
{
public:
	VulkanPipelineResource();


	void createTextureSampler();
	void createCommandBuffers();
	bool hasStencilComponent(VkFormat format);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void copyBufferToBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void createVertexBuffer(VkDeviceSize bufferSize,
		void* srcData,
		VkBuffer &vertexBuffer,
		VkDeviceMemory &vertexBufferMemory);

	void createIndexBuffer(VkDeviceSize bufferSize,
		void* srcData,
		VkBuffer &vertexBuffer,
		VkDeviceMemory &vertexBufferMemory);

	void createTextureImage(unsigned char* pixels,
		int texWidth,
		int texHeight,
		int texChannel,
		VkImage textureImage,
		VkDeviceMemory textureImageMemory);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	
	void createUniformBuffers(VkDeviceSize bufferSize);
	void createPreUniformBuffers(VkDeviceSize bufferSize);
	void updateUniformBuffer();
	VkDeviceMemory GetUboMemory();
	VkDeviceMemory GetPreUboMemory();

	std::vector<VkBuffer> GetUniformBuffers()
	{
		return uniformBuffers;
	}

	std::vector<VkBuffer> GetPreEntityUniformBuffers()
	{
		return preEntityUniformBuffers;
	}

	VkSampler GetTextureSampler()
	{
		return textureSampler;
	}

private:
	VulkanSceneManager* vulkanSceneManager;
	int imageIndex;
	VkSampler textureSampler;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<VkBuffer> preEntityUniformBuffers;
	std::vector<VkDeviceMemory> preEntityUniformBuffersMemory;
	std::vector<VkCommandBuffer> commandBuffers;
};

