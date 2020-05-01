#version 450

#extension GL_ARB_separate_shader_objects : enable
layout(early_fragment_tests) in;


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(set = 1,binding = 0) uniform sampler2D texSampler;
layout(set = 1,binding = 1) uniform PUniformBufferObject {
    vec4 CameraInfo;
    vec4 ScreenInfo;
} pubo;
//layout(set = 2,binding = 0) uniform sampler2D Gbuffer;

layout(input_attachment_index = 0, set = 2, binding = 0) uniform  subpassInput A;
layout(input_attachment_index = 1, set = 2, binding = 1) uniform  subpassInput B;
layout(input_attachment_index = 2, set = 2, binding = 2) uniform  subpassInput C;
layout(input_attachment_index = 3, set = 2, binding = 3) uniform  subpassInput D;


layout(location = 0) out vec4 LightingResult;

void main() {


   LightingResult = subpassLoad(B);

}