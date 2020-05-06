#include "ForwardLightingMaterial.h"

ForwardLightingMaterial::ForwardLightingMaterial()
{
}

void ForwardLightingMaterial::CreateDescriptorSet()
{
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(ForwardLigtingBufferObject);

	std::vector<VkWriteDescriptorSet> descriptorWrites = {};
	descriptorWrites.resize(1);
	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	descriptorSet = RenderingResourceLocater::get_descriptor_forward()->allocDescriptorSet(descriptorWrites);

}

void ForwardLightingMaterial::CreateDescriptorBuffer()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	RM->createBuffer(sizeof(ForwardLigtingBufferObject),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		buffer,
		bufferMemory);
}

void ForwardLightingMaterial::UpdateDescriptorSet()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	void* data;
	RM->mapMemory(
		bufferMemory,
		sizeof(ForwardLigtingBufferObject),
		&data);

	memcpy(
		data,
		&(ubo),
		sizeof(ForwardLigtingBufferObject));

	RM->unMapMemory(
		bufferMemory);
}
