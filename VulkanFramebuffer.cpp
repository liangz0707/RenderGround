#include "VulkanFramebuffer.h"

VkFramebuffer VulkanFramebuffer::createFrameBuffer(std::array<VkImageView, 2> attachments, VkExtent2D swapChainExtent) {
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

void VulkanFramebuffer::createSwapChainFrameBuffers(VulkanSwapChain * vulkanSwapChain) {
	vVulkanFrameBuffer.resize(vulkanSwapChain->GetSwapChainImageSize());
	for (int i = 0; i < vulkanSwapChain->GetSwapChainImageSize(); i++)
	{
		std::array<VkImageView, 2> attachments = 
		{
			vulkanSwapChain->GetSwapChainImageViewByIndex(i) , 
			depthImageView 
		};
		vVulkanFrameBuffer[i] = createFrameBuffer(attachments, vulkanSwapChain->GetSwapChainImageExtent());
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