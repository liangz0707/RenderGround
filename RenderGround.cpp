#include "RenderGround.h"

VulkanRenderGround::VulkanRenderGround()
{

}

void VulkanRenderGround::init(HINSTANCE windowInstance, HWND window)
{
	vulkanApplication = new VulkanApplication(800, 600);
	vulkanApplication->createInstance();
	vulkanApplication->createSurface(windowInstance, window);
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
}

void VulkanRenderGround::run()
{
	while (true)
	{
		drawFrame();
	}

}

void VulkanRenderGround::drawFrame()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	
	RM->WaitForFences();

	uint32_t imageIndex;

	VkResult vkResult = RM->AcquireNextImageKHR(imageIndex);
	if (vkResult == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return ;
	}
	else if (vkResult != VK_SUCCESS && vkResult != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	RM->CheckPrivousFrameFinishend(imageIndex);

	vulkanPipelineResource->updateUniformBuffer(imageIndex);
	vulkanPipelineResource->updateTexture(imageIndex);
	vulkanSceneManager->updateModel();


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

void VulkanRenderGround::cleanupSwapChain() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	RM->GetFramebuffer()->destroyDepthResource();
	RM->GetFramebuffer()->destroySwapChainFrameBuffers();

	vulkanCommandBuffer->destroyCommandBuffer();

	vulkanRenderPass->destroyGraphicPipelines();
	vulkanRenderPass->destroyRenderPass();

	RM->GetSwapChain()->destroySwapChainImageViews();
	RM->GetSwapChain()->destroySwapChain();

	vulkanPipelineResource->destroyUniformBuffers();

	vulkanRenderPass->destroyUniformDescriptorPool();
}

void VulkanRenderGround::cleanup()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	vkDeviceWaitIdle(RM->GetDevice()->GetInstance());

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
}

void VulkanRenderGround::recreateSwapChain()
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


void VulkanRenderGround::mainLoop() {
}


