#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
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
	std::vector<VkDescriptorSet> uniformDescriptorSets;
	VkDescriptorSetLayout uniformDescriptorSetLayout;
	VulkanGraphicPipeline* vulkanGraphicPipeline;

public :
	VulkanRenderPass();
	VkRenderPass GetInstance();
	
	void createRenderPass();
	void createGraphicPipelines(VulkanPipelineResource *vulkanPipelineResource);

	void createUniformDescriptorSetLayout();
	void createUniformDescriptorPool();

	void createUniformDescriptorSets(VulkanFramebuffer* vulkanFramebuffer,
		std::vector<VkBuffer> uniformBuffers);

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

