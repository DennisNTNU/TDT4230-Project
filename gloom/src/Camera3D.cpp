#include "Camera3D.h"

#include <iostream>

Camera3D::Camera3D() :
		_thirdPersonViewDistance(20000.0),
		_needsMatrixUpdate(true),
		_smooth(false),
		_thirdPerson(0),
		_position(0.0, 0.0, 0.0),
		_velocity(0.0, 0.0, 0.0),
		_viewVector(-1.0, 0.0, 0.0),
		_upVector(0.0, 0.0, 1.0),
		_sideVector(0.0, 1.0, 0.0),
		_eulerSpeeds(0.0, 0.0, 0.0, 0.0),
		_eulerSpeed(0.0),
		_fov(60),
		_aspect(16.0 / 9.0)
	{
	}


	Camera3D::~Camera3D()
	{
	}

	void Camera3D::init(glm::f64vec3 position, double aspect, double fov)
	{
		_position = position;
		_aspect = aspect;
		_fov = fov;
	}

	void Camera3D::update(double timestep)
	{
		if (_needsMatrixUpdate)
		{
			_position += _velocity * timestep;

			const double tmp = timestep * _fov;
			pitching(_eulerSpeeds.x * tmp);
			yawing(_eulerSpeeds.y * tmp);
			rolling(_eulerSpeeds.z * tmp);
			lookSide(_eulerSpeeds.w * tmp);

			if (_smooth)
			{
				double decayedWithinSeconds = 0.2;
				double timeConst = decayedWithinSeconds / 5.0;
				double decayRate = timestep / timeConst;
				double factor = exp(- decayRate);
				_eulerSpeeds = factor * _eulerSpeeds;
				_velocity = exp(-decayRate) * _velocity;
			} else {
				_eulerSpeeds = 0.1 * _eulerSpeeds;
				_velocity = 0.2 * _velocity;
			}
			
			_viewVector = glm::normalize(_viewVector);
			_upVector = glm::normalize(_upVector);
			_sideVector = glm::normalize(_sideVector);

			_speed = sqrt(_velocity.x * _velocity.x + _velocity.y * _velocity.y + _velocity.z * _velocity.z);
			_eulerSpeed = glm::length(_eulerSpeeds);

			if (_speed < 0.01 && _eulerSpeed < 0.01)
			{
				_needsMatrixUpdate = false;
			}
			else
			{
				_needsMatrixUpdate = true;
			}
		}
		if (_thirdPerson > 0)
		{
			setPosition(getThirdPersonPos(_thirdPerson-1) - _thirdPersonViewDistance*getView());
		}
	}

	void Camera3D::calcCamMatrix()
	{
		if (_needsMatrixUpdate) {
			_viewMatrix = glm::lookAt(_position, _position + _viewVector, _upVector);
			_perspectiveMatrix = glm::perspective(_fov, _aspect, 0.50, 50000000.0);
			_cameraMatrix = _perspectiveMatrix * _viewMatrix;
		}
	}

	void Camera3D::moveFrontBack(double magnitude)
	{
		addVel(magnitude * _viewVector);
	}

	void Camera3D::moveSide(double magnitude)
	{
		addVel(magnitude * _sideVector);
	}

	void Camera3D::moveUpDown(double magnitude)
	{
		addVel(magnitude * _upVector);
	}

	void Camera3D::zoom(double zoomFactor)
	{
		if (_thirdPerson)
		{
			_thirdPersonViewDistance *= zoomFactor;
		}
		else
		{
			_fov *= zoomFactor;
			if (_fov >= 3.03) {
				_fov = 3.03;
			}
			_needsMatrixUpdate = true;
		}
	}

	void Camera3D::setPosition(glm::f64vec3 position)
	{
		_position = position;
		_velocity = glm::f64vec3(0.0, 0.0, 0.0);
		_needsMatrixUpdate = true;
	}

	void Camera3D::setOrientation(glm::f64vec3 view, glm::f64vec3 up)
	{
		//view and up are assumed to be orthonormal
		_viewVector = view;
		_upVector = up;
		_sideVector = glm::cross(_viewVector, _upVector);
		_needsMatrixUpdate = true;
	}

	void Camera3D::toggleThirdPerson()
	{
		_thirdPerson += 1;

		if (_thirdPerson > _thirdPersonPositions.size())
		{
			_thirdPerson = 0;
		}
	}

	void Camera3D::removeThirdPerson()
	{
		_thirdPerson = 0;
	}

	void Camera3D::pitchingD(double magnitude)
	{
		_eulerSpeeds.x += magnitude;

		_needsMatrixUpdate = true;
	}

	void Camera3D::yawingD(double magnitude)
	{
		_eulerSpeeds.y += magnitude;

		_needsMatrixUpdate = true;
	}

	void Camera3D::rollingD(double magnitude)
	{
		_eulerSpeeds.z += magnitude;

		_needsMatrixUpdate = true;
	}


	void Camera3D::lookSideD(double magnitude)
	{
		_eulerSpeeds.w += magnitude;

		_needsMatrixUpdate = true;
	}

	glm::f64vec3 Camera3D::getThirdPersonPos(unsigned char id)
	{
		std::map<std::string, glm::f64vec3>::iterator mit = _thirdPersonPositions.begin();
		for (unsigned int i = 0; i < id; i++)
		{
			mit++;
		}
		return mit->second;
	}

	void Camera3D::setThirdPersonPos(std::string id, glm::f64vec3 pos)
	{
		std::map<std::string, glm::f64vec3>::iterator mit = _thirdPersonPositions.begin();
		for (; mit != _thirdPersonPositions.end(); mit++)
		{
			if (mit->first == id)
			{
				mit->second = pos;
				return;
			}
		}

		_thirdPersonPositions.insert(std::pair<std::string, glm::f64vec3> (id, pos));
	}

	void Camera3D::pitching(double magnitude)
	{
		glm::f64vec4 temp = (glm::rotate(glm::f64mat4(1.0f), magnitude, _sideVector) * glm::f64vec4(_viewVector, 1.0f));
		_viewVector[0] = temp[0]; _viewVector[1] = temp[1]; _viewVector[2] = temp[2];

		_upVector = glm::cross(_sideVector, _viewVector);

		/*temp = (glm::rotate(glm::mat4(1.0f), magnitude, _sideVector) * glm::vec4(_upVector, 1.0f));
		_upVector[0] = temp[0]; _upVector[1] = temp[1]; _upVector[2] = temp[2];*/

		_needsMatrixUpdate = true;
	}

	void Camera3D::yawing(double magnitude)
	{
		glm::f64vec4 temp = glm::rotate(glm::f64mat4(1.0f), magnitude, _upVector) * glm::f64vec4(_viewVector, 1.0f);
		_viewVector[0] = temp[0]; _viewVector[1] = temp[1]; _viewVector[2] = temp[2];

		_sideVector = glm::cross(_viewVector, _upVector);

		_needsMatrixUpdate = true;
	}

	void Camera3D::rolling(double magnitude)
	{
		glm::f64vec4 temp = (glm::rotate(glm::f64mat4(1.0f), magnitude, _viewVector) * glm::f64vec4(_upVector, 1.0f));
		_upVector[0] = temp[0]; _upVector[1] = temp[1]; _upVector[2] = temp[2];

		_sideVector = glm::cross(_viewVector, _upVector);

		_needsMatrixUpdate = true;
	}

	void Camera3D::lookSide(double magnitude) {

		glm::f64vec3 axis(0.0f, 0.0f, 1.0f);
		glm::f64mat4 identityMatrix(1.0f);
		glm::f64vec4 temp = (glm::rotate(identityMatrix, magnitude, axis) * glm::f64vec4(_viewVector, 1.0f));
		_viewVector[0] = temp[0]; _viewVector[1] = temp[1]; _viewVector[2] = temp[2];

		temp = (glm::rotate(identityMatrix, magnitude, axis) * glm::f64vec4(_upVector, 1.0f));
		_upVector[0] = temp[0]; _upVector[1] = temp[1]; _upVector[2] = temp[2];

		_sideVector = glm::cross(_viewVector, _upVector);

		_needsMatrixUpdate = true;
	}