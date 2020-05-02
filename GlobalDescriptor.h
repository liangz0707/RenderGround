#pragma once
#include "IVulkanDescriptor.h"
class GlobalDescriptor :
	public IVulkanDescriptor
{
public:

	virtual void createDescriptorSetLayout();
	virtual void createDescriptorPool();
};

