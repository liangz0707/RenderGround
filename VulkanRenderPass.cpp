#include "VulkanRenderPass.h"

VulkanRenderPass::VulkanRenderPass()
{

}

VkRenderPass VulkanRenderPass::GetInstance()
{
	return renderPass;
}


VkPipelineLayout VulkanRenderPass::GetPipelineLayout()
{
	return vulkanGraphicPipeline->GetPipelineLayout();
}

void VulkanRenderPass::createDefaultRenderPass() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	
	VkAttachmentDescription colorAttachment = {};
	//The format of the color attachment should match the format of the swap chain image
	colorAttachment.format = RM->GetSwapChain()->GetSwapChainImageFormat();
	//we're not doing anything with multisampling yet, so we'll stick to 1 sample.
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	//下面两项分别表示渲染开始前和渲染结束后
	/*
		VK_ATTACHMENT_LOAD_OP_LOAD: Preserve the existing contents of the attachment
		VK_ATTACHMENT_LOAD_OP_CLEAR: Clear the values to a constant at the start
		VK_ATTACHMENT_LOAD_OP_DONT_CARE: Existing contents are undefined; we don't care about them
	*/
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	/*
		VK_ATTACHMENT_STORE_OP_STORE: Rendered contents will be stored in memory and can be read later
		VK_ATTACHMENT_STORE_OP_DONT_CARE: Contents of the framebuffer will be undefined after the rendering operation
	*/
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	/*
	Textures and framebuffers in Vulkan are represented by VkImage objects with
	a certain pixel format,
	however the layout of the pixels in memory can change based on what
	you're trying to do with an image.
	*/
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//finalLayout表示渲染Pass结束之后Image的布局如何自动转换。
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


	//根据在attachemtn描述的索引来定义attachmentRef。
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//一个渲染Pass由多个Subpass组合而成，Subpass是一系列的渲染操作，依赖于前几个pass的framebuffer的内容
	//当我们把pass分组，Vulkan能够重新排序渲染顺序降低带宽提升性能。
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	//指定这组Subpass用到的所有的attachment的Ref
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	/*
	pInputAttachments: Attachments that are read from a shader
	pResolveAttachments: Attachments used for multisampling color attachments
	pDepthStencilAttachment: Attachment for depth and stencil data
	pPreserveAttachments: Attachments that are not used by this subpass, but for which the data must be preserved
	*/

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	// 指定全部要用到的attachment。
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	// 指定所有的Subpass，不同的Subpass可以有不同的Attachment组合，在SubpassInfo中指定Ref
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	if (vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}

	//specify memory and execution dependencies between subpasses.
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;

	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;
}

void VulkanRenderPass::destroyDefaultRenderPass() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	vkDestroyRenderPass(vkDevice, renderPass, nullptr);
}

void VulkanRenderPass::createDeferredRenderPass()
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
	gbufferAAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription gbufferBAttachment = {};
	gbufferBAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
	gbufferBAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	gbufferBAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	gbufferBAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	gbufferBAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	gbufferBAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	gbufferBAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	gbufferBAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription gbufferCAttachment = {};
	gbufferCAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
	gbufferCAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	gbufferCAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	gbufferCAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	gbufferCAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	gbufferCAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	gbufferCAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	gbufferCAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription gbufferDAttachment = {};
	gbufferDAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
	gbufferDAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	gbufferDAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	gbufferDAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	gbufferDAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	gbufferDAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	gbufferDAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	gbufferDAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription gbufferEAttachment = {};
	gbufferEAttachment.format = VK_FORMAT_B10G11R11_UFLOAT_PACK32;
	gbufferEAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	gbufferEAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	gbufferEAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	gbufferEAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	gbufferEAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	gbufferEAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	gbufferEAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

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

	VkAttachmentReference gbufferARef = {};
	gbufferARef.attachment = 0;
	gbufferARef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference gbufferBRef = {};
	gbufferBRef.attachment = 1;
	gbufferARef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference gbufferCRef = {};
	gbufferCRef.attachment = 2;
	gbufferCRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference gbufferDRef = {};
	gbufferDRef.attachment = 3;
	gbufferDRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference gbufferERef = {};
	gbufferERef.attachment = 4;
	gbufferERef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 5;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 6;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


	// step1
	std::vector< VkAttachmentReference> attachmentRef1 =
	{
		gbufferARef,
		gbufferBRef,
		gbufferCRef,
		gbufferDRef,
		gbufferERef,
		depthAttachmentRef
	};

	VkSubpassDescription subpass1 = {};
	subpass1.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass1.colorAttachmentCount = 5;
	subpass1.pColorAttachments = attachmentRef1.data();
	subpass1.pDepthStencilAttachment = &depthAttachmentRef;

	// step2
	std::vector< VkAttachmentReference> attachmentRef2 =
	{
		colorAttachmentRef,
		depthAttachmentRef
	};
	
	VkSubpassDescription subpass2 = {};
	subpass2.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass2.colorAttachmentCount = 1;
	subpass2.pColorAttachments = attachmentRef2.data();
	subpass2.pDepthStencilAttachment = &depthAttachmentRef;



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

void VulkanRenderPass::destroyDeferredRenderPass()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	vkDestroyRenderPass(vkDevice, renderPass, nullptr);
}

void VulkanRenderPass::createForwardRenderPass()
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
	forAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	forAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


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


	// step1
	std::vector< VkAttachmentReference> attachmentRef1 =
	{
		forAttachmentRef,
		depthAttachmentRef
	};

	VkSubpassDescription subpass1 = {};
	subpass1.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass1.colorAttachmentCount = 1;
	subpass1.pColorAttachments = attachmentRef1.data();
	subpass1.pDepthStencilAttachment = &depthAttachmentRef;

	// step2
	std::vector< VkAttachmentReference> attachmentRef2 =
	{
		colorAttachmentRef,
		depthAttachmentRef
	};

	VkSubpassDescription subpass2 = {};
	subpass2.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass2.colorAttachmentCount = 1;
	subpass2.pColorAttachments = attachmentRef2.data();
	subpass2.pDepthStencilAttachment = &depthAttachmentRef;


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

void VulkanRenderPass::destroyForwardRenderPass()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	vkDestroyRenderPass(vkDevice, renderPass, nullptr);
}

void VulkanRenderPass::createToScreenPipelines(VulkanPipelineResource *vulkanPipelineResource)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	vulkanGraphicPipeline = new VulkanGraphicPipeline(vulkanPipelineResource);

	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

	auto vertShaderCode = Utility::readFile("vert.spv");
	auto fragShaderCode = Utility::readFile("frag.spv");

	vertShaderModule = RM->createShaderModule(vertShaderCode);
	fragShaderModule = RM->createShaderModule(fragShaderCode);

	vulkanPipelineResource->createUniformDescriptorSets(
		vulkanPipelineResource->GetUniformBuffers(), 
		uniformDescriptorSets);

	vulkanGraphicPipeline->createGraphicsPipeline(
		vertShaderModule,
		fragShaderModule,
		RM->GetExtent(),
		renderPass);

	vkDestroyShaderModule(RM->GetDevice()->GetInstance(), fragShaderModule, nullptr);
	vkDestroyShaderModule(RM->GetDevice()->GetInstance(), vertShaderModule, nullptr);
}

void VulkanRenderPass::destroyToScreenPipelines()
{
	vulkanGraphicPipeline->destroyGraphicPipeline();
}

void VulkanRenderPass::createDeferredGeometryPipelines(VulkanPipelineResource* vulkanPipelineResource)
{
}

void VulkanRenderPass::destroyDeferredGeometryPipelines()
{
}

void VulkanRenderPass::createDeferredLightingPipelines(VulkanPipelineResource* vulkanPipelineResource)
{
}

void VulkanRenderPass::destroyDeferredLightingPipelines()
{
}

void VulkanRenderPass::createForwardPipelines(VulkanPipelineResource* vulkanPipelineResource)
{
}

void VulkanRenderPass::destroyForwardPipelines()
{
}

VkDescriptorSet VulkanRenderPass::GetUniformDescriptorSetByIndex(int i)
{
	return uniformDescriptorSets[i];
}
