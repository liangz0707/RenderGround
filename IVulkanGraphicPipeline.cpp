#include "IVulkanGraphicPipeline.h"
IVulkanGraphicPipeline::IVulkanGraphicPipeline()
{
}

void IVulkanGraphicPipeline::createGraphicsPipeline(
	VkShaderModule vertShaderModule, 
	VkShaderModule fragShaderModule,
	int subpass,
	VkRenderPass renderpass,
	VkPipelineLayout layout)
{

}

void IVulkanGraphicPipeline::destroyGraphicPipeline()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	vkDestroyPipeline(vkDevice, graphicsPipeline, nullptr);
}

