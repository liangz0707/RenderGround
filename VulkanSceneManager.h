#pragma once
#include "Common.h"
#include "VulkanModel.h"
class VulkanSceneManager
{

public:
	VulkanSceneManager();
	void LoadModel();


private:
	std::vector<VulkanModel*> vulkanModels;
};

