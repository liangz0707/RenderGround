#pragma once
#include "Common.h"
#include "VulkanModel.h"
#include "VulkanRModel.h"
#include "VulkanPipelineResource.h"
#include "VulkanRTexture.h"
#include "VulkanTexture.h"

class VulkanModel;
class VulkanTexture;
class VulkanRModel;
class VulkanRTexture;
class VulkanPipelineResource;


class VulkanSceneManager
{

public:
	VulkanSceneManager();

	void loadRenderModel(VulkanModel* vulkanModel);
	void unloadRenderModel(VulkanModel* vulkanModel);
	void loadTexture(VulkanTexture* vulkanTexture);
	void unloadTexture(VulkanTexture* vulkanTexture);

	void SetPipelineResource(VulkanPipelineResource* vulkanPipelineResource)
	{
		this->vulkanPipelineResource = vulkanPipelineResource;
	}

	std::vector<VulkanRModel*> GetStaticModel()
	{
		return vulkanModels;
	}

	std::vector<VulkanRTexture*> GetTexture()
	{
		return vulkanTextures;
	}

	VulkanRTexture* GetTextureByIndex(int index)
	{
		return vulkanTextures[index];
	}

private:
	std::vector<VulkanRModel*> vulkanModels;
	std::vector<VulkanRTexture*> vulkanTextures;
	VulkanPipelineResource* vulkanPipelineResource;
};

