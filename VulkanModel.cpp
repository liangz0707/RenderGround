#include "VulkanModel.h"


#include "Utility.h"

VulkanModel::VulkanModel()
{
	Utility::loadModel(MODEL_PATH, vertices, indices);
	pixels = Utility::loadImage(TEXTURE_PATH, texWidth, texHeight, texChannel);
}



void VulkanModel::createTextureImageView()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	textureImageView = RM->createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}


