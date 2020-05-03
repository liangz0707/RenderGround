#pragma once
#include "IVulkanGraphicPipeline.h"
class DeferredLightingPipeline :
	public IVulkanGraphicPipeline
{
public:
	virtual void  createGraphicsPipeline(
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule,
		int subpass,
		VkRenderPass renderpass,
		VkPipelineLayout layout);

};

