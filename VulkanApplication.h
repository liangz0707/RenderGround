#pragma once
#include "Common.h"
#include "Utility.h"


class VulkanApplication
{

private :
	VkInstance instance;
	QueueFamilyIndices indices;
	VkPhysicalDevice physicalDevice;
	VkSurfaceKHR surface;
	int windowWidth;
	int windowHeight;

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


	int GetWindowHeight()
	{
		return windowHeight;
	}
	int GetWindowWidth()
	{
		return windowWidth;
	}

	VulkanApplication(int,int);
	void createSurface(HINSTANCE windowInstance, HWND window);

	void checkExtension();
	void createInstance();
	void destroyInstance();
	bool checkValidationLayerSupport();
	void destroySurface();
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	int rateDeviceSuitability(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	
};