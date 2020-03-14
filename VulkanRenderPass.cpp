#include "VulkanRenderPass.h"

VulkanRenderPass::VulkanRenderPass(VulkanPipelineResource * vulkanPipelineResource)
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

void VulkanRenderPass::createDescriptorSets(VulkanFramebuffer* vulkanFramebuffer,
	std::vector<VkBuffer> uniformBuffers, 
	std::vector<VkBuffer> preEntityUniformBuffers) {
	
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = vulkanFramebuffer->GetFrameBufferSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	std::vector<VkDescriptorSetLayout> layouts(layoutsSize, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(layoutsSize);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(layoutsSize);
	if (vkAllocateDescriptorSets(vkDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < layoutsSize; i++) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = VK_WHOLE_SIZE;// sizeof(UniformBufferObject);

		VkDescriptorBufferInfo preBufferInfo = {};
		preBufferInfo.buffer = preEntityUniformBuffers[i];
		preBufferInfo.offset = 0;
		preBufferInfo.range = VK_WHOLE_SIZE;// sizeof(UniformBufferObject);

		/*
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;
		*/
		std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};


		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		/*
		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;
		*/
		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];//对应SetLayout
		descriptorWrites[1].dstBinding = 2;  //对应SetLayout当中的Bading数字
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = &preBufferInfo;


		vkUpdateDescriptorSets(vkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}


}

void VulkanRenderPass::createDescriptorPool() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = RM->GetFramebuffer()->GetFrameBufferSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();
	std::array<VkDescriptorPoolSize, 3> poolSizes = {};

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(layoutsSize);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(layoutsSize);
	poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[2].descriptorCount = static_cast<uint32_t>(layoutsSize);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(layoutsSize);

	if (vkCreateDescriptorPool(vkDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void VulkanRenderPass::createDescriptorSetLayout() {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = RM->GetFramebuffer()->GetFrameBufferSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding uboPreEntityLayoutBinding = {};
	uboPreEntityLayoutBinding.binding = 2;
	uboPreEntityLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboPreEntityLayoutBinding.descriptorCount = 1;
	uboPreEntityLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	uboPreEntityLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	std::array<VkDescriptorSetLayoutBinding, 3> bindings = { uboLayoutBinding, samplerLayoutBinding,uboPreEntityLayoutBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
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

	vulkanGraphicPipeline->createGraphicsPipeline(vertShaderModule,
		fragShaderModule,
		RM->GetExtent(),
		&descriptorSetLayout,
		renderPass);

}

