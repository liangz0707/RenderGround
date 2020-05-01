#include "VulkanGraphicPipeline.h"
IVulkanGraphicPipeline::IVulkanGraphicPipeline()
{
}

void IVulkanGraphicPipeline::createGraphicsPipeline(
	VkShaderModule vertShaderModule, 
	VkShaderModule fragShaderModule)
{

}

void IVulkanGraphicPipeline::destroyGraphicPipeline()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	vkDestroyPipeline(vkDevice, graphicsPipeline, nullptr);
}

