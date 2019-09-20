#include "Earth.h"

#include <glm/gtc/matrix_transform.hpp>

#include "SphereGeometry.h"

Earth::Earth(Gloom::Shader* shader, GLTexture texture)
{
	_shader = shader;
	_tex1_day = texture;
	_tex2_night = texture;
	_modelMatrix = glm::mat4(1.0);
	initVAOSphere(&_vaoID, &_indexCount, 48, 64, 6378.0);

	_orbit._time = 0.0;
	_orbit._trueAnomaly = 0.0;
	_orbit._semiMajorAxis = 149598023.0;
	_orbit._eccentricity = 0.0167086;
	_orbit._centralMass = 1.98855e30;
	_orbit.orientOrbit(0.0, 0.0, 3.98660533);

}

Earth::~Earth()
{
}

void Earth::draw(glm::mat4 * perspView, glm::vec3* sunPos, glm::vec3* camPos)
{
	_shader->activate();

	glBindVertexArray(_vaoID);
	// https://open.gl/textures 
	// tutorial for multiple textures
	glUniformMatrix4fv(3, 1, GL_FALSE, &(_modelMatrix[0][0]));
	glUniformMatrix4fv(4, 1, GL_FALSE, &((*perspView)[0][0]));
	glUniform3f(5, sunPos->x, sunPos->y, sunPos->z);
	glUniform3f(6, camPos->x, camPos->y, camPos->z);
	glUniform1f(7, float(_orbit._time));

	/*
	glActiveTexture(_tex1.glTexID);
	glBindTexture(GL_TEXTURE_2D, _tex1.id);
	glUniform1i(5, _tex1.glTexID - GL_TEXTURE0);

	glActiveTexture(_tex2.glTexID);
	glBindTexture(GL_TEXTURE_2D, _tex2.id);
	glUniform1i(6, _tex2.glTexID - GL_TEXTURE0);
	*/
	glBindTextureUnit(0, _tex1_day.id);
	glBindTextureUnit(1, _tex2_night.id);
	glBindTextureUnit(2, _tex3_clouds.id);
	glBindTextureUnit(3, _tex4_water.id);
	glBindTextureUnit(4, _tex5_height.id);
	

	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_SHORT, 0);

	_shader->deactivate();
}

void Earth::addNightsideTexture(GLTexture texture)
{
	_tex2_night = texture;
}

void Earth::addCloudTexture(GLTexture texture)
{
	_tex3_clouds = texture;
}

void Earth::addWaterMaskTexture(GLTexture texture)
{
	_tex4_water = texture;
}

void Earth::addHeightTexture(GLTexture texture)
{
	_tex5_height = texture;
}

glm::mat4 Earth::update(double dt)
{
	_orbit.update(dt);

	/*
	_modelMatrix = glm::translate(glm::mat4(1.0f), earthPos);
	_modelMatrix = glm::rotate(_modelMatrix, float(2.0 * 3.141592653589793238462 * _orbit._time / 86164.0), glm::vec3(0.0, 0.0, 1.0));
	
	_modelMatrix = glm::rotate(glm::mat4(1.0f), float(2.0 * 3.141592653589793238462 * _orbit._time / 86164.0), glm::vec3(0.0, 0.0, 1.0));
	_modelMatrix = glm::translate(_modelMatrix, earthPos);
	*/
	_modelMatrix = glm::rotate(glm::mat4(1.0f), float(2.0 * 3.141592653589793238462 * _orbit._time / 86164.0), glm::vec3(0.0, 0.0, 1.0));

	glm::vec3 relSunPos = -getPosition();
	glm::mat4 sunModelMatrix = glm::rotate(glm::mat4(1.0f), 23.4f * 3.14159265f / 180.0f, glm::vec3(1.0, 0.0, 0.0));
	sunModelMatrix = glm::translate(sunModelMatrix, relSunPos);
	return sunModelMatrix;
}

glm::f64vec3 Earth::getPosition()
{
	//return glm::f64vec3(_modelMatrix[3].x, _modelMatrix[3].y, _modelMatrix[3].z);
	return 0.002*_orbit.getGlobalSatPosition();
}
