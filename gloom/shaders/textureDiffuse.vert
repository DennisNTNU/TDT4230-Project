#version 450 core

in layout(location = 0) vec3 vertexPosition;
in layout(location = 1) vec3 vertexNormal;
in layout(location = 2) vec2 vertexUV;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec2 fragmentUV;

uniform layout(location = 3) mat4 modelMatrix;
uniform layout(location = 4) mat4 perspectiveView;
uniform layout(location = 5) float satScale;

void main(){

	vec4 globalPos = modelMatrix * vec4(vertexPosition, 1.0);
	gl_Position = perspectiveView * globalPos;

	fragmentPosition = globalPos.xyz;
	fragmentNormal = vertexNormal;
	fragmentUV = vertexUV;
}