#include "DeferredPipelineLayouts.h"
#include "BatchDescriptor.h"
#include "RenderingResourceLocater.h"

void DeferredPipelineLayouts::CreatePipelineLayout()
{

	VkDescriptorSetLayout batch = RenderingResourceLocater::get_descriptor_batch()->getSetLayout();
	VkDescriptorSetLayout global = RenderingResourceLocater::get_descriptor_global()->getSetLayout();
	VkDescriptorSetLayout post = RenderingResourceLocater::get_descriptor_post()->getSetLayout();
	VkDescriptorSetLayout deferred = RenderingResourceLocater::get_descriptor_deferred()->getSetLayout();
	VkDescriptorSetLayout forward = RenderingResourceLocater::get_descriptor_forward()->getSetLayout();


	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();


	/*Gbuffer Geometroy*/
	VkPipelineLayout vkPipelineLayout_DeferredGeometry;
	int deferredGeometryLayoutsCount = 2;
	VkDescriptorSetLayout deferredGeometryLayouts[2] =
	{
		batch,
		global
	};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo0 = {};
	pipelineLayoutInfo0.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo0.setLayoutCount = deferredGeometryLayoutsCount; // Optional
	pipelineLayoutInfo0.pSetLayouts = deferredGeometryLayouts; // Optional
	pipelineLayoutInfo0.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo0.pPushConstantRanges = nullptr; // Optional


	RM->createPipelineLayout(&pipelineLayoutInfo0, &vkPipelineLayout_DeferredGeometry);


	/*Gbuffer Lighting*/
	VkPipelineLayout vkPipelineLayout_DeferredLighting;
	int deferredLightingLayoutsCount = 2;
	VkDescriptorSetLayout deferredLightingLayouts[2] =
	{
		deferred,
		global
	};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo1 = {};
	pipelineLayoutInfo1.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo1.setLayoutCount = deferredLightingLayoutsCount; // Optional
	pipelineLayoutInfo1.pSetLayouts = deferredLightingLayouts; // Optional
	pipelineLayoutInfo1.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo1.pPushConstantRanges = nullptr; // Optional


	RM->createPipelineLayout(&pipelineLayoutInfo1, &vkPipelineLayout_DeferredLighting);

	/*Forward Lighting*/
	VkPipelineLayout vkPipelineLayout_ForwardLighting;
	int forwardLayoutsCount = 3;
	VkDescriptorSetLayout forwardLayouts[3] =
	{
		forward,
		batch,
		global
	};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo2 = {};
	pipelineLayoutInfo2.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo2.setLayoutCount = forwardLayoutsCount; // Optional
	pipelineLayoutInfo2.pSetLayouts = forwardLayouts; // Optional
	pipelineLayoutInfo2.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo2.pPushConstantRanges = nullptr; // Optional


	RM->createPipelineLayout(&pipelineLayoutInfo2, &vkPipelineLayout_ForwardLighting);

	/*Post Lighting*/
	VkPipelineLayout vkPipelineLayout_Post;
	int postLayoutsCount = 2;
	VkDescriptorSetLayout postLayouts[2] =
	{
		post,
		global
	};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo3 = {};
	pipelineLayoutInfo3.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo3.setLayoutCount = postLayoutsCount; // Optional
	pipelineLayoutInfo3.pSetLayouts = postLayouts; // Optional
	pipelineLayoutInfo3.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo3.pPushConstantRanges = nullptr; // Optional

	RM->createPipelineLayout(&pipelineLayoutInfo3, &vkPipelineLayout_Post);

	/*ToScreen Lighting*/
	VkPipelineLayout vkPipelineLayout_ToScreen;
	int toScreenLayoutsCount = 2;
	VkDescriptorSetLayout toScreenLayouts[2] =
	{
		post,
		global
	};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo4 = {};
	pipelineLayoutInfo4.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo4.setLayoutCount = toScreenLayoutsCount; // Optional
	pipelineLayoutInfo4.pSetLayouts = toScreenLayouts; // Optional
	pipelineLayoutInfo4.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo4.pPushConstantRanges = nullptr; // Optional

	RM->createPipelineLayout(&pipelineLayoutInfo4, &vkPipelineLayout_ToScreen);

	layouts.clear();
	layouts.push_back(vkPipelineLayout_DeferredGeometry);
	layouts.push_back(vkPipelineLayout_DeferredLighting);
	layouts.push_back(vkPipelineLayout_ForwardLighting);
	layouts.push_back(vkPipelineLayout_Post);
	layouts.push_back(vkPipelineLayout_ToScreen);
}
