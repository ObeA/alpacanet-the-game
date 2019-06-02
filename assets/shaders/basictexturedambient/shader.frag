#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec2 inTexCoord;
layout (location = 3) in vec3 inFragPos;

layout(location = 0) out vec4 outFragColor;

void main() {
    vec3 textureColor = texture(texSampler, inTexCoord).rgb;

    outFragColor = vec4(textureColor, 1.0);
}