#pragma once
#include "IVulkanDescriptor.h"
class ForwardLightingDescriptor :
	public IVulkanDescriptor
{
public:
	virtual void createDescriptorSetLayout();
	virtual void createDescriptorPool();
};

