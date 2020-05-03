#pragma once
#include "Common.h"
#include "IVulkanGraphicPipeline.h"

class IVulkanGraphicPipeline;

class VulkanMaterial
{
public:

	VulkanMaterial();

	void Update(PreEntityUniformBufferObject ubo);

	void SetDescriptorSet(VkDescriptorSet vkDescriptorSet)
	{
		this->vkDescriptorSet = vkDescriptorSet;
	}
	VkDescriptorSet CreateObjectDescriptorSet(
		VkBuffer preEntityUniformBuffer,
		VkSampler vkTextureSampler,
		VkImageView vkTextureImageView,
		VkDescriptorSetLayout layouts,
		VkDescriptorPool pool);

	void CreatePreUniformBuffer(
		VkDeviceSize bufferSize,
		VkBuffer& preObjectBuffer,
		VkDeviceMemory& vertexBufferMemory);

	VkDescriptorSet GetDescriptorSet()
	{
		return vkDescriptorSet;
	}

	void SetDescriptorSetBufferMemory(VkDeviceMemory vkDescriptorSetBufferMemory)
	{
		this->vkDescriptorSetBufferMemory = vkDescriptorSetBufferMemory;
	}

	VkDeviceMemory GetDescriptorSetBufferMemory()
	{
		return vkDescriptorSetBufferMemory;
	}

	void SetDescriptorSetBuffer(VkBuffer vkDescriptorSetBuffer)
	{
		this->vkDescriptorSetBuffer = vkDescriptorSetBuffer;
	}

	VkBuffer GetDescriptorSetBuffer()
	{
		return vkDescriptorSetBuffer;
	}

private:
	VkBuffer vkDescriptorSetBuffer;
	VkDeviceMemory vkDescriptorSetBufferMemory;
	VkDescriptorSet vkDescriptorSet;
};

