#include "CoordinateSystem.h"
#include <glm/gtc/matrix_transform.hpp>

CoordinateSystem::CoordinateSystem(Gloom::Shader * shader)
{
	_singleColor = false;
	_shader = shader;
	_modelMatrix = glm::mat4(1.0);
	initVAO();
}

CoordinateSystem::CoordinateSystem(Gloom::Shader * shader, unsigned char r, unsigned char g, unsigned char b)
{
	_singleColor = true;
	_shader = shader;
	_modelMatrix = glm::mat4(1.0);
	_r = r;
	_g = g;
	_b = b;
	initVAO();
}

CoordinateSystem::~CoordinateSystem()
{
}

void CoordinateSystem::initVAO()
{
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);
	//for a coordinate system, we need, one index buffer, one position buffer, one color buffer.

	unsigned int bufferIDs[3];
	glGenBuffers(3, bufferIDs);

	// indexBuffer
	unsigned char* indices = new unsigned char[6];
	indices[0] = 0; indices[1] = 1;
	indices[2] = 0; indices[3] = 2;
	indices[4] = 0; indices[5] = 3;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned char), indices, GL_STATIC_DRAW);
	delete[] indices;

	// position vertex attribute buffer
	double* vertexPos = new double[3 * 4];
	vertexPos[0] = 0.0; vertexPos[1] = 0.0; vertexPos[2] = 0.0;
	vertexPos[3] = 2.0*6378.0; vertexPos[4] = 0.0; vertexPos[5] = 0.0;
	vertexPos[6] = 0.0; vertexPos[7] = 2.0*6378.0; vertexPos[8] = 0.0;
	vertexPos[9] = 0.0; vertexPos[10] = 0.0; vertexPos[11] = 2.0*6378.0;

	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(double), vertexPos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);
	glEnableVertexAttribArray(0);

	// color vertex attribute buffer
	unsigned char* vertexColors = new unsigned char[4 * 4];
	if (_singleColor) {
		vertexColors[0]  = _r; vertexColors[1]  = _g; vertexColors[2]  = _b; vertexColors[3]  = 255;
		vertexColors[4]  = _r; vertexColors[5]  = _g; vertexColors[6]  = _b; vertexColors[7]  = 255;
		vertexColors[8]  = _r; vertexColors[9]  = _g; vertexColors[10] = _b; vertexColors[11] = 255;
		vertexColors[12] = _r; vertexColors[13] = _g; vertexColors[14] = _b; vertexColors[15] = 255;
	} else {
		vertexColors[0]  = 100; vertexColors[1]  = 100; vertexColors[2]  = 100; vertexColors[3]  = 255;
		vertexColors[4]  = 255; vertexColors[5]  = 0;   vertexColors[6]  = 0;   vertexColors[7]  = 255;
		vertexColors[8]  = 0;   vertexColors[9]  = 255; vertexColors[10] = 0;   vertexColors[11] = 255;
		vertexColors[12] = 0;   vertexColors[13] = 0;   vertexColors[14] = 255; vertexColors[15] = 255;
	}

	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(unsigned char), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4 * sizeof(unsigned char), 0);
	glEnableVertexAttribArray(1);

	delete[] vertexPos;
	delete[] vertexColors;
}

void CoordinateSystem::draw(glm::mat4* perspView)
{
	_shader->activate();

	glBindVertexArray(_vaoID);

	// Uploading the perspectiveView and model matrices
	glUniformMatrix4fv(2, 1, GL_FALSE, &(_modelMatrix[0][0]));
	glUniformMatrix4fv(3, 1, GL_FALSE, &((*perspView)[0][0]));

	// Draw your scene here
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, 0);

	_shader->deactivate();
}

void CoordinateSystem::setModelMatrix(glm::mat4 * modelMatrix)
{
	_modelMatrix = *modelMatrix;
}

void CoordinateSystem::setPosition(glm::vec3 pos)
{
	_modelMatrix = glm::translate(glm::mat4(1.0f), pos);
}
