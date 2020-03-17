#include "VulkanRModel.h"

VulkanRModel::VulkanRModel()
{
	ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}
