#include "VulkanPipelineResource.h"

VulkanPipelineResource::VulkanPipelineResource()
{

}

void VulkanPipelineResource::createUniformBuffers(VkDeviceSize bufferSize)
{

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	uniformBuffers.resize(RM->GetFramebuffer()->GetFrameBufferSize());
	uniformBuffersMemory.resize(RM->GetFramebuffer()->GetFrameBufferSize());

	for (size_t i = 0; i < uniformBuffers.size(); i++)
	{
		RM->createBuffer(bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i],
			uniformBuffersMemory[i]);
	}
}

void VulkanPipelineResource::createPreUniformBuffer(
	VkDeviceSize bufferSize, 
	VkBuffer& preObjectBuffer,
	VkDeviceMemory& vertexBufferMemory)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	RM->createBuffer(bufferSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		preObjectBuffer,
		vertexBufferMemory);
}


void VulkanPipelineResource::updateUniformBuffer(int swapChainImageIndex) {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.proj = glm::perspective(glm::radians(45.0f), RM->GetExtent().width / (float)RM->GetExtent().height, 0.1f, 10.0f);
	/*
	GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted.
	*/
	ubo.proj[1][1] *= -1;

	void* data;
	RM->mapMemory(uniformBuffersMemory[swapChainImageIndex], sizeof(ubo), &data);
	memcpy(data, &ubo, sizeof(ubo));
	RM->unMapMemory(uniformBuffersMemory[swapChainImageIndex]);



}

void VulkanPipelineResource::updateTexture(int swapChainImageIndex)
{

}

VkDeviceMemory VulkanPipelineResource::GetUboMemoryByIndex(int swapChainImageIndex)
{
	return uniformBuffersMemory[swapChainImageIndex];
}


void VulkanPipelineResource::createObjectDescriptorPool() {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = RM->GetFramebuffer()->GetFrameBufferSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	// Model Number
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(layoutsSize);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(layoutsSize);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(layoutsSize);

	if (vkCreateDescriptorPool(vkDevice, &poolInfo, nullptr, &objectDescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}


void VulkanPipelineResource::createObjectDescriptorSetLayout() {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = RM->GetFramebuffer()->GetFrameBufferSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 0;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding uboPreEntityLayoutBinding = {};
	uboPreEntityLayoutBinding.binding = 1;
	uboPreEntityLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboPreEntityLayoutBinding.descriptorCount = 1;
	uboPreEntityLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	uboPreEntityLayoutBinding.pImmutableSamplers = nullptr; // Optional

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { samplerLayoutBinding,uboPreEntityLayoutBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &objectDescriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}


VkDescriptorSet VulkanPipelineResource::createObjectDescriptorSet(
	VkBuffer preEntityUniformBuffer,
	VkSampler vkTextureSampler,
	VkImageView vkTextureImageView)
{
	VkDescriptorSet vkDescriptorSet;
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	VkDescriptorSetLayout layouts = objectDescriptorSetLayout;
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = objectDescriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
	allocInfo.pSetLayouts = &layouts;

	if (vkAllocateDescriptorSets(vkDevice, &allocInfo, &vkDescriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = vkTextureImageView;
	imageInfo.sampler = vkTextureSampler;

	VkDescriptorBufferInfo preBufferInfo = {};
	preBufferInfo.buffer = preEntityUniformBuffer;
	preBufferInfo.offset = 0;
	preBufferInfo.range = VK_WHOLE_SIZE;// sizeof(UniformBufferObject);

	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = vkDescriptorSet;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &imageInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = vkDescriptorSet;//对应SetLayout
	descriptorWrites[1].dstBinding = 1;  //对应SetLayout当中的Bading数字
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pBufferInfo = &preBufferInfo;

	vkUpdateDescriptorSets(vkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

	return vkDescriptorSet;
}




void VulkanPipelineResource::createVertexBuffer(VkDeviceSize bufferSize,
										void * srcData,
										VkBuffer &vertexBuffer,
										VkDeviceMemory &vertexBufferMemory) {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();


	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	// Buffer can be used as source in a memory transfer operation.
	RM->createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory);

	// 将buffer的内存映射到可访问的CPU内存。
	void* data;
	RM->mapMemory(stagingBufferMemory,  bufferSize, &data);
	// 由于缓存机制，并不会立即拷贝
	memcpy(data, srcData, (size_t)bufferSize);
	RM->unMapMemory(stagingBufferMemory);

	// Buffer can be used as destination in a memory transfer operation.
	// 现在申请的Vertexbuffer是DeviceLocal，所以无法用Map进行映射。
	// 需要从stagingBuffer到vertexBuffer
	RM->createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer,
		vertexBufferMemory);

	copyBufferToBuffer(stagingBuffer, vertexBuffer, bufferSize);

	RM->destroyBuffer(stagingBuffer);
	RM->freeMemory(stagingBufferMemory);
}

void VulkanPipelineResource::createIndexBuffer(VkDeviceSize bufferSize,
	void* srcData,
	VkBuffer &vertexBuffer,
	VkDeviceMemory &vertexBufferMemory)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();


	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	// Buffer can be used as source in a memory transfer operation.
	RM->createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory);

	// 将buffer的内存映射到可访问的CPU内存。
	void* data;
	RM->mapMemory(stagingBufferMemory, bufferSize, &data);
	// 由于缓存机制，并不会立即拷贝
	memcpy(data, srcData, (size_t)bufferSize);
	RM->unMapMemory(stagingBufferMemory);

	// Buffer can be used as destination in a memory transfer operation.
	// 现在申请的Vertexbuffer是DeviceLocal，所以无法用Map进行映射。
	// 需要从stagingBuffer到vertexBuffer
	RM->createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer,
		vertexBufferMemory);

	copyBufferToBuffer(stagingBuffer, vertexBuffer, bufferSize);

	RM->destroyBuffer(stagingBuffer);
	RM->freeMemory(stagingBufferMemory);
}


void VulkanPipelineResource::createCommandBuffers()
{
}


void VulkanPipelineResource::createTextureSampler() {
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;

	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;

	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	RM->createSampler(&samplerInfo, textureSampler);
}


bool VulkanPipelineResource::hasStencilComponent(VkFormat format) {
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void VulkanPipelineResource::copyBufferToBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	
	VulkanSingleTimeCommandBuffer* commandBuffer = new VulkanSingleTimeCommandBuffer(RM->GetCommandPool());

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer->GetInstance(), srcBuffer, dstBuffer, 1, &copyRegion);

	delete commandBuffer;
}

void VulkanPipelineResource::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VulkanSingleTimeCommandBuffer *commandBuffer = new VulkanSingleTimeCommandBuffer(RM->GetCommandPool());

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
		commandBuffer->GetInstance(),
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	delete commandBuffer;
}

void VulkanPipelineResource::createTextureImage(unsigned char* pixels,
	int texWidth,
	int texHeight,
	int texChannel,
	VkImage &textureImage,
	VkDeviceMemory &textureImageMemory
)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	//The pixels are laid out row by row with 4 bytes per pixel 
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	RM->createBuffer(imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		stagingBuffer,
		stagingBufferMemory);

	void* data;
	RM->mapMemory(stagingBufferMemory, imageSize, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	RM->unMapMemory(stagingBufferMemory);

	Utility::freeImage(pixels);


	RM->createImage(texWidth, texHeight,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		textureImage,
		textureImageMemory
	);

	transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	RM->destroyBuffer(stagingBuffer);
	RM->freeMemory(stagingBufferMemory);
}

void VulkanPipelineResource::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VulkanSingleTimeCommandBuffer* commandBuffer = new VulkanSingleTimeCommandBuffer(RM->GetCommandPool());

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (hasStencilComponent(format)) {
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else {
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}


	vkCmdPipelineBarrier(
		commandBuffer->GetInstance(),
		sourceStage,
		destinationStage ,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	delete (commandBuffer);
}


