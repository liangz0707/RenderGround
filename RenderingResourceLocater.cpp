#include "RenderingResourceLocater.h"

IVulkanGraphicPipeline*  RenderingResourceLocater::pipeline_postprocess = (0);
IVulkanGraphicPipeline* RenderingResourceLocater::pipeline_forward = 0;
IVulkanGraphicPipeline* RenderingResourceLocater::pipeline_deferred_geometry = 0;
IVulkanGraphicPipeline* RenderingResourceLocater::pipeline_deferred_lighting = 0;

IVulkanRenderPass* RenderingResourceLocater::pass_deferred = 0;
IVulkanRenderPass* RenderingResourceLocater::pass_forward = 0;
IVulkanRenderPass* RenderingResourceLocater::pass_default = 0;

IVulkanPipelineLayout* RenderingResourceLocater::pipeline_layout = 0;

VulkanSceneManager* RenderingResourceLocater::vulkanSceneManager = 0;

VulkanSampler* RenderingResourceLocater::sampler = 0;

GlobalRenderData* RenderingResourceLocater::global_render_data = 0;