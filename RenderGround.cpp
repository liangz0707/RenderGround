#include "RenderGround.h"

RenderGround::RenderGround()
{

}

void RenderGround::run()
{
	vulkanApplication = new VulkanApplication();
	vulkanApplication->createWindow();
	vulkanApplication->createInstance();
	vulkanApplication->createSurface();
	vulkanApplication->pickPhysicalDevice();

	VulkanDevice* vulkanDevice = new VulkanDevice(vulkanApplication);
	vulkanDevice->createLogicalDevice();

	VulkanResourceManager::SetResourceManager(vulkanDevice, vulkanApplication);
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	RM->createCommandPool();
	RM->CreateRenderState();

	VulkanSwapChain* vulkanSwapChain = new VulkanSwapChain();
	vulkanSwapChain->createSwapChain();
	vulkanSwapChain->createSwapChainImageViews();
	RM->SetSwapChain(vulkanSwapChain);

	vulkanPipelineResource = new VulkanPipelineResource();
	vulkanPipelineResource->createTextureSampler();
	vulkanPipelineResource->createUniformBuffers(sizeof(UniformBufferObject));
	vulkanPipelineResource->createObjectDescriptorSetLayout();
	vulkanPipelineResource->createObjectDescriptorPool();

	vulkanRenderPass = new VulkanRenderPass();
	vulkanRenderPass->createRenderPass();
	vulkanRenderPass->createUniformDescriptorSetLayout();
	vulkanRenderPass->createUniformDescriptorPool();
	vulkanRenderPass->createUniformDescriptorSets(
		vulkanPipelineResource->GetUniformBuffers());
	vulkanRenderPass->createGraphicPipelines(vulkanPipelineResource);

	VulkanModel* vulkanModel = new VulkanModel();
	VulkanTexture* vulkanTexture = new VulkanTexture();
	vulkanSceneManager = new VulkanSceneManager();
	vulkanSceneManager->SetPipelineResource(vulkanPipelineResource);
	vulkanSceneManager->loadTexture(vulkanTexture);
	vulkanSceneManager->loadRenderModel(vulkanModel);

	VulkanFramebuffer* vulkanFrameBuffer = new VulkanFramebuffer();
	vulkanFrameBuffer->createDepthResource(vulkanSwapChain);
	vulkanFrameBuffer->createSwapChainFrameBuffers(vulkanSwapChain, vulkanRenderPass);
	RM->SetFramebuffer(vulkanFrameBuffer);

	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();
	vulkanCommandBuffer = new VulkanFrameRenderCommandBuffer();
	vulkanCommandBuffer->createCommandBuffer(RM->GetCommandPool(), commandBufferSize);

	VulkanTestRendering* vulkanRendering = new VulkanTestRendering(vulkanRenderPass);
	vulkanRendering->SetSceneManager(vulkanSceneManager);
	vulkanRendering->Config(vulkanCommandBuffer);

	RM->CreateSync();

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
	
	//vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	RM->WaitForFences();


	uint32_t imageIndex;
	//VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
	VkResult vkResult = RM->AcquireNextImageKHR(imageIndex);
	if (vkResult == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return ;
	}
	else if (vkResult != VK_SUCCESS && vkResult != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	/*createGraphicsPipeline
	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	// Mark the image as now being in use by this frame
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];
	*/
	RM->CheckPrivousFrameFinishend(imageIndex);

	vulkanPipelineResource->updateUniformBuffer(imageIndex);
	vulkanPipelineResource->updateTexture(imageIndex);

	//manually need to restore the fence to the unsignaled state by resetting it with the vkResetFences call.
	//vkResetFences(device, 1, &inFlightFences[currentFrame]);
	RM->ResetFence();

	RM->GraphicQueueSubmit(vulkanCommandBuffer->GetCommandBufferByIndex(imageIndex));


	vkResult = RM->PresentQueueSubmit(imageIndex);

	int framebufferResized = false;
	if (vkResult == VK_ERROR_OUT_OF_DATE_KHR || vkResult == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapChain();
	}
	else if (vkResult != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	RM->UpdateRenderState();

}

void RenderGround::cleanupSwapChain() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	/*
	vkDestroyImageView(device, depthImageView, nullptr);
	vkDestroyImage(device, depthImage, nullptr);
	vkFreeMemory(device, depthImageMemory, nullptr);
	*/
	RM->GetFramebuffer()->destroyDepthResource();

	/*
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
	}
	*/
	RM->GetFramebuffer()->destroySwapChainFrameBuffers();

	/*
	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	*/
	vulkanCommandBuffer->destroyCommandBuffer();

	/*
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	*/
	vulkanRenderPass->destroyGraphicPipelines();

	//vkDestroyRenderPass(device, renderPass, nullptr);
	vulkanRenderPass->destroyRenderPass();

	/*
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		vkDestroyImageView(device, swapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(device, swapChain, nullptr);
	*/
	RM->GetSwapChain()->destroySwapChainImageViews();
	RM->GetSwapChain()->destroySwapChain();

	/*
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
		vkDestroyBuffer(device, preEntityUniformBuffers[i], nullptr);
		vkFreeMemory(device, preEntityUniformBuffersMemory[i], nullptr);
	}
	*/
	vulkanPipelineResource->destroyUniformBuffers();

	//vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	vulkanRenderPass->createUniformDescriptorPool();
}

void RenderGround::cleanup()
{
	cleanupSwapChain();


	vulkanApplication->destroySurface();
	vulkanApplication->destroyInstance();
	vulkanApplication->destroyWindow();
}

void RenderGround::recreateSwapChain()
{
	/*
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	int width = 0, height = 0;
	glfwGetFramebufferSize(RM->GetApplication()->GetWindow(), &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(RM->GetApplication()->GetWindow(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(RM->GetDevice()->GetInstance());

	cleanupSwapChain();

	createSwapChain();
	//The image views need to be recreated because they are based directly on the swap chain images. 
	createImageViews();
	//The render pass needs to be recreated because it depends on the format of the swap chain images
	createRenderPass();
	//Viewport and scissor rectangle size is specified during graphics pipeline creation
	//It is possible to avoid this by using dynamic state for the viewports and scissor rectangles. 
	createGraphicsPipeline();
	createDepthResources();
	//Finally, the framebuffers and command buffers also directly depend on the swap chain images.
	createFramebuffers();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	*/
}


void RenderGround::mainLoop() {




}


