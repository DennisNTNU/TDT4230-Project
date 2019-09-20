#pragma once
#include <glm/glm.hpp>

#include "gloom/shader.hpp"
#include "TextureManager.h"

class TexturedPlane
{
public:
	TexturedPlane(Gloom::Shader* shader, GLTexture texture);
	~TexturedPlane();

	void draw(glm::mat4* perspView);

	void translate(glm::vec3 displace);
private:
	void initVAO();

	glm::mat4 _modelMatrix;

	Gloom::Shader* _shader;

	unsigned int _vaoID;
	GLTexture _tex;
};

