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
#include "Scene.h"
using namespace std;

int main(int argc, char **argv) {
	///////////////////////////////////////////////////////////////////////////
	// TESTING MATERIAL
	///////////////////////////////////////////////////////////////////////////

	Material *m = new Material();
	cout << "m->toString():" << endl;
	cout << m->toString() << endl;

	m->read("f 1 2 3 4 5 6 7 8");
	cout << "m->toString():" << endl;
	cout << m->toString() << endl;
	
	cout << "Color: " << glm::to_string(m->getColor()) << endl;
	cout << "Kd: " << m->getKd() << endl;
	cout << "Ks: " << m->getKs() << endl;
	cout << "Shininess: " << m->getShine() << endl;
	cout << "Transmittance: " << m->getTransmittance() << endl;
	cout << "IndexRefraction: " << m->getIndexRefraction() << endl;

	delete m;
	
	///////////////////////////////////////////////////////////////////////////
	// TESTING SPHERE
	///////////////////////////////////////////////////////////////////////////

	Sphere *sphere = new Sphere();	
	sphere->read("s 4.5 6.7 8.9 11.3");
	cout << "sphere->toString():" << endl;
	cout << sphere->toString() << endl;

	cout << "sphere->getMaterialIndex() BEFORE: " << sphere->getMaterialIndex() << endl;
	sphere->setMaterialIndex(2);
	cout << "sphere->getMaterialIndex() AFTER: " << sphere->getMaterialIndex() << endl;
	
	delete sphere;

	///////////////////////////////////////////////////////////////////////////
	// TESTING SCENE
	///////////////////////////////////////////////////////////////////////////

	Scene *scene = new Scene();	

	cout << "scene->toString():" << endl;
	cout << scene->toString() << endl;

	cout << "Light cnt: " << scene->getLightCnt() << endl;
	cout << "Sphere cnt: " << scene->getSurfaceCnt() << endl;
	cout << "Material cnt: " << scene->getMaterialCnt() << endl;
	cout << "Buffer width: " << scene->getBufferWidth() << endl;
	cout << "Buffer height: " << scene->getBufferHeight() << endl;

	float *imageBuffer = scene->getImageBuffer();

	if (imageBuffer) {
		cout << "ERROR: IMAGE BUFFER NOT NULL" << endl;
	}
	else {
		cout << "IMAGE BUFFER NULL" << endl;
	}

	scene->loadNFF("C:/Users/Kevin Ho/Documents/CS450 Homework/CS_450_548_Assign03_Files/balls01.txt", 640, 480);
	cout << "scene->toString() AFTER LOAD:" << endl;
	cout << scene->toString() << endl;

	cout << "Light cnt: " << scene->getLightCnt() << endl;
	cout << "Sphere cnt: " << scene->getSurfaceCnt() << endl;
	cout << "Material cnt: " << scene->getMaterialCnt() << endl;
	cout << "Buffer width: " << scene->getBufferWidth() << endl;
	cout << "Buffer height: " << scene->getBufferHeight() << endl;

	imageBuffer = scene->getImageBuffer();

	if (imageBuffer) {
		cout << "IMAGE BUFFER NOT NULL" << endl;
	}
	else {
		cout << "ERROR: IMAGE BUFFER NULL" << endl;
	}

	delete scene;

	int x = 0;
	std::cin >> x;

	return 0;
}