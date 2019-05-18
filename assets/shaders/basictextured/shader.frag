#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D shadowSampler;
layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inFragPos;
layout (location = 4) in vec3 inLightPos;
layout (location = 5) in vec4 inShadowCoord;
layout (location = 6) in vec3 inViewPos;

layout(location = 0) out vec4 outFragColor;

#define ambient 0.5
float specularStrength = 0.5;
vec3 lightColor = vec3(1.0);

float shadowProj(vec4 shadowCoord, vec2 off)
{
    float shadow = 1.0;
    if ( shadowCoord.z > -1.0 && shadowCoord.z < 1.0 )
    {
        float dist = texture( shadowSampler, shadowCoord.st + off ).r;
        if ( shadowCoord.w > 0.0 && dist < shadowCoord.z )
        {
            shadow = ambient;
        }
    }
    return shadow;
}

void main() {
    vec3 textureColor = texture(texSampler, inTexCoord).rgb;

    // float shadow = shadowProj(inShadowCoord / inShadowCoord.w, vec2(0.0));

	vec3 norm = normalize(inNormal);
	vec3 lightDir = normalize(inLightPos - inFragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(inViewPos - inFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor; 

	vec3 result = (ambient + diffuse + specular) * textureColor;

    outFragColor = vec4(result, 1.0);
}