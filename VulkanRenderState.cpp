#include "VulkanRenderState.h"

VulkanRenderState::VulkanRenderState()
{
	maxFlightFrame = MAX_FRAMES_IN_FLIGHT;
	currentFrame = 0;
}
