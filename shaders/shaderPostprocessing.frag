#version 450

#extension GL_ARB_separate_shader_objects : enable
layout(early_fragment_tests) in;


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(set = 0,binding = 0) uniform Post {
    vec4 color;
} post;


layout(input_attachment_index = 0, set = 0, binding = 1) uniform  subpassInput A;

layout(set = 1,binding = 0) uniform Global {
    mat4 view;
    mat4 proj;
    vec4 CameraInfo;
    vec4 ScreenInfo;
} global;

layout(location = 0) out vec4 LightingResult;

void main() {
    LightingResult = vec4(0.5, 0.5, 0.5, 0.5);
}