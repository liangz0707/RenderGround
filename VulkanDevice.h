#pragma once
#include "Common.h"

#include "VulkanApplication.h"

class VulkanApplication;

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
	void destroyLogicalDevice();

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

