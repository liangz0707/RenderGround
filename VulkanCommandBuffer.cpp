#include "VulkanCommandBuffer.h"

VulkanSingleTimeCommandBuffer::VulkanSingleTimeCommandBuffer(VkCommandPool commandPool)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	RM->allocCommandBuffer(&allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	this->commandPool = commandPool;

}


VulkanSingleTimeCommandBuffer::~VulkanSingleTimeCommandBuffer()
{
	vkEndCommandBuffer(commandBuffer);
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(RM->GetDevice()->GetGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(RM->GetDevice()->GetGraphicQueue());
	vkFreeCommandBuffers(RM->GetDevice()->GetInstance(), commandPool, 1, &commandBuffer);
}

VulkanFrameRenderCommandBuffer::VulkanFrameRenderCommandBuffer()
{

}



VulkanFrameRenderCommandBuffer::~VulkanFrameRenderCommandBuffer()
{
}

void VulkanFrameRenderCommandBuffer::createCommandBuffer(VkCommandPool commandPool, size_t commandBufferSize)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	commandBuffers.resize(commandBufferSize);
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(RM->GetDevice()->GetInstance(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void VulkanFrameRenderCommandBuffer::destroyCommandBuffer()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	vkFreeCommandBuffers(RM->GetDevice()->GetInstance(), commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

}

VkCommandBuffer VulkanFrameRenderCommandBuffer::VulkanCommandBegin(int index)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffers[index], &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	return commandBuffers[index];
}

void VulkanFrameRenderCommandBuffer::VulkanCommandEnd(int index)
{
	if (vkEndCommandBuffer(commandBuffers[index]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

