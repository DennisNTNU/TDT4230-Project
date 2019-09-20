#version 450 core

in layout(location = 0) vec3 vertexPosition;
in layout(location = 1) vec3 vertexNormal;
in layout(location = 2) vec2 vertexUV;

uniform layout(location = 3) mat4 modelMatrix;
uniform layout(location = 4) mat4 perspectiveView;

layout(binding = 4) uniform sampler2D heightTexture;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec2 fragmentUV;

void main()
{

	vec3 globalPos = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
	vec3 globalNormal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;

	globalPos += 100 * (texture(heightTexture, vertexUV).x - 0.1f) * globalNormal;

	gl_Position = perspectiveView * vec4(globalPos, 1.0);

	fragmentPosition = globalPos;
	fragmentNormal = globalNormal;
	fragmentUV = vertexUV;
}