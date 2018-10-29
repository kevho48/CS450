#include "Light.h"

Light::Light()
{
	
}

Light::Light(glm::vec3 pos)
{
	this->position = pos;
}
Light::Light(glm::vec3 pos, glm::vec3 color)
{
	this->position = pos;
	this->color = color;
}

glm::vec3 Light::getPos()
{
	return position;
}
glm::vec3 Light::getColor()
{
	return color;
}
void Light::read(std::string data)
{
	std::stringstream ss(data);
	char extra[80];

	ss >> extra;
	ss >> position.x >> position.y >> position.z;

	if (ss >> color.r)
	{
		ss >> color.g >> color.b;
	}
	else
	{
		color.r = 1;
		color.g = 1;
		color.b = 1;
	}
}
std::string Light::toString()
{
	std::string output = "l ";
	output += std::to_string(position.x) + " ";
	output += std::to_string(position.y) + " ";
	output += std::to_string(position.z) + " ";
	output += std::to_string(color.r) + " ";
	output += std::to_string(color.g) + " ";
	output += std::to_string(color.b);

	return output;
}

Light::~Light()
{

}