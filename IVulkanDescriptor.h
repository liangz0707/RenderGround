#pragma once
#include "Common.h"
#include "IVulkanMaterial.h"
#include "VulkanResourceManager.h"
class IVulkanDescriptor
{
public:
	IVulkanDescriptor();

	virtual VkDescriptorSet allocDescriptorSet(std::vector<VkWriteDescriptorSet>);
	virtual void freeDescriptorSet(VkDescriptorSet vkDescriptorSet);

	virtual void createDescriptorPool() = 0;
	virtual void destroyDescriptorPool();

	virtual void createDescriptorSetLayout() = 0;
	virtual void destroyDescriptorSetLayout();

	virtual VkDescriptorSetLayout getSetLayout() {
		return setLayout;
	}

	virtual VkDescriptorPool getPool()
	{
		return pool;
	}


protected:
	VkDescriptorPool pool;
	VkDescriptorSetLayout setLayout;
};

