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
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;
	VulkanGraphicPipeline* vulkanGraphicPipeline;

	VulkanPipelineResource* vulkanPipelineResource;

public :
	VulkanRenderPass(VulkanPipelineResource* vulkanPipelineResource);
	VkRenderPass GetInstance();
	void createRenderPass();
	void createGraphicPipelines();
	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSets(VulkanFramebuffer* vulkanFramebuffer,
		std::vector<VkBuffer> uniformBuffers, 
		std::vector<VkBuffer> preEntityUniformBuffers);
	
	VkDescriptorSet GetDescriptorSetByIndex(int i)
	{
		return descriptorSets[i];
	}

	VulkanGraphicPipeline *GetGraphicPipeline()
	{
		return vulkanGraphicPipeline;
	}

	VkPipelineLayout GetPipelineLayout();
};

