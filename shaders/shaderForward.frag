#version 450

#extension GL_ARB_separate_shader_objects : enable
layout(early_fragment_tests) in;


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 LightingResult;

layout(set = 0, binding = 0) uniform  ForwardLighting {
    vec4 lightPosition;
    vec4 lightColor;
}lighting;


layout(set = 1,binding = 0) uniform Batch {
    mat4 model;
} batch;

layout(set = 2,binding = 0) uniform Global {
    mat4 view;
    mat4 proj;
    vec4 CameraInfo;
    vec4 ScreenInfo;
} global;

void main() {
    LightingResult = vec4(1,1,0,0);
 
}