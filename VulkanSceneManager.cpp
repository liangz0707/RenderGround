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

	vulkanPipelineResource->createIndexBuffer(
		sizeof(vulkanModel->GetIndex()[0])* vulkanModel->GetIndex().size(),
		vulkanModel->GetIndex().data(),
		vkIndexBuffer, vkIndexDeviceMemory);

	vulkanPipelineResource->createVertexBuffer(
		sizeof(vulkanModel->GetVertex()[0]) * vulkanModel->GetVertex().size(),
		vulkanModel->GetVertex().data(),
		vkVertexBuffer, vkVertexDeviceMemory);

	vulkanRModel->SetVertexBuffer(vkVertexBuffer);
	vulkanRModel->SetIndexBuffer(vkIndexBuffer);
	vulkanRModel->SetIndexSize(vulkanModel->GetIndex().size());

	vulkanModels.push_back(vulkanRModel);

}

void VulkanSceneManager::unloadRenderModel(VulkanModel* vulkanModel)
{
}
