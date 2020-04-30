#pragma once
#include "VulkanRenderPass.h"
class DeferredRenderPass :
	public IVulkanRenderPass
{
public:
	DeferredRenderPass();
	virtual void createRenderPass();
};

