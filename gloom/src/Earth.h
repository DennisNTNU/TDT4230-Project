#pragma once
#include <glm/glm.hpp>
#include "gloom/shader.hpp"
#include "TextureManager.h"
#include "Orbit.h"

class Earth
{
public:
	Earth(Gloom::Shader* shader, GLTexture texture);
	~Earth();

	void draw(glm::mat4 * perspView, glm::vec3* sunPos, glm::vec3* camPos);
	void addNightsideTexture(GLTexture texture);
	void addCloudTexture(GLTexture texture);
	void addWaterMaskTexture(GLTexture texture);
	void addHeightTexture(GLTexture texture);

	glm::mat4 update(double dt);

	glm::f64vec3 getPosition();
	double getTime() { return _orbit._time; }
	double getPeriod() { return _orbit.getPeriod(); }

private:

	Gloom::Shader* _shader;

	Orbit _orbit;

	glm::mat4 _modelMatrix;

	unsigned int _vaoID;
	unsigned int _indexCount;
	GLTexture _tex1_day;
	GLTexture _tex2_night;
	GLTexture _tex3_clouds;
	GLTexture _tex4_water;
	GLTexture _tex5_height;
};

