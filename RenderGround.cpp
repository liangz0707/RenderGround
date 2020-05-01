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

	width = vulkanSwapChain->GetSwapChainImageExtent().width;
	height = vulkanSwapChain->GetSwapChainImageExtent().height;
	RenderingSettingLocater::provide(vulkanSwapChain->GetSwapChainImageExtent());
	/* ===================== Create Layout ===================== */

	layout = new PipelineLayout();
	layout->createUniformDescriptorPool();
	layout->createUniformDescriptorSetLayout();

	layout->createObjectDescriptorPool();
	layout->createObjectDescriptorSetLayout();

	layout->createGbufferDescriptorPool();
	layout->createGbufferDescriptorSetLayout();

	layout->CreatePipelineLayout();

	RenderingResourceLocater::provide(layout);


	/* ===================== Create Samplter ===================== */

	sampler = new VulkanSampler();
	sampler->createTextureSampler();
	RenderingResourceLocater::provide(sampler);

	/* ===================== Create RenderPass ===================== */

	deferredPass = new DeferredRenderPass();
	deferredPass->createRenderPass();

	//forwardPass = new ForwardRenderPass();
	//forwardPass->createRenderPass();

	RenderingResourceLocater::provide(deferredPass);
	RenderingResourceLocater::provide(forwardPass);

	/* ===================== Create Shader ======================= */

	deferredGeometryShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/DeferredGeometry_vert.spv", "shaders/Compiled/DeferredGeometry_frag.spv");
	deferredLightingShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/DeferredLighting_vert.spv", "shaders/Compiled/DeferredLighting_frag.spv");
	forwardShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/Forward_vert.spv", "shaders/Compiled/Forward_frag.spv");
	postprocessShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/Postprocessing_vert.spv", "shaders/Compiled/Postprocessing_frag.spv");
	toScreenShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/ToScreen_vert.spv", "shaders/Compiled/ToScreen_frag.spv");
	

	/* ===================== Create Pipeline ===================== */

	DeferredGeomtryPipeline* dgp =	new DeferredGeomtryPipeline();
	dgp->createGraphicsPipeline(	deferredGeometryShader->GetVertexShader(),	deferredGeometryShader->GetFragmentShader());
	DeferredLightingPipeline* dlp = new DeferredLightingPipeline();
	dlp->createGraphicsPipeline(	deferredLightingShader->GetVertexShader(),	deferredLightingShader->GetFragmentShader());
	ForwardPipeline* forw =			new ForwardPipeline();
	forw->createGraphicsPipeline(	forwardShader->GetVertexShader(),			forwardShader->GetFragmentShader());
	PostprocessPipeline* pp =		new PostprocessPipeline();
	pp->createGraphicsPipeline(		postprocessShader->GetVertexShader(),		postprocessShader->GetFragmentShader());
	ToScreenPipeline* tos =			new ToScreenPipeline();
	tos->createGraphicsPipeline(	toScreenShader->GetVertexShader(),			toScreenShader->GetFragmentShader());

	RenderingResourceLocater::provide(dgp);
	RenderingResourceLocater::provide(dlp);
	RenderingResourceLocater::provide(forw);
	RenderingResourceLocater::provide(pp);
	RenderingResourceLocater::provide(tos);

	/* ===================== Load Assets ===================== */

	VulkanModel* vulkanModel = new VulkanModel();
	VulkanTexture* vulkanTexture = new VulkanTexture();

	vulkanSceneManager = new VulkanSceneManager();
	RenderingResourceLocater::provide(vulkanSceneManager);

	vulkanSceneManager->loadTexture(vulkanTexture);

	VulkanRModel* model = vulkanSceneManager->loadRenderModel(vulkanModel);
	VulkanMaterial * material = vulkanSceneManager->loadMaterial();
	model->SetMaterial(material);
	

	/* ===================== Create FrameBuffer ===================== */
	VulkanFramebuffer* vulkanFrameBuffer = new VulkanFramebuffer();
	vulkanFrameBuffer->createDepthResource();
	vulkanFrameBuffer->createDeferredColorBufferResource();
	vulkanFrameBuffer->createDeferredFrameBuffer(RenderingResourceLocater::get_pass_deferred());
	RM->SetFramebuffer(vulkanFrameBuffer);

	/* ===================== Create GlobalData ===================== */

	globalData = new GlobalRenderData();
	globalData->createUniformBuffers(sizeof(UniformBufferObject));
	globalData->createUniformDescriptorSets(RenderingResourceLocater::get_layout()->GetUniformDescriptorPool(),
		RenderingResourceLocater::get_layout()->GetUniformDescriptorSetLayout());
	globalData->createGbufferDescriptorSet(RenderingResourceLocater::get_layout()->GetGbufferDescriptorPool(),
		RenderingResourceLocater::get_layout()->GetGbufferDescriptorSetLayout());
	RenderingResourceLocater::provide(globalData);

	/* ===================== Create CommandBuffer ===================== */
	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();
	vulkanCommandBuffer = new VulkanFrameRenderCommandBuffer();
	vulkanCommandBuffer->createCommandBuffer(RM->GetCommandPool(), commandBufferSize);

	vulkanRendering = new VulkanDeferredRendering();
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

	
	if (vkResult == VK_ERROR_OUT_OF_DATE_KHR || vkResult == VK_SUBOPTIMAL_KHR || IsResized()) {
		
		recreateSwapChain();
	}
	else if (vkResult != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	RM->UpdateRenderState();

}

void VulkanRenderGround::cleanupSwapChain() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();


	RM->GetFramebuffer()->destroySwapChainFrameBuffers();
	RM->GetFramebuffer()->destroyDepthResource();
	RM->GetFramebuffer()->destroyDeferredColorBufferResource();

	vulkanCommandBuffer->destroyCommandBuffer();


	RenderingResourceLocater::get_layout()->destroyUniformDescriptorPool();
	RenderingResourceLocater::get_layout()->destroyUniformDescriptorSetLayout();
	RenderingResourceLocater::get_layout()->destroyGbufferDescriptorPool();
	RenderingResourceLocater::get_layout()->destroyGbufferDescriptorSetLayout();

	RenderingResourceLocater::get_pipeline_deferred_geometry()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_deferred_lighting()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_forward()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_postprocess()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_to_screen()->destroyGraphicPipeline();


	RenderingResourceLocater::get_pass_deferred()->destroyRenderPass();

	RM->GetSwapChain()->destroySwapChainImageViews();
	RM->GetSwapChain()->destroySwapChain();

	globalData->destroyUniformBuffers();


}

void VulkanRenderGround::cleanup()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	vkDeviceWaitIdle(RM->GetDevice()->GetInstance());

	cleanupSwapChain();

	sampler->destroyTextureSampler();


	vulkanSceneManager->unloadTextures();
	vulkanSceneManager->unloadModels();
	vulkanSceneManager->unloadMaterials();

	deferredGeometryShader->destoryShaderModules(RM->GetDevice());
	deferredLightingShader->destoryShaderModules(RM->GetDevice());
	forwardShader->destoryShaderModules(RM->GetDevice());
	postprocessShader->destoryShaderModules(RM->GetDevice());
	toScreenShader->destoryShaderModules(RM->GetDevice());

	RenderingResourceLocater::get_layout()->destroyObjectDescriptorPool();
	RenderingResourceLocater::get_layout()->destroyObjectDescriptorSetLayout();
	RenderingResourceLocater::get_layout()->DestroyPipelineLayout();

	RM->DestroySync();
	RM->DestroyCommandPool();
	RM->GetDevice()->destroyLogicalDevice();
	
	vulkanApplication->destroySurface();
	vulkanApplication->destroyInstance();
}

void VulkanRenderGround::recreateSwapChain()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	updateWindowSize();

	vkDeviceWaitIdle(RM->GetDevice()->GetInstance());

	cleanupSwapChain();

	/* ===================== Create SwapChain ===================== */

	RM->GetSwapChain()->createSwapChain();
	RM->GetSwapChain()->createSwapChainImageViews();

	width = RM->GetSwapChain()->GetSwapChainImageExtent().width;
	height = RM->GetSwapChain()->GetSwapChainImageExtent().height;

	RenderingSettingLocater::provide(RM->GetSwapChain()->GetSwapChainImageExtent());

	/* ===================== Create Layout ===================== */

	RenderingResourceLocater::get_layout()->createUniformDescriptorPool();
	RenderingResourceLocater::get_layout()->createUniformDescriptorSetLayout();

	RenderingResourceLocater::get_layout()->createGbufferDescriptorPool();
	RenderingResourceLocater::get_layout()->createGbufferDescriptorSetLayout();



	/* ===================== Create Samplter ===================== */

	/* ===================== Create RenderPass ===================== */
	RenderingResourceLocater::get_pass_deferred()->createRenderPass();

	
	/* ===================== Create Shader ======================= */

	/* ===================== Create Pipeline ===================== */
	RenderingResourceLocater::get_pipeline_deferred_geometry()->createGraphicsPipeline(deferredGeometryShader->GetVertexShader(), deferredGeometryShader->GetFragmentShader());
	RenderingResourceLocater::get_pipeline_deferred_lighting()->createGraphicsPipeline(deferredLightingShader->GetVertexShader(), deferredLightingShader->GetFragmentShader());
	RenderingResourceLocater::get_pipeline_forward()->createGraphicsPipeline(forwardShader->GetVertexShader(), forwardShader->GetFragmentShader());
	RenderingResourceLocater::get_pipeline_postprocess()->createGraphicsPipeline(postprocessShader->GetVertexShader(), postprocessShader->GetFragmentShader());
	RenderingResourceLocater::get_pipeline_to_screen()->createGraphicsPipeline(toScreenShader->GetVertexShader(), toScreenShader->GetFragmentShader());

	/* ===================== Create FrameBuffer ===================== */
	RM->GetFramebuffer()->createDepthResource();
	RM->GetFramebuffer()->createDeferredColorBufferResource();
	RM->GetFramebuffer()->createDeferredFrameBuffer(RenderingResourceLocater::get_pass_deferred());

	/* ===================== Create GlobalData ===================== */

	RenderingResourceLocater::get_global_render_data()->createUniformBuffers(sizeof(UniformBufferObject));
	RenderingResourceLocater::get_global_render_data()->createUniformDescriptorSets(
		RenderingResourceLocater::get_layout()->GetUniformDescriptorPool(),
		RenderingResourceLocater::get_layout()->GetUniformDescriptorSetLayout());
	RenderingResourceLocater::get_global_render_data()->createGbufferDescriptorSet(
		RenderingResourceLocater::get_layout()->GetGbufferDescriptorPool(),
		RenderingResourceLocater::get_layout()->GetGbufferDescriptorSetLayout());

	/* ===================== Create CommandBuffer ===================== */

	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();
	vulkanCommandBuffer->createCommandBuffer(RM->GetCommandPool(), commandBufferSize);

	vulkanRendering->Config(vulkanCommandBuffer);
}


void VulkanRenderGround::mainLoop() {
}


