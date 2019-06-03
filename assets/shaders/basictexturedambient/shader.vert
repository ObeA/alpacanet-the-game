#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 depthBias;
    vec3 lightPos;
	vec3 viewPos;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec2 outTexCoord;
layout(location = 3) out vec3 outFragPos;

const mat4 biasMat = mat4(
0.5, 0.0, 0.0, 0.0,
0.0, 0.5, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.5, 0.5, 0.0, 1.0 );

void main() {
    outColor = inColor;
    outTexCoord = inTexCoord;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition.xyz, 1.0);
    outFragPos = vec3(ubo.model * vec4(inPosition, 1.0));
}