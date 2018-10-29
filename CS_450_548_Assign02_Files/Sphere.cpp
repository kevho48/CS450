#include "Sphere.h"

Sphere::Sphere()
{

}

Sphere::Sphere(glm::vec3 center, double radius)
{
	this->position = center;
	this->radius = radius;
}

glm::vec3 Sphere::getCenter()
{
	return position;
}

double Sphere::getRadius()
{
	return radius;
}

void Sphere::read(std::string data)
{
	std::stringstream ss(data);
	char extra[80];

	ss >> extra;
	
	ss >> position.x >> position.y >> position.z;
	
	if (ss >> radius)
	{

	}

}

std::string Sphere::toString()
{
	std::string output = "s ";
	output += std::to_string(position.x) + " ";
	output += std::to_string(position.y) + " ";
	output += std::to_string(position.z) + " ";
	output += std::to_string(radius);

	return output;
}

Sphere::~Sphere()
{

}
