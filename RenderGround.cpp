#include "RenderGround.h"

RenderGround::RenderGround()
{

}

void RenderGround::run()
{
	vulkanApplication = new VulkanApplication(800, 600);
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

	vulkanRendering = new VulkanTestRendering(vulkanRenderPass);
	vulkanRendering->SetSceneManager(vulkanSceneManager);
	vulkanRendering->Config(vulkanCommandBuffer);

	RM->CreateSync();

	while (!glfwWindowShouldClose(vulkanApplication->GetWindow()))
	{
		glfwPollEvents();
		drawFrame();
	}

	vkDeviceWaitIdle(vulkanDevice->GetInstance());


	cleanup();
}

void RenderGround::drawFrame()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	
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

	
	if (vkResult == VK_ERROR_OUT_OF_DATE_KHR || vkResult == VK_SUBOPTIMAL_KHR || vulkanApplication->IsResized()) {
		
		recreateSwapChain();
	}
	else if (vkResult != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	RM->UpdateRenderState();

}

void RenderGround::cleanupSwapChain() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	RM->GetFramebuffer()->destroyDepthResource();
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
	vulkanRenderPass->destroyUniformDescriptorPool();
}

void RenderGround::cleanup()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	cleanupSwapChain();

	vulkanRenderPass->destroyUniformDescriptorSetLayout();

	vulkanPipelineResource->destroyObjectDescriptorPool();
	vulkanPipelineResource->destroyObjectDescriptorSetLayout();
	vulkanPipelineResource->destroyTextureSampler();

	vulkanSceneManager->unloadTextures();
	vulkanSceneManager->unloadModels();
	
	RM->DestroySync();
	RM->DestroyCommandPool();
	RM->GetDevice()->destroyLogicalDevice();
	
	vulkanApplication->destroySurface();
	vulkanApplication->destroyInstance();
	vulkanApplication->destroyWindow();
}

void RenderGround::recreateSwapChain()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	vulkanApplication->updateWindowSize();

	vkDeviceWaitIdle(RM->GetDevice()->GetInstance());

	cleanupSwapChain();

	RM->GetSwapChain()->createSwapChain();
	RM->GetSwapChain()->createSwapChainImageViews();

	vulkanRenderPass->createRenderPass();
	vulkanPipelineResource->createUniformBuffers(sizeof(UniformBufferObject));

	//vulkanRenderPass->createUniformDescriptorSetLayout();
	vulkanRenderPass->createUniformDescriptorPool();
	vulkanRenderPass->createUniformDescriptorSets(
		vulkanPipelineResource->GetUniformBuffers());
	vulkanRenderPass->createGraphicPipelines(vulkanPipelineResource);

	RM->GetFramebuffer()->createDepthResource(RM->GetSwapChain());
	RM->GetFramebuffer()->createSwapChainFrameBuffers(RM->GetSwapChain(), vulkanRenderPass);

	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();
	vulkanCommandBuffer->createCommandBuffer(RM->GetCommandPool(), commandBufferSize);

	vulkanRendering->Config(vulkanCommandBuffer);
}


void RenderGround::mainLoop() {
}


