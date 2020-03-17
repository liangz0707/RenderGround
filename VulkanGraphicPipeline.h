#pragma once
#include "Common.h"
#include "VulkanApplication.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanModel.h"

#include "VulkanPipelineResource.h"

class VulkanPipelineResource;
class VulkanGraphicPipeline
{

public:
	VulkanGraphicPipeline();

	void createGraphicsPipeline(
		VulkanPipelineResource *vulkanPipelineResource,
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule,
		VkExtent2D swapChainExtent,
		VkDescriptorSetLayout descriptorSetLayout,
		VkRenderPass renderPass);

	void destroyGraphicPipeline();

	VkPipeline GetInstance()
	{
		return graphicsPipeline;
	}

	VkPipelineLayout GetPipelineLayout()
	{
		return vkPipelineLayout;
	}
	

private:
	VkPipeline graphicsPipeline;
	VkPipelineLayout vkPipelineLayout;
};


