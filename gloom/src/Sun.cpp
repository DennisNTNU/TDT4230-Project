#include "Sun.h"

#include "SphereGeometry.h"


Sun::Sun(Gloom::Shader* shader, Gloom::Shader* shader_Corona, GLTexture texture, GLTexture texture_corona)
{
	double sunRadius = 0.01*695508.0;
	_shader = shader;
	_shader_corona = shader_Corona;
	_tex = texture;
	_tex_corona = texture_corona;
	_modelMatrix = glm::mat4(1.0);
	initVAOSphereNoNormals(&_vaoID, &_indexCount, 16, 24, sunRadius);

	_indexCount_corona = 6;
	unsigned char indices[6] = { 0, 1, 2,  1, 3, 2 };
	double scaleAdjust = 2.18;
	double vertexPoses[3 * 4] = { -scaleAdjust*sunRadius,-scaleAdjust*sunRadius, 0.0,  scaleAdjust*sunRadius,-scaleAdjust*sunRadius, 0.0,
								  -scaleAdjust*sunRadius, scaleAdjust*sunRadius, 0.0,  scaleAdjust*sunRadius, scaleAdjust*sunRadius, 0.0 };
	double vertexUVs[2 * 4] = {0.0, 0.0,  1.0, 0.0, 0.0, 1.0, 1.0, 1.0};

	glGenVertexArrays(1, &_vaoID_corona);
	glBindVertexArray(_vaoID_corona);

	unsigned int vboIDs[3];
	glGenBuffers(3, vboIDs); // indices, positions, UVs

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount_corona*sizeof(unsigned char), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(double), vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(double), vertexUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), 0);
	glEnableVertexAttribArray(1);
}


Sun::~Sun()
{
}

void Sun::draw(glm::mat4 * perspView)
{
	_shader->activate();

	glBindVertexArray(_vaoID);

	glUniformMatrix4fv(2, 1, GL_FALSE, &(_modelMatrix[0][0]));
	glUniformMatrix4fv(3, 1, GL_FALSE, &((*perspView)[0][0]));

	glBindTextureUnit(0, _tex.id);

	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_SHORT, 0);

	_shader->deactivate();

	_shader_corona->activate();

	glBindVertexArray(_vaoID_corona);

	glUniformMatrix4fv(2, 1, GL_FALSE, &(_modelMatrix[0][0]));
	glUniformMatrix4fv(3, 1, GL_FALSE, &((*perspView)[0][0]));

	glBindTextureUnit(0, _tex_corona.id);

	glDrawElements(GL_TRIANGLES, _indexCount_corona, GL_UNSIGNED_BYTE, 0);

	_shader_corona->deactivate();
}

void Sun::setModelMatrix(glm::mat4 * modelMatrix)
{
	_modelMatrix = *modelMatrix;
}
