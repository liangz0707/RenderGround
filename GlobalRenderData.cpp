#include "GlobalRenderData.h"



void GlobalRenderData::createUniformDescriptorSets(VkDescriptorPool pool, VkDescriptorSetLayout layout) {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t layoutsSize = RM->GetSwapChain()->GetSwapChainImageSize();
	VkDevice vkDevice = RM->GetDevice()->GetInstance();

	std::vector<VkDescriptorSetLayout> layouts(layoutsSize, layout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
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

void GlobalRenderData::updateUniformBuffer(int swapChainImageIndex) {

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	// 模型数据抽取到模型当中
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	// 摄像机数据
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


GlobalRenderData::GlobalRenderData()
{
}

void GlobalRenderData::createUniformBuffers(VkDeviceSize bufferSize)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	int uniformBufferSize = RM->GetSwapChain()->GetSwapChainImageSize();

	uniformBuffers.resize(uniformBufferSize);
	uniformBuffersMemory.resize(uniformBufferSize);

	for (size_t i = 0; i < uniformBuffers.size(); i++)
	{
		RM->createBuffer(bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i],
			uniformBuffersMemory[i]);
	}
}

void GlobalRenderData::destroyUniformBuffers()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	for (size_t i = 0; i < uniformBuffers.size(); i++)
	{
		vkDestroyBuffer(RM->GetDevice()->GetInstance(), uniformBuffers[i], nullptr);
		vkFreeMemory(RM->GetDevice()->GetInstance(), uniformBuffersMemory[i], nullptr);
	}
}