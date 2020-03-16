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

void VulkanRenderPass::createUniformDescriptorSets(
	VulkanFramebuffer* vulkanFramebuffer,
	std::vector<VkBuffer> uniformBuffers) {
	
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = vulkanFramebuffer->GetFrameBufferSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	std::vector<VkDescriptorSetLayout> layouts(layoutsSize, uniformDescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = uniformDescriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(layoutsSize);
	allocInfo.pSetLayouts = layouts.data();

	uniformDescriptorSets.resize(layoutsSize);
	if (vkAllocateDescriptorSets(vkDevice, &allocInfo, uniformDescriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < layoutsSize; i++) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = VK_WHOLE_SIZE;// sizeof(UniformBufferObject);
		
		std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = uniformDescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(vkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}


void VulkanRenderPass::createUniformDescriptorPool() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = RM->GetFramebuffer()->GetFrameBufferSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	std::array<VkDescriptorPoolSize, 1> poolSizes = {};

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(layoutsSize);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(layoutsSize);

	if (vkCreateDescriptorPool(vkDevice, &poolInfo, nullptr, &uniformDescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void VulkanRenderPass::createUniformDescriptorSetLayout() {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = RM->GetFramebuffer()->GetFrameBufferSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional


	std::array<VkDescriptorSetLayoutBinding, 1> bindings = { uboLayoutBinding};

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &uniformDescriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}


void VulkanRenderPass::createRenderPass() {

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

void VulkanRenderPass::createGraphicPipelines()
{

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	vulkanGraphicPipeline = new VulkanGraphicPipeline();

	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

	auto vertShaderCode = Utility::readFile("vert.spv");
	auto fragShaderCode = Utility::readFile("frag.spv");

	vertShaderModule = RM->createShaderModule(vertShaderCode);
	fragShaderModule = RM->createShaderModule(fragShaderCode);

	VkDescriptorSetLayout layouts[] = { 
		uniformDescriptorSetLayout 
	//	,vulkanpipeline
	};
	int layoutCount = 1;
	vulkanGraphicPipeline->createGraphicsPipeline(vertShaderModule,
		fragShaderModule,
		RM->GetExtent(),
		layouts,
		layoutCount,
		renderPass);

}

