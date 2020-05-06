#pragma once
#include "IVulkanMaterial.h"
#include "RenderingResourceLocater.h"

struct ForwardLigtingBufferObject {
	glm::vec4 lightPosition;
	glm::vec4 lightColor;
};

class ForwardLightingMaterial :
	public IVulkanMaterial
{
public:
	ForwardLightingMaterial();
	virtual void CreateDescriptorSet();
	virtual void CreateDescriptorBuffer();
	virtual void UpdateDescriptorSet();

protected:
	ForwardLigtingBufferObject ubo;
};

