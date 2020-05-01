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


layout(location = 0) out vec4 GbufferA;

void main() {
    GbufferA = vec4(0.5, 0.5, 0.5, 0.5);
}