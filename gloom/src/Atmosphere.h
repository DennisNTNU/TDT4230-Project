#pragma once
#include <glm/glm.hpp>
#include "gloom/shader.hpp"
#include "TextureManager.h"

class Atmosphere
{
public:
	Atmosphere(Gloom::Shader* shader);
	~Atmosphere();

	void draw(glm::mat4 * view, glm::mat4 * projection, glm::vec3* camPos, glm::vec3* sunPos, glm::vec3* earthPos);

	glm::mat4 _modelMatrix;

private:

	void initVAO();

	Gloom::Shader* _shader;

	unsigned int _vaoID;
	unsigned int _numIndices;
};

