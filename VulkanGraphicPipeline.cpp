#include "VulkanGraphicPipeline.h"
IVulkanGraphicPipeline::IVulkanGraphicPipeline()
{
}

void IVulkanGraphicPipeline::createGraphicsPipeline(
	VkShaderModule vertShaderModule, 
	VkShaderModule fragShaderModule,
	VkExtent2D swapChainExtent,
	VkRenderPass renderPass)
{

}

void IVulkanGraphicPipeline::destroyGraphicPipeline()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	vkDestroyPipeline(vkDevice, graphicsPipeline, nullptr);
}

