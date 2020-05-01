#include "RenderGround.h"
#include "RenderingResourceLocater.h"
#include "GlobalRenderData.h"
#include "VulkanSampler.h"
#include "VulkanShaders.h"
#include "RenderingSettingLocater.h"

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


	/* ===================== Create SwapChain ===================== */
	VulkanSwapChain* vulkanSwapChain = new VulkanSwapChain();
	vulkanSwapChain->createSwapChain();
	vulkanSwapChain->createSwapChainImageViews();
	RM->SetSwapChain(vulkanSwapChain);

	RenderingSettingLocater::provide(vulkanSwapChain->GetSwapChainImageExtent());
	/* ===================== Create Layout ===================== */

	layout = new PipelineLayout();
	layout->createUniformDescriptorPool();
	layout->createObjectDescriptorPool();
	layout->createUniformDescriptorSetLayout();
	layout->createObjectDescriptorSetLayout();
	layout->CreatePipelineLayout();

	RenderingResourceLocater::provide(layout);


	/* ===================== Create GlobalData ===================== */

	globalData = new GlobalRenderData();
	globalData->createUniformBuffers(sizeof(UniformBufferObject));
	globalData->createUniformDescriptorSets(RenderingResourceLocater::get_layout()->GetUniformDescriptorPool(),
											RenderingResourceLocater::get_layout()->GetUniformDescriptorSetLayout());

	RenderingResourceLocater::provide(globalData);
	

	/* ===================== Create Samplter ===================== */

	sampler = new VulkanSampler();
	sampler->createTextureSampler();
	RenderingResourceLocater::provide(sampler);

	/* ===================== Create RenderPass ===================== */

	deferredPass = new DeferredRenderPass();
	deferredPass->createRenderPass();

	forwardPass = new ForwardRenderPass();
	//forwardPass->createRenderPass();

	RenderingResourceLocater::provide(deferredPass);
	RenderingResourceLocater::provide(forwardPass);

	/* ===================== Create Shader ======================= */

	deferredGeometryShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/DeferredGeometry_vert.spv", "shaders/Compiled/DeferredGeometry_frag.spv");
	deferredLightingShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/DeferredLighting_vert.spv", "shaders/Compiled/DeferredLighting_frag.spv");
	forwardShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/Forward_vert.spv", "shaders/Compiled/Forward_frag.spv");
	postprocessShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/Postprocessing_vert.spv", "shaders/Compiled/Postprocessing_frag.spv");


	/* ===================== Create Pipeline ===================== */

	DeferredGeomtryPipeline* dgp =	new DeferredGeomtryPipeline();
	dgp->createGraphicsPipeline(	deferredGeometryShader->GetVertexShader(),	deferredGeometryShader->GetFragmentShader());
	DeferredLightingPipeline* dlp = new DeferredLightingPipeline();
	dlp->createGraphicsPipeline(	deferredLightingShader->GetVertexShader(),	deferredLightingShader->GetFragmentShader());
	ForwardPipeline* forw =			new ForwardPipeline();
	//forw->createGraphicsPipeline(	forwardShader->GetVertexShader(),			forwardShader->GetFragmentShader());
	PostprocessPipeline* pp =		new PostprocessPipeline();
	//pp->createGraphicsPipeline(		postprocessShader->GetVertexShader(),		postprocessShader->GetFragmentShader());

	RenderingResourceLocater::provide(dgp);
	RenderingResourceLocater::provide(dlp);
	RenderingResourceLocater::provide(forw);
	RenderingResourceLocater::provide(pp);

	/* ===================== Load Assets ===================== */

	VulkanModel* vulkanModel = new VulkanModel();
	VulkanTexture* vulkanTexture = new VulkanTexture();

	vulkanSceneManager = new VulkanSceneManager();
	RenderingResourceLocater::provide(vulkanSceneManager);

	vulkanSceneManager->loadTexture(vulkanTexture);

	VulkanRModel* model = vulkanSceneManager->loadRenderModel(vulkanModel);
	VulkanMaterial * material = vulkanSceneManager->loadMaterial();
	model->SetMaterial(material);
	

	VulkanFramebuffer* vulkanFrameBuffer = new VulkanFramebuffer();
	vulkanFrameBuffer->createDepthResource();
	vulkanFrameBuffer->createDeferredColorBufferResource();
	vulkanFrameBuffer->createDeferredFrameBuffer(RenderingResourceLocater::get_pass_deferred());
	//vulkanFrameBuffer->createSwapChainFrameBuffers(RenderingResourceLocater::get_pass_deferred());
	RM->SetFramebuffer(vulkanFrameBuffer);

	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();
	vulkanCommandBuffer = new VulkanFrameRenderCommandBuffer();
	vulkanCommandBuffer->createCommandBuffer(RM->GetCommandPool(), commandBufferSize);

	vulkanRendering = new VulkanTestRendering();
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

	globalData->updateUniformBuffer(imageIndex);

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


	RenderingResourceLocater::get_pipeline_deferred_geometry()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_deferred_lighting()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_forward()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_postprocess()->destroyGraphicPipeline();

	RenderingResourceLocater::get_pass_forward()->destroyRenderPass();


	RenderingResourceLocater::get_pass_deferred()->destroyRenderPass();
	RenderingResourceLocater::get_pass_forward()->destroyRenderPass();

	RM->GetSwapChain()->destroySwapChainImageViews();
	RM->GetSwapChain()->destroySwapChain();

	globalData->destroyUniformBuffers();

	RenderingResourceLocater::get_layout()->destroyUniformDescriptorPool();
	RenderingResourceLocater::get_layout()->destroyUniformDescriptorSetLayout();

}

void VulkanRenderGround::cleanup()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	vkDeviceWaitIdle(RM->GetDevice()->GetInstance());

	cleanupSwapChain();

	sampler->destroyTextureSampler();


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

	RenderingSettingLocater::provide(RM->GetSwapChain()->GetSwapChainImageExtent());

	RenderingResourceLocater::get_layout()->createUniformDescriptorPool();
	RenderingResourceLocater::get_layout()->createUniformDescriptorSetLayout();
	RenderingResourceLocater::get_layout()->CreatePipelineLayout();

	RenderingResourceLocater::get_global_render_data()->createUniformBuffers(sizeof(UniformBufferObject));
	RenderingResourceLocater::get_global_render_data()->createUniformDescriptorSets(RenderingResourceLocater::get_layout()->GetUniformDescriptorPool(),
		RenderingResourceLocater::get_layout()->GetUniformDescriptorSetLayout());


	RenderingResourceLocater::get_pass_deferred()->createRenderPass();

	
	RenderingResourceLocater::get_pipeline_deferred_geometry()->createGraphicsPipeline(deferredGeometryShader->GetVertexShader(), deferredGeometryShader->GetFragmentShader());
	RenderingResourceLocater::get_pipeline_deferred_lighting()->createGraphicsPipeline(deferredLightingShader->GetVertexShader(), deferredLightingShader->GetFragmentShader());


	/*
	RenderingResourceLocater::get_pipeline_forward()->createGraphicsPipeline();
	RenderingResourceLocater::get_pipeline_postprocess()->createGraphicsPipeline();
	*/

	RM->GetFramebuffer()->createDepthResource();
	RM->GetFramebuffer()->createDeferredColorBufferResource();
	RM->GetFramebuffer()->createDeferredFrameBuffer(RenderingResourceLocater::get_pass_deferred());

	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();
	vulkanCommandBuffer->createCommandBuffer(RM->GetCommandPool(), commandBufferSize);

	vulkanRendering->Config(vulkanCommandBuffer);
}


void VulkanRenderGround::mainLoop() {
}


