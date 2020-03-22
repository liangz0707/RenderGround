#include "VulkanRModel.h"

VulkanRModel::VulkanRModel()
{
}

void VulkanRModel::SetMaterial(VulkanMaterial* material)
{
	this->material = material;
}

void VulkanRModel::Update()
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	material->Update(ubo);
}