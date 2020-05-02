#pragma once
#include "IVulkanDescriptor.h"
class DeferredLightingDescriptor :
	public IVulkanDescriptor
{
public:
	virtual void createDescriptorSetLayout();
	virtual void createDescriptorPool();
};

