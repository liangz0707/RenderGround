#pragma once

#include "Common.h"
#include "VulkanCommandBuffer.h"
#include "VulkanResourceManager.h"
#include "VulkanSceneManager.h"
#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"

class VulkanSampler
{
public:
	VulkanSampler();
	void createTextureSampler();
	void destroyTextureSampler();

	VkSampler GetInstance() {
		return textureSampler;
	}
private:
	VkSampler textureSampler;

};

