#include "VulkanTestRendering.h"

VulkanTestRendering::VulkanTestRendering()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();

	vulkanPipelineResource = new VulkanPipelineResource();
	vulkanRenderPass = new VulkanRenderPass(vulkanPipelineResource);



	vulkanCommandBuffer = new VulkanFrameRenderCommandBuffer(vulkanPipelineResource->GetCommandPool(), commandBufferSize);


	for (int i = 0; i < commandBufferSize; i++)
	{
		VkCommandBuffer commandBuffer = vulkanCommandBuffer->VulkanCommandBegin(i);
		;
		Render(commandBuffer,
				RM->GetFramebuffer()->GetFrameBufferByIndex(i),
				RM->GetFramebuffer()->GetFrameBufferExtent(),
				vulkanRenderPass->GetDescriptorSetByIndex(i));
		
		vulkanCommandBuffer->VulkanCommandEnd(i);
	}
}

void VulkanTestRendering::Render(VkCommandBuffer commandBuffer,
	VkFramebuffer frameBuffer,
	VkExtent2D extend,
	VkDescriptorSet descriptorSet
)
{
	// 这里才是真正的把renderPass和swapchaing中的Framebuffer联系起来
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = vulkanRenderPass->GetInstance();
	// framebuffer当中的attachment数量需要和renderPass当中的对应。
	renderPassInfo.framebuffer = frameBuffer;

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = extend;

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// 指定这个RenderPass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// 绑定其中的一个subpass的graphicsPipeline的状态。
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanRenderPass->GetGraphicPipeline()->GetInstance());

	for (auto staticModel : vulkanSceneManager->GetStaticModel())
	{
		VkBuffer vertexBuffers[] = { staticModel->GetVertexBuffer() };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, staticModel->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanRenderPass->GetPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);
		//vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(staticModel->GetIndexSize()), 1, 0, 0, 0);

	}
	
	vkCmdEndRenderPass(commandBuffer);
	
}
