#pragma once
#include"Common.h"
#include "IVulkanGraphicPipeline.h"

class IVulkanPipelineLayout
{
public:
	virtual void CreatePipelineLayout() = 0;
	virtual VkPipelineLayout GetPipelineLayoutByIndex(int index)
	{
		return layouts[index];
	}

	virtual void DestroyPipelineLayout();
protected:
	std::vector<VkPipelineLayout> layouts;
};

