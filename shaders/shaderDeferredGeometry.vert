#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

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
    gl_Position = global.proj * global.view * batch.model * vec4(inPosition, 1.0);
    //gl_Position = vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}