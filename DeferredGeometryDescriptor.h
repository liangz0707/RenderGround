#pragma once
#include "IVulkanDescriptor.h"
class DeferredGeometryDescriptor :
	public IVulkanDescriptor
{
public:
	virtual void createDescriptorSetLayout();
	virtual void createDescriptorPool();
};

