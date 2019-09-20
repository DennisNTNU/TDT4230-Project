#version 460 core

in layout(location=0) vec3 vertexPosition;
in layout(location=1) vec2 vertexUV;

out vec2 fragmentUV;

uniform layout(location = 2) mat4 modelMatrix;
uniform layout(location = 3) mat4 perspectiveView;

void main()
{
	mat3 billboardTransform = mat3(1.0);

	
	// should have been the MV matrix not the VP (or MVP) matrix?
	billboardTransform[0] = normalize(transpose(perspectiveView)[0].xyz);
	billboardTransform[1] = normalize(transpose(perspectiveView)[1].xyz);
	billboardTransform[2] = cross(billboardTransform[0], billboardTransform[1]);

	

	vec4 globalVertexPos = modelMatrix * vec4(0.0, 0.0, 0.0, 1.0) + vec4(billboardTransform * vertexPosition, 0.0);
	//vec4 globalVertexPos = modelMatrix * vec4(vertexPosition, 1.0);

	gl_Position = perspectiveView * (globalVertexPos);
	
	fragmentUV = vertexUV;
}