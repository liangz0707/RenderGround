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


protected:
	VkDescriptorPool pool;
	VkDescriptorSetLayout setLayout;
};

