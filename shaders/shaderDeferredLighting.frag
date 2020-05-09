#version 450

#extension GL_ARB_separate_shader_objects : enable
layout(early_fragment_tests) in;


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;


layout(input_attachment_index = 0, set = 0, binding = 0) uniform  subpassInput A;
layout(input_attachment_index = 1, set = 0, binding = 1) uniform  subpassInput B;
layout(input_attachment_index = 2, set = 0, binding = 2) uniform  subpassInput C;
layout(input_attachment_index = 3, set = 0, binding = 3) uniform  subpassInput D;

layout(set = 0, binding = 4) uniform  DeferredLighting {
    vec4 lightPosition;
    vec4 lightColor;
}lighting;

layout(set = 1,binding = 0) uniform Global {
    mat4 view;
    mat4 proj;
    vec4 CameraInfo;
    vec4 ScreenInfo;
} global;

layout(location = 0) out vec4 LightingResult;

void main() {


   vec4 A = subpassLoad(A);
   vec4 B = subpassLoad(B);
   vec4 C = subpassLoad(C);
   vec4 D = subpassLoad(D);


   LightingResult = A+B+C+D;

}