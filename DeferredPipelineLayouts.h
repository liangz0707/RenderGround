#pragma once
#include "IVulkanPipelineLayout.h"

#include "BatchDescriptor.h"
#include "GlobalDescriptor.h"
#include "DeferredLightingDescriptor.h"
#include "PostDescriptor.h"
#include "ForwardLightingDescriptor.h"

class DeferredPipelineLayouts :
	public IVulkanPipelineLayout
{
public:
	virtual void CreatePipelineLayout() ;
};

