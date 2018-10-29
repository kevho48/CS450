#include "Material.h"

Material::Material()
{

}

Material::~Material()
{

}

glm::vec3 Material::getColor()
{
	return diffuseColor;
}

float Material::getKd()
{
	return kd;
}

float Material::getKs()
{
	return ks;
}

float Material::getShine()
{
	return shininess;
}

float Material::getTransmittance()
{
	return transmittance;
}

float Material::getIndexRefraction()
{
	return refractionIndex;
}

void Material::read(std::string data)
{
	std::stringstream ss(data);
	char extra[80];

	ss >> extra;

	ss >> diffuseColor.r >> diffuseColor.g >> diffuseColor.b >> kd >> ks >> shininess >> transmittance;

	if (ss >> refractionIndex)
	{

	}

}

std::string Material::toString()
{
	std::string output = "f ";
	output += std::to_string(diffuseColor.r) + " ";
	output += std::to_string(diffuseColor.g) + " ";
	output += std::to_string(diffuseColor.b) + " ";
	output += std::to_string(kd) + " ";
	output += std::to_string(ks) + " ";
	output += std::to_string(shininess) + " ";
	output += std::to_string(transmittance) + " ";
	output += std::to_string(refractionIndex);

	return output;
}
