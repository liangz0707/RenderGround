#pragma once
#include "Common.h"
class VulkanResource
{
public:
	VulkanResource(VkDeviceSize vkDeviceSize, void* data);
	~VulkanResource();
	void createBuffer();

private:
	VkDeviceMemory vkDeviceMemory;
	VkBuffer vkBuffer;
	void* data;
};

