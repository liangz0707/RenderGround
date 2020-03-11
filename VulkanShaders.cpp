#include "VulkanShaders.h"


VulkanShader::VulkanShader(VulkanDevice* vulkanDevice,std::string shaderFileName)
{
	auto shaderCode = Utility::readFile("vert.spv");
	createShaderModule(vulkanDevice,shaderCode);
}

VkShaderModule VulkanShader::GetShaderInstance()
{
	return shaderModule;
}

VkShaderModule VulkanShader::createShaderModule(VulkanDevice* vulkanDevice, const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(vulkanDevice->GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
	return shaderModule;
}

VulkanShaders::VulkanShaders(VulkanDevice* vulkanDevice,std::string VertexShaderFileName, std::string FragmentShaderFileName)
{
	vertexShader = new VulkanShader(vulkanDevice,VertexShaderFileName);
	fragmentShader = new VulkanShader(vulkanDevice,FragmentShaderFileName);
}

VkShaderModule VulkanShaders::GetVertexShader()
{
	return vertexShader->GetShaderInstance();
}

VkShaderModule VulkanShaders::GetFragmentShader()
{
	return fragmentShader->GetShaderInstance();
}