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
public :
	VulkanRenderPass();
	VkRenderPass GetInstance();
	
	void createRenderPass();
	void destroyRenderPass();
	void createGraphicPipelines(VulkanPipelineResource *vulkanPipelineResource);
	void destroyGraphicPipelines();


	VkDescriptorSet GetUniformDescriptorSetByIndex(int i);

	VulkanGraphicPipeline *GetGraphicPipeline()
	{
		return vulkanGraphicPipeline;
	}

	VkPipelineLayout GetPipelineLayout();

private:
	VkRenderPass renderPass;
	VulkanGraphicPipeline* vulkanGraphicPipeline;
	std::vector<VkDescriptorSet> uniformDescriptorSets;
};

