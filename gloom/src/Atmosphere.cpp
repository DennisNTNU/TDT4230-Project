#include "Atmosphere.h"



Atmosphere::Atmosphere(Gloom::Shader* shader)
{
	_shader = shader;
	_modelMatrix = glm::mat4(1.0);
	initVAO();
}


Atmosphere::~Atmosphere()
{
}

void Atmosphere::draw(glm::mat4 * view, glm::mat4 * projection, glm::vec3* camPos, glm::vec3* sunPos, glm::vec3* earthPos)
{

	_shader->activate();

	glBindVertexArray(_vaoID);

	glUniformMatrix4fv(1, 1, GL_FALSE, &((*view)[0][0]));
	glUniformMatrix4fv(2, 1, GL_FALSE, &((*projection)[0][0]));
	glUniform3f(3, camPos->x, camPos->y, camPos->z);
	glUniform3f(4, sunPos->x, sunPos->y, sunPos->z);
	glUniform3f(5, earthPos->x, earthPos->y, earthPos->z);
	glUniform1f(6, 6378.0 - 10.0);
	glUniform1f(7, 6478.0 + 150.0);

	glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_BYTE, 0);

	_shader->deactivate();
}

void Atmosphere::initVAO()
{
	double aspect = 16.0 / 9.0;
	_numIndices = 6;
	unsigned char indices[6] = {0, 1, 2,  1, 3, 2};
	double vertexPoses[3*4] = { -0.9*aspect,-0.9, 0.0,  0.9*aspect,-0.9, 0.0,
								-0.9*aspect, 0.9, 0.0,  0.9*aspect, 0.9, 0.0 };

	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	unsigned int vboIDs[2];
	glGenBuffers(2, vboIDs); // indices, positions

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices*sizeof(unsigned char), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(double), vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);
	glEnableVertexAttribArray(0);

}
