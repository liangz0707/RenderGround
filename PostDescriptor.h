#pragma once
#include "IVulkanDescriptor.h"
class PostDescriptor :
	public IVulkanDescriptor
{
public:
	virtual void createDescriptorSetLayout();
	virtual void createDescriptorPool();
};

