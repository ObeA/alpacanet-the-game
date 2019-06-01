#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D shadowSampler;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inViewVec;
layout(location = 3) in vec3 inLightVec;
layout(location = 4) in vec4 inShadowCoord;

layout(location = 0) out vec4 outColor;

#define AMBIENT 0.2

float textureProj(vec4 shadowCoord, vec2 off) {
    float shadow = 1.0;
    if(shadowCoord.z > -1.0 && shadowCoord.z < 1.0) {
        float dist = texture(shadowSampler, shadowCoord.st + off).r;
        if (shadowCoord.w > 0.0 && dist < shadowCoord.z) {
            shadow = AMBIENT;
        }
    }
    return shadow;
}

float filterPCF(vec4 sc) {
    ivec2 texDim = textureSize(shadowSampler, 0);
    float scale = 1.5;
    float dx = scale * 1.0 / float(texDim.x);
    float dy = scale * 1.0 / float(texDim.y);

    float shadowFactor = 0.0;
    int count = 0;
    int range = 1;

    for (int x = -range; x <= range; x++) {
        for (int y = -range; y <= range; y++)
        {
            shadowFactor += textureProj(sc, vec2(dx*x, dy*y));
            count++;
        }

    }

    return shadowFactor / count;
}

void main() {
    // float shadow = textureProj(inShadowCoord / inShadowCoord.w, vec2(0.0));
    float shadow = filterPCF(inShadowCoord / inShadowCoord.w);

    vec3 N = normalize(inNormal);
    vec3 L = normalize(inLightVec);
    vec3 V = normalize(inViewVec);
    vec3 R = normalize(-reflect(L, N));
    vec3 diffuse = max(dot(N, L), AMBIENT) * inColor;

    outColor = vec4(diffuse * shadow, 1.0);
}