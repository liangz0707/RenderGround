#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "IVulkanGraphicPipeline.h"
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

