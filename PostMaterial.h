#pragma once
#include "IVulkanMaterial.h"
#include "RenderingResourceLocater.h"
struct PostBufferObject {
	glm::vec4 postColor;
};

class PostMaterial :
	public IVulkanMaterial
{
public:
	PostMaterial();
	virtual void CreateDescriptorSet();
	virtual void CreateDescriptorBuffer();
	virtual void UpdateDescriptorSet();

protected:

	PostBufferObject ubo;
};

