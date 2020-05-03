#include "DeferredLightingDescriptor.h"


void DeferredLightingDescriptor::createDescriptorPool()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	std::array<VkDescriptorPoolSize, 2> poolSizes = {};

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(8);

	poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(8);


	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(8);

	if (vkCreateDescriptorPool(vkDevice, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}


void DeferredLightingDescriptor::createDescriptorSetLayout() {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	VkDescriptorSetLayoutBinding gbufferALayoutBinding = {};
	gbufferALayoutBinding.binding = 0;
	gbufferALayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	gbufferALayoutBinding.descriptorCount = 1;
	gbufferALayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding gbufferBLayoutBinding = {};
	gbufferBLayoutBinding.binding = 1;
	gbufferBLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	gbufferBLayoutBinding.descriptorCount = 1;
	gbufferBLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding gbufferCLayoutBinding = {};
	gbufferCLayoutBinding.binding = 2;
	gbufferCLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	gbufferCLayoutBinding.descriptorCount = 1;
	gbufferCLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding gbufferDLayoutBinding = {};
	gbufferDLayoutBinding.binding = 3;
	gbufferDLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	gbufferDLayoutBinding.descriptorCount = 1;
	gbufferDLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding lightingBinding = {};
	lightingBinding.binding = 4;
	lightingBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	lightingBinding.descriptorCount = 1;
	lightingBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 5> bindings = {
		gbufferALayoutBinding,
		gbufferBLayoutBinding,
		gbufferCLayoutBinding,
		gbufferDLayoutBinding,
		lightingBinding
	};

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &setLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}