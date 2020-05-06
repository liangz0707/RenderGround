#pragma once
#include "IVulkanMaterial.h"

struct ScreenBufferObject {
	glm::vec4 postColor;
};

class ScreenMaterial :
	public IVulkanMaterial
{
public:
	ScreenMaterial();
	virtual void CreateDescriptorSet();
	virtual void CreateDescriptorBuffer();
	virtual void UpdateDescriptorSet();

protected:

	ScreenBufferObject ubo;
};

