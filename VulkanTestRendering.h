#pragma once

#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanPipelineResource.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanSceneManager.h"
class VulkanSceneManager;
class VulkanTestRendering
{
public:
	VulkanTestRendering();
	void Render(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, VkExtent2D extent, VkDescriptorSet descriptorSet);

private:
	VulkanSceneManager* vulkanSceneManager;
	VulkanFrameRenderCommandBuffer* vulkanCommandBuffer;
	VulkanPipelineResource* vulkanPipelineResource;
	VulkanRenderPass* vulkanRenderPass;
};

