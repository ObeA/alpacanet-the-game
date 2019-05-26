#version 450

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inColor;
layout (location = 2) in float inAlpha;
layout (location = 3) in float inSize;
layout (location = 4) in float inRotation;

layout (location = 0) out vec4 outColor;
layout (location = 1) out float outAlpha;
layout (location = 2) out flat int outType;
layout (location = 3) out float outRotation;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 depthBias;
    mat4 lightPos;
} ubo;

out gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
};

void main () 
{
	outColor = inColor;
	outAlpha = inAlpha;
	outRotation = inRotation;
	  
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPos);	
	
	// Base size of the point sprites
	float spriteSize = 8.0 * inSize;

	// Scale particle size depending on camera projection
	vec4 eyePos = (ubo.view * ubo.model) * vec4(inPos.xyz, 1.0);
	vec4 projectedCorner = ubo.proj * vec4(0.5 * spriteSize, 0.5 * spriteSize, eyePos.z, eyePos.w);
	gl_PointSize = 10 * projectedCorner.x / projectedCorner.w;	
}