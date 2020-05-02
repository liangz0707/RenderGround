#pragma once

#include "Common.h"
#include "IVulkanDescriptor.h"
class IVulkanMaterial
{
public:
	virtual std::vector<VkWriteDescriptorSet> GetDescriptorWrite() {}
	virtual std::vector< VkDescriptorSetLayoutBinding> GetDescriptorBinding() {}

};

