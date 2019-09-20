#pragma once
#include <glm/glm.hpp>

#include "gloom/shader.hpp"

class Plane
{
public:
	Plane(Gloom::Shader* shader);
	~Plane();

	void draw(glm::mat4* perspView);
private:
	void initVAO();

	glm::mat4 _modelMatrix;

	Gloom::Shader* _shader;

	unsigned int _vaoID;
};

