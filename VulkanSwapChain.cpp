#include "VulkanSwapChain.h"

VulkanSwapChain::VulkanSwapChain()
{

}

VkImageView VulkanSwapChain::GetSwapChainImageViewByIndex(int index)
{
	return swapChainImageViews[index];
}

VkExtent2D VulkanSwapChain::GetSwapChainImageExtent()
{
	return swapChainExtent;
}

VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	std::cout << "PysichalDivice Swap Chain available VK_PRESENT_MODE:" << std::endl;
	for (const auto& availablePresentMode : availablePresentModes)
	{
		std::cout << "\t" << "VK_PRESENT_MODE:" << availablePresentMode << std::endl;
	}

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	std::cout << "PysichalDivice Swap Chain available FORMATE and COLORSPACE:" << std::endl;

	for (const auto& availableFormat : availableFormats) {
		std::cout << "\t" << "FORMATE:" << availableFormat.format << std::endl;
		std::cout << "\t" << "COLORSPACE:" << availableFormat.colorSpace << std::endl;
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}

VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	if (capabilities.currentExtent.width != UINT32_MAX) {

		std::cout << "Swap Chain Size:" << std::endl;
		std::cout << "\t" << "actualExtent:" << capabilities.currentExtent.width << "  " << capabilities.currentExtent.height << std::endl;

		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(RM->GetApplication()->GetWindow(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		std::cout << "Swap Chain Size:" << std::endl;
		std::cout << "\t" << "actualExtent:" << actualExtent.width << "  " << actualExtent.height << std::endl;

		std::cout << "\t" << "min:" << capabilities.minImageExtent.width << "  " << capabilities.minImageExtent.height << std::endl;
		std::cout << "\t" << "max" << capabilities.maxImageExtent.width << "  " << capabilities.maxImageExtent.height << std::endl;
		return actualExtent;
	}
}



void  VulkanSwapChain::createSwapChain() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	SwapChainSupportDetails swapChainSupport = Utility::querySwapChainSupport(RM->GetApplication()->GetVkPhysicalDevice(), RM->GetApplication()->GetVkSurfaceKHR());

	//sRGB and R8G8B8  ColorSpace is Used to HDR
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);

	//
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

	// Size
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	// image Count
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}
	std::cout << "swap chain image size:" << std::endl;
	std::cout << "\t" << "MIN:" << swapChainSupport.capabilities.minImageCount << std::endl;
	std::cout << "\t" << "MAX:" << swapChainSupport.capabilities.maxImageCount << std::endl;

	// Create Info
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = RM->GetApplication()->GetVkSurfaceKHR();

	createInfo.minImageCount = imageCount;
	//SwapChain如果格式是SRGB，那么在写入的时候会自动进行SRGB编码，开放操作。，所以Shader里面需要进行编码操作。
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;

	// Image Usage： we're going to render directly to them
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	// Next, we need to specify how to handle swap chain images that will be used across multiple queue families
	QueueFamilyIndices indices = RM->GetApplication()->findQueueFamilies(RM->GetApplication()->GetVkPhysicalDevice());
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(),indices.presentFamily.value() };

	// An image is owned by one queue family at a time and ownership must be explicitly transfered before using it in another queue family. This option offers the best performance.
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		// 指定需要共用这个SwapChain的queue
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	//Images can be used across multiple queue families without explicit ownership transfers.
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}
	//We can specify that a certain transform should be applied to images in the swap chain if it is supported
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

	//The compositeAlpha field specifies if the alpha channel should be used for blending with other windows in the window system. 
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;//VK_FALSE;
	//如果SwapChain出错或者重新指定大小，那么需要重新创建时需要指定原始的。
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	//
	if (vkCreateSwapchainKHR(RM->GetDevice()->GetInstance(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	//交换链现在已经创建，所以剩下的就是检索其中VkImages的句柄
	//这些映像是由交换链实现创建的，一旦交换链被销毁，它们将被自动清除
	vkGetSwapchainImagesKHR(RM->GetDevice()->GetInstance(), swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(RM->GetDevice()->GetInstance(), swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	std::cout << "Swap chain Images size:" << imageCount << std::endl;

}


void VulkanSwapChain::destroySwapChain()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	vkDestroySwapchainKHR(RM->GetDevice()->GetInstance(), swapChain, nullptr);
}

void VulkanSwapChain::createSwapChainImageViews() {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	swapChainImageViews.resize(swapChainImages.size());
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		swapChainImageViews[i] = RM->createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

void VulkanSwapChain::destroySwapChainImageViews() {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		 RM->destroyImageView(swapChainImageViews[i]);
	}
}


bool VulkanSwapChain::isDeviceSuitable(VkPhysicalDevice device) {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
	// 检查QueueFamily
	QueueFamilyIndices indices = RM->GetApplication()->findQueueFamilies(RM->GetApplication()->GetVkPhysicalDevice());

	bool extensionsSupported = Utility::checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport =Utility:: querySwapChainSupport(device, RM->GetApplication()->GetVkSurfaceKHR());
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

VkFormat VulkanSwapChain::GetSwapChainImageFormat()
{
	return swapChainImageFormat;
}

size_t VulkanSwapChain::GetSwapChainImageSize()
{
	return swapChainImages.size();
}