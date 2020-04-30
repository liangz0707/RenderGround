#include "VulkanSceneManager.h"
#include "RenderingResourceLocater.h"

VulkanSceneManager::VulkanSceneManager()
{

}

VulkanMaterial* VulkanSceneManager::loadMaterial()
{
	VkDeviceSize size = sizeof(PreEntityUniformBufferObject);
	VulkanMaterial *material = new VulkanMaterial();
	VkBuffer preEntityUniformBuffer;
	VkDeviceMemory preEntityUniformBufferMemory;

	material->CreatePreUniformBuffer(
		size,
		preEntityUniformBuffer,
		preEntityUniformBufferMemory
		);

	VkDescriptorSet vkDescriptorSet =
		material->CreateObjectDescriptorSet(
			preEntityUniformBuffer,
			RenderingResourceLocater::get_sampler()->GetInstance(),
			GetTextureByIndex(0)->GetImageView(), 
			RenderingResourceLocater::get_layout()->GetEntityDescriptorSetLayout(),
			RenderingResourceLocater::get_layout()->GetEntityDescriptorPool()
		);
		
	material->SetDescriptorSet(vkDescriptorSet);
	material->SetDescriptorSetBuffer(preEntityUniformBuffer);
	material->SetDescriptorSetBufferMemory(preEntityUniformBufferMemory);

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	void* data;
	RM->mapMemory(
		material->GetDescriptorSetBufferMemory(),
		sizeof(PreEntityUniformBufferObject),
		&data);

	memcpy(
		data,
		&preEntityUniformBufferMemory,
		sizeof(PreEntityUniformBufferObject));

	RM->unMapMemory(
		preEntityUniformBufferMemory);

	vulkanMaterials.push_back(material);
	return material;
}

VulkanRModel* VulkanSceneManager::loadRenderModel(VulkanModel* vulkanModel)
{
	VulkanRModel* vulkanRModel = new VulkanRModel();

	VkBuffer vkIndexBuffer;
	VkDeviceMemory vkIndexDeviceMemory;

	VkBuffer vkVertexBuffer;
	VkDeviceMemory vkVertexDeviceMemory;

	vulkanRModel->createIndexBuffer(
		sizeof(vulkanModel->GetIndex()[0])* vulkanModel->GetIndex().size(),
		vulkanModel->GetIndex().data(),
		vkIndexBuffer, vkIndexDeviceMemory);

	vulkanRModel->createVertexBuffer(
		sizeof(vulkanModel->GetVertex()[0]) * vulkanModel->GetVertex().size(),
		vulkanModel->GetVertex().data(),
		vkVertexBuffer, vkVertexDeviceMemory);

	vulkanRModel->SetIndexSize(vulkanModel->GetIndex().size());
	vulkanRModel->SetVertexBuffer(vkVertexBuffer);
	vulkanRModel->SetIndexBuffer(vkIndexBuffer);
	vulkanRModel->SetVertexBufferMemory(vkVertexDeviceMemory);
	vulkanRModel->SetIndexBufferMemory(vkIndexDeviceMemory);

	vulkanModels.push_back(vulkanRModel);
	return vulkanRModel;
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

	RUtility::createTextureImage(
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

void VulkanSceneManager::updateModel()
{
	for (auto model : vulkanModels)
	{
		model->Update();
	}
}

void VulkanSceneManager::updateMaterial()
{
	for (auto material : vulkanMaterials)
	{
		//material->Update();
	}
}
