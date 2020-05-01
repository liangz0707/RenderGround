#pragma once
#include "VulkanGraphicPipeline.h"
class ToScreenPipeline :
	public IVulkanGraphicPipeline
{
public:
	virtual void  createGraphicsPipeline(
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule);

};

