#version 450

layout (location = 0) in vec2 fragUV;
layout (location = 1) in flat uint fragTextureID;

layout (location = 0) out vec4 outColor;

layout (push_constant) uniform Push{
    mat4 viewTransform;
} push;

layout (set = 0, binding = 0) uniform sampler2D textures[1];


void main(){
	outColor = vec4(1.0, 1.0, 1.0, 1.0);
}