//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"

#include <iostream>

int glWindowWidth = 1000;
int glWindowHeight = 800;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2028;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

gps::Camera myCamera(
	glm::vec3(0.0f, 0.0f, 7.5f),
	glm::vec3(0.0f, 0.0f, 1000.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 0.05f;


bool pressedKeys[1024];
float angleY = 0.0f;
GLfloat lightAngle;

gps::Model3D nanosuit;
gps::Model3D ground;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D drawer;
gps::Model3D casa;
gps::Model3D car;
gps::Model3D rock;
gps::Model3D inLight;
//gps::Model3D others;
gps::Model3D rat;
gps::Model3D man;
gps::Model3D cat;
gps::Model3D sirena;
gps::Model3D others;
gps::Model3D plants;

gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
gps::Shader depthMapShader;
gps::Shader skyboxShader;

GLuint shadowMapFBO;
GLuint depthMapTexture;

gps::SkyBox mySkyBox;

bool showDepthMap;
bool canPressF = true;
bool canPressC = true;
bool fogActive = false;
bool crouchActive = false;
bool inTheBox = true;
bool canPressN = true;
bool isDay = true;

glm::vec3 ratPosition = glm::vec3(-7.88132f, 0.0f, 5.308771f); // Initial rat position
float ratSpeed = 0.05f; // Speed of rat movement

bool moveUpRight = true; // Flag to control movement direction

//security lamp
//3.0633 0.14 -1.23119
glm::vec3 pointLightPosition1 = glm::vec3(3.0633f, 0.3f, -1.23119f);
glm::vec3 pointLightColor1 = glm::vec3(0.0f, 0.0f, 0.0f);

float constant1 = 1.0f;
float linear1 = 0.0045f;
float quadratic1 = 0.2f;
float intensity1 = 7.0f;

//street lamp
//5.8879 0.08 -5.26523
glm::vec3 pointLightPosition2 = glm::vec3(5.8879f, 2.0f, -5.26523f);
glm::vec3 pointLightColor2 = glm::vec3(0.84f, 0.796f, 0.1254f);

float constant2 = 1.0f;
float linear2 = 0.0045f;
float quadratic2 = 0.2f;
float intensity2 = 10.0f;

//siren lamp
//10.762 0.02 0.509858
glm::vec3 pointLightPosition3 = glm::vec3(10.962f, 0.6f, 0.509858f);
glm::vec3 pointLightColor3 = glm::vec3(0.1f, 0.0f, 0.0f);

float constant3 = 1.0f;
float linear3 = 0.0045f;
float quadratic3 = 0.2f;
float intensity3 = 60.0f;

//cat lamp
//5.37156 0.14 2.90604
glm::vec3 pointLightPosition4 = glm::vec3(5.37156f, 0.2f, 2.90604f);
//initially black
glm::vec3 pointLightColor4 = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 pointLightColor4 = glm::vec3(0.5215f, 0.1490f, 0.7686f);

float constant4 = 1.0f;
float linear4 = 0.0045f;
float quadratic4 = 0.2f;
float intensity4 = 7.0f;

glm::vec3 spotLightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
float spotLightCutOff = glm::cos(glm::radians(23.5f));

//headlight
glm::vec3 pointLightPosition5 = glm::vec3(0.0f, 0.0f, 0.0f);
//initially black
glm::vec3 pointLightColor5 = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 pointLightColor5 = glm::vec3(0.9333f, 1.0f, 0.2901f);

float constant5 = 1.0f;
float linear5 = 0.0045f;
float quadratic5 = 0.2f;
float intensity5 = 7.0f;

glm::vec3 spotLightDirection5 = glm::vec3(0.0f, 0.0f, -1.0f);
float spotLightCutOff5 = glm::cos(glm::radians(7.5f));


GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "Window resized to width: %d and height: %d\n", width, height);
	glViewport(0, 0, width, height);

	// Update the global variables for window width and height
	glWindowWidth = width;
	glWindowHeight = height;

	// Update any projection matrices or viewport-related code here
	// For example, update the projection matrix:
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}


void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	static double lastX = xpos;
	static double lastY = ypos;

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;  // Inverted y-axis for typical camera controls

	lastX = xpos;
	lastY = ypos;

	// Pass xOffset and yOffset to your camera rotation function
	myCamera.processMouseMovement(xOffset, yOffset, true);
}

void processMovement()
{
	if (pressedKeys[GLFW_KEY_Q]) {
		angleY -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angleY += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}

	if ((myCamera.getCameraPosition().x > 3.711) && (myCamera.getCameraPosition().x < 5.7002) && (myCamera.getCameraPosition().z > -2.8626) && (myCamera.getCameraPosition().z < -0.1033) && inTheBox) {
		//in the box
		std::cout << "in the box";
		//change the light color in red
		myCustomShader.useShaderProgram();
		pointLightColor1 = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor1"), 1, glm::value_ptr(pointLightColor1));
		inTheBox = false;
	}
	else if (!((myCamera.getCameraPosition().x > 3.711) && (myCamera.getCameraPosition().x < 5.7002) && (myCamera.getCameraPosition().z > -2.8626) && (myCamera.getCameraPosition().z < -0.1033))) {
		//outside the box
		//change the light color in white
		myCustomShader.useShaderProgram();
		pointLightColor1 = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor1"), 1, glm::value_ptr(pointLightColor1));

		inTheBox = true;
	}


	if (pressedKeys[GLFW_KEY_F] && canPressF) {
		myCustomShader.useShaderProgram();
		fogActive = !fogActive;
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "fogEnabled"), fogActive);
		canPressF = false;
	}
	else if (!pressedKeys[GLFW_KEY_F]) {

		canPressF = true;
	}

	//sprint
	if (pressedKeys[GLFW_KEY_LEFT_SHIFT]) {
		cameraSpeed = 0.2f;
	}
	else {
		cameraSpeed = 0.05f;
	}

	//view coordinates
	if (pressedKeys[GLFW_KEY_I]) {
		std::cout << "Camera position: " << myCamera.getCameraPosition().x << " " << myCamera.getCameraPosition().y << " " << myCamera.getCameraPosition().z << std::endl;
		myCamera.getCameraPosition();

	}

	//crouch
	if (pressedKeys[GLFW_KEY_C] && canPressC) {
		myCamera.crouch();
		crouchActive = !crouchActive;
		canPressC = false;
	}
	else if (!pressedKeys[GLFW_KEY_C]) {

		canPressC = true;
	}

	//day-night
	if (pressedKeys[GLFW_KEY_N] && canPressN) {
		myCustomShader.useShaderProgram();
		if (isDay) {
			lightColor = glm::vec3(0.0f, 0.0f, 0.0f);

			//turn on the headlight
			pointLightColor5 = glm::vec3(0.9333f, 1.0f, 0.2901f);
			glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor5"), 1, glm::value_ptr(pointLightColor5));

			//turn on the cat lamp
			pointLightColor4 = glm::vec3(0.5215f, 0.1490f, 0.7686f);
			glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor4"), 1, glm::value_ptr(pointLightColor4));
		}
		else {
			//black
			lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

			//turn off the headlight
			pointLightColor5 = glm::vec3(0.0f, 0.0f, 0.0f);
			glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor5"), 1, glm::value_ptr(pointLightColor5));

			//turn off the cat lamp
			pointLightColor4 = glm::vec3(0.0f, 0.0f, 0.0f);
			glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor4"), 1, glm::value_ptr(pointLightColor4));
		}

		lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
		glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

		isDay = !isDay;
		canPressN = false;
	}
	else if(!pressedKeys[GLFW_KEY_N]) {
		canPressN = true;
	}
}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//window scaling for HiDPI displays
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	//for sRBG framebuffer
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

	//for antialising
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

#if not defined (__APPLE__)
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	nanosuit.LoadModel("objects/nanosuit/nanosuit.obj");
	ground.LoadModel("objects/ground/ground.obj");
	lightCube.LoadModel("objects/cube/cube.obj");
	screenQuad.LoadModel("objects/quad/quad.obj");
	casa.LoadModel("objects/casa/casa.obj");
	car.LoadModel("objects/car/car.obj");
	drawer.LoadModel("objects/drawer/drawer.obj");
	rock.LoadModel("objects/rock/rock.obj");
	inLight.LoadModel("objects/inLight/inLight.obj");
	//others.LoadModel("objects/others/others.obj");
	rat.LoadModel("objects/rat/rat.obj");
	man.LoadModel("objects/man/man.obj");
	cat.LoadModel("objects/cat/cat.obj");
	sirena.LoadModel("objects/sirena/sirena.obj");
	others.LoadModel("objects/others/others.obj");
	plants.LoadModel("objects/plants/plants.obj");
}

void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightShader.useShaderProgram();
	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();
	depthMapShader.loadShader("shaders/shadow.vert", "shaders/shadow.frag");
	depthMapShader.useShaderProgram();
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();


}

void initUniforms() {
	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 0.5f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	//security lamp
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "constant1"), constant1);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "linear1"), linear1);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "quadratic1"), quadratic1);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "intensity1"), intensity1);


	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightPosition1"), 1, glm::value_ptr(pointLightPosition1));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor1"), 1, glm::value_ptr(pointLightColor1));

	//street lamp
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "constant2"), constant2);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "linear2"), linear2);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "quadratic2"), quadratic2);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "intensity2"), intensity2);

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightPosition2"), 1, glm::value_ptr(pointLightPosition2));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor2"), 1, glm::value_ptr(pointLightColor2));

	//siren lamp
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "constant3"), constant3);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "linear3"), linear3);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "quadratic3"), quadratic3);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "intensity3"), intensity3);

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightPosition3"), 1, glm::value_ptr(pointLightPosition3));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor3"), 1, glm::value_ptr(pointLightColor3));

	//cat lamp
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "constant4"), constant4);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "linear4"), linear4);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "quadratic4"), quadratic4);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "intensity4"), intensity4);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightCutOff"), spotLightCutOff);

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightPosition4"), 1, glm::value_ptr(pointLightPosition4));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor4"), 1, glm::value_ptr(pointLightColor4));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightDirection"), 1, glm::value_ptr(spotLightDirection));

	//headlight
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "constant5"), constant5);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "linear5"), linear5);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "quadratic5"), quadratic5);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "intensity5"), intensity5);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightCutOff5"), spotLightCutOff5);

	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightPosition5"), 1, glm::value_ptr(pointLightPosition5));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor5"), 1, glm::value_ptr(pointLightColor5));
	glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "spotLightDirection5"), 1, glm::value_ptr(spotLightDirection5));


	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

}

void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	glGenFramebuffers(1, &shadowMapFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 lightView = glm::lookAt(lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightView = glm::lookAt(glm::vec3(lightRotation * glm::vec4(lightDir, 1.0f)), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = 0.1f, far_plane = 5.0f;
	glm::mat4 lightProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
	return lightSpaceTrMatrix;
}

void initSkybox()
{
	std::vector<const GLchar*> faces;
	faces.push_back("skybox/right.tga");
	faces.push_back("skybox/left.tga");
	faces.push_back("skybox/top.tga");
	faces.push_back("skybox/bottom.tga");
	faces.push_back("skybox/back.tga");
	faces.push_back("skybox/front.tga");
	mySkyBox.Load(faces);
}

void drawObjects(gps::Shader shader, bool depthPass) {

	shader.useShaderProgram();

	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	//nanosuit.Draw(shader);

	//apply shades to the man also
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -2.0f));
	model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));




	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	rock.Draw(shader);
	casa.Draw(shader);
	ground.Draw(shader);
	car.Draw(shader);
	drawer.Draw(shader);
	inLight.Draw(shader);
	man.Draw(shader);
	cat.Draw(shader);
	others.Draw(shader);
	sirena.Draw(shader);
	plants.Draw(shader);


	//rat movement

	if (moveUpRight) {
		ratPosition.x += ratSpeed; // Move right
		ratPosition.z -= ratSpeed; // Move up
	}
	else {
		ratPosition.x -= ratSpeed; // Move left
		ratPosition.z += ratSpeed; // Move down
	}

	// Check if rat reached a certain distance to change direction
	if (glm::abs(ratPosition.x + 7.88132f) > 3.0f || glm::abs(ratPosition.z - 5.308771f) > 3.0f) {
		moveUpRight = !moveUpRight; // Reverse direction
	}

	// Update the model matrix for the rat
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -2.0f));
	model = glm::translate(model, ratPosition);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	rat.Draw(shader);

	//if it is day, draw the skybox else black
	if (!depthPass && isDay)
	{
		mySkyBox.Draw(skyboxShader, view, projection);
	}
	else {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
}




void renderScene() {

	// depth maps creation pass
	//TODO - Send the light-space transformation matrix to the depth map creation shader and
	//		 render the scene in the depth map

	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"), 1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	drawObjects(depthMapShader, true);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// render depth map on screen - toggled with the M key

	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT);

		screenQuadShader.useShaderProgram();

		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);
	}
	else {

		// final scene rendering pass (with shadows)

		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myCustomShader.useShaderProgram();

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		//bind the shadow map
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		drawObjects(myCustomShader, false);

		//draw a white cube around the light

		lightShader.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = lightRotation;
		model = glm::translate(model, 1.0f * lightDir);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		lightCube.Draw(lightShader);
	}
}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char* argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	initOpenGLState();
	initObjects();
	initSkybox();
	initShaders();
	initUniforms();
	initFBO();

	glCheckError();

	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();

		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();



	return 0;
}
