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

class DeferredGeomtryPipeline;
class ForwardPipeline;
class DeferredLightingPipeline;
class PostprocessPipeline;
class IVulkanPipelineLayout;
class VulkanSceneManager;
class IVulkanRenderPass;
class VulkanSampler;

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

    static IVulkanPipelineLayout* get_layout() { return pipeline_layout; }

    static VulkanSampler* get_sampler() { return sampler; }

    static VulkanSceneManager* get_scene_manager() { return vulkanSceneManager; }

    static GlobalRenderData* get_global_render_data() { return global_render_data; }

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

    static void provide(IVulkanPipelineLayout* service)
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

    static void provide(VulkanSampler* service)
    {
        sampler = service;
    }


private:
    static IVulkanGraphicPipeline* pipeline_postprocess;
    static IVulkanGraphicPipeline* pipeline_forward;
    static IVulkanGraphicPipeline* pipeline_deferred_geometry;
    static IVulkanGraphicPipeline* pipeline_deferred_lighting;
    static IVulkanGraphicPipeline* pipeline_to_screen;
    
    static IVulkanRenderPass* pass_deferred;
    static IVulkanRenderPass* pass_forward;
    static IVulkanRenderPass* pass_default;
    
    static IVulkanPipelineLayout* pipeline_layout;

    static VulkanSceneManager* vulkanSceneManager;

    static VulkanSampler* sampler;

    static GlobalRenderData* global_render_data;
};

