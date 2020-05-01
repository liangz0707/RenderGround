#pragma once
#include "Common.h"
#include "VulkanApplication.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanModel.h"


class IVulkanGraphicPipeline
{

public:
	IVulkanGraphicPipeline();

	virtual void createGraphicsPipeline(
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule);

	virtual void destroyGraphicPipeline();

	virtual VkPipeline GetInstance()
	{
		return graphicsPipeline;
	}

protected:
	VkPipeline graphicsPipeline;
};


