#pragma once

#include <glm/glm.hpp>

#include "gloom/shader.hpp"
#include "TextureManager.h"

class Sun
{
public:
	Sun(Gloom::Shader* shader, Gloom::Shader* shader_Corona, GLTexture texture, GLTexture texture_corona);
	~Sun();

	void draw(glm::mat4 * perspView);
	
	void setModelMatrix(glm::mat4* modelMatrix);

private:

	glm::mat4 _modelMatrix;

	Gloom::Shader* _shader;
	unsigned int _vaoID;
	unsigned int _indexCount;
	GLTexture _tex;

	Gloom::Shader* _shader_corona;
	unsigned int _vaoID_corona;
	unsigned int _indexCount_corona;
	GLTexture _tex_corona;

};

