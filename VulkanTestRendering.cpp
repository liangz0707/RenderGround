#include "VulkanTestRendering.h"

VulkanTestRendering::VulkanTestRendering()
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();
	size_t commandBufferSize = RM->GetFramebuffer()->GetFrameBufferSize();

	vulkanPipelineResource = new VulkanPipelineResource();
	//vulkanRenderPass = new VulkanRenderPass(RM)



	vulkanCommandBuffer = new VulkanFrameRenderCommandBuffer(vulkanPipelineResource->GetCommandPool(), commandBufferSize);


	for (int i = 0; i < commandBufferSize; i++)
	{
		VkCommandBuffer commandBuffer = vulkanCommandBuffer->VulkanCommandBegin(i);
		;
		Render(commandBuffer,
				RM->GetFramebuffer()->GetFrameBufferByIndex(i),
			RM->GetFramebuffer()->GetFrameBufferExtent());
		
		vulkanCommandBuffer->VulkanCommandEnd(i);
	}
}

void VulkanTestRendering::Render(VkCommandBuffer commandBuffer,
	VkFramebuffer frameBuffer,
	VkExtent2D extend
)
{
	/*
	// ������������İ�renderPass��swapchaing�е�Framebuffer��ϵ����
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	// framebuffer���е�attachment������Ҫ��renderPass���еĶ�Ӧ��
	renderPassInfo.framebuffer = frameBuffer;

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = extend;

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// ָ�����RenderPass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// �����е�һ��subpass��graphicsPipeline��״̬��
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
	//vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);
	*/
}