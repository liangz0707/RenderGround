#pragma once
#include "IVulkanMaterial.h"
#include "RenderingResourceLocater.h"


struct BatchBufferObject {
	glm::mat4 model;
	glm::vec4 color;
	glm::vec1 smoothness;
	glm::vec1 metallic;
	glm::vec1 AO;
	glm::vec1 specular;
};


class BasePBRMaterial :
	public IVulkanMaterial
{
public:
	BasePBRMaterial();
	virtual void CreateDescriptorSet();
	virtual void CreateDescriptorBuffer();
	virtual void UpdateDescriptorSet();

protected:
	BatchBufferObject ubo;
};


