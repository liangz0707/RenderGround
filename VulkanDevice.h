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
	VkDevice GetDevice()
	{
		return device;
	}
};

