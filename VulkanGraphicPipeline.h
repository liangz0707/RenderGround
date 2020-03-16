#pragma once
#include "Common.h"
#include "VulkanApplication.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanModel.h"
class VulkanGraphicPipeline
{

public:
	VulkanGraphicPipeline();

	void createGraphicsPipeline(
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule,
		VkExtent2D swapChainExtent,
		VkDescriptorSetLayout* descriptorSetLayout,
		int layoutCount,
		VkRenderPass renderPass);

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


