#pragma once
#include "VulkanGraphicPipeline.h"
class DeferredGeomtryPipeline :
	public IVulkanGraphicPipeline
{
public:
	DeferredGeomtryPipeline();
	virtual void  createGraphicsPipeline(
		VkShaderModule vertShaderModule,
		VkShaderModule fragShaderModule);

};

