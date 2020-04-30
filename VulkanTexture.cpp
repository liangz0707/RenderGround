#include "VulkanTexture.h"

VulkanTexture::VulkanTexture()
{
	texWidth = 0;
	texHeight = 0;
	texChannels = 0;
	pixel = loadImage(TEXTURE_PATH, texWidth, texHeight, texChannels);
}

VulkanTexture::VulkanTexture(std::string texturePath)
{
	texWidth = 0;
	texHeight = 0;
	texChannels = 0;
	pixel = loadImage(texturePath,texWidth, texHeight, texChannels);
}

unsigned char* VulkanTexture::loadImage(std::string ImagePath, int& texWidth, int& texHeight, int& texChannels)
{
	return RUtility::loadImage(ImagePath, texWidth, texHeight, texChannels);
}
