#include "VulkanFramebuffer.h"

VkFramebuffer VulkanFramebuffer::createFrameBuffer(std::array<VkImageView, 2> attachments,
	VkExtent2D swapChainExtent
	, VulkanRenderPass* vulkanRenderPass) {
	VkFramebuffer vkFramebuffer;
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkFramebufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.renderPass = vulkanRenderPass->GetInstance();
	createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	createInfo.pAttachments = attachments.data();
	createInfo.width = swapChainExtent.width;
	createInfo.height = swapChainExtent.height;
	createInfo.layers = 1;
	vkFramebuffer = RM->createFramebuffer(&createInfo);
	return vkFramebuffer;
}

void VulkanFramebuffer::createSwapChainFrameBuffers(VulkanSwapChain * vulkanSwapChain, VulkanRenderPass* vulkanRenderPass) {
	vVulkanFrameBuffer.resize(vulkanSwapChain->GetSwapChainImageSize());
	for (int i = 0; i < vulkanSwapChain->GetSwapChainImageSize(); i++)
	{
		std::array<VkImageView, 2> attachments = 
		{
			vulkanSwapChain->GetSwapChainImageViewByIndex(i) , 
			depthImageView 
		};
		vVulkanFrameBuffer[i] = createFrameBuffer(attachments, vulkanSwapChain->GetSwapChainImageExtent(), vulkanRenderPass);
	}
}

void VulkanFramebuffer::createDepthResource(VulkanSwapChain* vulkanSwapChain)
{ 
	VulkanResourceManager * RM = VulkanResourceManager::GetResourceManager();
	RM->createImage(vulkanSwapChain->GetSwapChainImageExtent().width,
		vulkanSwapChain->GetSwapChainImageExtent().height,
		RM->findDepthFormat(),
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		depthImage,
		depthImageMemory
	);

	depthImageView = RM->createImageView(depthImage, RM->findDepthFormat(), VK_IMAGE_ASPECT_DEPTH_BIT);

}


void VulkanFramebuffer::destroyDepthResource()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	
	vkDestroyImageView(RM->GetDevice()->GetInstance(), depthImageView, nullptr);
	vkDestroyImage(RM->GetDevice()->GetInstance(), depthImage, nullptr);
	vkFreeMemory(RM->GetDevice()->GetInstance(), depthImageMemory, nullptr);

}

void VulkanFramebuffer::destroySwapChainFrameBuffers()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	for (size_t i = 0; i < vVulkanFrameBuffer.size(); i++) {
		vkDestroyFramebuffer(RM->GetDevice()->GetInstance(), vVulkanFrameBuffer[i], nullptr);
	}
}