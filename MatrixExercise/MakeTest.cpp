#include <iostream>
#include <cstring>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;

void printRM(string name, glm::mat3 &M)
{
	cout << name << ":" << endl;
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)																	
		{
			cout << M[col][row] << ", ";
		}
		cout << endl;
	}
}

int main(int argc, char **argv)
{
	cout << "BEGIN MATRIX EXERCISE" << endl;
	glm::mat3 M = glm::mat3(1,2,3,4,5,6,7,8,9);
	cout << glm::to_string(M) << endl;
	printRM("M", M);
	
	glm::mat3 T;
	printRM("T", T);

	glm::mat3 I = glm::mat3(1.0);
	printRM("I", I);

	glm::mat3 Z = glm::mat3(0.0);
	printRM("Z", Z);

	glm::mat3 H = glm::mat3(5.0);
	printRM("H", H);

	glm::vec3 U = glm::normalize(glm::vec3(1, 1, 0));
	glm::vec3 V = glm::normalize(glm::vec3(-1, 1, 0));
	glm::vec3 W = glm::normalize(glm::cross(U, V));
	glm::mat3 B = glm::mat3(U, V, W);

	printRM("M", M);
	printRM("T", T);
	printRM("I", I);
	printRM("[U,V,W]", B);

	glm::mat3 Mt = glm::transpose(M);
	printRM("Mt", Mt);
	
	glm::mat3 G = M + H;
	printRM("M + F", G);

	glm::mat3 Eight = 8.0f*I;
	printRM("Eight", Eight);

	glm::mat3 A = glm::mat3
	(
		1, 2, 3,
		0, -1, 1,
		3, 2, 1
	);

	glm::mat3 C = glm::mat3
	(
		5,4,1,
		6,3,1,
		5,2,2
	);

	glm::mat3 D = A*C;
	printRM("D", D);

	glm::mat3 T = glm::mat3(5, 0, 0, 0, 2, 0, 0, 0, -1);
	glm::vec3 v = glm::vec3(7, 16, 9);
	glm::vec3 w = T*v;

	cout << "w: " << glm::to_string(w) << endl;

	int i;
	cin >> i;

	return 0;
}