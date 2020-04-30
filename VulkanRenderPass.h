#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanFramebuffer.h"

class VulkanFramebuffer;

class VulkanSwapChain;
class IVulkanGraphicPipeline;
	
class IVulkanRenderPass
{
public :
	IVulkanRenderPass();
	virtual VkRenderPass GetInstance();
	virtual void createRenderPass();
	virtual void destroyRenderPass();

protected:
	VkRenderPass renderPass;
};

