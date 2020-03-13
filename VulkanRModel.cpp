#include "VulkanRModel.h"

VulkanRModel::VulkanRModel(VulkanModel* vulkanModel)
{
	vulkanModel->GetVertex();
	vulkanModel->GetIndex();
	
}
