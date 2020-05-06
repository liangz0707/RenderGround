#pragma once
#include "IVulkanDescriptor.h"
class ScreenDescriptor :
	public IVulkanDescriptor
{
	public:
	virtual void createDescriptorSetLayout();
	virtual void createDescriptorPool();
};

