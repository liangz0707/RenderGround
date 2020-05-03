#pragma once
#include "IVulkanGraphicPipeline.h"
class PostprocessPipeline :
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

