#pragma once
#include "Common.h"
#include "VulkanGraphicPipeline.h"
class IVulkanPipelineLayout
{
public:
	IVulkanPipelineLayout();
	virtual void CreatePipelineLayout();
	virtual void DestroyPipelineLayout();

	virtual VkPipelineLayout GetInstance() {
		return vkPipelineLayout;
	}

	virtual VkDescriptorPool GetUniformDescriptorPool() {
		return uniformDescriptorPool;
	}

	virtual VkDescriptorPool GetEntityDescriptorPool() {
		return entityUniformDescriptorPool;
	}

	virtual VkDescriptorSetLayout GetUniformDescriptorSetLayout() {

		return uniformDescriptorSetLayout;
	}

	virtual VkDescriptorSetLayout GetEntityDescriptorSetLayout() {

		return entityUniformDescriptorSetLayout;
	}

	void createUniformDescriptorSetLayout();
	void destroyUniformDescriptorSetLayout();
	void createUniformDescriptorPool();
	void destroyUniformDescriptorPool();
	void createUniformDescriptorSets();

	VkDescriptorSet createObjectDescriptorSet(
		VkBuffer preEntityUniformBuffer,
		VkSampler vkTextureSampler,
		VkImageView vkTextureImageView);

	void createObjectDescriptorPool();
	void destroyObjectDescriptorPool();
	void createObjectDescriptorSetLayout();
	void destroyObjectDescriptorSetLayout();

protected:

	VkDescriptorPool uniformDescriptorPool;
	VkDescriptorPool entityUniformDescriptorPool;

	VkDescriptorSetLayout uniformDescriptorSetLayout;
	VkDescriptorSetLayout entityUniformDescriptorSetLayout;

	std::vector<VkDescriptorSet> uniformDescriptorSets;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkPipelineLayout vkPipelineLayout;
};

