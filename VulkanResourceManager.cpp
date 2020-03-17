#include "VulkanResourceManager.h"

VulkanResourceManager* VulkanResourceManager::vulkanResourceManager = NULL;

VulkanResourceManager::VulkanResourceManager(VulkanDevice* vulkanDevice,
	VulkanApplication* vulkanInstance)
{
	this->vulkanDevice = vulkanDevice;
	this->vulkanInstance = vulkanInstance;
}

VkFormat VulkanResourceManager::findDepthFormat() {
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

VkExtent2D VulkanResourceManager::GetExtent()
{
	return vulkanSwapChain->GetSwapChainImageExtent();
}

void VulkanResourceManager::SetFramebuffer(VulkanFramebuffer* vulkanFramebuffer)
{
	this->vulkanFramebuffer = vulkanFramebuffer;
}

void VulkanResourceManager::SetSwapChain(VulkanSwapChain* vulkanSwapChain)
{
	this->vulkanSwapChain = vulkanSwapChain;

}

VulkanFramebuffer* VulkanResourceManager::GetFramebuffer()
{
	return vulkanFramebuffer;
}

VulkanSwapChain* VulkanResourceManager::GetSwapChain()
{
	return vulkanSwapChain;
}

QueueFamilyIndices VulkanResourceManager::findQueueFamilies() {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(vulkanInstance->GetVkPhysicalDevice(), &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(vulkanInstance->GetVkPhysicalDevice(), &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		// 可以提交图形渲染数据
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		// 可以提交绘制到Surface命令
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(vulkanInstance->GetVkPhysicalDevice(), i, vulkanInstance->GetVkSurfaceKHR(), &presentSupport);
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

void VulkanResourceManager::mapMemory(VkDeviceMemory memory, VkDeviceSize size, void** data)
{
	vkMapMemory(vulkanDevice->GetInstance(), memory, 0, size, 0, data);
}

void VulkanResourceManager::unMapMemory(VkDeviceMemory memory)
{
	vkUnmapMemory(vulkanDevice->GetInstance(), memory);
}

VkSemaphore VulkanResourceManager::CreateSemaphore(VkSemaphoreCreateInfo* vkSemaphoreCreateInfo)
{
	VkSemaphore vkSemaphore;

	if (vkCreateSemaphore(vulkanDevice->GetInstance(), vkSemaphoreCreateInfo, nullptr, &vkSemaphore) != VK_SUCCESS) {
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}
	return vkSemaphore;
}


VkShaderModule VulkanResourceManager::createShaderModule(const std::vector<char>& code) {
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

VkFence VulkanResourceManager::CreateFence(VkFenceCreateInfo* vkFenceCreateInfo)
{
	VkFence vkFence;

	if(vkCreateFence(vulkanDevice->GetInstance(), vkFenceCreateInfo, nullptr, &vkFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}
		
	return vkFence;

}

void VulkanResourceManager::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies();

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = 0; // Optional
	if (vkCreateCommandPool(vulkanDevice->GetInstance(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void VulkanResourceManager::DestroyCommandPool()
{
	vkDestroyCommandPool(vulkanDevice->GetInstance(), commandPool, nullptr);
}

VkCommandPool VulkanResourceManager::GetCommandPool()
{
	return commandPool;
}

VkFormat VulkanResourceManager::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(vulkanInstance->GetVkPhysicalDevice(), format, &props);
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	throw std::runtime_error("failed to find supported format!");
}

void VulkanResourceManager::createBuffer(VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkBuffer& buffer,
	VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(vulkanDevice->GetInstance(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(vulkanDevice->GetInstance(), buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Utility::findMemoryType(vulkanInstance->GetVkPhysicalDevice(), memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(vulkanDevice->GetInstance(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(vulkanDevice->GetInstance(), buffer, bufferMemory, 0);
}

void VulkanResourceManager::createImage(uint32_t width, uint32_t height,
	VkFormat format,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkImage& image,
	VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(vulkanDevice->GetInstance(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(vulkanDevice->GetInstance(), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Utility::findMemoryType(vulkanInstance->GetVkPhysicalDevice(), memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(vulkanDevice->GetInstance(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(vulkanDevice->GetInstance(), image, imageMemory, 0);
}

void VulkanResourceManager::destroyImage(VkImage image)
{
	vkDestroyImage(vulkanDevice->GetInstance(), image, nullptr);
}

void VulkanResourceManager::allocCommandBuffer(VkCommandBufferAllocateInfo* allocInfo, VkCommandBuffer* commandBuffer)
{
	vkAllocateCommandBuffers(vulkanDevice->GetInstance(), allocInfo, commandBuffer);
}

void VulkanResourceManager::destroyBuffer(VkBuffer buffer)
{
	vkDestroyBuffer(vulkanDevice->GetInstance(), buffer, nullptr);
}

void VulkanResourceManager::freeMemory(VkDeviceMemory memory)
{
	vkFreeMemory(vulkanDevice->GetInstance(), memory, nullptr);
}

void VulkanResourceManager::createSampler(
	VkSamplerCreateInfo* samplerInfo, 
	VkSampler& sampler)
{
	if (vkCreateSampler(vulkanDevice->GetInstance(), samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void VulkanResourceManager::createPipelineLayout(VkPipelineLayoutCreateInfo* pipelineLayoutInfo, VkPipelineLayout* pipelineLayout)
{
	if (vkCreatePipelineLayout(vulkanDevice->GetInstance(), pipelineLayoutInfo, nullptr, pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void VulkanResourceManager::createGraphicsPipeline(VkGraphicsPipelineCreateInfo* pipelineLayoutInfo, VkPipeline* pipeline)
{
	if (vkCreateGraphicsPipelines(vulkanDevice->GetInstance(), VK_NULL_HANDLE, 1, pipelineLayoutInfo, nullptr, pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

}

VkFramebuffer VulkanResourceManager::createFramebuffer(VkFramebufferCreateInfo* framebufferInfo)
{
	VkFramebuffer vkFramebuffer;
	if (vkCreateFramebuffer(vulkanDevice->GetInstance(), framebufferInfo, nullptr, &vkFramebuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create framebuffer!");
	}
	return vkFramebuffer;
}

VkImageView VulkanResourceManager::createImageView(VkImage image,
	VkFormat format,
	VkImageAspectFlags flags)
{
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;
	createInfo.subresourceRange.aspectMask = flags;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(vulkanDevice->GetInstance(), &createInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create Image View");

	}
	return imageView;
}

void VulkanResourceManager::destroyImageView(VkImageView imageView)
{
	vkDestroyImageView(vulkanDevice->GetInstance(), imageView, nullptr);
}

void VulkanResourceManager::SyncWaitForFences(VkFence vkFence)
{
	vkWaitForFences(vulkanDevice->GetInstance(), 1, &vkFence, VK_TRUE, UINT64_MAX);

}

void VulkanResourceManager::CreateSync() {

	int maxFrameinFlight = vulkanRenderState->GetMaxFlightFrame();
	// 同步一个Flight内部的
	imageAvailableSemaphores.resize(maxFrameinFlight);
	renderFinishedSemaphores.resize(maxFrameinFlight);
	// 同步不同的FLIGHT之间的
	inFlightFences.resize(maxFrameinFlight);
	imagesInFlight.resize(vulkanFramebuffer->GetFrameBufferSize(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	//by default, fences are created in the unsignaled state. 
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < maxFrameinFlight; i++) {
		imageAvailableSemaphores[i] = CreateSemaphore(&semaphoreInfo);
		renderFinishedSemaphores[i] = CreateSemaphore(&semaphoreInfo);
		inFlightFences[i] = CreateFence(&fenceInfo);
	}
}

void VulkanResourceManager::DestroySync()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(vulkanDevice->GetInstance(), renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(vulkanDevice->GetInstance(), imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(vulkanDevice->GetInstance(), inFlightFences[i], nullptr);
	}
}

void VulkanResourceManager::WaitForFences()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	int currFrame = GetRenderState()->GetCurrentFrame();
	RM->SyncWaitForFences(inFlightFences[currFrame]);
}

VkResult VulkanResourceManager::AcquireNextImageKHR(uint32_t& imageIndex)
{
	int currFrame = vulkanRenderState->GetCurrentFrame();
	VkResult result = vkAcquireNextImageKHR(vulkanDevice->GetInstance(),
		vulkanSwapChain->GetInstance(), 
		UINT64_MAX, 
		imageAvailableSemaphores[currFrame],
		VK_NULL_HANDLE, &imageIndex);

	return result;
}

void  VulkanResourceManager::CheckPrivousFrameFinishend(uint32_t imageIndex)
{

	int currFrame = vulkanRenderState->GetCurrentFrame();

	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(vulkanDevice->GetInstance(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	// Mark the image as now being in use by this frame
	imagesInFlight[imageIndex] = inFlightFences[currFrame];
}

void VulkanResourceManager::ResetFence()
{
	int currFrame = vulkanRenderState->GetCurrentFrame();
	vkResetFences(vulkanDevice->GetInstance(), 1, &inFlightFences[currFrame]);

}

void VulkanResourceManager::GraphicQueueSubmit(VkCommandBuffer vkCommandBuffer)
{
	int currFrame = vulkanRenderState->GetCurrentFrame();
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	// 等待获取图像完成的信号。
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currFrame] };
	// 在何时进入等待状态。
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &vkCommandBuffer;

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(vulkanDevice->GetGraphicQueue(), 1, &submitInfo, inFlightFences[currFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}
}

VkResult VulkanResourceManager::PresentQueueSubmit(uint32_t imageIndex)
{
	VkPresentInfoKHR presentInfo = {};

	int currFrame = vulkanRenderState->GetCurrentFrame();
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currFrame] };

	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { vulkanSwapChain->GetInstance() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	VkResult result = vkQueuePresentKHR(vulkanDevice->GetPresentQueue(), &presentInfo);
	vkQueueWaitIdle(vulkanDevice->GetPresentQueue());

	return result;
}

void VulkanResourceManager::createTextureSampler(VkSamplerCreateInfo *samplerInfo,VkSampler& sampler)
{
	if (vkCreateSampler(vulkanDevice->GetInstance()
		, samplerInfo,
		nullptr, &sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void VulkanResourceManager::UpdateRenderState()
{
	vulkanRenderState->Update();
}