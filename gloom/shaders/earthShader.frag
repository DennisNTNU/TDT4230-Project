#version 450 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec2 fragmentUV;

uniform layout(location = 3) mat4 modelMatrix;
uniform layout(location = 5) vec3 sunPos;
uniform layout(location = 6) vec3 camPos;
uniform layout(location = 7) float time;

layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D albedoNight;
layout(binding = 2) uniform sampler2D cloud;
layout(binding = 3) uniform sampler2D waterMask;

out vec4 color;

void main()
{
	
	vec3 sunDir = normalize(sunPos - fragmentPosition);

	// Calualting Surface color
	vec3 viewDir = normalize(camPos - fragmentPosition);
	vec3 reflectDir = normalize(2.0*dot(sunDir, fragmentNormal)*fragmentNormal - sunDir);
	vec4 sunColor = vec4(1.0, 0.9, 0.7, 0.0);
	float specular = 1.0 * texture(waterMask, fragmentUV).z * min(pow(min(max(0.0, dot(reflectDir, viewDir)), 1.0), 32.0), 1.0);
	vec4 daySurfaceColor = clamp(texture(albedo, fragmentUV) + specular*sunColor, 0.0, 1.0);



	// Calculating cloud shadows
	float cosAngleSun = dot(sunDir, fragmentNormal);
	float mag = (cosAngleSun - 1.0) / 200.0;
	vec3 side = normalize(cross(fragmentNormal, sunDir));
	vec3 sideSide = normalize(cross(fragmentNormal, side));
	float shadowDirLongitudinal = side.z;
	float shadowDirLatitudinal = sideSide.z;
	float angle = 20.0 * time / 86164.0;
	vec4 cloudShadow = texture(cloud, vec2(fragmentUV.x - angle - mag * shadowDirLongitudinal, fragmentUV.y - 5.0* mag*shadowDirLatitudinal));
	vec4 clouds = texture(cloud, vec2(fragmentUV.x - angle, fragmentUV.y));
	vec4 daySurfaceShadow = vec4(-cloudShadow.xyz*0.3 + daySurfaceColor.xyz*(1.0 - clouds.x), 1.0);

	// Adding clouds
	vec4 daySideColor = vec4(clouds.xyz*1.3 + daySurfaceShadow.xyz, 1.0);

	// Night side color with clouds
	vec4 nightSideColor = 0.6*(texture(albedoNight, fragmentUV) - clouds*0.5);

	// Factor used to choose between dayside color or nightside color
	float a = 1.0 / (1.0 + pow(2.718281828, -14.7*(cosAngleSun + 0.1)));

	color = vec4(a*daySideColor.xyz + (1.0 - a)*nightSideColor.xyz, 1.0);
}