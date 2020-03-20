#include "VulkanApplication.h"

VulkanApplication ::VulkanApplication(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
}

void VulkanApplication::createInstance()
{
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	/*
	应用程序的信息
	*/
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Render Ground";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

#if defined(_WIN32)

	enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME); 

#elif defined(__ANDROID__)

	enabledExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);

#elif defined(__linux__)

	enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);


#endif

	createInfo.enabledExtensionCount = enabledExtensions.size();
	createInfo.ppEnabledExtensionNames = enabledExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	checkExtension();
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}

void VulkanApplication::destroyInstance()
{
	vkDestroyInstance(instance, nullptr);
}

bool VulkanApplication::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	std::cout << "available layers:" << std::endl;
	for (const auto& layerProperties : availableLayers) {
		std::cout << "\t" << layerProperties.layerName << std::endl;
	}

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

void VulkanApplication::checkExtension()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
#ifndef INFO
	std::cout << "available extensions:" << std::endl;

	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}
#endif // INFO
}



void VulkanApplication::createSurface(HINSTANCE windowInstance,HWND window) {
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.hinstance = windowInstance;
	surfaceCreateInfo.hwnd = window;
	
	VkResult result =
		vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface);
	assert(result == VK_SUCCESS);
}

void VulkanApplication::updateWindowSize()
{

	ResetResizedFlag();
}

QueueFamilyIndices VulkanApplication::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		// 可以提交图形渲染数据
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		// 可以提交绘制到Surface命令
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

bool VulkanApplication::isDeviceSuitable(VkPhysicalDevice device) {

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
	// 检查QueueFamily
	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionsSupported =Utility::checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = Utility::querySwapChainSupport(device,surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void VulkanApplication::pickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// Use an ordered map to automatically sort candidates by increasing score
	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : devices) {
		if (isDeviceSuitable(device))
		{
			int score = rateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}
	}

	// Check if the best candidate is suitable at all
	if (candidates.rbegin()->first > 0) {
		physicalDevice = candidates.rbegin()->second;
	}
	else {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

int VulkanApplication::rateDeviceSuitability(VkPhysicalDevice device) {
	int score = 0;

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	// 检查图形显卡支持特性和图形显卡类型
	bool check = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		deviceFeatures.geometryShader;
	if (check)
		std::cout << deviceProperties.deviceName << std::endl;

	// Discrete GPUs have a significant performance advantage
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	// Application can't function without geometry shaders
	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	return score;
}


void VulkanApplication::destroySurface() {
	vkDestroySurfaceKHR(instance, surface, nullptr);
}