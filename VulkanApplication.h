#pragma once
#include "Common.h"
#include "Utility.h"


class VulkanApplication
{

private :
	VkInstance instance;
	GLFWwindow* window;
	QueueFamilyIndices indices;
	VkPhysicalDevice physicalDevice;
	VkSurfaceKHR surface;


	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation",
	 "VK_LAYER_RENDERDOC_Capture"
	};

public:	

	VkPhysicalDevice GetVkPhysicalDevice() {
		return physicalDevice;
	}
	VkInstance GetVkInstance() {
		return instance;
	}
	VkSurfaceKHR GetVkSurfaceKHR() {
		return surface;
	}
	GLFWwindow* GetWindow() {
		return window;
	}

	bool framebufferResized = false;
	VulkanApplication();
	void initWindow();
	void checkExtension();
	void createInstance();
	bool checkValidationLayerSupport();
	void createSurface();
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	int rateDeviceSuitability(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	
};