#pragma once
#include "Common.h"
#include "VulkanModel.h"
#include "VulkanResourceManager.h"

class VulkanResourceManager;

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


	void SetDescriptorSet(VkDescriptorSet vkDescriptorSet)
	{
		this->vkDescriptorSet = vkDescriptorSet;
	}

	VkDescriptorSet GetDescriptorSet()
	{
		return vkDescriptorSet;
	}

	void SetDescriptorSetBufferMemory(VkDeviceMemory vkDescriptorSetBufferMemory)
	{
		this->vkDescriptorSetBufferMemory = vkDescriptorSetBufferMemory;
	}

	VkDeviceMemory GetDescriptorSetBufferMemory()
	{
		return vkDescriptorSetBufferMemory;
	}

	void SetDescriptorSetBuffer(VkBuffer vkDescriptorSetBuffer)
	{
		this->vkDescriptorSetBuffer = vkDescriptorSetBuffer;
	}

	VkBuffer GetDescriptorSetBuffer()
	{
		return vkDescriptorSetBuffer;
	}

	PreEntityUniformBufferObject GetUniformBuffer()
	{
		return ubo;
	}

	void Update();
private:
	glm::vec4 position;
	PreEntityUniformBufferObject ubo;

	VkDescriptorSet vkDescriptorSet;
	size_t indexSize;
	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
	VkBuffer vkDescriptorSetBuffer;
	VkDeviceMemory vkDescriptorSetBufferMemory;
	VkDeviceMemory vertexBufferMemory;
	VkDeviceMemory indexBufferMemory;
};

