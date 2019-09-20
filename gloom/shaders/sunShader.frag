#version 460 core

in vec2 fragmentUV;

out vec4 color;

uniform layout(binding = 0) sampler2D mySampler;

void main(){
	color = 1.5*texture(mySampler, fragmentUV);
}