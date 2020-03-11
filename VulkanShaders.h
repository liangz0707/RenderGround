#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanDevice.h"
class VulkanShader {
	std::string fileName = "";
	VkShaderModule shaderModule;

public:
	VulkanShader(VulkanDevice* vulkanDevice, 
		std::string shaderFileName);

	VkShaderModule GetShaderInstance();
	VkShaderModule createShaderModule(VulkanDevice* vulkanDevice, const std::vector<char>& code);
};

class VulkanShaders
{
	VulkanShader* vertexShader;
	VulkanShader* fragmentShader;
	VulkanDevice* vulkanDevice;

public:
	VulkanShaders(
		VulkanDevice* vulkanDevice,
		std::string VertexShaderFileName,
		std::string FragmentShaderFileName);

	VkShaderModule GetVertexShader();
	VkShaderModule GetFragmentShader();

};

