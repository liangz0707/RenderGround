#include "IVulkanMaterial.h"


VkDescriptorSet IVulkanMaterial::GetDescriptorSet() {
	return descriptorSet;
}
void IVulkanMaterial::UpdateDescriptorSet() {};

void IVulkanMaterial::DestroyMaterial() {
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	RM->destroyBuffer(buffer);
	RM->freeMemory(bufferMemory);

};