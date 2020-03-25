#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanFramebuffer.h"

class VulkanFramebuffer;

class VulkanSwapChain;
class VulkanGraphicPipeline;
class VulkanPipelineResource;
	
class VulkanRenderPass
{
public :
	VulkanRenderPass();
	VkRenderPass GetInstance();
	
	void createDefaultRenderPass();
	void destroyDefaultRenderPass();

	void createDeferredRenderPass();
	void destroyDeferredRenderPass();

	void createForwardRenderPass();
	void destroyForwardRenderPass();

	void createToScreenPipelines(VulkanPipelineResource *vulkanPipelineResource);
	void destroyToScreenPipelines();

	void createDeferredGeometryPipelines(VulkanPipelineResource* vulkanPipelineResource);
	void destroyDeferredGeometryPipelines();

	void createDeferredLightingPipelines(VulkanPipelineResource* vulkanPipelineResource);
	void destroyDeferredLightingPipelines();

	void createForwardPipelines(VulkanPipelineResource* vulkanPipelineResource);
	void destroyForwardPipelines();

	VkDescriptorSet GetUniformDescriptorSetByIndex(int i);

	VulkanGraphicPipeline *GetGraphicPipeline()
	{
		return vulkanGraphicPipeline;
	}

	VkPipelineLayout GetPipelineLayout();

private:
	VkRenderPass renderPass;
	VulkanGraphicPipeline* vulkanGraphicPipeline;
	std::vector<VkDescriptorSet> uniformDescriptorSets;
};

