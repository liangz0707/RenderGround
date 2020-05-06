#include "RenderingResourceLocater.h"

IVulkanGraphicPipeline* RenderingResourceLocater::pipeline_postprocess = (0);
IVulkanGraphicPipeline* RenderingResourceLocater::pipeline_forward = 0;
IVulkanGraphicPipeline* RenderingResourceLocater::pipeline_deferred_geometry = 0;
IVulkanGraphicPipeline* RenderingResourceLocater::pipeline_deferred_lighting = 0;
IVulkanGraphicPipeline* RenderingResourceLocater::pipeline_to_screen = 0;


IVulkanRenderPass* RenderingResourceLocater::pass_deferred = 0;
IVulkanRenderPass* RenderingResourceLocater::pass_forward = 0;
IVulkanRenderPass* RenderingResourceLocater::pass_default = 0;

VulkanPipelineLayout* RenderingResourceLocater::pipeline_layout = 0;

VulkanSceneManager* RenderingResourceLocater::vulkanSceneManager = 0;

VulkanSampler* RenderingResourceLocater::sampler = 0;

GlobalRenderData* RenderingResourceLocater::global_render_data = 0;

BatchDescriptor* RenderingResourceLocater::descriptor_batch = 0;
PostDescriptor* RenderingResourceLocater::descriptor_post = 0;
ForwardLightingDescriptor* RenderingResourceLocater::descriptor_forward = 0;
DeferredLightingDescriptor* RenderingResourceLocater::descriptor_deferred = 0;
GlobalDescriptor* RenderingResourceLocater::descriptor_global = 0;
ScreenDescriptor* RenderingResourceLocater::descriptor_screen = 0;

IVulkanPipelineLayout* RenderingResourceLocater::pipeline_layouts_deferred = 0;