#pragma once
#include "Common.h"
#include "VulkanModel.h"
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

	void Update()
	{
		VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo = {};
	
		ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		void* data;
		RM->mapMemory(
			vkDescriptorSetBufferMemory,
			sizeof(PreEntityUniformBufferObject),
			&data);

		memcpy(
			data,
			&(ubo),
			sizeof(PreEntityUniformBufferObject));

		RM->unMapMemory(
			vkDescriptorSetBufferMemory);
	}
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

