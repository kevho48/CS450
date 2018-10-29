#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>
#include <algorithm>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Object.h"
#include "MeshShader.h"
#include "MeshMaterial.h"
#include "Light.h"
#include "Camera.h"
#include "Settings.h"
using namespace std;

GLFWwindow* window = NULL;
int windowWidth = 800;
int windowHeight = 600;
int framebufferWidth = windowWidth;
int framebufferHeight = windowHeight;

const float CAMERA_WALK_SPEED = 0.1;
const float CAMERA_ROTATE_SPEED = 5.0f;

const float MOUSE_ROTATE_SPEED = 30.0f;

const float OBJECT_ROTATE_SPEED = 5.0f;
const float OBJECT_TRANSLATE_SPEED = 0.1;

const bool DEBUG_PRINT = true;

Object *myObject = NULL;
Camera *camera = NULL;
MeshMaterial *material = NULL;
Light *light = NULL;

bool firstMouseMove = true;
double mouseX = -1;
double mouseY = -1;

static void error_callback(int error, const char* description) {
	cerr << "ERROR " << error << ": " << description << endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {		
		
		case GLFW_KEY_SPACE:
			myObject->resetTransformations();
			if(DEBUG_PRINT) cout << "Object: Reset" << endl;
			break;
		case GLFW_KEY_F:
			myObject->scale(glm::vec3(0.8, 1, 1));
			if (DEBUG_PRINT) cout << "Object: Scale X 0.8" << endl;
			break;
		case GLFW_KEY_G:
			myObject->scale(glm::vec3(1.25, 1, 1));
			if (DEBUG_PRINT) cout << "Object: Scale X 1.25" << endl;
			break;
		case GLFW_KEY_J:
			myObject->translate(glm::vec3(0, 0, -OBJECT_TRANSLATE_SPEED));
			if (DEBUG_PRINT) cout << "Object: Translate Z -" << OBJECT_TRANSLATE_SPEED << endl;
			break;
		case GLFW_KEY_L:
			myObject->translate(glm::vec3(0, 0, OBJECT_TRANSLATE_SPEED));
			if (DEBUG_PRINT) cout << "Object: Translate Z +" << OBJECT_TRANSLATE_SPEED << endl;
			break;
		case GLFW_KEY_I:
			myObject->translate(glm::vec3(0, OBJECT_TRANSLATE_SPEED, 0));
			if (DEBUG_PRINT) cout << "Object: Translate Y +" << OBJECT_TRANSLATE_SPEED << endl;
			break;
		case GLFW_KEY_K:
			myObject->translate(glm::vec3(0, -OBJECT_TRANSLATE_SPEED, 0));
			if (DEBUG_PRINT) cout << "Object: Translate Y -" << OBJECT_TRANSLATE_SPEED << endl;
			break;
		case GLFW_KEY_U:
			myObject->rotate(-glm::radians(OBJECT_ROTATE_SPEED), glm::vec3(0, 1, 0));
			if (DEBUG_PRINT) cout << "Object: Rotate Y -" << OBJECT_ROTATE_SPEED << endl;
			break;
		case GLFW_KEY_O:
			myObject->rotate(glm::radians(OBJECT_ROTATE_SPEED), glm::vec3(0, 1, 0));
			if (DEBUG_PRINT) cout << "Object: Rotate Y +" << OBJECT_ROTATE_SPEED << endl;
			break;

		
		case GLFW_KEY_W:
			camera->forward(CAMERA_WALK_SPEED);
			if (DEBUG_PRINT) cout << "Camera: Forward +" << CAMERA_WALK_SPEED << endl;
			break;
		case GLFW_KEY_S:
			camera->forward(-CAMERA_WALK_SPEED);
			if (DEBUG_PRINT) cout << "Camera: Forward -" << CAMERA_WALK_SPEED << endl;
			break;
		case GLFW_KEY_A:
			camera->strafeRight(-CAMERA_WALK_SPEED);
			if (DEBUG_PRINT) cout << "Camera: Strafe left " << CAMERA_WALK_SPEED << endl;
			break;
		case GLFW_KEY_D:
			camera->strafeRight(CAMERA_WALK_SPEED);
			if (DEBUG_PRINT) cout << "Camera: Strafe right " << CAMERA_WALK_SPEED << endl;
			break;

		case GLFW_KEY_1:
			cout << "Material: Gold" << endl;			
			material->setShininess(10.0);			
			material->setKd(glm::vec3(0.0, 0.0, 0.0));
			material->setKs(glm::vec3(1.0, 0.71, 0.29));
			break;

		case GLFW_KEY_2:
			cout << "Material: Red Rubber" << endl;
			material->setShininess(10.0);
			material->setKd(glm::vec3(1.0, 0.0, 0.0));
			material->setKs(glm::vec3(0.7,0.04,0.04));			
			break;

		case GLFW_KEY_3:
			cout << "Material: Shiny Red Plastic" << endl;
			material->setShininess(32.0);
			material->setKd(glm::vec3(1,0,0));
			material->setKs(glm::vec3(0.7, 0.6, 0.6));
			break;

		case GLFW_KEY_4:
			cout << "Material: Iron" << endl;
			material->setShininess(10.0);
			material->setKd(glm::vec3(0.0, 0.0, 0.0));
			material->setKs(glm::vec3(0.56, 0.57, 0.58));
			break;	

		case GLFW_KEY_MINUS:
		{
			float shiny = material->getShininess();
			shiny -= 1.0;
			if (shiny < 0.0) shiny = 0;
			material->setShininess(shiny);
			cout << "Shininess: " << material->getShininess() << endl;
			break;
		}

		case GLFW_KEY_EQUAL:
		{
			float shiny = material->getShininess();
			shiny += 1.0;			
			material->setShininess(shiny);
			cout << "Shininess: " << material->getShininess() << endl;
			break;
		}
							
		default:
			break;
		}	
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouseMove) {
		mouseX = xpos;
		mouseY = ypos;
		firstMouseMove = false;
	}

	double xDiff = xpos - mouseX;
	double yDiff = ypos - mouseY;

	xDiff /= framebufferWidth;
	yDiff /= framebufferHeight;

	camera->rotateRight(glm::radians(-xDiff*MOUSE_ROTATE_SPEED));
	camera->rotateUp(glm::radians(-yDiff*MOUSE_ROTATE_SPEED));

	mouseX = xpos;
	mouseY = ypos;
}

int main(int argc, char **argv) {

	if (argc < 2) {
		cout << "ERROR: Must pass in 3D model filename!" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "BEGIN OPENGL EXERCISE" << endl;

	// Set error callback
	glfwSetErrorCallback(error_callback);

	// (Try to) initialize GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// MAC-SPECIFIC: Convince Mac that, yes, we want to use OpenGL version 3.3 (and NOT 2.1)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	// Create our GLFW window
	window = glfwCreateWindow(	windowWidth, windowHeight, 
								"Update Your Memes You're Outdated", 
								NULL, NULL);

	// Were we able to make it?
	if (!window) {
		// Kill GLFW and exit program
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// We want to draw to this window, so make the OpenGL context associated with this window our current context.
	glfwMakeContextCurrent(window);

	// Basically, turning VSync on (so we will wait until the screen is updated once before swapping the back and front buffers
	glfwSwapInterval(1);

	// Set our keyboard callback function, so that we can process keyboard input!
	glfwSetKeyCallback(window, key_callback);

	// Set mouse movement callback
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// MAC-SPECIFIC: Some issues occur with using OpenGL core and GLEW; so, we'll use the experimental version of GLEW
	glewExperimental = true;

	// (Try to) initalize GLEW
	GLenum err = glewInit();

	// Call glGetError() to clear it out (can occur after glewInit() for some reason)
	glGetError();

	if (GLEW_OK != err) {
		// We couldn't start GLEW, so we've got to go.
		// Kill GLFW and get out of here
		cout << "ERROR: GLEW could not start: " << glewGetErrorString(err) << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	cout << "GLEW initialized; version ";
	cout << glewGetString(GLEW_VERSION) << endl;

	GLint glMajor, glMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
	glGetIntegerv(GL_MINOR_VERSION, &glMinor);
	cout << "OpenGL context version: ";
	cout << glMajor << "." << glMinor << endl;
	cout << "Supported GLSL version is ";
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// Load shader program
#ifdef GRAD_VERSION
	MeshShader *shader = new MeshShader("../shaders/SimpleG.vs", "../shaders/SimpleG.ps");
#else
	MeshShader *shader = new MeshShader("../shaders/Simple.vs", "../shaders/Simple.ps");
#endif

	// Create material
	material = new MeshMaterial();
	material->setShininess(32.0);
	material->setKd(glm::vec3(1, 0, 0));
	material->setKs(glm::vec3(0.7, 0.6, 0.6));

	// Create Light
	light = new Light(glm::vec3(0.0, 2.0, 1.0), glm::vec3(3, 3, 3));
		
	// Load model
	Model *model = new Model();
	if (!model->load(argv[1])) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Create object
	myObject = new Object();
	myObject->setModel(model);
	
	// Create camera
	camera = new Camera(glm::vec3(0.0, 2.0, 2.0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0),
						glm::radians(100.0f), 0.01, 1000.0, framebufferWidth, framebufferHeight);

	// Set the background color to a shade of blue
	glClearColor(0.0f, 0.0f, 0.7f, 1.0f);	

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable shader program
	shader->activate();
	
	while (!glfwWindowShouldClose(window)) {
		//cout << "Rendering..." << endl;

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		glViewport(0, 0, framebufferWidth, framebufferHeight);

		// Update framebuffer size
		camera->updateBufferSize(framebufferWidth, framebufferHeight);

		// Clear the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update view and projection
		shader->setViewAndProjection(camera);

		// Set material
		shader->setMaterial(material);

		// Set light
		shader->setLight(light);
		
		// Draw object
		myObject->draw(shader);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		this_thread::sleep_for(chrono::milliseconds(15));
	}

	// Cleanup
	delete camera;
	delete material;
	delete myObject;
	delete model;
	delete shader;
	delete light;

	// Destroy window and stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
