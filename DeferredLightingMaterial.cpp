#include "DeferredLightingMaterial.h"

DeferredLightingMaterial::DeferredLightingMaterial()
{
}

void DeferredLightingMaterial::CreateDescriptorSet()
{
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(DeferredLigtingBufferObject);

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	VkDescriptorImageInfo imageInfoGbufferA = {};
	imageInfoGbufferA.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfoGbufferA.imageView = RM->GetFramebuffer()->GetGbufferAImageView();

	VkDescriptorImageInfo imageInfoGbufferB = {};
	imageInfoGbufferB.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfoGbufferB.imageView = RM->GetFramebuffer()->GetGbufferBImageView();

	VkDescriptorImageInfo imageInfoGbufferC = {};
	imageInfoGbufferC.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfoGbufferC.imageView = RM->GetFramebuffer()->GetGbufferCImageView();

	VkDescriptorImageInfo imageInfoGbufferD = {};
	imageInfoGbufferD.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfoGbufferD.imageView = RM->GetFramebuffer()->GetGbufferDImageView();

	std::vector<VkWriteDescriptorSet> descriptorWrites;
	descriptorWrites.resize(5);

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &imageInfoGbufferA;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfoGbufferB;

	descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[2].dstBinding = 2;
	descriptorWrites[2].dstArrayElement = 0;
	descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	descriptorWrites[2].descriptorCount = 1;
	descriptorWrites[2].pImageInfo = &imageInfoGbufferC;

	descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[3].dstBinding = 3;
	descriptorWrites[3].dstArrayElement = 0;
	descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	descriptorWrites[3].descriptorCount = 1;
	descriptorWrites[3].pImageInfo = &imageInfoGbufferD;

	descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[4].dstBinding = 4;
	descriptorWrites[4].dstArrayElement = 0;
	descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[4].descriptorCount = 1;
	descriptorWrites[4].pBufferInfo = &bufferInfo;

	descriptorSet = RenderingResourceLocater::get_descriptor_deferred()->allocDescriptorSet(descriptorWrites);

}

void DeferredLightingMaterial::CreateDescriptorBuffer()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	RM->createBuffer(sizeof(DeferredLigtingBufferObject),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		buffer,
		bufferMemory);
}

void DeferredLightingMaterial::UpdateDescriptorSet()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	void* data;
	RM->mapMemory(
		bufferMemory,
		sizeof(DeferredLigtingBufferObject),
		&data);

	memcpy(
		data,
		&(ubo),
		sizeof(DeferredLigtingBufferObject));

	RM->unMapMemory(
		bufferMemory);
}
