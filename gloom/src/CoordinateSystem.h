#pragma once
#include <../vendor/glm/glm/glm.hpp>

#include "gloom/shader.hpp"

class CoordinateSystem
{
public:
	CoordinateSystem(Gloom::Shader * shader);
	CoordinateSystem(Gloom::Shader * shader, unsigned char r, unsigned char g, unsigned char b);
	~CoordinateSystem();

	void draw(glm::mat4* perspView);
	void setModelMatrix(glm::mat4* modelMatrix);
	void setPosition(glm::vec3 pos);

private:
	void initVAO();

	bool _singleColor;

	unsigned char _r;
	unsigned char _g;
	unsigned char _b;

	glm::mat4 _modelMatrix;

	Gloom::Shader* _shader;

	unsigned int _vaoID;
};

