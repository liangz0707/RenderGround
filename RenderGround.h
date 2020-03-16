#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanPipelineResource.h"
#include "VulkanCommandBuffer.h"
#include "VulkanResourceManager.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "VulkanTestRendering.h"
#include "VulkanSceneManager.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSync.h"

class VulkanSync;
class VulkanTestRendering;
class VulkanRenderPass;
class VulkanFramebuffer;
class VulkanSceneManager;
class VulkanFrameRenderCommandBuffer;
class VulkanSingleTimeCommandBuffer;

class RenderGround
{
public:
	RenderGround();
	void run();
	void drawFrame();
	void mainLoop();

public:
	VulkanFrameRenderCommandBuffer* vulkanCommandBuffer;
	VulkanSync* vulkanSync;
	VulkanPipelineResource* vulkanPipelineResource;
	VulkanSceneManager* vulkanSceneManager;
};

