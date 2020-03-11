#include "VulkanModel.h"


#include "Utility.h"

VulkanModel::VulkanModel()
{
	Utility::loadModel(MODEL_PATH, vertices, indices);
	pixels = Utility::loadImage(TEXTURE_PATH, texWidth, texHeight, texChannel);
}

void VulkanModel::createTextureImage()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	unsigned char* pixels = Utility::loadImage(TEXTURE_PATH.c_str(), texWidth, texHeight, texChannel);
	//The pixels are laid out row by row with 4 bytes per pixel 
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	RM->createBuffer(imageSize,
	VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	VK_MEMORY_PROPERTY_HOST_COHERENT_BIT| VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
	stagingBuffer,
	stagingBufferMemory);

	void* data;
	RM->mapMemory(stagingBufferMemory, imageSize, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	RM->unMapMemory(stagingBufferMemory);

	Utility::freeImage(pixels);


	RM->createImage(texWidth, texHeight,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		textureImage,
		textureImageMemory
	);

	//transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	//copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	//transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	RM->destroyBuffer(stagingBuffer);
	RM->freeMemory(stagingBufferMemory);
}

void VulkanModel::createTextureImageView()
{
}


