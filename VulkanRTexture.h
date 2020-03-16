#pragma once
#include "Common.h"
class VulkanRTexture
{


public:
	VulkanRTexture(int,int,int,VkImage,VkImageView,VkDeviceMemory);

	int GetWidth()
	{
		return texWidth;
	}

	int GetHeight()
	{
		return texHeight;
	}

	int GetChannel()
	{
		return texChannel;
	}

	VkImageView GetImageView()
	{
		return vkTextureImageView;
	}

private:
	int texWidth;
	int texHeight;
	int texChannel;

	VkImage vkTextureImage;
	VkImageView vkTextureImageView;
	VkDeviceMemory vkTextureImageMemory;

};

