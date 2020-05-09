#include "RenderGround.h"
#include "RenderingResourceLocater.h"
#include "GlobalRenderData.h"
#include "VulkanSampler.h"
#include "VulkanShaders.h"
#include "RenderingSettingLocater.h"
#include "DeferredPipelineLayouts.h"
#include "ScreenMaterial.h"

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

	/* ===================== Create Descriptor ================= */
	BatchDescriptor* batchDescriptor = new BatchDescriptor();
	batchDescriptor->createDescriptorPool();
	batchDescriptor->createDescriptorSetLayout();

	PostDescriptor* postDescriptor = new PostDescriptor();
	postDescriptor->createDescriptorPool();
	postDescriptor->createDescriptorSetLayout();

	DeferredLightingDescriptor* deferredDescriptor = new DeferredLightingDescriptor();
	deferredDescriptor->createDescriptorPool();
	deferredDescriptor->createDescriptorSetLayout();

	GlobalDescriptor* globalDescriptor = new GlobalDescriptor();
	globalDescriptor->createDescriptorPool();
	globalDescriptor->createDescriptorSetLayout();

	ForwardLightingDescriptor* forwardDescriptor = new ForwardLightingDescriptor();
	forwardDescriptor->createDescriptorPool();
	forwardDescriptor->createDescriptorSetLayout();

	ScreenDescriptor* screenDescriptor = new ScreenDescriptor();
	screenDescriptor->createDescriptorPool();
	screenDescriptor->createDescriptorSetLayout();

	RenderingResourceLocater::provide(batchDescriptor);
	RenderingResourceLocater::provide(postDescriptor);
	RenderingResourceLocater::provide(deferredDescriptor);
	RenderingResourceLocater::provide(globalDescriptor);
	RenderingResourceLocater::provide(forwardDescriptor);
	RenderingResourceLocater::provide(screenDescriptor);

	/* ===================== Create Layout ===================== */

	DeferredPipelineLayouts *deferredPipelineLayouts = new DeferredPipelineLayouts();
	deferredPipelineLayouts->CreatePipelineLayout();

	RenderingResourceLocater::provide(deferredPipelineLayouts);


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

	RenderingResourceLocater::provide(deferredPass);

	/* ===================== Create Shader ======================= */

	deferredGeometryShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/DeferredGeometry_vert.spv", "shaders/Compiled/DeferredGeometry_frag.spv");
	deferredLightingShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/DeferredLighting_vert.spv", "shaders/Compiled/DeferredLighting_frag.spv");
	forwardShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/Forward_vert.spv", "shaders/Compiled/Forward_frag.spv");
	postprocessShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/Postprocessing_vert.spv", "shaders/Compiled/Postprocessing_frag.spv");
	toScreenShader = new VulkanShaders(RM->GetDevice(), "shaders/Compiled/ToScreen_vert.spv", "shaders/Compiled/ToScreen_frag.spv");
	

	/* ===================== Create Pipeline ===================== */

	DeferredGeomtryPipeline* dgp =	new DeferredGeomtryPipeline();
	dgp->createGraphicsPipeline(
		deferredGeometryShader->GetVertexShader(),
		deferredGeometryShader->GetFragmentShader(),
		0,
		deferredPass->GetInstance(),
		deferredPipelineLayouts->GetPipelineLayoutByIndex(0));

	DeferredLightingPipeline* dlp = new DeferredLightingPipeline();
	dlp->createGraphicsPipeline(	
		deferredLightingShader->GetVertexShader(),	
		deferredLightingShader->GetFragmentShader(),
		1,
		deferredPass->GetInstance(),
		deferredPipelineLayouts->GetPipelineLayoutByIndex(1));

	ForwardPipeline* forw =			new ForwardPipeline();
	forw->createGraphicsPipeline(	
		forwardShader->GetVertexShader(),			
		forwardShader->GetFragmentShader(),
		2,
		deferredPass->GetInstance(),
		deferredPipelineLayouts->GetPipelineLayoutByIndex(2));

	PostprocessPipeline* pp =		new PostprocessPipeline();
	pp->createGraphicsPipeline(		
		postprocessShader->GetVertexShader(),		
		postprocessShader->GetFragmentShader(),
		3,
		deferredPass->GetInstance(),
		deferredPipelineLayouts->GetPipelineLayoutByIndex(3));

	ToScreenPipeline* tos =			new ToScreenPipeline();
	tos->createGraphicsPipeline(	
		toScreenShader->GetVertexShader(),			
		toScreenShader->GetFragmentShader(),
		4,
		deferredPass->GetInstance(),
		deferredPipelineLayouts->GetPipelineLayoutByIndex(4));

	RenderingResourceLocater::provide(dgp);
	RenderingResourceLocater::provide(dlp);
	RenderingResourceLocater::provide(forw);
	RenderingResourceLocater::provide(pp);
	RenderingResourceLocater::provide(tos);


	/* ===================== Create FrameBuffer ===================== */
	VulkanFramebuffer* vulkanFrameBuffer = new VulkanFramebuffer();
	vulkanFrameBuffer->createDepthResource();
	vulkanFrameBuffer->createDeferredColorBufferResource();
	vulkanFrameBuffer->createDeferredFrameBuffer(RenderingResourceLocater::get_pass_deferred());
	RM->SetFramebuffer(vulkanFrameBuffer);

	/* ===================== Load Global Assets ===================== */

	vulkanSceneManager = new VulkanSceneManager();
	RenderingResourceLocater::provide(vulkanSceneManager);

	DeferredLightingMaterial* dm = new DeferredLightingMaterial();
	ForwardLightingMaterial* fm = new ForwardLightingMaterial();
	PostMaterial* pm = new PostMaterial();
	GlobalMaterial* gm = new GlobalMaterial();

	ScreenMaterial* pm2 = new ScreenMaterial();
	pm2->CreateDescriptorBuffer();
	pm2->CreateDescriptorSet();

	
	dm->CreateDescriptorBuffer();
	fm->CreateDescriptorBuffer();
	pm->CreateDescriptorBuffer();
	gm->CreateDescriptorBuffer();
	
	gm->CreateDescriptorSet();
	pm->CreateDescriptorSet();
	fm->CreateDescriptorSet();
	dm->CreateDescriptorSet();

	vulkanSceneManager->SetGlobalMaterial(gm);
	vulkanSceneManager->SetForwardMaterial(fm);
	vulkanSceneManager->SetPostMaterial(pm);
	vulkanSceneManager->SetDeferredMaterial(dm);

	RenderingResourceLocater::get_scene_manager()->GetDeferredMaterial()->UpdateDescriptorSet();
	RenderingResourceLocater::get_scene_manager()->GetForwardMaterial()->UpdateDescriptorSet();
	RenderingResourceLocater::get_scene_manager()->GetPostMaterial()->UpdateDescriptorSet();
	RenderingResourceLocater::get_scene_manager()->GetGlobalMaterial()->UpdateDescriptorSet();

	/* ===================== Load Object Assets ===================== */

	VulkanModel* vulkanModel = new VulkanModel();
	VulkanTexture* vulkanTexture = new VulkanTexture();


	vulkanSceneManager->loadTexture(vulkanTexture);

	VulkanRModel* model = vulkanSceneManager->loadRenderModel(vulkanModel);


	VulkanMaterial* material = vulkanSceneManager->loadMaterial();
	model->SetMaterial(material);


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


	RenderingResourceLocater::get_layout()->DestroyPipelineLayout();

	RenderingResourceLocater::get_pipeline_deferred_geometry()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_deferred_lighting()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_forward()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_postprocess()->destroyGraphicPipeline();
	RenderingResourceLocater::get_pipeline_to_screen()->destroyGraphicPipeline();


	RenderingResourceLocater::get_descriptor_global()->destroyDescriptorPool();
	RenderingResourceLocater::get_descriptor_global()->destroyDescriptorSetLayout();

	RenderingResourceLocater::get_descriptor_post()->destroyDescriptorPool();
	RenderingResourceLocater::get_descriptor_post()->destroyDescriptorSetLayout();

	RenderingResourceLocater::get_descriptor_deferred()->destroyDescriptorPool();
	RenderingResourceLocater::get_descriptor_deferred()->destroyDescriptorSetLayout();

	RenderingResourceLocater::get_descriptor_forward()->destroyDescriptorPool();
	RenderingResourceLocater::get_descriptor_forward()->destroyDescriptorSetLayout();

	RenderingResourceLocater::get_pipeline_layouts_deferred()->DestroyPipelineLayout();


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


	RenderingResourceLocater::get_descriptor_batch()->destroyDescriptorPool();
	RenderingResourceLocater::get_descriptor_batch()->destroyDescriptorSetLayout();


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

	/* ===================== Create Descriptor ================= */

	RenderingResourceLocater::get_descriptor_global()->createDescriptorPool();
	RenderingResourceLocater::get_descriptor_global()->createDescriptorSetLayout();

	RenderingResourceLocater::get_descriptor_post()->createDescriptorPool();
	RenderingResourceLocater::get_descriptor_post()->createDescriptorSetLayout();

	RenderingResourceLocater::get_descriptor_deferred()->createDescriptorPool();
	RenderingResourceLocater::get_descriptor_deferred()->createDescriptorSetLayout();

	RenderingResourceLocater::get_descriptor_forward()->createDescriptorPool();
	RenderingResourceLocater::get_descriptor_forward()->createDescriptorSetLayout();

	RenderingResourceLocater::get_pipeline_layouts_deferred()->CreatePipelineLayout();

	/* ===================== Create Material ================= */
	RenderingResourceLocater::get_scene_manager()->GetDeferredMaterial()->CreateDescriptorSet();
	RenderingResourceLocater::get_scene_manager()->GetForwardMaterial()->CreateDescriptorSet();
	RenderingResourceLocater::get_scene_manager()->GetPostMaterial()->CreateDescriptorSet();
	RenderingResourceLocater::get_scene_manager()->GetGlobalMaterial()->CreateDescriptorSet();

	RenderingResourceLocater::get_scene_manager()->GetDeferredMaterial()->UpdateDescriptorSet();
	RenderingResourceLocater::get_scene_manager()->GetForwardMaterial()->UpdateDescriptorSet();
	RenderingResourceLocater::get_scene_manager()->GetPostMaterial()->UpdateDescriptorSet();
	RenderingResourceLocater::get_scene_manager()->GetGlobalMaterial()->UpdateDescriptorSet();

	/* ===================== Create Layout ===================== */

	RenderingResourceLocater::get_layout()->createUniformDescriptorPool();
	RenderingResourceLocater::get_layout()->createUniformDescriptorSetLayout();

	RenderingResourceLocater::get_layout()->createGbufferDescriptorPool();
	RenderingResourceLocater::get_layout()->createGbufferDescriptorSetLayout();

	RenderingResourceLocater::get_layout()->CreatePipelineLayout();

	/* ===================== Create Samplter ===================== */

	/* ===================== Create RenderPass ===================== */
	RenderingResourceLocater::get_pass_deferred()->createRenderPass();

	
	/* ===================== Create Shader ======================= */

	/* ===================== Create Pipeline ===================== */
	RenderingResourceLocater::get_pipeline_deferred_geometry()->createGraphicsPipeline(
		deferredGeometryShader->GetVertexShader(), 
		deferredGeometryShader->GetFragmentShader(),
		0,
		RenderingResourceLocater::get_pass_deferred()->GetInstance(),
		RenderingResourceLocater::get_layout()->GetInstance()
		);
	RenderingResourceLocater::get_pipeline_deferred_lighting()->createGraphicsPipeline(
		deferredLightingShader->GetVertexShader(), 
		deferredLightingShader->GetFragmentShader(),
		1,
		RenderingResourceLocater::get_pass_deferred()->GetInstance(),
		RenderingResourceLocater::get_layout()->GetInstance()
		);
	RenderingResourceLocater::get_pipeline_forward()->createGraphicsPipeline(
		forwardShader->GetVertexShader(), 
		forwardShader->GetFragmentShader(),
		2,
		RenderingResourceLocater::get_pass_deferred()->GetInstance(),
		RenderingResourceLocater::get_layout()->GetInstance()
		);
	RenderingResourceLocater::get_pipeline_postprocess()->createGraphicsPipeline(
		postprocessShader->GetVertexShader(), 
		postprocessShader->GetFragmentShader(),
		3,
		RenderingResourceLocater::get_pass_deferred()->GetInstance(),
		RenderingResourceLocater::get_layout()->GetInstance()
		);
	RenderingResourceLocater::get_pipeline_to_screen()->createGraphicsPipeline(
		toScreenShader->GetVertexShader(),
		toScreenShader->GetFragmentShader(),
		4,
		RenderingResourceLocater::get_pass_deferred()->GetInstance(),
		RenderingResourceLocater::get_layout()->GetInstance()
		);

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


