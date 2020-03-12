#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanPipelineResource.h"
#include "VulkanCommandBuffer.h"
#include "VulkanResourceManager.h"

class RenderGround
{
public:
	RenderGround();
	void run();
	void drawFrame();
	void mainLoop();
};

