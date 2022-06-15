#type vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec4 color;
layout(location = 3) in uint textureIndex;
layout(location = 4) in uint entityIndex;
layout(location = 5) in float tilingFactor;

uniform mat3 u_viewProjection;

out vec4 fragColor;
out vec2 fragUV;
out float fragTilingFactor;
out uint fragTextureIndex;
out uint fragEntityIndex;

void main(){
    fragColor = color;
    fragUV = UV;
    fragTextureIndex = textureIndex;
    fragTilingFactor = tilingFactor;
	fragEntityIndex = entityIndex;
    gl_Position = vec4(u_viewProjection * vec3(position, 0.0), 0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 outColor;
layout(location = 1) out uint entityID;

uniform sampler2D u_textures[32];

in vec4 fragColor;
in vec2 fragUV;
in float fragTilingFactor;
in flat uint fragTextureIndex;
in flat uint fragEntityIndex;

void main(){
	entityID = fragEntityIndex;
    outColor = texture(u_textures[fragTextureIndex], fragUV * fragTilingFactor) * fragColor;
}