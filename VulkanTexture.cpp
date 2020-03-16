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
