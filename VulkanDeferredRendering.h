#pragma once

#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "IVulkanGraphicPipeline.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanSceneManager.h"
class VulkanSceneManager;
class VulkanDeferredRendering
{
public:
	VulkanDeferredRendering();
	virtual void Render(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, VkExtent2D extent, int index);
	virtual void Config(VulkanFrameRenderCommandBuffer* vulkanCommandBuffer);

};

