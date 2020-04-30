#include "VulkanDeferredRendering.h"
#include "RenderingResourceLocater.h"

VulkanDeferredRendering::VulkanDeferredRendering()
{
}

void VulkanDeferredRendering::Config(VulkanFrameRenderCommandBuffer* vulkanCommandBuffer)
{
	VulkanResourceManager* RM = VulkanResourceManager::GetResourceManager();

	for (int i = 0; i < vulkanCommandBuffer->GetCommandBufferSize(); i++)
	{
		VkCommandBuffer commandBuffer = vulkanCommandBuffer->VulkanCommandBegin(i);

		Render(commandBuffer,
			RM->GetFramebuffer()->GetFrameBufferByIndex(i),
			RM->GetExtent(),
			i);
		vulkanCommandBuffer->VulkanCommandEnd(i);
	}
}


void VulkanDeferredRendering::Render(VkCommandBuffer commandBuffer,
	VkFramebuffer frameBuffer,
	VkExtent2D extend,
	int i
	)
{
	// 这里才是真正的把renderPass和swapchaing中的Framebuffer联系起来
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = RenderingResourceLocater::get_pass_deferred()->GetInstance();
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
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, RenderingResourceLocater::get_pipeline_deferred_geometry()->GetInstance());

	std::vector<VulkanRModel*> vulkanModels = RenderingResourceLocater::get_scene_manager()->GetStaticModel();
	for (VulkanRModel* staticModel : vulkanModels)
	{
		VkBuffer vertexBuffers[] = { staticModel->GetVertexBuffer() };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, staticModel->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

		// TODO:需要从模型当中取出来。
		VkDescriptorSet descriptorSet[] = {
				//vulkanRenderPass->GetGraphicPipeline()->GetPipelineResource()->GetUniformDescriptorSetByIndex(i),  
				staticModel->GetMaterial()->GetDescriptorSet() };
		int descriptorSetNumber = 2;
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, RenderingResourceLocater::get_layout()->GetInstance(), 0, descriptorSetNumber, descriptorSet, 0, nullptr);

		//vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(staticModel->GetIndexSize()), 1, 0, 0, 0);

	}

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, RenderingResourceLocater::get_pipeline_deferred_lighting()->GetInstance());


	vkCmdEndRenderPass(commandBuffer);

}
