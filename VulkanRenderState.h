#pragma once
#include "Common.h"
class VulkanRenderState
{
public:
	VulkanRenderState();
	int GetMaxFlightFrame() {
		return maxFlightFrame;
	}
	int GetCurrentFrame()
	{
		return currentFrame;
	}

	void Update()
	{
		currentFrame = (currentFrame + 1) % maxFlightFrame;
	}
private:
	int currentFrame;
	int maxFlightFrame;
};

