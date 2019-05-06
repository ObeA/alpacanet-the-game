#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inViewVec;
layout (location = 4) in vec3 inLightVec;
layout (location = 5) in vec4 inShadowCoord;

layout(location = 0) out vec4 outFragColor;

#define ambient 0.1

void main() {
    vec3 textureColor = texture(texSampler, inTexCoord).rgb;

    //float shadow = textureProj(inShadowCoord / inShadowCoord.w, vec2(0.0));

    vec3 N = normalize(inNormal);
    vec3 L = normalize(inLightVec);
    vec3 V = normalize(inViewVec);
    vec3 R = normalize(-reflect(L, N));
    vec3 diffuse = max(dot(N, L), ambient) * inColor;
    //	vec3 specular = pow(max(dot(R, V), 0.0), 50.0) * vec3(0.75);

    outFragColor = vec4(diffuse * textureColor, 1.0);
}