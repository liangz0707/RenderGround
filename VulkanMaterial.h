#pragma once
#include "Common.h"
#include "VulkanGraphicPipeline.h"

class VulkanGraphicPipeline;

class VulkanMaterial
{
public:

	VulkanMaterial();

	void Update(UniformBufferObject ubo);

	void SetDescriptorSet(VkDescriptorSet vkDescriptorSet)
	{
		this->vkDescriptorSet = vkDescriptorSet;
	}

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

