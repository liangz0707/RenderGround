#version 450

#extension GL_ARB_separate_shader_objects : enable
layout(early_fragment_tests) in;


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 GbufferA;
layout(location = 1) out vec4 GbufferB;
layout(location = 2) out vec4 GbufferC;
layout(location = 3) out vec4 GbufferD;
layout(location = 4) out vec4 GbufferE;


layout(set = 0,binding = 0) uniform Batch {
    mat4 model;
    vec4 color;
    float smoothness;
    float metallic;
    float AO;
    float specular;
} batch;

layout(set = 1,binding = 0) uniform Global {
    mat4 view;
    mat4 proj;
} global;


void main() {
    GbufferA = vec4(1,0,0,0.8);
    GbufferB = vec4(1,0,0,0.8);
    GbufferC = vec4(0,1,0,0.4);
    GbufferD = vec4(1,1,0,0.5);
    GbufferE = vec4(0,0,1,0.3);
}