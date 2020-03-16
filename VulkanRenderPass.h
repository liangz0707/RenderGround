#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanPipelineResource.h"
#include "VulkanFramebuffer.h"

class VulkanFramebuffer;

class VulkanSwapChain;
class VulkanGraphicPipeline;
class VulkanPipelineResource;
	
class VulkanRenderPass
{
private:

	VkRenderPass renderPass;
	VkDescriptorPool uniformDescriptorPool;
	VkDescriptorPool objectDescriptorPool;
	std::vector<VkDescriptorSet> uniformDescriptorSets;
	VkDescriptorSetLayout uniformDescriptorSetLayout;
	VkDescriptorSetLayout objectDescriptorSetLayout;
	VulkanGraphicPipeline* vulkanGraphicPipeline;

	VulkanPipelineResource* vulkanPipelineResource;
	VkSampler vkSampler;

public :
	VulkanRenderPass(VulkanPipelineResource* vulkanPipelineResource);
	VkRenderPass GetInstance();
	void createSampler();
	void createRenderPass();
	void createGraphicPipelines();
	void createUniformDescriptorSetLayout();
	void createObjectDescriptorSetLayout();
	void createUniformDescriptorPool();
	void createObjectDescriptorPool();

	void createUniformDescriptorSets(VulkanFramebuffer* vulkanFramebuffer,
		std::vector<VkBuffer> uniformBuffers);

	VkDescriptorSet createObjectDescriptorSet(VkBuffer preEntityUniformBuffer,
		VkSampler vkTextureSampler,
		VkImageView vkTextureImageView);


	
	VkDescriptorSet GetUniformDescriptorSetByIndex(int i)
	{
		return uniformDescriptorSets[i];
	}


	VulkanGraphicPipeline *GetGraphicPipeline()
	{
		return vulkanGraphicPipeline;
	}

	VkPipelineLayout GetPipelineLayout();
};

