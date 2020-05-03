#pragma once
#include "VulkanApplication.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanModel.h"
#include "DeferredGeomtryPipeline.h"
#include "DeferredLightingPipeline.h"
#include "ForwardPipeline.h"
#include "PostprocessPipeline.h"
#include "VulkanPipelineLayout.h"
#include "DeferredRenderPass.h"
#include "ForwardRenderPass.h"
#include "VulkanSampler.h"
#include "GlobalRenderData.h"
#include "ToScreenPipeline.h"
#include "IVulkanDescriptor.h"
#include "BatchDescriptor.h"
#include "GlobalDescriptor.h"
#include "DeferredLightingDescriptor.h"
#include "PostDescriptor.h"
#include "ForwardLightingDescriptor.h"
#include "IVulkanPipelineLayout.h"

class DeferredGeomtryPipeline;
class ForwardPipeline;
class DeferredLightingPipeline;
class PostprocessPipeline;
class VulkanPipelineLayout;
class VulkanSceneManager;
class IVulkanRenderPass;
class VulkanSampler;
class IVulkanDescriptor;

class RenderingResourceLocater
{
public:
    static IVulkanGraphicPipeline* get_pipeline_deferred_geometry() { return pipeline_deferred_geometry; }
    static IVulkanGraphicPipeline* get_pipeline_postprocess() { return pipeline_postprocess; }
    static IVulkanGraphicPipeline* get_pipeline_deferred_lighting() { return pipeline_deferred_lighting; }
    static IVulkanGraphicPipeline* get_pipeline_forward() { return pipeline_forward; }
    static IVulkanGraphicPipeline* get_pipeline_to_screen() { return pipeline_to_screen; }
    

    static IVulkanRenderPass* get_pass_deferred() { return pass_deferred; }
    static IVulkanRenderPass* get_pass_forward() { return pass_forward; }
    static IVulkanRenderPass* get_pass_default() { return pass_default; }

    static IVulkanDescriptor* get_descriptor_batch() { return descriptor_batch; }
    static IVulkanDescriptor* get_descriptor_post() { return descriptor_post; }
    static IVulkanDescriptor* get_descriptor_global() { return descriptor_global; }
    static IVulkanDescriptor* get_descriptor_forward() { return descriptor_forward; }
    static IVulkanDescriptor* get_descriptor_deferred() { return descriptor_deferred; }

    static IVulkanPipelineLayout* get_pipeline_layouts_deferred() { return pipeline_layouts_deferred; }

    static VulkanPipelineLayout* get_layout() { return pipeline_layout; }

    static VulkanSampler* get_sampler() { return sampler; }

    static VulkanSceneManager* get_scene_manager() { return vulkanSceneManager; }

    static GlobalRenderData* get_global_render_data() { return global_render_data; }


    static void provide(IVulkanPipelineLayout* service)
    {
        pipeline_layouts_deferred = service;
    }

    static void provide(VulkanSampler* service)
    {
        sampler = service;
    }

    static void provide(GlobalRenderData* service)
    {
        global_render_data = service;
    }

    static void provide(DeferredGeomtryPipeline* service)
    {
        pipeline_deferred_geometry = service;
    }

    static void provide(DeferredLightingPipeline* service)
    {
        pipeline_deferred_lighting = service;
    }

    static void provide(ForwardPipeline* service)
    {
        pipeline_forward = service;
    }

    static void provide(PostprocessPipeline* service)
    {
        pipeline_postprocess = service;
    }

    static void provide(ToScreenPipeline* service)
    {
        pipeline_to_screen = service;
    }

    static void provide(VulkanPipelineLayout* service)
    {
        pipeline_layout = service;
    }

    static void provide(VulkanSceneManager* service)
    {
        vulkanSceneManager = service;
    }

    static void provide(DeferredRenderPass* service)
    {
        pass_deferred = service;
    }

    static void provide(ForwardRenderPass* service)
    {
        pass_forward = service;
    }

    /*                                 */

    static void provide(BatchDescriptor* service)
    {
        descriptor_batch = service;
    }


    static void provide(GlobalDescriptor* service)
    {
        descriptor_global = service;
    }

    static void provide(DeferredLightingDescriptor* service)
    {
        descriptor_deferred = service;
    }

    static void provide(ForwardLightingDescriptor* service)
    {
        descriptor_forward = service;
    }

    static void provide(PostDescriptor* service)
    {
        descriptor_post = service;
    }

private:
    static BatchDescriptor* descriptor_batch;
    static PostDescriptor* descriptor_post;
    static ForwardLightingDescriptor* descriptor_forward;
    static DeferredLightingDescriptor* descriptor_deferred;
    static GlobalDescriptor* descriptor_global;

    static IVulkanPipelineLayout* pipeline_layouts_deferred; 

    static IVulkanGraphicPipeline* pipeline_postprocess;
    static IVulkanGraphicPipeline* pipeline_forward;
    static IVulkanGraphicPipeline* pipeline_deferred_geometry;
    static IVulkanGraphicPipeline* pipeline_deferred_lighting;
    static IVulkanGraphicPipeline* pipeline_to_screen;
    
    static IVulkanRenderPass* pass_deferred;
    static IVulkanRenderPass* pass_forward;
    static IVulkanRenderPass* pass_default;
    
    static VulkanPipelineLayout* pipeline_layout;

    static VulkanSceneManager* vulkanSceneManager;

    static VulkanSampler* sampler;

    static GlobalRenderData* global_render_data;
};

