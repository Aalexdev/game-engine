#type vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec4 color;
layout (location = 3) in float thickness;
layout (location = 4) in float fade;
layout (location = 5) in uint entityIndex;

uniform mat4 u_viewProjectionMatrix;

out vec4 fragColor;
out vec2 fragUV;
out float fragThickness;
out float fragFade;
out uint fragEntityIndex;

void main(){
	fragColor = color;
	fragUV = UV;
	fragThickness = thickness;
	fragFade = fade;
	fragEntityIndex = entityIndex;
	gl_Position = u_viewProjectionMatrix * vec4(position, 1.0);
}

#type fragment
#version 330 core

layout (location = 0) out vec4 outColor;
layout (location = 1) out uint entityID;

in vec4 fragColor;
in vec2 fragUV;
in float fragThickness;
in float fragFade;
in flat uint fragEntityIndex;

void main(){
    
    // Calculate distance and fill circle with white
    float d = 1.0 - length(fragUV);
    float a = smoothstep(0.0, fragFade, d);
    a *= smoothstep(fragThickness + fragFade, fragThickness, d);

	if (a == 0.0){
		discard;
	}

    // Set output color
  	outColor = fragColor * a;
	entityID = fragEntityIndex;
} 