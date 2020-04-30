#pragma once
#include "Common.h"
#include "VulkanModel.h"
#include "VulkanRModel.h"
#include "VulkanRTexture.h"
#include "VulkanTexture.h"
#include "VulkanMaterial.h"

class VulkanMaterial;
class VulkanModel;
class VulkanTexture;
class VulkanRModel;
class VulkanRTexture;


class VulkanSceneManager
{

public:
	VulkanSceneManager();
	VulkanMaterial* loadMaterial();
	void updateModel();
	void updateMaterial();

	VulkanRModel* loadRenderModel(VulkanModel* vulkanModel);
	void unloadRenderModel(VulkanModel* vulkanModel);
	void loadTexture(VulkanTexture* vulkanTexture);
	void unloadTexture(VulkanTexture* vulkanTexture);
	void unloadModels();
	void unloadTextures();


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
	std::vector<VulkanMaterial*> vulkanMaterials;

};

