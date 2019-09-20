#include "Plane.h"



Plane::Plane(Gloom::Shader* shader)
{
	_shader = shader;
	_modelMatrix = glm::mat4(1.0);
	initVAO();
}


Plane::~Plane()
{
}

void Plane::draw(glm::mat4 * perspView)
{
	_shader->activate();

	glBindVertexArray(_vaoID);

	glUniformMatrix4fv(3, 1, GL_FALSE, &(_modelMatrix[0][0]));
	glUniformMatrix4fv(4, 1, GL_FALSE, &((*perspView)[0][0]));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);


	_shader->deactivate();
}

void Plane::initVAO()
{
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	unsigned int vboIDs[4];
	glGenBuffers(4, vboIDs);
	


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



	unsigned char vertexColors[4 * 4];
	for (int i = 0; i < 16; i++) {
		vertexColors[i] = 200;
	}

	vertexColors[3] = 255;
	vertexColors[7] = 255;
	vertexColors[11] = 255;
	vertexColors[15] = 255;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 16*sizeof(unsigned char), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4*sizeof(unsigned char), 0);
	glEnableVertexAttribArray(1);



	double vertexNormals[4 * 3];
	vertexPoses[0] = 0.0; vertexPoses[1] = 0.0; vertexPoses[2] = 1.0;
	vertexPoses[3] = 0.0; vertexPoses[4] = 0.0; vertexPoses[5] = 1.0;
	vertexPoses[6] = 0.0; vertexPoses[7] = 0.0; vertexPoses[8] = 1.0;
	vertexPoses[9] = 0.0; vertexPoses[10] = 0.0; vertexPoses[11] = 1.0;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(double), vertexNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_DOUBLE, GL_FALSE, 3*sizeof(double), 0);
	glEnableVertexAttribArray(2);

	delete[] indices;
}
