#include "VulkanSceneManager.h"

VulkanSceneManager::VulkanSceneManager()
{

}

void VulkanSceneManager::loadRenderModel(VulkanModel* vulkanModel)
{
	VulkanRModel* vulkanRModel = new VulkanRModel();
	VkBuffer vkIndexBuffer;
	VkDeviceMemory vkIndexDeviceMemory;
	VkBuffer vkVertexBuffer;
	VkDeviceMemory vkVertexDeviceMemory;

	VkBuffer preEntityUniformBuffer;
	VkDeviceMemory preEntityUniformBufferMemory;

	vulkanPipelineResource->createPreUniformBuffer(
		sizeof(PreEntityUniformBufferObject),
		preEntityUniformBuffer,
		preEntityUniformBufferMemory
	);

	VkDescriptorSet vkDescriptorSet = 
		vulkanPipelineResource->createObjectDescriptorSet(
				preEntityUniformBuffer,
				vulkanPipelineResource->GetTextureSampler(),
				GetTextureByIndex(0)->GetImageView()
			);

	vulkanPipelineResource->createIndexBuffer(
		sizeof(vulkanModel->GetIndex()[0])* vulkanModel->GetIndex().size(),
		vulkanModel->GetIndex().data(),
		vkIndexBuffer, vkIndexDeviceMemory);

	vulkanPipelineResource->createVertexBuffer(
		sizeof(vulkanModel->GetVertex()[0]) * vulkanModel->GetVertex().size(),
		vulkanModel->GetVertex().data(),
		vkVertexBuffer, vkVertexDeviceMemory);

	vulkanRModel->SetDescriptorSet(vkDescriptorSet);
	vulkanRModel->SetVertexBuffer(vkVertexBuffer);
	vulkanRModel->SetIndexBuffer(vkIndexBuffer);
	vulkanRModel->SetIndexSize(vulkanModel->GetIndex().size());

	vulkanModels.push_back(vulkanRModel);

}

void VulkanSceneManager::unloadRenderModel(VulkanModel* vulkanModel)
{

}

void VulkanSceneManager::loadTexture(VulkanTexture* vulkanTexture)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkImage vkTextureImage;
	VkDeviceMemory vkTextureImageDeviceMemory;
	vulkanPipelineResource->createTextureImage(
		vulkanTexture->GetData(),
		vulkanTexture->GetWidth(),
		vulkanTexture->GetHeight(),
		vulkanTexture->GetChannel(),
		vkTextureImage,
		vkTextureImageDeviceMemory);
	
	VkImageView vkTextureImageView =RM->createImageView(vkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	VulkanRTexture* vulkanRTexture = new VulkanRTexture(
		vulkanTexture->GetWidth(),
		vulkanTexture->GetHeight(),
		vulkanTexture->GetChannel(),
		vkTextureImage,
		vkTextureImageView,
		vkTextureImageDeviceMemory);
	vulkanTextures.push_back(vulkanRTexture);
}

void VulkanSceneManager::unloadTexture(VulkanTexture* vulkanTexture)
{
}


void VulkanSceneManager::UpdateModel()
{
	/*
		PreEntityUniformBufferObject eubo = {};

	eubo.CameraInfo = glm::vec4(1.0f);
	eubo.ScreenInfo = glm::vec4(0.5f);
	RM->mapMemory(preEntityUniformBuffersMemory[swapChainImageIndex], sizeof(eubo), &data);
	memcpy(data, &eubo, sizeof(eubo));
	RM->unMapMemory(preEntityUniformBuffersMemory[swapChainImageIndex]);
	*/
}