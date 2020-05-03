#include "IVulkanPipelineLayout.h"

void IVulkanPipelineLayout::DestroyPipelineLayout()
{
	for (auto layout : layouts)
	{
		VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
		VkDevice vkDevice = RM->GetDevice()->GetInstance();
		vkDestroyPipelineLayout(vkDevice, layout, nullptr);
	}
}
