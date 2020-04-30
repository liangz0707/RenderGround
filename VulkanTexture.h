#pragma once
#include "Utility.h"
#include "Common.h"

class RUtility;
class VulkanTexture
{
private :
	const std::string TEXTURE_PATH = "textures/chalet.jpg";
	unsigned char* pixel;
	int texWidth;
	int texHeight;
	int texChannels;

public:
	VulkanTexture();
	VulkanTexture(std::string );
	int GetWidth() { return texWidth; };
	int GetHeight() { return texHeight; };
	int GetChannel() { return texChannels; };
	unsigned char* GetData() { return pixel; };

	unsigned char* loadImage(std::string ImagePath,
		int& texWidth,
		int& texHeight,
		int& texChannels);

};

