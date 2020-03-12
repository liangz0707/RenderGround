#pragma once
#include "Common.h"
#include "Utility.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanPipelineResource.h"

class VulkanSwapChain;
class VulkanGraphicPipeline;
class VulkanPipelineResource;

class VulkanRenderPass
{
private:
	VulkanSwapChain* vulkanSwapChain;

	VulkanDevice* vulkanDevice;
	VulkanApplication* vulkanApplication;

	VkRenderPass renderPass;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VulkanGraphicPipeline* vulkanGraphicPipeline;

	VulkanPipelineResource* vulkanipelineResource;

public :
	VulkanRenderPass(VulkanApplication* vulkanApplication,VulkanSwapChain * vulkanSwapChain, VulkanDevice* vulkanDevice);
	VkRenderPass GetInstance();
	void createRenderPass();
	void createGraphicPipelines();
	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSets(std::vector<VkBuffer> uniformBuffers, VkSampler textureSampler, VkImageView textureImageView, std::vector<VkBuffer> preEntityUniformBuffers);

};

