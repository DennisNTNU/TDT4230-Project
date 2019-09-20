#version 460 core

in layout(location = 0) vec3 vertexPosition;
in layout(location = 1) vec2 vertexUV;

out vec2 fragmentUV;

uniform layout(location = 2) mat4 modelMatrix;
uniform layout(location = 3) mat4 perspectiveView;

void main(){

	gl_Position = perspectiveView * modelMatrix * vec4(vertexPosition, 1.0);
	fragmentUV = vertexUV;
}