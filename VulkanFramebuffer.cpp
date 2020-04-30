#include "VulkanFramebuffer.h"

VkFramebuffer VulkanFramebuffer::createFrameBuffer(std::vector<VkImageView> attachments,
	VkExtent2D extent
	, IVulkanRenderPass* vulkanRenderPass) {
	VkFramebuffer vkFramebuffer;
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	VkFramebufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.renderPass = vulkanRenderPass->GetInstance();
	createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	createInfo.pAttachments = attachments.data();
	createInfo.width = extent.width;
	createInfo.height = extent.height;
	createInfo.layers = 1;
	vkFramebuffer = RM->createFramebuffer(&createInfo);
	return vkFramebuffer;
}

void VulkanFramebuffer::createSwapChainFrameBuffers(IVulkanRenderPass* vulkanRenderPass) 
{

	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	vVulkanFrameBuffer.resize(RM->GetSwapChain()->GetSwapChainImageSize());
	for (int i = 0; i < RM->GetSwapChain()->GetSwapChainImageSize(); i++)
	{
		std::vector<VkImageView> attachments = 
		{
			RM->GetSwapChain()->GetSwapChainImageViewByIndex(i) ,
			depthImageView 
		};
		vVulkanFrameBuffer[i] = createFrameBuffer(attachments, RM->GetSwapChain()->GetSwapChainImageExtent(), vulkanRenderPass);
	}
}

void VulkanFramebuffer::createDeferredFrameBuffer(IVulkanRenderPass* vulkanRenderPass)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	vVulkanFrameBuffer.resize(RM->GetSwapChain()->GetSwapChainImageSize());
	for (int i = 0; i < RM->GetSwapChain()->GetSwapChainImageSize(); i++)
	{
		std::vector<VkImageView> attachments =
		{
			gbufferAImageView ,
			gbufferBImageView ,
			gbufferCImageView ,
			gbufferDImageView ,
			gbufferEImageView ,
			RM->GetSwapChain()->GetSwapChainImageViewByIndex(i) ,
			depthImageView
		};
		vVulkanFrameBuffer[i] = createFrameBuffer(attachments, RM->GetSwapChain()->GetSwapChainImageExtent(), vulkanRenderPass);
	}
}

void VulkanFramebuffer::createForwardFrameBuffer(IVulkanRenderPass* vulkanRenderPass)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	vVulkanFrameBuffer.resize(RM->GetSwapChain()->GetSwapChainImageSize());
	for (int i = 0; i < RM->GetSwapChain()->GetSwapChainImageSize(); i++)
	{
		std::vector<VkImageView> attachments =
		{
			colorBufferImageView,
			RM->GetSwapChain()->GetSwapChainImageViewByIndex(i) ,
			depthImageView
		};
		vVulkanFrameBuffer[i] = createFrameBuffer(attachments, RM->GetSwapChain()->GetSwapChainImageExtent(), vulkanRenderPass);
	}
}

void VulkanFramebuffer::destroySwapChainFrameBuffers()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	for (size_t i = 0; i < vVulkanFrameBuffer.size(); i++) {
		vkDestroyFramebuffer(RM->GetDevice()->GetInstance(), vVulkanFrameBuffer[i], nullptr);
	}
}

void VulkanFramebuffer::createDepthResource()
{ 
	VulkanResourceManager * RM = VulkanResourceManager::GetResourceManager();
	RM->createImage(
		RM->GetExtent().width,
		RM->GetExtent().height,
		RM->findDepthFormat(),
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT| VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
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

void VulkanFramebuffer::createDeferredColorBufferResource()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	// A
	RM->createImage(
		RM->GetExtent().width,
		RM->GetExtent().height,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		gbufferAImage,
		gbufferAImageMemory
		);

	gbufferAImageView = RM->createImageView(gbufferAImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	// B
	RM->createImage(
		RM->GetExtent().width,
		RM->GetExtent().height,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		gbufferBImage,
		gbufferBImageMemory
		);

	gbufferBImageView = RM->createImageView(gbufferBImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	// C
	RM->createImage(
		RM->GetExtent().width,
		RM->GetExtent().height,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		gbufferCImage,
		gbufferCImageMemory
		);

	gbufferCImageView = RM->createImageView(gbufferCImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	// D
	RM->createImage(
		RM->GetExtent().width,
		RM->GetExtent().height,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		gbufferDImage,
		gbufferDImageMemory
		);

	gbufferDImageView = RM->createImageView(gbufferDImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	// E
	RM->createImage(
		RM->GetExtent().width,
		RM->GetExtent().height,
		VK_FORMAT_B10G11R11_UFLOAT_PACK32,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		gbufferEImage,
		gbufferEImageMemory
		);

	gbufferEImageView = RM->createImageView(gbufferEImage, VK_FORMAT_B10G11R11_UFLOAT_PACK32, VK_IMAGE_ASPECT_COLOR_BIT);
}

void VulkanFramebuffer::destroyDeferredColorBufferResource()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	vkDestroyImageView(RM->GetDevice()->GetInstance(), gbufferAImageView, nullptr);
	vkDestroyImage(RM->GetDevice()->GetInstance(), gbufferAImage, nullptr);
	vkFreeMemory(RM->GetDevice()->GetInstance(), gbufferAImageMemory, nullptr);

	vkDestroyImageView(RM->GetDevice()->GetInstance(), gbufferBImageView, nullptr);
	vkDestroyImage(RM->GetDevice()->GetInstance(), gbufferBImage, nullptr);
	vkFreeMemory(RM->GetDevice()->GetInstance(), gbufferBImageMemory, nullptr);

	vkDestroyImageView(RM->GetDevice()->GetInstance(), gbufferCImageView, nullptr);
	vkDestroyImage(RM->GetDevice()->GetInstance(), gbufferCImage, nullptr);
	vkFreeMemory(RM->GetDevice()->GetInstance(), gbufferCImageMemory, nullptr);

	vkDestroyImageView(RM->GetDevice()->GetInstance(), gbufferDImageView, nullptr);
	vkDestroyImage(RM->GetDevice()->GetInstance(), gbufferDImage, nullptr);
	vkFreeMemory(RM->GetDevice()->GetInstance(), gbufferDImageMemory, nullptr);

	vkDestroyImageView(RM->GetDevice()->GetInstance(), gbufferEImageView, nullptr);
	vkDestroyImage(RM->GetDevice()->GetInstance(), gbufferEImage, nullptr);
	vkFreeMemory(RM->GetDevice()->GetInstance(), gbufferEImageMemory, nullptr);
}

void VulkanFramebuffer::createForwardColorBufferResource()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	RM->createImage(
		RM->GetExtent().width,
		RM->GetExtent().height,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		colorBufferImage,
		colorBufferImageMemory
		);

	colorBufferImageView = RM->createImageView(colorBufferImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void VulkanFramebuffer::destroyForwardColorBufferResource()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	vkDestroyImageView(RM->GetDevice()->GetInstance(), colorBufferImageView, nullptr);
	vkDestroyImage(RM->GetDevice()->GetInstance(), colorBufferImage, nullptr);
	vkFreeMemory(RM->GetDevice()->GetInstance(), colorBufferImageMemory, nullptr);
}
