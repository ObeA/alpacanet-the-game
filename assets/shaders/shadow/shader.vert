#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout (binding = 0) uniform UBO
{
    mat4 depthMVP;
} ubo;
 
void main() {
	gl_Position = ubo.depthMVP * vec4(inPosition, 1.0);
}