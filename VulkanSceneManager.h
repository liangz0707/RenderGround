#pragma once
#include "Common.h"
#include "VulkanModel.h"
#include "VulkanRModel.h"
#include "VulkanRTexture.h"
#include "VulkanTexture.h"
#include "VulkanMaterial.h"
#include "IVulkanMaterial.h"

class VulkanMaterial;
class VulkanModel;
class VulkanTexture;
class VulkanRModel;
class VulkanRTexture;
class IVulkanMaterial;

class VulkanSceneManager
{

public:
	VulkanSceneManager();
	VulkanMaterial* loadMaterial();
	void unloadMaterials();
	void updateModel();
	void updateMaterial();

	VulkanRModel* loadRenderModel(VulkanModel* vulkanModel);
	void unloadRenderModel(VulkanModel* vulkanModel);
	void loadTexture(VulkanTexture* vulkanTexture);
	void unloadTexture(VulkanTexture* vulkanTexture);
	void unloadModels();
	void unloadTextures();
	void unloadGlobalMaterials();
	void unloadObjectMaterials();

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


	IVulkanMaterial* GetDeferredMaterial()
	{
		return deferredMaterial;
	}

	IVulkanMaterial* GetForwardMaterial()
	{
		return forwardMaterial;
	}

	IVulkanMaterial* GetPostMaterial()
	{
		return postMaterial;
	}

	IVulkanMaterial* GetGlobalMaterial()
	{
		return globalMaterial;
	}

	void SetDeferredMaterial(IVulkanMaterial* material)
	{
		 deferredMaterial = material;
	}

	void SetForwardMaterial(IVulkanMaterial* material)
	{
		 forwardMaterial = material;
	}

	void SetPostMaterial(IVulkanMaterial* material)
	{
		 postMaterial = material;
	}

	void SetGlobalMaterial(IVulkanMaterial* material)
	{
		 globalMaterial = material;
	}

private:
	std::vector<VulkanRModel*> vulkanModels;
	std::vector<VulkanRTexture*> vulkanTextures;
	std::vector<VulkanMaterial*> vulkanMaterials;

	std::vector<IVulkanMaterial*> objectMaterials;

	IVulkanMaterial* postMaterial;
	IVulkanMaterial* globalMaterial;
	IVulkanMaterial* forwardMaterial;
	IVulkanMaterial* deferredMaterial;

};

