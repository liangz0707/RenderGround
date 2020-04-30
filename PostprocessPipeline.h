#pragma once
#include "VulkanGraphicPipeline.h"
class PostprocessPipeline :
	public IVulkanGraphicPipeline
{
public:
	virtual void  createGraphicsPipeline(
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule);

};

