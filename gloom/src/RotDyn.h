#pragma once

#include <../vendor/glm/glm/glm.hpp>
#include <../vendor/glm/glm/gtc/quaternion.hpp>
//#include <../vendor/glm/glm/ext/quaternion_double.hpp>

class RotDyn //Rotational dynamics
{
public:
	RotDyn();
	~RotDyn(){}

	void update(double dt);
	void setDesiredQuat(double q0, double q1, double q2, double q3);

	glm::f64vec3 angularVelocity;
	glm::f64vec4 quaternion;
	glm::dquat quaternion_;

	glm::f64vec4 quaternionDesired;
	glm::dquat quaternionDesired_;

	glm::f64vec3 torque;
	glm::f64vec3 acc;
	glm::f64vec4 errorQuaternion;

	glm::f64mat3 momentOfInertia;
	glm::f64mat3 invMomentOfInertia;
};