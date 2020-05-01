%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderDeferredGeometry.vert -o shaders/Compiled/DeferredGeometry_vert.spv
%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderDeferredGeometry.frag -o shaders/Compiled/DeferredGeometry_frag.spv

%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderForward.vert -o shaders/Compiled/Forward_vert.spv
%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderForward.frag -o shaders/Compiled/Forward_frag.spv

%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderToScreen.vert -o shaders/Compiled/ToScreen_vert.spv
%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderToScreen.frag -o shaders/Compiled/ToScreen_frag.spv

%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderDeferredLighting.vert -o shaders/Compiled/DeferredLighting_vert.spv
%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderDeferredLighting.frag -o shaders/Compiled/DeferredLighting_frag.spv

%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderPostprocessing.vert -o shaders/Compiled/Postprocessing_vert.spv
%VK_SDK_PATH%/Bin/glslc.exe shaders/shaderPostprocessing.frag -o shaders/Compiled/Postprocessing_frag.spv

