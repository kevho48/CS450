#include <iostream>
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
	int x = 0;
	cin >> x;
	return 0;
}