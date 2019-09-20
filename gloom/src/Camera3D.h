#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera3D
{
public:
	Camera3D();
	~Camera3D();

	void init(glm::f64vec3 position, double aspect, double fov);

	void update(double timestep);
	void calcCamMatrix();

	void moveFrontBack(double magnitude);
	void moveSide(double magnitude);
	void moveUpDown(double magnitude);

	void addVel(glm::f64vec3 inputVel) { _velocity += inputVel; _needsMatrixUpdate = true; }
	void zoom(double zoomFactor);

	// newwer smooth functions, these add velocity, and the cam state is changed in the update function.
	void pitchingD(double magnitude); //
	void yawingD(double magnitude);
	void rollingD(double magnitude);
	void lookSideD(double magnitude);


	//setters
	void setPosition(glm::f64vec3 position);
	void setOrientation(glm::f64vec3 view, glm::f64vec3 up);
	void toggleSmooth() { _smooth = !_smooth; }
	void toggleThirdPerson();
	void removeThirdPerson();
	void setThirdPersonPos(std::string id, glm::f64vec3 pos);

	//getters
	glm::f64vec3 getPosition() { return _position; }
	glm::f64mat4 getCameraMatrix() { return _cameraMatrix; }
	glm::f64mat4 getViewMatrix() { return _viewMatrix; }
	glm::f64mat4 getProjectionMatrix() { return _perspectiveMatrix; }

	glm::f64vec3 getView() { return _viewVector; }
	glm::f64vec3 getUp() { return _upVector; }
	glm::f64vec3 getSide() { return _sideVector; }

	glm::f64vec3 getThirdPersonPos(unsigned char id);

	bool getSmooth() { return _smooth; }
	unsigned char getThirdPerson() { return _thirdPerson; }
	double getFOV() { return _fov; }


	double oldMousePosX;
	double oldMousePosY;
	double _thirdPersonViewDistance;

private:

	// old non smooth functions, these change the cam state directly
	void pitching(double magnitude); //
	void yawing(double magnitude);
	void rolling(double magnitude);
	void lookSide(double magnitude);

	bool _needsMatrixUpdate;
	bool _smooth;
	unsigned char _thirdPerson;
	std::map<std::string, glm::f64vec3> _thirdPersonPositions;

	glm::f64vec3 _position;
	glm::f64vec3 _velocity;
	double _speed;

	glm::f64vec3 _viewVector;
	glm::f64vec3 _upVector;
	glm::f64vec3 _sideVector;

	glm::f64vec4 _eulerSpeeds;
	double _eulerSpeed;

	double _fov;
	double _zoomSpeed;
	double _aspect;


	glm::f64mat4 _viewMatrix;
	glm::f64mat4 _perspectiveMatrix;
	glm::f64mat4 _cameraMatrix;

};