#pragma once

#include "Common.h"

#include "Utility.h"
#include "VulkanApplication.h"
#include "VulkanDevice.h"
#include "VulkanResourceManager.h"


class VulkanSwapChain
{
private:
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

public :
	VulkanSwapChain();
	void createSwapChain();
	void destroySwapChain();


	void createSwapChainImageViews();
	void destroySwapChainImageViews();

	VkSwapchainKHR GetInstance()
	{
		return swapChain;
	}

	VkFormat GetSwapChainImageFormat();
	VkExtent2D GetSwapChainImageExtent();
	size_t GetSwapChainImageSize();
	VkImageView GetSwapChainImageViewByIndex(int index);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	bool isDeviceSuitable(VkPhysicalDevice device);
};

