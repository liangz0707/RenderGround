#include "RenderGround.h"

RenderGround::RenderGround()
{
}

void RenderGround::run()
{
	VulkanApplication* vulkanApplication = new VulkanApplication();
	vulkanApplication->initWindow();
	vulkanApplication->createInstance();
	vulkanApplication->createSurface();
	vulkanApplication->pickPhysicalDevice();

	VulkanDevice* vulkanDevice = new VulkanDevice(vulkanApplication);
	vulkanDevice->createLogicalDevice();

	VulkanSwapChain* vulkanSwapChain = new VulkanSwapChain(vulkanApplication, vulkanDevice);

	vulkanSwapChain->createSwapChain();
	vulkanSwapChain->createSwapChainImageViews();

	VulkanResourceManager::SetResourceManager(vulkanDevice,
		vulkanApplication, vulkanSwapChain);
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	VulkanFramebuffer* vulkanFrameBuffer = new VulkanFramebuffer();

	vulkanFrameBuffer->createDepthResource(vulkanSwapChain);
	vulkanFrameBuffer->createSwapChainFrameBuffers(vulkanSwapChain);

	RM->SetFramebuffer(vulkanFrameBuffer);
	RM->createCommandPool();
	RM->createSyncObjects();

	VulkanPipelineResource* vulkanPipelineResource = new VulkanPipelineResource();
	vulkanPipelineResource->createUniformBuffers(sizeof(UniformBufferObject));
	vulkanPipelineResource->createPreUniformBuffers(sizeof(UniformBufferObject));

	VulkanRenderPass* vulkanRenderPass = new VulkanRenderPass(vulkanPipelineResource);

	vulkanRenderPass->createRenderPass();
	vulkanRenderPass->createDescriptorSetLayout();
	vulkanRenderPass->createDescriptorPool();
	vulkanRenderPass->createDescriptorSets(
		vulkanFrameBuffer,
		vulkanPipelineResource->GetUniformBuffers(),
		vulkanPipelineResource->GetPreEntityUniformBuffers());
	vulkanRenderPass->createGraphicPipelines();


	VulkanModel* vulkanModel = new VulkanModel();
	VulkanSceneManager* vulkanSceneManager = new VulkanSceneManager();
	vulkanSceneManager->loadRenderModel(vulkanModel);

	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();

	VulkanFrameRenderCommandBuffer* vulkanCommandBuffer = new VulkanFrameRenderCommandBuffer(RM->GetCommandPool(), commandBufferSize);

	VulkanTestRendering* vulkanRendering = new VulkanTestRendering(vulkanRenderPass);
	vulkanRendering->SetSceneManager(vulkanSceneManager);
	vulkanRendering->Config(vulkanCommandBuffer);
	while (!glfwWindowShouldClose(vulkanApplication->GetWindow()))
	{
		glfwPollEvents();
		drawFrame();
	}

	//clean();

	vkDeviceWaitIdle(vulkanDevice->GetInstance());
}

void RenderGround::drawFrame()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	//imageAvailableSemaphores在图像获取完成之后会变成signaled状态。，也是图像可以开始绘制的时间点。
	//最后一个参数用来获取swapchain当中的image，用于指定对应的commandbuffer
	VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		//recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	// Mark the image as now being in use by this frame
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	updateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	// 等待获取图像完成的信号。
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	// 在何时进入等待状态。
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	//manually need to restore the fence to the unsignaled state by resetting it with the vkResetFences call.
	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(presentQueue, &presentInfo);
	vkQueueWaitIdle(presentQueue);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	
}

void RenderGround::mainLoop() {




}


