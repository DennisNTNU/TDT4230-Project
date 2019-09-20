#version 430 core

in layout(location = 0) vec3 vertexPosition;

out vec3 fragmentPosition;

uniform layout(location = 1) mat4 viewMatrix;
uniform layout(location = 2) mat4 projectionMatrix;

void main(){
	
	mat4 viewPureRotation = viewMatrix;
	viewPureRotation[3] = vec4(0.0, 0.0, 0.0, 1.0);

	vec4 viewPureTranslation = viewMatrix[3];
	vec4 inverseTranslation = - transpose(viewPureRotation) * viewPureTranslation;

	vec3 newVertexPos = 3.8*vertexPosition + vec3(0.0, 0.0, -5.0);

	vec4 globalPosition = transpose(viewPureRotation) * vec4(newVertexPos, 1.0) + inverseTranslation;



	gl_Position = projectionMatrix*vec4(newVertexPos, 1.0);
	fragmentPosition = globalPosition.xyz;
}