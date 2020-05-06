#pragma once
#include "IVulkanMaterial.h"
#include "RenderingResourceLocater.h"



struct DeferredLigtingBufferObject {
	glm::vec4 lightPosition;
	glm::vec4 lightColor;
};


class DeferredLightingMaterial :
	public IVulkanMaterial
{
public:
	DeferredLightingMaterial();
	virtual void CreateDescriptorSet();
	virtual void CreateDescriptorBuffer();
	virtual void UpdateDescriptorSet();

protected:
	DeferredLigtingBufferObject ubo;
};

