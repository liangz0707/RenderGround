#pragma once

#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanPipelineResource.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"

class VulkanTestRendering
{
public:
	VulkanTestRendering();
	void Render(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, VkExtent2D extent);

private:
	VulkanFrameRenderCommandBuffer* vulkanCommandBuffer;
	VulkanPipelineResource* vulkanPipelineResource;
	VulkanRenderPass* vulkanRenderPass;
};

