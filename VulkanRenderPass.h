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
	VkDescriptorPool textureDescriptorPool;
	std::vector<VkDescriptorSet> uniformDescriptorSets;
	std::vector<VkDescriptorSet> textureDescriptorSets;
	VkDescriptorSetLayout uniformDescriptorSetLayout;
	VkDescriptorSetLayout textureDescriptorSetLayout;
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
	void createTextureDescriptorSetLayout();
	void createUniformDescriptorPool();
	void createTextureDescriptorPool();

	void createUniformDescriptorSets(VulkanFramebuffer* vulkanFramebuffer,
		std::vector<VkBuffer> uniformBuffers, 
		std::vector<VkBuffer> preEntityUniformBuffers);

	void createTextureDescriptorSets(VulkanFramebuffer* vulkanFramebuffer,
		VkSampler vkTextureSampler ,
		VkImageView vkTextureImageView);
	
	VkDescriptorSet GetUniformDescriptorSetByIndex(int i)
	{
		return uniformDescriptorSets[i];
	}

	VkDescriptorSet GetTextureDescriptorSetByIndex(int i)
	{
		return textureDescriptorSets[i];
	}

	VulkanGraphicPipeline *GetGraphicPipeline()
	{
		return vulkanGraphicPipeline;
	}

	VkPipelineLayout GetPipelineLayout();
};

