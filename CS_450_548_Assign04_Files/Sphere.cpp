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

bool Sphere::getIntersection(glm::vec3 rayStart, glm::vec3 rayDir, HitRecord &record)
{
	//E = ray start
	//D = raydir
	//C = sphere center
	//. = dot product glm::dot()
	float a = glm::dot(rayDir, rayDir);
	float b = 2 * glm::dot(rayDir, (rayStart - position));
	float c = glm::dot((rayStart - position), (rayStart - position)) - pow(radius,2);
	float discriminant = pow(b, 2) - (4 * a * c);
	
	if (discriminant < 0)
		return false;
	else
	{
		float t1 = (-b + pow(discriminant, 0.5))/(2*a);
		float t2 = (-b - pow(discriminant, 0.5))/(2*a);
		
		if (abs(t1) > abs(t2))
		{
			record.setT(t2);
			record.setPoint(rayStart + t2*rayDir);
		}
		else
		{
			record.setT(t1);
			record.setPoint(rayStart + t1 * rayDir);
		}

		record.setMatIndex(materialIndex);

		record.setNormal((record.getPoint() - position)/2.0f);
		
	}

	return true;
}

int Sphere::getMaterialIndex()
{
	return materialIndex;
}

void Sphere::setMaterialIndex(int matIndex)
{
	materialIndex = matIndex;
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
