#pragma once
#include "IVulkanMaterial.h"
#include "RenderingResourceLocater.h"

struct GlobalBufferObject {
	glm::mat4 view;
	glm::mat4 proj;
};

class GlobalMaterial :
	public IVulkanMaterial
{
public:
	GlobalMaterial();
	virtual void CreateDescriptorSet();
	virtual void CreateDescriptorBuffer();
	virtual void UpdateDescriptorSet();

protected:

	GlobalBufferObject ubo;
};

