#include "Satellite.h"

#include <iostream>
#include <vector>
#include <../vendor/glm/glm/gtc/matrix_transform.hpp>
#include "RotDyn.h"


Satellite::Satellite(Gloom::Shader* shaderOrbit, Gloom::Shader* shaderSat, GLTexture texture, double eccentricity, double semiMajorAxis)
{
	_orbitFrame = new CoordinateSystem(shaderOrbit, 200, 200, 200);
	_orbitFrameSat = new CoordinateSystem(shaderOrbit, 20, 200, 200);
	_bodyFrame = new CoordinateSystem(shaderOrbit);//, 200, 20, 200);
	_debugFrame = new CoordinateSystem(shaderOrbit);//, 200, 20, 200);

	orbit._time = 0.0;
	orbit._trueAnomaly = 0.4;
	orbit._semiMajorAxis = semiMajorAxis;
	orbit._eccentricity = eccentricity;


	_vaoIDSat = 0;
	_vaoIDOrbit = 0;
	_vaoIDDetails = 0;
	_shaderOrbit = shaderOrbit;
	_shaderSat = shaderSat;

	_modelMatrixOrbit = glm::f64mat4(1.0);
	_modelMatrixSat = glm::f64mat4(1.0);

	_satTex = texture;

	_rotDyn = RotDyn();

	initVAOOrbit();
	initVAOSat();
}

Satellite::~Satellite()
{
	delete _orbitFrame;
	delete _orbitFrameSat;
	delete _bodyFrame;
	delete _debugFrame;
}

void Satellite::orientOrbit(double longitudeOfAscendingNode, double inclination, double argumentOfPeriapsis)
{
	orbit.orientOrbit(longitudeOfAscendingNode, inclination, argumentOfPeriapsis);

	calcOrbitModelMatrix();

	initVAODetails();
}

void Satellite::setOrbitParameters(double newEccentricity, double newSemiMajorAxis)
{
	orbit.setOrbitParameters(newEccentricity, newSemiMajorAxis);
	initVAOOrbit();
}

void Satellite::setOrbitParamFromPeri(double periapsisDistance, double periapsisSpeed)
{
	orbit.setOrbitParamFromPeri(periapsisDistance, periapsisSpeed);
	initVAOOrbit();
}

void Satellite::increasePeriSpeed(double factor)
{
	orbit.increasePeriSpeed(factor);
	initVAOOrbit();
}

void Satellite::updateSatellite(double dt, glm::f64vec3 centralMassPos)
{
	orbit.update(dt);
	_rotDyn.update(dt);
	_centralMassPosition = centralMassPos;
	calcOrbitModelMatrix();
}

void Satellite::setDesiredQuat(double q0, double q1, double q2, double q3)
{
	_rotDyn.setDesiredQuat(q0, q1, q2, q3);
}

glm::f64vec3 Satellite::getSatPosition()
{
	return orbit.getGlobalSatPosition() + _centralMassPosition;
}

void Satellite::draw(glm::mat4 * perspView, glm::vec3* lightPos, bool showOrbitDetails)
{
	if (showOrbitDetails) {
		drawOrbit(perspView);
		drawDetails(perspView);
	}
	drawSatellite(perspView, lightPos);
}

void Satellite::drawOrbit(glm::mat4 * perspView)
{
	_shaderOrbit->activate();

	glBindVertexArray(_vaoIDOrbit);
	glm::mat4 modelMat_float = _modelMatrixOrbit;

	glUniformMatrix4fv(2, 1, GL_FALSE, &(modelMat_float[0][0]));
	glUniformMatrix4fv(3, 1, GL_FALSE, &((*perspView)[0][0]));

	if (orbit.getEcc() < 1.0)
	{
		glDrawArrays(GL_LINE_LOOP, 0, _vertexCount);
	}
	else
	{
		glDrawArrays(GL_LINE_STRIP, 0, _vertexCount);
	}

	_shaderOrbit->deactivate();
}

void Satellite::drawSatellite(glm::mat4 * perspView, glm::vec3* lightPos)
{
	_shaderSat->activate();

	glBindTextureUnit(0, _satTex.id);

	glBindVertexArray(_vaoIDSat);
	glm::mat4 modelMatSat_float = _modelMatrixSat;

	glUniformMatrix4fv(3, 1, GL_FALSE, &(modelMatSat_float[0][0]));
	glUniformMatrix4fv(4, 1, GL_FALSE, &((*perspView)[0][0]));
	glUniform1f(5, 200.0f);
	glUniform3f(6, lightPos->x, lightPos->y, lightPos->z);

	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_BYTE, 0);

	_shaderSat->deactivate();
}

void Satellite::drawDetails(glm::mat4 * perspView)
{

	glm::mat4 modelMat_float = _modelMatrixOrbit;
	_orbitFrame->setModelMatrix(&modelMat_float);
	_orbitFrame->draw(perspView);

	_orbitFrameSat->draw(perspView);

	glm::mat4 scale = glm::mat4(1.0);
	float scalef = 0.002f;
	scale[0][0] = scalef;
	scale[1][1] = scalef;
	scale[2][2] = scalef; 

	modelMat_float = _modelMatrixSat;
	modelMat_float = modelMat_float*scale;
	_bodyFrame->setModelMatrix(&modelMat_float);
	_bodyFrame->draw(perspView);

	double distance = orbit.getSMA() * (1.0 - orbit.getEcc() * orbit.getEcc()) / (1.0 + orbit.getEcc() * cos(orbit.getTA()));
	glm::f64vec4 globalSatPos = _modelMatrixOrbit * glm::f64vec4(distance*cos(orbit.getTA()), distance*sin(orbit.getTA()), 0.0, 1.0);
	modelMat_float = glm::translate(glm::f64mat4(1.0f), glm::f64vec3(globalSatPos.x, globalSatPos.y, globalSatPos.z));
	modelMat_float = modelMat_float*scale;
	_debugFrame->setModelMatrix(&modelMat_float);
	_debugFrame->draw(perspView);

	_shaderOrbit->activate();

	glBindVertexArray(_vaoIDDetails);

	modelMat_float = glm::translate(glm::mat4(1.0f), glm::vec3(_centralMassPosition));
	glUniformMatrix4fv(2, 1, GL_FALSE, &(modelMat_float[0][0]));
	glUniformMatrix4fv(3, 1, GL_FALSE, &((*perspView)[0][0]));

	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINE_STRIP, 2, _vertexCount_LOAN - 2);

	glDrawArrays(GL_LINES, _vertexCount_LOAN, 4);
	glDrawArrays(GL_LINE_STRIP, _vertexCount_LOAN + 4, _vertexCount_INC - 4);

	glDrawArrays(GL_LINE_STRIP, _vertexCount_LOAN + _vertexCount_INC, _vertexCount_AOP + 1);
	
	_shaderOrbit->deactivate();
}

void Satellite::calcOrbitModelMatrix()
{
	_modelMatrixOrbit = glm::translate(glm::f64mat4(1.0), _centralMassPosition);
	_modelMatrixOrbit = glm::rotate(_modelMatrixOrbit, orbit.getLOAN(), glm::f64vec3(0.0, 0.0, 1.0));
	_modelMatrixOrbit = glm::rotate(_modelMatrixOrbit, orbit.getInc(), glm::f64vec3(1.0, 0.0, 0.0));
	_modelMatrixOrbit = glm::rotate(_modelMatrixOrbit, orbit.getAOP(), glm::f64vec3(0.0, 0.0, 1.0));
	calcSatModelMatrix();
}

void Satellite::calcSatModelMatrix()
{
	double distance = orbit.getSMA() * (1.0 - orbit.getEcc() * orbit.getEcc()) / (1.0 + orbit.getEcc() * cos(orbit.getTA()));
	glm::f64vec4 globalSatPos = _modelMatrixOrbit * glm::f64vec4(distance*cos(orbit.getTA()), distance*sin(orbit.getTA()), 0.0, 1.0);

	//glm::f64vec4 q = glm::f64vec4(_rotDyn.quaternion_[0], _rotDyn.quaternion_[1], _rotDyn.quaternion_[2], _rotDyn.quaternion_[3]);
	//glm::f64mat4(1.0f)

	_modelMatrixSat = glm::translate(glm::f64mat4(1.0f), glm::f64vec3(globalSatPos.x, globalSatPos.y, globalSatPos.z))*glm::mat4_cast(_rotDyn.quaternion_);
}

void Satellite::initVAOOrbit()
{
	/* distinguish between two cases: e < 1 and e > 1
	 * for e < 1 orbit is elliptic or circular 
	 * a position is defined for all angles and function is continuous
	 * for e > 1 orbit is hyperbolic
	 * a position is undefined and discontinuous
	 * at angles acos(-1/e) and 2 pi - acos(-1/e)
	 */
	std::vector<double> vertexPoses2;

	double twopi = 2.0*3.141592653589793238462;
	double maxAngle = twopi;
	double true_anomaly = 0.0;

	if (orbit.getEcc() > 1.0)
	{
		maxAngle = 0.99 * acos(-1.0 / orbit.getEcc());
		true_anomaly = - maxAngle;
	}
	
	
	double currentDistance = 0.0;
	double deltaAngle = twopi / 384.0;

	while (true_anomaly < maxAngle)
	{
		currentDistance = orbit.getSMA() * (1.0 - orbit.getEcc()*orbit.getEcc()) / (1.0 + orbit.getEcc() * cos(true_anomaly));

		vertexPoses2.push_back(currentDistance * cos(true_anomaly));
		vertexPoses2.push_back(currentDistance * sin(true_anomaly));
		vertexPoses2.push_back(0.0);

		true_anomaly += deltaAngle / (1.0 + abs(orbit.getEcc()) * 63.0 * pow(2.718281828, - (true_anomaly - twopi / 2.0) * (true_anomaly - twopi / 2.0)));
	}

	_vertexCount = int(vertexPoses2.size() / 3);

	unsigned char* vertexColors = new unsigned char[4 * _vertexCount];
	for (unsigned int i = 0; i < _vertexCount; i++)
	{
		vertexColors[4 * i + 0] = 255;
		vertexColors[4 * i + 1] = 255;
		vertexColors[4 * i + 2] = 0;
		vertexColors[4 * i + 3] = 255;
	}

	if (_vaoIDOrbit == 0)
	{
		glGenVertexArrays(1, &_vaoIDOrbit);
	}
	else
	{
		glDeleteBuffers(2, _vboIDs_Orbit);
	}

	glBindVertexArray(_vaoIDOrbit);
	glGenBuffers(2, _vboIDs_Orbit);

	glBindBuffer(GL_ARRAY_BUFFER, _vboIDs_Orbit[0]);
	glBufferData(GL_ARRAY_BUFFER, 3* _vertexCount*sizeof(double), &(vertexPoses2[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3*sizeof(double), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vboIDs_Orbit[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * _vertexCount*sizeof(unsigned char), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4 * sizeof(unsigned char), 0);
	glEnableVertexAttribArray(1);

	delete[] vertexColors;
}

void Satellite::initVAOSat()
{
	const int vertexCount = 24;

	double vertexPoses[3 * vertexCount] =
	{
		 1.0, 1.0, 2.0,  1.0,-1.0, 2.0,  1.0,-1.0,-2.0,  1.0, 1.0,-2.0,
		-1.0, 1.0, 2.0,  1.0, 1.0, 2.0,  1.0, 1.0,-2.0, -1.0, 1.0,-2.0,
		-1.0,-1.0, 2.0, -1.0, 1.0, 2.0, -1.0, 1.0,-2.0, -1.0,-1.0,-2.0,
		 1.0,-1.0, 2.0, -1.0,-1.0, 2.0, -1.0,-1.0,-2.0,  1.0,-1.0,-2.0,
		-1.0, 1.0, 2.0, -1.0,-1.0, 2.0,  1.0,-1.0, 2.0,  1.0, 1.0, 2.0,
		 1.0, 1.0,-2.0,  1.0,-1.0,-2.0, -1.0,-1.0,-2.0, -1.0, 1.0,-2.0
	};

	double vertexNormals[3 * vertexCount] =
	{
		 1.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 0.0,
		 0.0, 1.0, 0.0,  0.0, 1.0, 0.0,  0.0, 1.0, 0.0,  0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
		 0.0,-1.0, 0.0,  0.0,-1.0, 0.0,  0.0,-1.0, 0.0,  0.0,-1.0, 0.0,
		 0.0, 0.0, 1.0,  0.0, 0.0, 1.0,  0.0, 0.0, 1.0,  0.0, 0.0, 1.0,
		 0.0, 0.0,-1.0,  0.0, 0.0,-1.0,  0.0, 0.0,-1.0,  0.0, 0.0,-1.0
	};

	double vertexUVs[2 * vertexCount] =
	{
		1.0, 0.0,  0.0, 0.0,  0.0, 2.0,  1.0, 2.0,
		1.0, 0.0,  0.0, 0.0,  0.0, 2.0,  1.0, 2.0,
		1.0, 0.0,  0.0, 0.0,  0.0, 2.0,  1.0, 2.0,
		1.0, 0.0,  0.0, 0.0,  0.0, 2.0,  1.0, 2.0,
		1.0, 0.0,  0.0, 0.0,  0.0, 1.0,  1.0, 1.0,
		1.0, 0.0,  0.0, 0.0,  0.0, 1.0,  1.0, 1.0
	};

	const int triangleCount = 12;
	_indexCount = triangleCount * 3;
	unsigned char indices[triangleCount * 3] =
	{
		 0, 1, 2,  0, 2, 3,
		 4, 5, 6,  4, 6, 7,
		 8, 9,10,  8,10,11,
		12,13,14, 12,14,15,
		16,17,18, 16,18,19,
		20,21,22, 20,22,23
	};

	if (_vaoIDSat == 0)
	{
		glGenVertexArrays(1, &_vaoIDSat);
	}
	else
	{
		glDeleteBuffers(4, _vboIDs_Sat);
	}
	glBindVertexArray(_vaoIDSat);

	glGenBuffers(4, _vboIDs_Sat);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIDs_Sat[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * sizeof(unsigned char), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vboIDs_Sat[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(double), vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vboIDs_Sat[2]);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(double), vertexNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_TRUE, 3 * sizeof(double), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, _vboIDs_Sat[3]);
	glBufferData(GL_ARRAY_BUFFER, 2 * vertexCount * sizeof(double), vertexUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_DOUBLE, GL_TRUE, 2 * sizeof(double), 0);
	glEnableVertexAttribArray(2);
}

void Satellite::initVAODetails()
{
	// 128 verts for a complete circle
	// 128 * _longOfAscNode / (2 * pi) verts for a circle arc of subtending angle _longOfAscNode
	
	int vertexCountArc_LOAN = int(128.0 * orbit.getLOAN() / (2.0 * 3.14159265358979)) + 1;
	_vertexCount_LOAN = 2 + vertexCountArc_LOAN + 1;

	int vertexCountArc_INC = int(128.0 * orbit.getInc() / (2.0 * 3.14159265358979)) + 1;
	_vertexCount_INC = 4 + vertexCountArc_INC + 1;

	_vertexCount_AOP = int(128.0 * orbit.getAOP() / (2.0 * 3.14159265358979)) + 1;

	int totalVertexCount = _vertexCount_LOAN + _vertexCount_INC + _vertexCount_AOP + 1;


	double* vertexPoses = new double[3 * totalVertexCount];
	unsigned char* vertexColors = new unsigned char[4 * totalVertexCount];

	vertexPoses[0] = 0.0; vertexPoses[1] = 0.0; vertexPoses[2] = 0.0;
	vertexPoses[3] = 2.0*6378.0 * cos(orbit.getLOAN()); vertexPoses[4] = 2.0*6378.0 * sin(orbit.getLOAN()); vertexPoses[5] = 0.0;

	vertexColors[0] = 250; vertexColors[1] = 0; vertexColors[2] = 0; vertexColors[3] = 255;
	vertexColors[4] = 250; vertexColors[5] = 0; vertexColors[6] = 0; vertexColors[7] = 255;

	int vertexIndex = 6;
	int vertexIndexColor = 8;

	double angleResolution = (2.0 * 3.14159265358979) / 128.0;
	for (int i = 0; i < vertexCountArc_LOAN; i++)
	{

		vertexPoses[vertexIndex++] = 1.5*6378.0 * cos(i * angleResolution);
		vertexPoses[vertexIndex++] = 1.5*6378.0 * sin(i * angleResolution);
		vertexPoses[vertexIndex++] = 0.0;

		vertexColors[vertexIndexColor++] = 250;
		vertexColors[vertexIndexColor++] = 0;
		vertexColors[vertexIndexColor++] = 0;
		vertexColors[vertexIndexColor++] = 255;
	}

	vertexPoses[vertexIndex++] = 1.5*6378.0 * cos(orbit.getLOAN());
	vertexPoses[vertexIndex++] = 1.5*6378.0 * sin(orbit.getLOAN());
	vertexPoses[vertexIndex++] = 0.0;

	vertexColors[vertexIndexColor++] = 250;
	vertexColors[vertexIndexColor++] = 0;
	vertexColors[vertexIndexColor++] = 0;
	vertexColors[vertexIndexColor++] = 255;


	double inc_arc_radius = 1.5 * 6378.0;
	vertexPoses[vertexIndex++] = 0.0; vertexPoses[vertexIndex++] = 0.0; vertexPoses[vertexIndex++] = 0.0;
	vertexPoses[vertexIndex++] = - 1.3 * inc_arc_radius * sin(orbit.getLOAN()); vertexPoses[vertexIndex++] = 1.3 * inc_arc_radius * cos(orbit.getLOAN()); vertexPoses[vertexIndex++] = 0.0;
	vertexPoses[vertexIndex++] = 0.0; vertexPoses[vertexIndex++] = 0.0; vertexPoses[vertexIndex++] = 0.0;
	vertexPoses[vertexIndex++] = - 1.3 *inc_arc_radius * sin(orbit.getLOAN()) * cos(orbit.getInc()); vertexPoses[vertexIndex++] = 1.3 * inc_arc_radius * cos(orbit.getLOAN()) * cos(orbit.getInc()); vertexPoses[vertexIndex++] = 1.3 * inc_arc_radius * sin(orbit.getInc());

	vertexColors[vertexIndexColor++] = 0; vertexColors[vertexIndexColor++] = 250; vertexColors[vertexIndexColor++] = 0; vertexColors[vertexIndexColor++] = 255;
	vertexColors[vertexIndexColor++] = 0; vertexColors[vertexIndexColor++] = 250; vertexColors[vertexIndexColor++] = 0; vertexColors[vertexIndexColor++] = 255;
	vertexColors[vertexIndexColor++] = 0; vertexColors[vertexIndexColor++] = 250; vertexColors[vertexIndexColor++] = 0; vertexColors[vertexIndexColor++] = 255;
	vertexColors[vertexIndexColor++] = 0; vertexColors[vertexIndexColor++] = 250; vertexColors[vertexIndexColor++] = 0; vertexColors[vertexIndexColor++] = 255;

	for (int i = 0; i < vertexCountArc_INC; i++)
	{
		vertexPoses[vertexIndex++] = -inc_arc_radius * sin(orbit.getLOAN()) * cos(i * angleResolution);
		vertexPoses[vertexIndex++] = inc_arc_radius * cos(orbit.getLOAN()) * cos(i * angleResolution);
		vertexPoses[vertexIndex++] = inc_arc_radius * sin(i * angleResolution);

		vertexColors[vertexIndexColor++] = 0;
		vertexColors[vertexIndexColor++] = 250;
		vertexColors[vertexIndexColor++] = 0;
		vertexColors[vertexIndexColor++] = 255;
	}

	vertexPoses[vertexIndex++] = -inc_arc_radius * sin(orbit.getLOAN()) * cos(orbit.getInc());
	vertexPoses[vertexIndex++] = inc_arc_radius * cos(orbit.getLOAN()) * cos(orbit.getInc());
	vertexPoses[vertexIndex++] = inc_arc_radius * sin(orbit.getInc());

	vertexColors[vertexIndexColor++] = 0;
	vertexColors[vertexIndexColor++] = 250;
	vertexColors[vertexIndexColor++] = 0;
	vertexColors[vertexIndexColor++] = 255;


	double aop_arc_radius = 1.5*6378.0;
	for (unsigned int i = 0; i < _vertexCount_AOP; i++)
	{
		double temp = i * angleResolution;

		vertexPoses[vertexIndex++] = aop_arc_radius * (cos(orbit.getLOAN()) * cos(temp) - sin(orbit.getLOAN()) * cos(orbit.getInc()) * sin(temp));
		vertexPoses[vertexIndex++] = aop_arc_radius * (sin(orbit.getLOAN()) * cos(temp) + cos(orbit.getLOAN()) * cos(orbit.getInc()) * sin(temp));
		vertexPoses[vertexIndex++] = aop_arc_radius * (sin(orbit.getInc()) * sin(temp));

		vertexColors[vertexIndexColor++] = 250;
		vertexColors[vertexIndexColor++] = 50;
		vertexColors[vertexIndexColor++] = 255;
		vertexColors[vertexIndexColor++] = 255;
	}

	vertexPoses[vertexIndex++] = aop_arc_radius * (cos(orbit.getLOAN()) * cos(orbit.getAOP()) - sin(orbit.getLOAN()) * cos(orbit.getInc()) * sin(orbit.getAOP()));
	vertexPoses[vertexIndex++] = aop_arc_radius * (sin(orbit.getLOAN()) * cos(orbit.getAOP()) + cos(orbit.getLOAN()) * cos(orbit.getInc()) * sin(orbit.getAOP()));
	vertexPoses[vertexIndex++] = aop_arc_radius * (sin(orbit.getInc()) * sin(orbit.getAOP()));

	vertexColors[vertexIndexColor++] = 250;
	vertexColors[vertexIndexColor++] = 50;
	vertexColors[vertexIndexColor++] = 255;
	vertexColors[vertexIndexColor++] = 255;

	if (_vaoIDDetails == 0)
	{
		glGenVertexArrays(1, &_vaoIDDetails);
	}
	else
	{
		glDeleteBuffers(2, _vboIDs_Details);
	}
	glBindVertexArray(_vaoIDDetails);

	glGenBuffers(2, _vboIDs_Details);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vboIDs_Details[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(double) * totalVertexCount, vertexPoses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vboIDs_Details[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(unsigned char) * totalVertexCount, vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4 * sizeof(unsigned char), 0);
	glEnableVertexAttribArray(1);

	delete[] vertexPoses;
	delete[] vertexColors;
}
