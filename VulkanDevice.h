#pragma once
#include "Common.h"

#include "VulkanApplication.h"

class VulkanDevice
{
private:
	VulkanApplication *vulkanInstance;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
public:
	VulkanDevice(VulkanApplication *vulkanInstance);
	void createLogicalDevice();
	VkQueue GetGraphicQueue() {
		return graphicsQueue;
	}
	VkQueue GetPresentQueue() {
		return presentQueue;
	}
	VkDevice GetInstance()
	{
		return device;
	}
};

