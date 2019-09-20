#version 450 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec2 fragmentUV;

out vec4 color;

uniform layout(location = 3) mat4 modelMatrix;
uniform layout(location = 6) vec3 lightPos;

uniform layout(binding = 0) sampler2D mySampler;

void main() {

	vec4 lightDir = vec4(normalize(lightPos - fragmentPosition), 0.0);

	vec4 textureColor = texture(mySampler, fragmentUV);

	//float a = pow(max(4.641588834e-4, 1.5*dot((transpose(modelMatrix)*lightDir).xyz, fragmentNormal)), 0.3);

	float a = 1.3 / (1.0 + pow(2.718281828, -14.7*(dot((transpose(modelMatrix)*lightDir).xyz, fragmentNormal) + 0.1)));

	color = vec4(a*textureColor.xyz, textureColor.w);
}