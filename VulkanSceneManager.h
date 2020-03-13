#pragma once
#include "Common.h"
#include "VulkanModel.h"
#include "VulkanRModel.h"
#include "VulkanPipelineResource.h"

class VulkanModel;
class VulkanRModel;
class VulkanPipelineResource;

class VulkanSceneManager
{

public:
	VulkanSceneManager();

	void loadRenderModel(VulkanModel* vulkanModel);
	void unloadRenderModel(VulkanModel* vulkanModel);

	std::vector<VulkanRModel*> GetStaticModel()
	{
		return vulkanModels;
	}

private:
	std::vector<VulkanRModel*> vulkanModels;
	VulkanPipelineResource* vulkanPipelineResource;
};

