#pragma once
#include "IVulkanDescriptor.h"
class BatchDescriptor :
	public IVulkanDescriptor
{
public:
	virtual void createDescriptorSetLayout();
	virtual void createDescriptorPool();
};

