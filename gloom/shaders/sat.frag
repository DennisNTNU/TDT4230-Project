#version 450 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec2 fragmentUV;

out vec4 color;

uniform layout(location = 3) mat4 modelMatrix;
uniform layout(location = 6) vec3 lightPos;

uniform layout(binding = 0) sampler2D mySampler;

void main() {

	vec3 satLightSegment = lightPos - fragmentPosition;
	vec4 lightDir = vec4(normalize(lightPos - fragmentPosition), 0.0);
	vec4 textureColor = texture(mySampler, fragmentUV);

	float a = length(satLightSegment); a = a*a;
	float b = dot(satLightSegment, fragmentPosition);
	float t0 = - b / a;
	vec3 closestPoint = fragmentPosition + t0*satLightSegment;

	float f = 1.0;

	if (t0 > 0)
	{	// sat is behind earth
		f = 0.2 + 1.3 / (1.0 + pow(2.718281828, -14.7*(dot((transpose(modelMatrix)*lightDir).xyz, fragmentNormal) + 0.1)) 
						     + pow(2.718281828, -(length(closestPoint) - 6378.0) / 50));
	}
	else
	{
		f = 0.2 + 1.3 / (1.0 + pow(2.718281828, -14.7*(dot((transpose(modelMatrix)*lightDir).xyz, fragmentNormal) + 0.1)));
	}

	//float f = pow(max(4.641588834e-4, 1.5*dot((transpose(modelMatrix)*lightDir).xyz, fragmentNormal)), 0.3);
	color = vec4(f*textureColor.xyz, textureColor.w);
}



