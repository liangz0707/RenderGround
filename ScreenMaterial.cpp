#include "ScreenMaterial.h"
#include "PostMaterial.h"

ScreenMaterial::ScreenMaterial()
{
}

void ScreenMaterial::CreateDescriptorSet()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = VK_WHOLE_SIZE;

	VkDescriptorImageInfo imageInfoGbufferE = {};
	imageInfoGbufferE.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfoGbufferE.imageView = RM->GetFramebuffer()->GetGbufferEImageView();

	std::vector<VkWriteDescriptorSet> descriptorWrites = {};
	descriptorWrites.resize(2);
	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfoGbufferE;
	
	descriptorSet = RenderingResourceLocater::get_descriptor_screen()->allocDescriptorSet(descriptorWrites);

}

void ScreenMaterial::CreateDescriptorBuffer()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	RM->createBuffer(sizeof(ScreenBufferObject),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		buffer,
		bufferMemory);
}

void ScreenMaterial::UpdateDescriptorSet()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	void* data;
	RM->mapMemory(
		bufferMemory,
		sizeof(ScreenBufferObject),
		&data);

	memcpy(
		data,
		&(ubo),
		sizeof(ScreenBufferObject));

	RM->unMapMemory(
		bufferMemory);
}
