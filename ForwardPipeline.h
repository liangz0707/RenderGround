#pragma once
#include "VulkanGraphicPipeline.h"
class ForwardPipeline :
	public IVulkanGraphicPipeline
{
public:
	virtual void  createGraphicsPipeline(
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule);

};

