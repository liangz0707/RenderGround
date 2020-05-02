#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(set = 0,binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(set = 1,binding = 1) uniform PUniformBufferObject {
    mat4 model;
    vec4 CameraInfo;
    vec4 ScreenInfo;
} pubo;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;


void main() {
    gl_Position = ubo.proj * ubo.view * pubo.model * vec4(inPosition, 1.0);
    //gl_Position = vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}