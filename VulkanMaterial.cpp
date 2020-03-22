#include "VulkanMaterial.h"

VulkanMaterial::VulkanMaterial()
{
}

void VulkanMaterial::Update(UniformBufferObject ubo)
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
