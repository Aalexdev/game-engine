#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in uint entityIndex;

uniform mat4 u_viewProjectionMatrix;

out vec4 fragColor;
out uint fragEntityIndex;

void main(){
	fragColor = color;
	fragEntityIndex = entityIndex;
	gl_Position = u_viewProjectionMatrix * vec4(position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 outColor;
layout(location = 1) out uint entityID;

in vec4 fragColor;
in flat uint fragEntityIndex;

void main(){
	entityID = fragEntityIndex;
	outColor = vec4(1, 0, 0, 1);
}
