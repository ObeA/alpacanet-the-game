#version 450

layout (location = 0) in vec4 inColor;
layout (location = 1) in float inAlpha;

layout (location = 0) out vec4 outFragColor;

void main () 
{
	outFragColor.a = inAlpha;
	outFragColor.rgb = inColor.rgb;	
}