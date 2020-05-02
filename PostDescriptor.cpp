#include "PostDescriptor.h"

void PostDescriptor::createDescriptorPool()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	std::array<VkDescriptorPoolSize, 3> poolSizes = {};

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(1 * 10);

	poolSizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(1 * 10);

	poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[2].descriptorCount = static_cast<uint32_t>(1 * 10);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(111);

	if (vkCreateDescriptorPool(vkDevice, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}


void PostDescriptor::createDescriptorSetLayout() {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = RM->GetSwapChain()->GetSwapChainImageSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &setLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}