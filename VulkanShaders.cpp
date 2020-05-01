#include "VulkanShaders.h"


VulkanShader::VulkanShader(VulkanDevice* vulkanDevice,std::string shaderFileName)
{
	auto shaderCode = RUtility::readFile(shaderFileName);
	shaderModule = createShaderModule(vulkanDevice,shaderCode);
	fileName = shaderFileName;
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
	if (vkCreateShaderModule(vulkanDevice->GetInstance(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
	return shaderModule;
}

void VulkanShader::destoryShaderModule(VulkanDevice* vulkanDevice) {

	vkDestroyShaderModule(vulkanDevice->GetInstance(), shaderModule, nullptr);

}

VulkanShaders::VulkanShaders(VulkanDevice* vulkanDevice,std::string VertexShaderFileName, std::string FragmentShaderFileName)
{
	vertexShader = new VulkanShader(vulkanDevice,VertexShaderFileName);
	fragmentShader = new VulkanShader(vulkanDevice,FragmentShaderFileName);
}

void VulkanShaders::destoryShaderModules(VulkanDevice* vulkanDevice)
{
	vertexShader->destoryShaderModule(vulkanDevice);
	fragmentShader->destoryShaderModule(vulkanDevice);
}

VkShaderModule VulkanShaders::GetVertexShader()
{
	return vertexShader->GetShaderInstance();
}

VkShaderModule VulkanShaders::GetFragmentShader()
{
	return fragmentShader->GetShaderInstance();
}