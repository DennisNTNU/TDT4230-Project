#include "Orbit.h"

#include <cmath>

#include "glm/gtc/matrix_transform.hpp"


Orbit::Orbit()
{
	_centralMass = 5.97237e24;
}


Orbit::~Orbit()
{
}


void Orbit::orientOrbit(double longitudeOfAscendingNode, double inclination, double argumentOfPeriapsis)
{
	const double PI = 3.141592653589793238462;
	_longOfAscNode = longitudeOfAscendingNode;
	_inclination = inclination;
	_argOfPeriapsis = argumentOfPeriapsis;

	while (abs(_longOfAscNode) > 2.0*PI) {
		_longOfAscNode -= 2.0 * PI * _longOfAscNode / abs(_longOfAscNode);
	}
	while (abs(_inclination) > 2.0*PI) {
		_inclination -= 2.0 * PI * _inclination / abs(_inclination);
	}
	while (abs(_argOfPeriapsis) > 2.0*PI) {
		_argOfPeriapsis -= 2.0 * PI * _argOfPeriapsis / abs(_argOfPeriapsis);
	}
}


void Orbit::setOrbitParameters(double newEccentricity, double newSemiMajorAxis)
{
	_eccentricity = newEccentricity;
	_semiMajorAxis = newSemiMajorAxis;
}


void Orbit::setOrbitParamFromPeri(double periapsisDistance, double periapsisSpeed)
{
	const double earthGravityParam = GRAVITATIONAL_CONST * _centralMass;

	_eccentricity = periapsisDistance * periapsisSpeed * periapsisSpeed / earthGravityParam - 1.0;
	_semiMajorAxis = periapsisDistance / (1.0 - _eccentricity);
}


void Orbit::increasePeriSpeed(double factor)
{
	double temp = factor*factor*(1.0 + _eccentricity);
	_semiMajorAxis = _semiMajorAxis*(1.0 - _eccentricity) / (2.0 - temp);
	_eccentricity = temp - 1.0;
}


void Orbit::update(double dt)
{
	_time += dt;
	double period = getPeriod();
	if (_time > period)
	{
		double timeInPeriods = _time / period;
		_time = period * (timeInPeriods - floor(timeInPeriods));
	}

	const double earthGravityParam = GRAVITATIONAL_CONST * _centralMass;

	const double meanMotion = sqrt(earthGravityParam / abs(_semiMajorAxis*_semiMajorAxis*_semiMajorAxis));

	double meanAnomaly = meanMotion * _time;

	double eccentricAnomaly = calcEccentricAnomaly(meanAnomaly);

	if (_eccentricity < 1.0)
	{ // Elliptic Orbit
		_trueAnomaly = 2.0 * atan2(sqrt((1.0 + _eccentricity) / (1.0 - _eccentricity))*sin(eccentricAnomaly / 2.0), cos(eccentricAnomaly / 2.0));
	}
	else if (_eccentricity > 1.0)
	{ // Hyperbolic Orbit
		_trueAnomaly = 2.0 * atan2(sqrt((_eccentricity + 1.0) / (_eccentricity - 1.0))*sinh(eccentricAnomaly / 2.0), cosh(eccentricAnomaly / 2.0));
	}
}

glm::f64vec3 Orbit::getLocalSatPosition()
{

	double distance = _semiMajorAxis * (1.0 - _eccentricity*_eccentricity) / (1.0 + _eccentricity * cos(_trueAnomaly));

	glm::f64vec3 satPosOrbitFrame(distance*cos(_trueAnomaly), distance*sin(_trueAnomaly), 0.0);

	return satPosOrbitFrame;
}

glm::f64vec3 Orbit::getGlobalSatPosition()
{
	glm::f64mat4 localToGlobal = glm::f64mat4(1.0);
	localToGlobal = glm::rotate(localToGlobal, _longOfAscNode, glm::f64vec3(0.0, 0.0, 1.0));
	localToGlobal = glm::rotate(localToGlobal, _inclination, glm::f64vec3(1.0, 0.0, 0.0));
	localToGlobal = glm::rotate(localToGlobal, _argOfPeriapsis, glm::f64vec3(0.0, 0.0, 1.0));

	double distance;

	distance = _semiMajorAxis * (1.0 - _eccentricity*_eccentricity) / (1.0 + _eccentricity * cos(_trueAnomaly));

	glm::f64vec4 satPosOrbitFrame(distance*cos(_trueAnomaly), distance*sin(_trueAnomaly), 0.0, 1.0);
	glm::f64vec4 satPosInertialFrame = localToGlobal * satPosOrbitFrame;

	return glm::f64vec3(satPosInertialFrame.x, satPosInertialFrame.y, satPosInertialFrame.z);
}


double Orbit::calcEccentricAnomaly(double meanAnomaly)
{
	const double tolerance = 1e-12;
	double previousEccAnomaly = meanAnomaly; //3.141592653589793238462;//meanAnomaly;
	double eccAnomaly = 0.0;

	double error = 1.0;
	int steps = 0;
	int maxSteps = 500;

	if (_eccentricity < 1.0)
	{
		previousEccAnomaly = meanAnomaly + _eccentricity * _eccentricity * sin(meanAnomaly);

		while (error > tolerance)
		{	// newtons method step
			eccAnomaly = previousEccAnomaly - (previousEccAnomaly - _eccentricity*sin(previousEccAnomaly) - meanAnomaly) / (1.0 - _eccentricity*cos(previousEccAnomaly));

			error = abs(eccAnomaly - previousEccAnomaly);
			previousEccAnomaly = eccAnomaly;
			steps++;
			if (steps > maxSteps) {
				break;
			}
		}
	}
	else if (_eccentricity > 1.0)
	{
		double stepSize = 1.0;
		previousEccAnomaly = asinh(meanAnomaly);

		while (error > tolerance)
		{	// newtons method step
			eccAnomaly = previousEccAnomaly - stepSize*(_eccentricity*sinh(previousEccAnomaly) - previousEccAnomaly - meanAnomaly) / (_eccentricity*cosh(previousEccAnomaly) - 1.0);

			error = abs(eccAnomaly - previousEccAnomaly);
			previousEccAnomaly = eccAnomaly;
			steps++;
			if (steps > maxSteps)
			{
				break;
			}
		}
	}

	stepCount = steps;
	finalError = error;

	return eccAnomaly;
}