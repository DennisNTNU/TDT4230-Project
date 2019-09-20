
// Local headers
#include <iostream>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

#include "TextureManager.h"
#include "Camera3D.h"
#include "Plane.h"
#include "TexturedPlane.h"
#include "Earth.h"
#include "Moon.h"
#include "Sun.h"
#include "Stars.h"
#include "Satellite.h"
#include "Atmosphere.h"

struct InputHelperStruct
{
	int scrollInput = 0;
	bool showOrbitDetails = false;
	double timewarpFactor = 1.0;

	bool increaseLOAN = false;
	bool increaseINC = false;
	bool increaseAOP = false;

	bool resetOrbit = false;

	bool increaseVp = false;
	bool decreaseVp = false;

	int oldKeyState_H = 0;
	int oldKeyState_G = 0;
	int oldKeyState_T = 0;
	int oldKeyState_dot = 0;
	int oldKeyState_comma = 0;

	double earthDistance = 10000.0;

	int swapIntervalIndex = 0;
	const int swapIntervals[4] = { 1, 2, 4, 8 };
	int oldKeyState_N = 0;




	int satAnimationPhase = 0;
};

static InputHelperStruct inputHelperStruct;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	inputHelperStruct.scrollInput = int(yoffset);
}

// Function for handling keypresses
void handleKeyboardInput(GLFWwindow* window, Camera3D* cs);
void printKeyBindings();

void initOpenGLSettings(GLFWwindow* window)
{

	// Enable depth (Z) buffer (accept "closest" fragment)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// enalbe alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Configure miscellaneous OpenGL settings
	//glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	// Set default colour after clearing the colour buffer
	//glClearColor(0.3f, 0.5f, 0.8f, 1.0f); //Sky blue
	//glClearColor(0.075, 0.125, 0.2, 1.0); //Dark blue
	//glClearColor(0.05, 0.0833333, 0.13333, 1.0); //Black blue
	glClearColor(0.0, 0.0, 0.0, 1.0); //Black

	// Hide mouse for camera controls
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetScrollCallback(window, scroll_callback);

}

void runProgram(GLFWwindow* window)
{
	initOpenGLSettings(window);

	printf("Compiling Shaders ...\n");

	// Initializing Shaders
	Gloom::Shader lineShader;
	lineShader.makeBasicShader("../gloom/shaders/lineShading.vert", "../gloom/shaders/lineShading.frag");
	Gloom::Shader diffuseShader;
	diffuseShader.makeBasicShader("../gloom/shaders/textureDiffuse.vert", "../gloom/shaders/textureDiffuse.frag");

	Gloom::Shader satShader;
	satShader.makeBasicShader("../gloom/shaders/sat.vert", "../gloom/shaders/sat.frag");

	Gloom::Shader earthShader;
	earthShader.makeBasicShader("../gloom/shaders/earthShader.vert", "../gloom/shaders/earthShader.frag");
	Gloom::Shader atmosphereShader;
	atmosphereShader.makeBasicShader("../gloom/shaders/atmosphere.vert", "../gloom/shaders/atmosphere.frag");
	Gloom::Shader sunShader;
	sunShader.makeBasicShader("../gloom/shaders/sunShader.vert", "../gloom/shaders/sunShader.frag");
	Gloom::Shader sunCoronaShader;
	sunCoronaShader.makeBasicShader("../gloom/shaders/sunShader_Corona.vert", "../gloom/shaders/sunShader.frag");

	// Initializing Texture Manager
	TextureManager textureManager;

	// Initializing camera
	Camera3D camera;
	camera.init(glm::f64vec3(4.0*6378.0, 0.0, 2.0*6378.0), 16.0/9.0, 60.0*3.14159265358979/180.0);
	camera.toggleSmooth();
	glfwGetCursorPos(window, &(camera.oldMousePosX), &(camera.oldMousePosY));
	camera.calcCamMatrix();

	printf("Loading Textures ...\n");
	
	// Setting up models/meshes/geometry

	printf("\t Earth diffuse ...\n");
	//Earth earth(&earthShader, textureManager.getTexture("../gloom/textures/earth.png"));
	Earth earth(&earthShader, textureManager.getTexture("../gloom/textures/hires/earth_8k.png"));

	printf("\t Earth diffuse night ...\n");
	//earth.addNightsideTexture(textureManager.getTexture("../gloom/textures/earth_night.png"));
	earth.addNightsideTexture(textureManager.getTexture("../gloom/textures/hires/earth_night_hi_res.png"));

	printf("\t Earth clouds ...\n");
	//earth.addCloudTexture(textureManager.getTexture("../gloom/textures/clouds.png"));
	earth.addCloudTexture(textureManager.getTexture("../gloom/textures/hires/clouds_8k.png"));

	printf("\t Earth specular ...\n");
	//earth.addWaterMaskTexture(textureManager.getTexture("../gloom/textures/water.png"));
	earth.addWaterMaskTexture(textureManager.getTexture("../gloom/textures/hires/water_8k.png"));

	printf("\t Earth heightmap ...\n");
	earth.addHeightTexture(textureManager.getTexture("../gloom/textures/earth_elev.png"));
	//earth.addHeightTexture(textureManager.getTexture("../gloom/textures/earth_elev_8k.png"));

	CoordinateSystem ecf(&lineShader, 130, 130, 130);
	Atmosphere atmosphere(&atmosphereShader);

	printf("\t Moon diffuse ...\n");
	Moon moon(&diffuseShader, textureManager.getTexture("../gloom/textures/moon.png"));

	printf("\t Sun diffuse ...\n");
	Sun sun(&sunShader, &sunCoronaShader, textureManager.getTexture("../gloom/textures/sun.png"), textureManager.getTexture("../gloom/textures/sun_corona.png"));
	
	printf("\t Sat diffuse ...\n");
	Satellite satellite(&lineShader, &satShader, textureManager.getTexture("../gloom/textures/sat.png"), 0.01, 6378.0 + 600.0);
	// longitudeOfAscendingNode,  inclination,  argumentOfPeriapsis
	satellite.orientOrbit(3.14159265358979323 / 5.0, 98.696 * 3.14159265358979323 / 180.0, 3.14159265358979323 / 3.0);

	printf("Loading starcatalog ...\n");
	Stars stars(&lineShader, "../gloom/stars/hygfull.csv");


	// Time related variables
	double dt;
	double timestamp;
	double timestamp2;
	double satAnimTimestamp;
	double renderTimestamp;
	timestamp = glfwGetTime();
	timestamp2 = timestamp;
	renderTimestamp = timestamp;

	printKeyBindings();

	printf("Starting rendering loop ...\n");

	//-------------------------------------------------------------
	// Rendering Loop
	//-------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// delta time
		dt = glfwGetTime() - timestamp;
		timestamp = timestamp + dt;

		if (timestamp - renderTimestamp > 0.01666666)
		{	
			dt = timestamp - renderTimestamp;
			renderTimestamp = timestamp;
			if (timestamp - timestamp2 > 0.8)
			{
				timestamp2 = timestamp;
				glfwSetWindowTitle(window, ("Orbit's 2U Cubesat Visualization - Fps: " + std::to_string(1/dt)).c_str());

				/*
				std::cout << "q\t" << satellite._rotDyn.quaternion[0] << " ; "<< satellite._rotDyn.quaternion[1] << " ; "<< satellite._rotDyn.quaternion[2] << " ; "<< satellite._rotDyn.quaternion[3] << "\n";
				std::cout << "qd\t" << satellite._rotDyn.quaternionDesired[0] << " ; "<< satellite._rotDyn.quaternionDesired[1] << " ; "<< satellite._rotDyn.quaternionDesired[2] << " ; "<< satellite._rotDyn.quaternionDesired[3] << "\n";
				std::cout << "qe\t" << satellite._rotDyn.errorQuaternion[0] << " ; "<< satellite._rotDyn.errorQuaternion[1] << " ; "<< satellite._rotDyn.errorQuaternion[2] << " ; "<< satellite._rotDyn.errorQuaternion[3] << "\n";
				std::cout << "t\t" << satellite._rotDyn.torque[0] << " ; "<< satellite._rotDyn.torque[1] << " ; "<< satellite._rotDyn.torque[2] << "\n";
				std::cout << "a\t" << satellite._rotDyn.acc[0] << " ; "<< satellite._rotDyn.acc[1] << " ; "<< satellite._rotDyn.acc[2] << "\n";
				std::cout << "w\t" << satellite._rotDyn.angularVelocity[0] << " ; "<< satellite._rotDyn.angularVelocity[1] << " ; "<< satellite._rotDyn.angularVelocity[2] << "\n\n";
				*/

				switch (inputHelperStruct.satAnimationPhase)
				{
				case 0:
					satellite.setDesiredQuat(1, 0.0, 0.0, 0.0);
					break;
				case 1:
				{
					glm::f64vec3 satpos = glm::normalize(satellite.getSatPosition());
					double theta = acos(satpos[0]);

					double length = sqrt(satpos[1]*satpos[1] + satpos[2]*satpos[2]);
					double nx = 0.0;
					double ny = -satpos[2]/length;
					double nz = satpos[1]/length;

					satellite.setDesiredQuat(cos(theta/2.0), nx, ny*sin(theta/2.0), nz*sin(theta/2.0));

					inputHelperStruct.satAnimationPhase = 2;
					break;
				}
				case 2:
				{
					double error = satellite._rotDyn.errorQuaternion[1]*satellite._rotDyn.errorQuaternion[1] + satellite._rotDyn.errorQuaternion[2]*satellite._rotDyn.errorQuaternion[2] + satellite._rotDyn.errorQuaternion[3]*satellite._rotDyn.errorQuaternion[3];
					if (error < 0.02)
					{
						satAnimTimestamp = glfwGetTime();
						inputHelperStruct.satAnimationPhase = 3;
						printf("Animation Phase 2 ...\n");
					}
					break;
				}
				case 3:
					if (timestamp - satAnimTimestamp > 10)
					{
						inputHelperStruct.satAnimationPhase = 0;
						printf("Animation End\n");
					}
					break;
				default:
					break;

				}

				//std::cout << "Sat Pos : " << satposs[0] << "; " << satposs[1] << "; " << satposs[2] << "\n";
				//std::cout << "DesQuat : " << satellite._rotDyn.quaternionDesired[0] << "; " << satellite._rotDyn.quaternionDesired[1] << "; " << satellite._rotDyn.quaternionDesired[2] << "; " << satellite._rotDyn.quaternionDesired[3] << "\n";
				//std::cout << "SatQuat : " << satellite._rotDyn.quaternion_[0] << "; " << satellite._rotDyn.quaternion_[1] << "; " << satellite._rotDyn.quaternion_[2] << "; " << satellite._rotDyn.quaternion_[3] << "\n";
			}

			// Handle other events
			glfwPollEvents();
			handleKeyboardInput(window, &camera);

			if (inputHelperStruct.increaseLOAN) // U
			{
				satellite.orientOrbit(satellite.orbit.getLOAN() + dt, satellite.orbit.getInc(), satellite.orbit.getAOP());
				inputHelperStruct.increaseLOAN = false;
			}
			if (inputHelperStruct.increaseINC) // I
			{
				satellite.orientOrbit(satellite.orbit.getLOAN(), satellite.orbit.getInc() + dt, satellite.orbit.getAOP());
				inputHelperStruct.increaseINC = false;
			}
			if (inputHelperStruct.increaseAOP) // O
			{
				satellite.orientOrbit(satellite.orbit.getLOAN(), satellite.orbit.getInc(), satellite.orbit.getAOP() + dt);
				inputHelperStruct.increaseAOP = false;
			}

			if (inputHelperStruct.increaseVp) // J
			{
				satellite.increasePeriSpeed(1.0 + 0.01*dt);
				inputHelperStruct.increaseVp = false;
			}
			if (inputHelperStruct.decreaseVp) // K
			{
				satellite.increasePeriSpeed(1.0 - 0.01*dt);
				inputHelperStruct.decreaseVp = false;
			}

			if (inputHelperStruct.resetOrbit) // P
			{	
				satellite.orientOrbit(3.141592653589793238462 / 5.0, 98.696 * 3.14159265358979323 / 180.0, 3.141592653589793238462 / 3.0);
				satellite.setOrbitParameters(0.4, 3.0*6378.0);
				inputHelperStruct.resetOrbit = false;
			}

			// Update Scene
			glm::mat4 sunModelMatrix = earth.update(dt * inputHelperStruct.timewarpFactor);
			moon.update(dt * inputHelperStruct.timewarpFactor);

			glm::vec3 earthPos = glm::vec3(0.0, 0.0, 0.0);
			glm::vec3 sunPos = glm::vec3(sunModelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			ecf.setPosition(glm::vec3(0.0, 0.0, 0.0));
			satellite.updateSatellite(dt * inputHelperStruct.timewarpFactor, earthPos);
			sun.setModelMatrix(&sunModelMatrix);

			// Update camera 
			camera.update(dt);
			
			if (inputHelperStruct.scrollInput != 0)
			{
				camera.zoom(pow(1.08, -inputHelperStruct.scrollInput));
				inputHelperStruct.scrollInput = 0;
			}
			camera.setThirdPersonPos("earth", earthPos);
			camera.setThirdPersonPos("sun", sunPos);
			camera.setThirdPersonPos("sat", satellite.getSatPosition());

			camera.calcCamMatrix();
			glm::mat4 perspView = camera.getCameraMatrix();
			//glm::mat4 viewMatrix = camera.getViewMatrix();
			//glm::mat4 projectionMatrix = camera.getProjectionMatrix();

			glm::vec3 camPos = camera.getPosition();
			inputHelperStruct.earthDistance = glm::length(camPos);
			if (inputHelperStruct.earthDistance < 6400.0) {
				camera.setPosition(camera.getPosition() * 6400.0 / inputHelperStruct.earthDistance);
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw your scene here

			//Uncomment to make stars into a kind of skybox
			stars._modelMatrix = glm::translate(glm::mat4(1.0f), camPos);

			stars.draw(&perspView);
			sun.draw(&perspView);
			moon.draw(&perspView, &sunPos);
			earth.draw(&perspView, &sunPos, &camPos);
			satellite.draw(&perspView, &sunPos, inputHelperStruct.showOrbitDetails);

			if (inputHelperStruct.showOrbitDetails)
			{
				ecf.draw(&perspView);
			}
			else
			{
				// atmosphere.draw(&viewMatrix, &projectionMatrix, &camPos, &sunPos, &earthPos);
			}

			// Flip buffers
			glfwSwapBuffers(window);
		}
	}
}


void handleKeyboardInput(GLFWwindow* window, Camera3D* cs)
{
	// Use escape key for terminating the GLFW window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	double nearEarthFactor = 1.0 - exp(-(inputHelperStruct.earthDistance - 6418.0)/1000.0);
	if (nearEarthFactor < 0.01) {
		nearEarthFactor = 0.01;
	}

	double camMoveSpeed = 0.5*6378.0*nearEarthFactor;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camMoveSpeed *= 6.0;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camMoveSpeed /= 10.0;
	}

	// Handling input that controls camera position ...
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cs->moveSide(-camMoveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cs->moveSide(camMoveSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		cs->moveUpDown(camMoveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		cs->moveUpDown(-camMoveSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cs->moveFrontBack(camMoveSpeed);

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cs->moveFrontBack(-camMoveSpeed);
	}


	int newKeyState_G = glfwGetKey(window, GLFW_KEY_G);
	if (inputHelperStruct.oldKeyState_G == GLFW_RELEASE && (newKeyState_G == GLFW_PRESS))
	{
		cs->toggleThirdPerson();
	}
	inputHelperStruct.oldKeyState_G = newKeyState_G;

	int newKeyState_T = glfwGetKey(window, GLFW_KEY_T);
	if (inputHelperStruct.oldKeyState_T == GLFW_RELEASE && (newKeyState_T == GLFW_PRESS))
	{
		cs->removeThirdPerson();
	}
	inputHelperStruct.oldKeyState_T = newKeyState_T;




	int newKeyState_H = glfwGetKey(window, GLFW_KEY_H);
	if (inputHelperStruct.oldKeyState_H == GLFW_RELEASE && newKeyState_H == GLFW_PRESS)
	{
		inputHelperStruct.showOrbitDetails = !inputHelperStruct.showOrbitDetails;
	}
	inputHelperStruct.oldKeyState_H = newKeyState_H;



	int newKeyState_dot = glfwGetKey(window, GLFW_KEY_PERIOD);
	if (inputHelperStruct.oldKeyState_dot == GLFW_RELEASE && newKeyState_dot == GLFW_PRESS)
	{
		inputHelperStruct.timewarpFactor *= 3.0;
		if (inputHelperStruct.timewarpFactor > 81.0 )
		{
			inputHelperStruct.timewarpFactor = 81.0;
		}
	}
	inputHelperStruct.oldKeyState_dot = newKeyState_dot;

	int newKeyState_comma = glfwGetKey(window, GLFW_KEY_COMMA);
	if (inputHelperStruct.oldKeyState_comma == GLFW_RELEASE && newKeyState_comma == GLFW_PRESS)
	{
		inputHelperStruct.timewarpFactor /= 3.0;
	}
	inputHelperStruct.oldKeyState_comma = newKeyState_comma;



	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		inputHelperStruct.increaseLOAN = true;
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		inputHelperStruct.increaseINC = true;
	}

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		inputHelperStruct.increaseAOP = true;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		inputHelperStruct.increaseVp = true;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		inputHelperStruct.decreaseVp = true;
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		inputHelperStruct.resetOrbit = true;
	}

	int newKeyState_N = glfwGetKey(window, GLFW_KEY_N);
	if (inputHelperStruct.oldKeyState_N == GLFW_RELEASE && (newKeyState_N == GLFW_PRESS))
	{
		inputHelperStruct.swapIntervalIndex = (inputHelperStruct.swapIntervalIndex + 1) % 4;
		glfwSwapInterval(inputHelperStruct.swapIntervals[inputHelperStruct.swapIntervalIndex]);
	}
	inputHelperStruct.oldKeyState_N = newKeyState_N;

	/*
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cs->rolling(1.5); }
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cs->rolling(-1.5); }*/


	int l_mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (l_mouse_state == GLFW_PRESS && inputHelperStruct.satAnimationPhase == 0)
	{
		inputHelperStruct.satAnimationPhase = 1;
		printf("Animation pahse 1 ...\n");
	}


	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	cs->pitchingD(0.1*(cs->oldMousePosY - ypos));
	cs->lookSideD(0.1*(cs->oldMousePosX - xpos));

	cs->oldMousePosX = xpos;
	cs->oldMousePosY = ypos;
	
}

void printKeyBindings()
{
	std::cout << "\nw,a,s,d,r,f : Move camera forward,left,back,right,up,down\n";
	std::cout << "g : toggle through points of view\n";
	std::cout << "h : hide atmosphere; show Orbit Details\n";
	std::cout << ", . : as in ksp, slow and speed up time\n";
	std::cout << "u : increase right ascention of ascending node\n";
	std::cout << "i : increase inclination\n";
	std::cout << "o : increase argument of periapsis\n";
	std::cout << "p : reset orbit\n";
	std::cout << "j,k : increase/decrease periapsis speed\n";
	std::cout << "n : increment glfwSwapInterval\n";
	std::cout << "";
	std::cout << "";
	std::cout << "";
	std::cout << "";
}
