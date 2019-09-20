#include "Moon.h"

#include <glm/gtc/matrix_transform.hpp>
#include "SphereGeometry.h"


Moon::Moon(Gloom::Shader* shader, GLTexture texture)
{
	
	_shader = shader;
	_tex = texture;
	_modelMatrix = glm::mat4(1.0);
	initVAOSphere(&_vaoID, &_indexCount, 16, 24, 1737.1);

	_orbit._time = 0.0;
	_orbit._trueAnomaly = 0.0;
	_orbit._semiMajorAxis = 384399.0;
	_orbit._eccentricity = 0.0549;
	_orbit._centralMass = 5.97237e24;
	_orbit.orientOrbit(0.0, (5.145 + 23.4)*3.14159265358979 / 180.0, 0.0);
}


Moon::~Moon()
{
}

void Moon::draw(glm::mat4 * perspView, glm::vec3* lightPos)
{
	_shader->activate();

	glBindVertexArray(_vaoID);

	glUniformMatrix4fv(3, 1, GL_FALSE, &(_modelMatrix[0][0]));
	glUniformMatrix4fv(4, 1, GL_FALSE, &((*perspView)[0][0]));
	glUniform1f(5, 1.0f);
	glUniform3f(6, lightPos->x, lightPos->y, lightPos->z);

	glBindTextureUnit(0, _tex.id);

	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_SHORT, 0);

	_shader->deactivate();
}

void Moon::setModelMatrix(glm::mat4 * modelMatrix)
{
	_modelMatrix = *modelMatrix;
}


void Moon::update(double dt)
{
	_orbit.update(dt); 
	glm::vec3 moonPos = getPosition();
	_modelMatrix = glm::translate(glm::mat4(1.0f), moonPos);
	_modelMatrix = glm::rotate(_modelMatrix, float(2.0 * 3.141592653589793238462 * _orbit._time / 2360591.51), glm::vec3(0.0, 0.0, 1.0));
}

glm::f64vec3 Moon::getPosition()
{
	//return glm::f64vec3(_modelMatrix[3].x, _modelMatrix[3].y, _modelMatrix[3].z);
	return 0.1 * _orbit.getGlobalSatPosition();
}