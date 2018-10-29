#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include "glm/glm.hpp"
using namespace std;

void printRM(string name, glm::mat4 &M) {
	cout << name << ":" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << M[j][i] << ", ";
		}
		cout << endl;
	}
}

void printRM(string name, glm::mat3 &M) {
	cout << name << ":" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << M[j][i] << ", ";
		}
		cout << endl;
	}
}

#endif
