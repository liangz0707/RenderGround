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
	vulkanRModel->SetDescriptorSetBuffer(preEntityUniformBuffer);
	vulkanRModel->SetDescriptorSetBufferMemory(preEntityUniformBufferMemory);
	vulkanRModel->SetVertexBuffer(vkVertexBuffer);
	vulkanRModel->SetIndexBuffer(vkIndexBuffer);
	vulkanRModel->SetVertexBufferMemory(vkVertexDeviceMemory);
	vulkanRModel->SetIndexBufferMemory(vkIndexDeviceMemory);
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
	VkImageView vkTextureImageView;
	VkDeviceMemory vkTextureImageDeviceMemory;
	VulkanRTexture* vulkanRTexture;

	vulkanPipelineResource->createTextureImage(
		vulkanTexture->GetData(),
		vulkanTexture->GetWidth(),
		vulkanTexture->GetHeight(),
		vulkanTexture->GetChannel(),
		vkTextureImage,
		vkTextureImageDeviceMemory);
	
	vkTextureImageView =RM->createImageView(
		vkTextureImage, 
		VK_FORMAT_R8G8B8A8_SRGB, 
		VK_IMAGE_ASPECT_COLOR_BIT);

	vulkanRTexture = new VulkanRTexture(
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

void VulkanSceneManager::unloadModels()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	for (auto model : vulkanModels)
	{
		RM->destroyBuffer(model->GetIndexBuffer());
		RM->destroyBuffer(model->GetVertexBuffer());
		RM->destroyBuffer(model->GetDescriptorSetBuffer());
		RM->freeMemory(model->GetDescriptorSetBufferMemory());
		RM->freeMemory(model->GetIndexBufferMemory());
		RM->freeMemory(model->GetVertexBufferMemory());
	}
	vulkanTextures.clear();
}

void VulkanSceneManager::unloadTextures()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	for (auto texture : vulkanTextures)
	{
		vkDestroyImage(vkDevice, texture->GetImage(),nullptr);
		vkDestroyImageView(vkDevice, texture->GetImageView(), nullptr);
		vkFreeMemory(vkDevice, texture->GetTextureImageMemory(), nullptr);
	}
	vulkanTextures.clear();

}


void VulkanSceneManager::UpdateModel()
{

}