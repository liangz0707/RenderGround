#pragma once

#include "Common.h"
#include "IVulkanDescriptor.h"
#include "VulkanResourceManager.h"

class VulkanResourceManager;
class IVulkanMaterial
{
public:
	virtual void CreateDescriptorSet() {};
	virtual void CreateDescriptorBuffer() {};

	virtual VkDescriptorSet GetDescriptorSet();
	virtual void UpdateDescriptorSet();

	virtual void DestroyMaterial();

protected:
	VkDescriptorSet descriptorSet;

	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
};

