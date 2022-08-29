#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in uint textureID;

layout (location = 0) out vec2 fragUV;
layout (location = 1) out flat uint fragTextureID;

layout (push_constant) uniform Push{
    mat4 viewTransform;
} push;

layout (set = 0, binding = 0) uniform sampler2D textures[1];

void main(){
	fragUV = uv;
	fragTextureID = textureID;
    gl_Position = push.viewTransform * vec4(position, 0.0, 1.0);
}