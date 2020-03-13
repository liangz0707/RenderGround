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

class VulkanTestRendering;
class VulkanRenderPass;
class VulkanFramebuffer;
class VulkanSceneManager;
class VulkanCommandBuffer;

class RenderGround
{
public:
	RenderGround();
	void run();
	void drawFrame();
	void mainLoop();
};

