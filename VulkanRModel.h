#pragma once
#include "Common.h"
#include "VulkanModel.h"
#include "VulkanMaterial.h"
#include "VulkanResourceManager.h"

class VulkanResourceManager;
class VulkanMaterial;

class VulkanRModel
{
public:
	VulkanRModel();
	void SetVertexBuffer(VkBuffer vertexBuffer)
	{
		this->vertexBuffer = vertexBuffer;
	}

	void SetIndexBuffer(VkBuffer indexBuffer)
	{
		this->indexBuffer = indexBuffer;
	}

	void SetIndexSize(size_t indexSize)
	{
		this->indexSize = indexSize;
	}

	void SetVertexBufferMemory(VkDeviceMemory vertexBufferMemory)
	{
		this->vertexBufferMemory = vertexBufferMemory;
	}

	void SetIndexBufferMemory(VkDeviceMemory indexBufferMemory)
	{
		this->indexBufferMemory = indexBufferMemory;
	}

	VkBuffer GetIndexBuffer()
	{
		return indexBuffer;
	}

	VkBuffer GetVertexBuffer()
	{
		return vertexBuffer;
	}

	VkDeviceMemory GetIndexBufferMemory()
	{
		return indexBufferMemory;
	}

	VkDeviceMemory GetVertexBufferMemory()
	{
		return vertexBufferMemory;
	}

	size_t GetIndexSize()
	{
		return indexSize;
	}

	VulkanMaterial* GetMaterial() {
		return material;
	}

	void SetMaterial(VulkanMaterial* material);

	void Update();
private:
	//glm::vec4 position;
	size_t indexSize;
	UniformBufferObject ubo;

	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkDeviceMemory indexBufferMemory;
	VulkanMaterial* material;
};

