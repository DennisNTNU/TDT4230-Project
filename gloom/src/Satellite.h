#pragma once

#include <../vendor/glm/glm/glm.hpp>
#include <iostream>

#include "gloom/shader.hpp"
#include "TextureManager.h"
#include "CoordinateSystem.h"
#include "Orbit.h"
#include "RotDyn.h"

class Satellite
{
public:
	Satellite(Gloom::Shader* shaderOrbit, Gloom::Shader* shaderSat, GLTexture texture, double eccentricity, double semiMajorAxis);
	~Satellite();
	void draw(glm::mat4 * perspView, glm::vec3* lightPos, bool showOrbitDetails);

	
	void orientOrbit(double longitudeOfAscendingNode, double inclination, double argumentOfPeriapsis);
	void setOrbitParameters(double newEccentricity, double newSemiMajorAxis);
	void setOrbitParamFromPeri(double periapsisDistance, double periapsisSpeed);
	void increasePeriSpeed(double factor);
	void updateSatellite(double dt, glm::f64vec3 centralMassPos);
	void setDesiredQuat(double q0, double q1, double q2, double q3);

	void printSatInfo()
	{
		std::cout << orbit.stepCount << "; " << orbit.finalError << " ; " << orbit._time << " ; " << orbit._semiMajorAxis << " ; " << orbit._eccentricity << " ; " << orbit._trueAnomaly << std::endl;
	}


	glm::f64vec3 getSatPosition();
	Orbit orbit;

	RotDyn _rotDyn;

private:
	void drawOrbit(glm::mat4 * perspView);
	void drawSatellite(glm::mat4 * perspView, glm::vec3* lightPos);
	void drawDetails(glm::mat4 * perspView);

	void calcOrbitModelMatrix();
	void calcSatModelMatrix();

	void initVAOOrbit();
	void initVAOSat();
	void initVAODetails();

	CoordinateSystem* _orbitFrame;
	CoordinateSystem* _orbitFrameSat;
	CoordinateSystem* _bodyFrame;
	CoordinateSystem* _debugFrame;

	Gloom::Shader* _shaderOrbit;
	Gloom::Shader* _shaderSat;

	glm::f64vec3 _centralMassPosition;
	glm::f64vec3 _globalSatPosition;

	glm::f64mat4 _modelMatrixOrbit;
	glm::f64mat4 _modelMatrixSat;

	GLTexture _satTex;

	unsigned int _vaoIDOrbit;
	unsigned int _vboIDs_Orbit[2];
	unsigned int _vertexCount;

	unsigned int _vaoIDSat;
	unsigned int _vboIDs_Sat[4];
	unsigned int _indexCount;

	unsigned int _vaoIDDetails;
	unsigned int _vboIDs_Details[2];
	unsigned int _vertexCount_LOAN;
	unsigned int _vertexCount_INC;
	unsigned int _vertexCount_AOP;
};

