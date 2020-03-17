#pragma once
#include "Common.h"
#include "VulkanResourceManager.h"

class VulkanResourceManager;

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

	VkImage GetImage()
	{
		return vkTextureImage;
	}

	VkDeviceMemory GetTextureImageMemory()
	{
		return vkTextureImageMemory;
	}

	~VulkanRTexture();

private:
	int texWidth;
	int texHeight;
	int texChannel;

	VkImage vkTextureImage;
	VkImageView vkTextureImageView;
	VkDeviceMemory vkTextureImageMemory;

};

