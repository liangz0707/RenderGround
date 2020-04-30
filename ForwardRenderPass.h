#pragma once
#include "VulkanRenderPass.h"
class ForwardRenderPass :
	public IVulkanRenderPass
{
public:
	ForwardRenderPass();
	virtual void createRenderPass();
};

