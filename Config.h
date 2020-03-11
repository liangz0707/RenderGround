#pragma once
const int WIDTH = 800;
const int HEIGHT = 600;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
"VK_LAYER_KHRONOS_validation",
 "VK_LAYER_RENDERDOC_Capture"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
