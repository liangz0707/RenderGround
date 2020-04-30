#pragma once

#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"

#include "VulkanSceneManager.h"

class IVulkanRendering
{
public:
	virtual void Render(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, VkExtent2D extent, VkDescriptorSet descriptorSet) = 0;
	virtual void Config(VulkanFrameRenderCommandBuffer* vulkanCommandBuffer) = 0;
};

