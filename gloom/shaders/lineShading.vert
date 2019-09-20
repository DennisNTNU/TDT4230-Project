#version 450 core

in layout(location = 0) highp vec3 vertexPosition;
in layout(location = 1) highp vec4 vertexColor;

out highp vec4 fragmentColor;

uniform layout(location = 2) highp mat4 modelMatrix;
uniform layout(location = 3) highp mat4 perspectiveView;

void main(){

	gl_Position = perspectiveView * modelMatrix * vec4(vertexPosition, 1.0);
	fragmentColor = vertexColor;
}