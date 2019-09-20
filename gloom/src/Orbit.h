#pragma once
#include <glm/glm.hpp>

#define GRAVITATIONAL_CONST 6.67408e-20 // length units are kilometers

class Orbit
{
public:
	Orbit();
	~Orbit();


	void orientOrbit(double longitudeOfAscendingNode, double inclination, double argumentOfPeriapsis);
	void setOrbitParameters(double newEccentricity, double newSemiMajorAxis);
	void setOrbitParamFromPeri(double periapsisDistance, double periapsisSpeed);
	void increasePeriSpeed(double factor);
	void update(double dt);


	glm::f64vec3 getLocalSatPosition();
	glm::f64vec3 getGlobalSatPosition();

	double getTA() { return _trueAnomaly; }
	double getSMA() { return _semiMajorAxis; }
	double getEcc() { return _eccentricity; };
	double getLOAN() { return _longOfAscNode; }
	double getInc() { return _inclination; }
	double getAOP() { return _argOfPeriapsis; }

	double getPeriod() { return 2.0 * 3.1415926535897932384626 * sqrt(_semiMajorAxis*_semiMajorAxis*_semiMajorAxis / (GRAVITATIONAL_CONST * _centralMass)); }

	double calcEccentricAnomaly(double meanAnomaly);


	int stepCount;
	double finalError;

	double _centralMass;

	double _longOfAscNode;
	double _inclination;
	double _argOfPeriapsis;

	double _eccentricity;
	double _semiMajorAxis;
	double _trueAnomaly;

	double _time;
};

