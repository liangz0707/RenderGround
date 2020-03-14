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

	VulkanResourceManager::SetResourceManager(vulkanDevice, vulkanApplication);

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	VulkanSwapChain* vulkanSwapChain = new VulkanSwapChain(vulkanApplication, vulkanDevice);

	vulkanSwapChain->createSwapChain();
	vulkanSwapChain->createSwapChainImageViews();

	RM->SetSwapChain(vulkanSwapChain);


	VulkanPipelineResource* vulkanPipelineResource = new VulkanPipelineResource();

	VulkanRenderPass* vulkanRenderPass = new VulkanRenderPass(vulkanPipelineResource);

	vulkanRenderPass->createRenderPass();


	VulkanFramebuffer* vulkanFrameBuffer = new VulkanFramebuffer();
	vulkanFrameBuffer->createDepthResource(vulkanSwapChain);
	vulkanFrameBuffer->createSwapChainFrameBuffers(vulkanSwapChain, vulkanRenderPass);

	RM->SetFramebuffer(vulkanFrameBuffer);

	vulkanPipelineResource->createUniformBuffers(sizeof(UniformBufferObject));
	vulkanPipelineResource->createPreUniformBuffers(sizeof(UniformBufferObject));



	RM->createCommandPool();
	RM->CreateRenderState();

	vulkanRenderPass->createDescriptorSetLayout();
	vulkanRenderPass->createDescriptorPool();
	vulkanRenderPass->createDescriptorSets(
		vulkanFrameBuffer,
		vulkanPipelineResource->GetUniformBuffers(),
		vulkanPipelineResource->GetPreEntityUniformBuffers());
	vulkanRenderPass->createGraphicPipelines();


	VulkanModel* vulkanModel = new VulkanModel();
	VulkanSceneManager* vulkanSceneManager = new VulkanSceneManager();
	vulkanSceneManager->SetPipelineResource(vulkanPipelineResource);
	vulkanSceneManager->loadRenderModel(vulkanModel);

	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();

	vulkanCommandBuffer = new VulkanFrameRenderCommandBuffer(RM->GetCommandPool(), commandBufferSize);

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
	imageIndex = RM->AcquireNextImageKHR();

	/*
	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	// Mark the image as now being in use by this frame
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];
	*/
	RM->CheckPrivousFrameFinishend(imageIndex);

	//updateUniformBuffer(imageIndex);

	//manually need to restore the fence to the unsignaled state by resetting it with the vkResetFences call.
	//vkResetFences(device, 1, &inFlightFences[currentFrame]);
	RM->ResetFence();

	RM->GraphicQueueSubmit(vulkanCommandBuffer->GetCommandBufferByIndex(imageIndex));

	RM->PresentQueueSubmit(imageIndex);

	RM->UpdateRenderState();

}

void RenderGround::mainLoop() {




}


