/*#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>
#include "glm/glm.hpp"					// NOTE: Need to compile, hence the quotes
#include "glm/gtx/string_cast.hpp"
#include "Light.h"
#include "Sphere.h"
#include "Camera.h"
using namespace std;

int main(int argc, char **argv) {
	///////////////////////////////////////////////////////////////////////////
	// TESTING LIGHT
	///////////////////////////////////////////////////////////////////////////

	Light *light = new Light();
	cout << light->toString() << endl;

	delete light;
	light = new Light(glm::vec3(1, 2, 3));
	cout << light->toString() << endl;

	delete light;
	light = new Light(glm::vec3(3, 2, 1), glm::vec3(0.6, 0.9, 0.1));
	cout << light->toString() << endl;

	light->read("l 4 5 6");
	cout << light->toString() << endl;

	light->read("l 7 8 9 0.2 0.7 0.8");
	cout << light->toString() << endl;

	cout << glm::to_string(light->getPos()) << endl;
	cout << glm::to_string(light->getColor()) << endl;

	delete light;

	///////////////////////////////////////////////////////////////////////////
	// TESTING SPHERE
	///////////////////////////////////////////////////////////////////////////

	Sphere *sphere = new Sphere();
	cout << sphere->toString() << endl;

	delete sphere;
	sphere = new Sphere(glm::vec3(2, 4, 6), 5);
	cout << sphere->toString() << endl;

	sphere->read("s 4.5 6.7 8.9 11.3");
	cout << sphere->toString() << endl;

	cout << glm::to_string(sphere->getCenter()) << endl;
	cout << sphere->getRadius() << endl;

	delete sphere;

	///////////////////////////////////////////////////////////////////////////
	// TESTING CAMERA
	///////////////////////////////////////////////////////////////////////////

	Camera *camera = new Camera();
	cout << camera->toString() << endl;

	delete camera;
	camera = new Camera(
		glm::vec3(1, 2, 3),
		glm::vec3(4, 5, 6),
		glm::vec3(0, 1, 0),
		45.0,
		0.2,
		glm::ivec2(800, 600));
	cout << camera->toString() << endl;

	camera->setEye(glm::vec3(9, 8, 7));
	cout << glm::to_string(camera->getEye()) << endl;

	camera->setLookAt(glm::vec3(2, 4, 6));
	cout << glm::to_string(camera->getLookAt()) << endl;

	camera->setUp(glm::vec3(0, -1, 0));
	cout << glm::to_string(camera->getUp()) << endl;

	camera->setAngle(65.7);
	cout << camera->getAngle() << endl;

	camera->setNearPlaneDist(2.3);
	cout << camera->getNearPlaneDist() << endl;

	camera->setViewportSize(glm::ivec2(1024, 768));
	cout << glm::to_string(camera->getViewportSize()) << endl;
	
	cout << camera->toString() << endl;

	camera->read(string("v\n")
		+ "from 3 2 1" + "\n"
		+ "at 6 5 4" + "\n"
		+ "up 1 1 0" + "\n"
		+ "angle 76.5" + "\n"
		+ "hither 0.23" + "\n"
		+ "resolution 1000 2000");
	cout << camera->toString() << endl;
	
	return 0;
}
*/