#pragma once
#include "VulkanGraphicPipeline.h"
class DeferredLightingPipeline :
	public IVulkanGraphicPipeline
{
public:
	virtual void  createGraphicsPipeline(
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule);

};

