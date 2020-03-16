#include "VulkanModel.h"


#include "Utility.h"

VulkanModel::VulkanModel()
{
	Utility::loadModel(MODEL_PATH, vertices, indices);
	
}
