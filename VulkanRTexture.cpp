#include "VulkanRTexture.h"



VulkanRTexture::VulkanRTexture(
	int texWidth, 
	int texHeight, 
	int texChannel,
	VkImage vkTextureImage,
	VkImageView vkTextureImageView,
	VkDeviceMemory vkTextureImageMemory)
{

	this->texWidth = texWidth;
	this->texHeight = texHeight;
	this->texChannel = texChannel;
	this->vkTextureImage = vkTextureImage;
	this->vkTextureImageView = vkTextureImageView;
	this->vkTextureImageMemory = vkTextureImageMemory;
}

VulkanRTexture::~VulkanRTexture()
{

}
