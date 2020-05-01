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


	// step1
	VkAttachmentReference colorReferences[5] = {
		{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, // Light, emissive materials write here!
		{ 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, // Light, emissive materials write here!
		{ 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, // Albedo
		{ 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, // Normals
		{ 4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, // Normals
	};

	VkAttachmentReference depthReference = { 6, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	VkSubpassDescription subpass1 = {};
	subpass1.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass1.colorAttachmentCount = 5;
	subpass1.pColorAttachments = colorReferences;
	subpass1.pDepthStencilAttachment = &depthReference;


	VkAttachmentReference inputReferences[6] = {
		{ 0, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, // Position
		{ 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, // Normal
		{ 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, // Position
		{ 3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, // Depth
		{ 4, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, // Depth
		{ 6, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL  }, // Depth
	};

	// step2
	VkAttachmentReference colorcReferences = { 5, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };


	VkSubpassDescription subpass2 = {};
	subpass2.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass2.colorAttachmentCount = 1;
	subpass2.pColorAttachments = &colorcReferences;
	subpass2.inputAttachmentCount = 6;
	subpass2.pInputAttachments = inputReferences;



	// Create RenderPass

	std::vector< VkSubpassDescription> subpass =
	{
		subpass1,
		subpass2
	};

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 2;
	renderPassInfo.pSubpasses = subpass.data();

	// Dependency

	VkSubpassDependency dependency1 = {};
	dependency1.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency1.dstSubpass = 0;
	dependency1.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency1.srcAccessMask = 0;
	dependency1.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency1.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency dependency2 = {};
	dependency2.srcSubpass = 0;
	dependency2.dstSubpass = 1;
	dependency2.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency2.srcAccessMask = 0;
	dependency2.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency2.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


	std::vector<VkSubpassDependency> dependencys =
	{
		dependency1,
		dependency2
	};

	renderPassInfo.dependencyCount = 2;
	renderPassInfo.pDependencies = dependencys.data();


	if (vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}
