#include "VulkanMaterial.h"

VulkanMaterial::VulkanMaterial()
{
}

void VulkanMaterial::Update(PreEntityUniformBufferObject ubo)
{

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	void* data;
	RM->mapMemory(
		vkDescriptorSetBufferMemory,
		sizeof(PreEntityUniformBufferObject),
		&data);

	memcpy(
		data,
		&(ubo),
		sizeof(PreEntityUniformBufferObject));

	RM->unMapMemory(
		vkDescriptorSetBufferMemory);
}

void VulkanMaterial::CreatePreUniformBuffer(
	VkDeviceSize bufferSize,
	VkBuffer& preObjectBuffer,
	VkDeviceMemory& vertexBufferMemory)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	RM->createBuffer(bufferSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		preObjectBuffer,
		vertexBufferMemory);
}



VkDescriptorSet VulkanMaterial::CreateObjectDescriptorSet(
	VkBuffer preEntityUniformBuffer,
	VkSampler vkTextureSampler,
	VkImageView vkTextureImageView,
	VkDescriptorSetLayout layouts,
	VkDescriptorPool pool)
{
	VkDescriptorSet vkDescriptorSet;
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
	allocInfo.pSetLayouts = &layouts;

	if (vkAllocateDescriptorSets(vkDevice, &allocInfo, &vkDescriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = vkTextureImageView;
	imageInfo.sampler = vkTextureSampler;

	VkDescriptorBufferInfo preBufferInfo = {};
	preBufferInfo.buffer = preEntityUniformBuffer;
	preBufferInfo.offset = 0;
	preBufferInfo.range = VK_WHOLE_SIZE;// sizeof(UniformBufferObject);

	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = vkDescriptorSet;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &imageInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = vkDescriptorSet;//对应SetLayout
	descriptorWrites[1].dstBinding = 1;  //对应SetLayout当中的Bading数字
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pBufferInfo = &preBufferInfo;

	vkUpdateDescriptorSets(vkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

	return vkDescriptorSet;
}