#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
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

public:
	VulkanApplication* vulkanApplication;

	DeferredRenderPass* deferredPass;
	ForwardRenderPass* forwardPass;

	VulkanFrameRenderCommandBuffer* vulkanCommandBuffer;
	VulkanSync* vulkanSync;
	VulkanSceneManager* vulkanSceneManager;
	VulkanTestRendering* vulkanRendering;

	// 
	PipelineLayout *layout;
	GlobalRenderData* globalData;
	VulkanSampler* sampler;


	VulkanShaders* deferredGeometryShader;
	VulkanShaders* deferredLightingShader;
	VulkanShaders* forwardShader;
	VulkanShaders* postprocessShader;
};

