#include "RotDyn.h"
#include <../vendor/glm/glm/glm.hpp>
#include <../vendor/glm/glm/gtc/quaternion.hpp>

static double signn(double signd)
{
	if (signd < 0)
	{
		return -1.0;
	}
	else
	{
		return 1.0;
	}
}

RotDyn::RotDyn()
{
	angularVelocity = glm::f64vec3(1.0, 1.0,-1.0);
	quaternion = glm::f64vec4(1.0, 0.0, 0.0, 0.0);
	quaternion = glm::normalize(quaternion);
	quaternionDesired = glm::f64vec4(1.0, 0.0, 0.0, 0.0);

	momentOfInertia = glm::f64mat3(1.0);
	invMomentOfInertia = glm::inverse(momentOfInertia);

	quaternion_[0] = 0;
	quaternion_[1] = 0;
	quaternion_[2] = 0;
	quaternion_[3] = 1;
}
void RotDyn::setDesiredQuat(double q0, double q1, double q2, double q3)
{
	quaternionDesired[0] = q0;
	quaternionDesired[1] = q1;
	quaternionDesired[2] = q2;
	quaternionDesired[3] = q3;
}

void RotDyn::update(double dt)
{

	//glm::f64vec4 errorQuaternion;
	
	// Normal quat product 
/*	errorQuaternion[0] =   quaternionDesired[0]*quaternion[0] - quaternionDesired[1]*quaternion[1] - quaternionDesired[2]*quaternion[2] - quaternionDesired[3]*quaternion[3];
	errorQuaternion[1] =   quaternionDesired[0]*quaternion[1] + quaternionDesired[1]*quaternion[0] - quaternionDesired[2]*quaternion[3] + quaternionDesired[3]*quaternion[2];
	errorQuaternion[2] =   quaternionDesired[0]*quaternion[2] + quaternionDesired[1]*quaternion[3] + quaternionDesired[2]*quaternion[0] - quaternionDesired[3]*quaternion[1];
	errorQuaternion[3] =   quaternionDesired[0]*quaternion[3] - quaternionDesired[1]*quaternion[2] + quaternionDesired[2]*quaternion[1] + quaternionDesired[3]*quaternion[0];

	// quat product with q des conjugated
	errorQuaternion[0] =   quaternionDesired[0]*quaternion[0] + quaternionDesired[1]*quaternion[1] + quaternionDesired[2]*quaternion[2] + quaternionDesired[3]*quaternion[3];
	errorQuaternion[1] =   quaternionDesired[0]*quaternion[1] - quaternionDesired[1]*quaternion[0] + quaternionDesired[2]*quaternion[3] - quaternionDesired[3]*quaternion[2];
	errorQuaternion[2] =   quaternionDesired[0]*quaternion[2] - quaternionDesired[1]*quaternion[3] - quaternionDesired[2]*quaternion[0] + quaternionDesired[3]*quaternion[1];
	errorQuaternion[3] =   quaternionDesired[0]*quaternion[3] + quaternionDesired[1]*quaternion[2] - quaternionDesired[2]*quaternion[1] - quaternionDesired[3]*quaternion[0];
*/
	//errorQuaternion = quaternion;

	// other try
	errorQuaternion[0] =   quaternionDesired[0]*quaternion[0] + quaternionDesired[1]*quaternion[1] + quaternionDesired[2]*quaternion[2] + quaternionDesired[3]*quaternion[3];
	errorQuaternion[1] = - quaternionDesired[1]*quaternion[0] + quaternionDesired[0]*quaternion[1] + quaternionDesired[3]*quaternion[2] - quaternionDesired[2]*quaternion[3];
	errorQuaternion[2] = - quaternionDesired[2]*quaternion[0] - quaternionDesired[3]*quaternion[1] + quaternionDesired[0]*quaternion[2] + quaternionDesired[1]*quaternion[3];
	errorQuaternion[3] = - quaternionDesired[3]*quaternion[0] + quaternionDesired[2]*quaternion[1] - quaternionDesired[1]*quaternion[2] + quaternionDesired[0]*quaternion[3];


	//glm::f64vec3 
	torque = - 0.6*angularVelocity - 0.2*glm::f64vec3(errorQuaternion[1], errorQuaternion[2], errorQuaternion[3])*signn(errorQuaternion[0]);


	//glm::f64vec3 
	acc = invMomentOfInertia*(torque - glm::cross(angularVelocity, momentOfInertia*angularVelocity));

	angularVelocity = angularVelocity + dt*acc;

	quaternion[0] = quaternion[0] + 0.5*dt*(- angularVelocity[0]*quaternion[1] - angularVelocity[1]*quaternion[2] - angularVelocity[2]*quaternion[3]);
	quaternion[1] = quaternion[1] + 0.5*dt*(  angularVelocity[0]*quaternion[0] - angularVelocity[1]*quaternion[3] + angularVelocity[2]*quaternion[2]);
	quaternion[2] = quaternion[2] + 0.5*dt*(  angularVelocity[0]*quaternion[3] + angularVelocity[1]*quaternion[0] - angularVelocity[2]*quaternion[1]);
	quaternion[3] = quaternion[3] + 0.5*dt*(- angularVelocity[0]*quaternion[2] + angularVelocity[1]*quaternion[1] + angularVelocity[2]*quaternion[0]);

	quaternion = glm::normalize(quaternion);


	quaternion_[0] = quaternion[1];
	quaternion_[1] = quaternion[2];
	quaternion_[2] = quaternion[3];
	quaternion_[3] = quaternion[0];

}