#version 430 core

in layout(location = 0) vec3 vertexPosition;
in layout(location = 1) vec4 vertexColor;
in layout(location = 2) vec3 vertexNormal;

out vec4 fragmentColor;

uniform layout(location = 3) mat4 modelMatrix;
uniform layout(location = 4) mat4 perspectiveView;

void main(){

	gl_Position = perspectiveView * modelMatrix * vec4(vertexPosition, 1.0);
	fragmentColor = vertexColor;
}