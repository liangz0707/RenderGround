#include "IVulkanDescriptor.h"
#include "VulkanResourceManager.h"


IVulkanDescriptor::IVulkanDescriptor()
{
}

void IVulkanDescriptor::destroyDescriptorSetLayout()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	vkDestroyDescriptorSetLayout(vkDevice, setLayout, nullptr);
}

VkDescriptorSet IVulkanDescriptor::allocDescriptorSet(std::vector< VkWriteDescriptorSet> dw)
{
	VkDescriptorSet vkDescriptorSet;
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	VkDescriptorSetLayout layouts = setLayout;
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
	allocInfo.pSetLayouts = &layouts;

	if (vkAllocateDescriptorSets(vkDevice, &allocInfo, &vkDescriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	vkUpdateDescriptorSets(vkDevice, static_cast<uint32_t>(dw.size()), dw
		.data(), 0, nullptr);

	return vkDescriptorSet;
}


void IVulkanDescriptor::destroyDescriptorPool()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	vkDestroyDescriptorPool(vkDevice, pool, nullptr);
}

void IVulkanDescriptor::freeDescriptorSet(VkDescriptorSet vkDescriptorSet)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	if (vkFreeDescriptorSets(vkDevice, pool, 1, &vkDescriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

