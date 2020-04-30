#include "ForwardRenderPass.h"


ForwardRenderPass::ForwardRenderPass()
{
}

void ForwardRenderPass::createRenderPass()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	// Decscriptor Generate

	VkAttachmentDescription forAttachment = {};
	forAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
	forAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	forAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	forAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	forAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	forAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	forAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	forAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = RM->GetSwapChain()->GetSwapChainImageFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = RM->findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	std::vector<VkAttachmentDescription> attachments = {
		forAttachment,
		colorAttachment,
		depthAttachment };


	// Subpass Generate

	VkAttachmentReference forAttachmentRef = {};
	forAttachmentRef.attachment = 0;
	forAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 1;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 2;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference forAttachmentRefInput = {};
	forAttachmentRef.attachment = 0;
	forAttachmentRef.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	// step1
	std::vector< VkAttachmentReference> attachmentRef1 =
	{
		forAttachmentRef
	};

	VkSubpassDescription subpass1 = {};
	subpass1.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass1.colorAttachmentCount = 1;
	subpass1.pColorAttachments = attachmentRef1.data();
	subpass1.pDepthStencilAttachment = &depthAttachmentRef;

	// step2
	std::vector< VkAttachmentReference> attachmentRef1Input =
	{
		forAttachmentRefInput
	};

	std::vector< VkAttachmentReference> attachmentRef2 =
	{
		colorAttachmentRef
	};

	VkSubpassDescription subpass2 = {};
	subpass2.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass2.colorAttachmentCount = 1;
	subpass2.pColorAttachments = attachmentRef2.data();
	subpass2.pDepthStencilAttachment = &depthAttachmentRef;
	subpass2.inputAttachmentCount = 1;
	subpass2.pInputAttachments = attachmentRef1Input.data();


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
