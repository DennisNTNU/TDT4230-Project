#include "TexturedPlane.h"

#include <../vendor/glm/glm/gtc/matrix_transform.hpp>
#include <iostream>

TexturedPlane::TexturedPlane(Gloom::Shader* shader, GLTexture texture)
{
	_shader = shader;
	_modelMatrix = glm::mat4(1.0);
	_modelMatrix = glm::scale(_modelMatrix, glm::vec3(16.0 / 9.0, 1.0, 1.0));
	_modelMatrix = glm::translate(_modelMatrix, glm::vec3(0.0, 0.0, 0.5));
	initVAO();
	_tex = texture;
}


TexturedPlane::~TexturedPlane()
{
}

void TexturedPlane::draw(glm::mat4 * perspView)
{
	_shader->activate();

	glBindVertexArray(_vaoID);
	glBindTexture(GL_TEXTURE_2D, _tex.id);

	glUniformMatrix4fv(2, 1, GL_FALSE, &(_modelMatrix[0][0]));
	glUniformMatrix4fv(3, 1, GL_FALSE, &((*perspView)[0][0]));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

	_shader->deactivate();
}

void TexturedPlane::translate(glm::vec3 displace)
{
	_modelMatrix = glm::translate(_modelMatrix, displace);
}

void TexturedPlane::initVAO()
{
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	unsigned int vboIDs[3];
	glGenBuffers(3, vboIDs);
	


	unsigned char* indices = new unsigned char[6];
	indices[0] = 0; indices[1] = 2; indices[2] = 1;
	indices[3] = 3; indices[4] = 2; indices[5] = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned char), indices, GL_STATIC_DRAW);



	double vertexPoses[4*3];
	vertexPoses[0] = -1.0; vertexPoses[1] = -1.0; vertexPoses[2] = 0.0;
	vertexPoses[3] = -1.0; vertexPoses[4] = 1.0; vertexPoses[5] = 0.0;
	vertexPoses[6] = 1.0; vertexPoses[7] = 1.0; vertexPoses[8] = 0.0;
	vertexPoses[9] = 1.0; vertexPoses[10] = -1.0; vertexPoses[11] = 0.0;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 3*4*sizeof(double), vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3*sizeof(double), 0);
	glEnableVertexAttribArray(0);



	double vertexUVs[4 * 2];
	vertexUVs[0] = 1.0; vertexUVs[1] = 0.0;
	vertexUVs[2] = 1.0; vertexUVs[3] = 1.0;
	vertexUVs[4] = 0.0; vertexUVs[5] = 1.0;
	vertexUVs[6] = 0.0; vertexUVs[7] = 0.0;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(double), vertexUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 2*sizeof(double), 0);
	glEnableVertexAttribArray(1);

	delete[] indices;
}
