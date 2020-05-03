#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "IVulkanGraphicPipeline.h"
#include "VulkanCommandBuffer.h"
#include "VulkanResourceManager.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "VulkanTestRendering.h"
#include "VulkanSceneManager.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSync.h"

#include "InputController.h"
#include "PipelineLayout.h"
#include "DeferredRenderPass.h"
#include "ForwardRenderPass.h"
#include "VulkanShaders.h"
#include "VulkanDeferredRendering.h"

class VulkanSync;
class VulkanTestRendering;
class IVulkanRenderPass;
class VulkanFramebuffer;
class VulkanSceneManager;
class VulkanFrameRenderCommandBuffer;
class VulkanSingleTimeCommandBuffer;

class GlobalRenderData;
class PipelineLayout;
class VulkanSampler;
class RenderingResourceLocater;

class VulkanRenderGround
{
public:
	VulkanRenderGround();
	void init(HINSTANCE windowInstance, HWND window);
	void run();
	void drawFrame();
	void mainLoop();
	void recreateSwapChain();
	void cleanupSwapChain();
	void cleanup();

	void updateWindowSize()
	{

		ResetResizedFlag();

	}

	void ReSize(int w, int h)
	{
		width = w;
		height = h;
		framebufferResized = true;
	}

	int GetHeight()
	{
		return height;
	}
	int GetWidth()
	{
		return width;
	}
	bool IsResized()
	{
		return framebufferResized;
	}

	void ResetResizedFlag()
	{
		framebufferResized = false;
	}


public:
	VulkanApplication* vulkanApplication;

	DeferredRenderPass* deferredPass;
	ForwardRenderPass* forwardPass;

	VulkanFrameRenderCommandBuffer* vulkanCommandBuffer;
	VulkanSync* vulkanSync;
	VulkanSceneManager* vulkanSceneManager;
	VulkanDeferredRendering* vulkanRendering;

	// 
	PipelineLayout *layout;
	GlobalRenderData* globalData;
	VulkanSampler* sampler;


	VulkanShaders* deferredGeometryShader;
	VulkanShaders* deferredLightingShader;
	VulkanShaders* forwardShader;
	VulkanShaders* postprocessShader;
	VulkanShaders* toScreenShader;


	bool framebufferResized = false;
	int height;
	int width;
};

