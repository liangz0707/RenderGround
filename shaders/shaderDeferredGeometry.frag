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


layout(set = 2,binding = 0) uniform texture2D texturec;

layout(location = 0) out vec4 GbufferA;
layout(location = 1) out vec4 GbufferB;
layout(location = 2) out vec4 GbufferC;
layout(location = 3) out vec4 GbufferD;
layout(location = 4) out vec4 GbufferE;

void main() {
    GbufferA = texture(texSampler, fragTexCoord);
    GbufferB = vec4(1,0,0,0.8);
    GbufferC = vec4(0,1,0,0.4);
    GbufferD = vec4(1,1,0,0.5);
    GbufferE = vec4(0,0,1,0.3);
}