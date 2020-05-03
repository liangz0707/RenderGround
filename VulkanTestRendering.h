#pragma once

#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "IVulkanGraphicPipeline.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanSceneManager.h"
class VulkanSceneManager;
class VulkanTestRendering
{
public:
	VulkanTestRendering();
	void Render(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, VkExtent2D extent,int index);
	void Config(VulkanFrameRenderCommandBuffer* vulkanCommandBuffer);

};

