#include "DeferredRenderPass.h"

DeferredRenderPass::DeferredRenderPass()
{
}

void DeferredRenderPass::createRenderPass()
{

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	// Decscriptor Generate

	VkAttachmentDescription gbufferAAttachment = {};
	gbufferAAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
	gbufferAAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	gbufferAAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	gbufferAAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	gbufferAAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	gbufferAAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	gbufferAAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	gbufferAAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription gbufferBAttachment = {};
	gbufferBAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
	gbufferBAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	gbufferBAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	gbufferBAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	gbufferBAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	gbufferBAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	gbufferBAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	gbufferBAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription gbufferCAttachment = {};
	gbufferCAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
	gbufferCAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	gbufferCAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	gbufferCAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	gbufferCAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	gbufferCAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	gbufferCAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	gbufferCAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription gbufferDAttachment = {};
	gbufferDAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
	gbufferDAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	gbufferDAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	gbufferDAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	gbufferDAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	gbufferDAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	gbufferDAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	gbufferDAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription gbufferEAttachment = {};
	gbufferEAttachment.format = VK_FORMAT_B10G11R11_UFLOAT_PACK32;
	gbufferEAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	gbufferEAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	gbufferEAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	gbufferEAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	gbufferEAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	gbufferEAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	gbufferEAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = RM->GetSwapChain()->GetSwapChainImageFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = RM->findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	std::vector<VkAttachmentDescription> attachments = {
		gbufferAAttachment,
		gbufferBAttachment,
		gbufferCAttachment,
		gbufferDAttachment,
		gbufferEAttachment,
		colorAttachment,
		depthAttachment };

	// Subpass Generate


	/* ===================== step1 :Deferred Geometry =================  */
	VkAttachmentReference gbufferReferences[5] = {
		{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL },
		{ 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, 
		{ 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, 
		{ 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, 
		{ 4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, 
	};

	VkAttachmentReference depthGbufferReference = { 6, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	VkSubpassDescription deferredGeomteryPass = {};
	deferredGeomteryPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	deferredGeomteryPass.colorAttachmentCount = 5;
	deferredGeomteryPass.pColorAttachments = gbufferReferences;
	deferredGeomteryPass.pDepthStencilAttachment = &depthGbufferReference;


	/* ===================== step2 :Deferred Lighting =================  */
	VkAttachmentReference lightingColorReferences = { 4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

	VkAttachmentReference inputGbufferReferences[6] = {
	{ 0, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, 
	{ 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, 
	{ 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, 
	{ 3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, 
	{ 6, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL  }, // Depth
	};

	VkSubpassDescription deferredLightingPass = {};
	deferredLightingPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	deferredLightingPass.colorAttachmentCount = 1;
	deferredLightingPass.pColorAttachments = &lightingColorReferences;
	deferredLightingPass.inputAttachmentCount = 5;
	deferredLightingPass.pInputAttachments = inputGbufferReferences;


	/* ===================== step3 : Forward =================  */
	VkAttachmentReference forwardColorReferences = { 4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

	VkAttachmentReference forwardDepthReference = { 6, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };


	VkSubpassDescription forwardPass = {};
	forwardPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	forwardPass.colorAttachmentCount = 1;
	forwardPass.pColorAttachments = &forwardColorReferences;
	forwardPass.pDepthStencilAttachment = &forwardDepthReference;

	/* ===================== step4 : Postpcess =================  */
	VkAttachmentReference postColorReferences = { 4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

	VkAttachmentReference postInputReferences = { 4, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};


	VkSubpassDescription postProcessPass = {};
	postProcessPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	postProcessPass.colorAttachmentCount = 1;
	postProcessPass.pColorAttachments = &postColorReferences;
	postProcessPass.inputAttachmentCount = 1;
	postProcessPass.pInputAttachments = &postInputReferences;

	/* ===================== step5 : ToScreen =================  */
	VkAttachmentReference colorcReferences = { 5, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

	VkAttachmentReference toScreenInputReferences = { 4, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };


	VkSubpassDescription toScreenPass = {};
	toScreenPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	toScreenPass.colorAttachmentCount = 1;
	toScreenPass.pColorAttachments = &colorcReferences;
	toScreenPass.inputAttachmentCount = 1;
	toScreenPass.pInputAttachments = &toScreenInputReferences;



	// Create RenderPass

	std::vector< VkSubpassDescription> subpasses =
	{
		deferredGeomteryPass,
		deferredLightingPass,
		forwardPass,
		postProcessPass,
		toScreenPass
	};

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount =5;
	renderPassInfo.pSubpasses = subpasses.data();

	// Dependency

	VkSubpassDependency beginToGeometryToDependency = {};
	beginToGeometryToDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	beginToGeometryToDependency.dstSubpass = 0;
	beginToGeometryToDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	beginToGeometryToDependency.srcAccessMask = 0;
	beginToGeometryToDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	beginToGeometryToDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency geometryToLightingDependency = {};
	geometryToLightingDependency.srcSubpass = 0;
	geometryToLightingDependency.dstSubpass = 1;
	geometryToLightingDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	geometryToLightingDependency.srcAccessMask = 0;
	geometryToLightingDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	geometryToLightingDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency lightingToFowardDependency = {};
	lightingToFowardDependency.srcSubpass = 1;
	lightingToFowardDependency.dstSubpass = 2;
	lightingToFowardDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	lightingToFowardDependency.srcAccessMask = 0;
	lightingToFowardDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	lightingToFowardDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency forwardToPostDependency = {};
	forwardToPostDependency.srcSubpass = 2;
	forwardToPostDependency.dstSubpass = 3;
	forwardToPostDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	forwardToPostDependency.srcAccessMask = 0;
	forwardToPostDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	forwardToPostDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency postToScreenDependency = {};
	postToScreenDependency.srcSubpass = 3;
	postToScreenDependency.dstSubpass = 4;
	postToScreenDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	postToScreenDependency.srcAccessMask = 0;
	postToScreenDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	postToScreenDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::vector<VkSubpassDependency> dependencys =
	{
		beginToGeometryToDependency,
		geometryToLightingDependency,
		lightingToFowardDependency,
		forwardToPostDependency,
		postToScreenDependency
	};

	renderPassInfo.dependencyCount = 5;
	renderPassInfo.pDependencies = dependencys.data();


	if (vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}
