#include "VulkanRModel.h"

VulkanRModel::VulkanRModel()
{
	ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void VulkanRModel::Update()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};

	ubo.model = glm::rotate(glm::mat4(1.0f), time*glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

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