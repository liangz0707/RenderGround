#include "VulkanRModel.h"

VulkanRModel::VulkanRModel()
{
}

void VulkanRModel::SetMaterial(VulkanMaterial* material)
{
	this->material = material;
}

void VulkanRModel::Update()
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	material->Update(ubo);
}

void VulkanRModel::createVertexBuffer(VkDeviceSize bufferSize,
	void* srcData,
	VkBuffer& vertexBuffer,
	VkDeviceMemory& vertexBufferMemory) {

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
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer,
		vertexBufferMemory);

	RUtility::copyBufferToBuffer(stagingBuffer, vertexBuffer, bufferSize);

	RM->destroyBuffer(stagingBuffer);
	RM->freeMemory(stagingBufferMemory);
}

void VulkanRModel::createIndexBuffer(VkDeviceSize bufferSize,
	void* srcData,
	VkBuffer& vertexBuffer,
	VkDeviceMemory& vertexBufferMemory)
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

	RUtility::copyBufferToBuffer(stagingBuffer, vertexBuffer, bufferSize);

	RM->destroyBuffer(stagingBuffer);
	RM->freeMemory(stagingBufferMemory);
}