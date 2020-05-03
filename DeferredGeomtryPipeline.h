#pragma once
#include "IVulkanGraphicPipeline.h"
class DeferredGeomtryPipeline :
	public IVulkanGraphicPipeline
{
public:
	DeferredGeomtryPipeline();
	virtual void  createGraphicsPipeline(
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule,
		int subpass,
		VkRenderPass renderpass,
		VkPipelineLayout layout);

};

