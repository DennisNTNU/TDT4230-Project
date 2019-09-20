#pragma once

#include <glm/glm.hpp>

#include "gloom/shader.hpp"
#include "TextureManager.h"
#include "Orbit.h"

class Moon
{
public:
	Moon(Gloom::Shader* shader, GLTexture texture);
	~Moon();

	void draw(glm::mat4 * perspView, glm::vec3* lightPos);
	
	void setModelMatrix(glm::mat4* modelMatrix);
	void update(double dt);

	glm::f64vec3 getPosition();
	double getTime() { return _orbit._time; }
	double getPeriod() { return _orbit.getPeriod(); }

private:

	Gloom::Shader* _shader;

	Orbit _orbit;

	glm::mat4 _modelMatrix;

	unsigned int _vaoID;
	unsigned int _indexCount;
	GLTexture _tex;

};

