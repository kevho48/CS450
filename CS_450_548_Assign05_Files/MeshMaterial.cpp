#include "MeshMaterial.h"

MeshMaterial::MeshMaterial()
{

}

float MeshMaterial::getShininess()
{
	return shiny;
}

glm::vec3 MeshMaterial::getKd()
{
	return kd;
}

glm::vec3 MeshMaterial::getKs()
{
	return ks;
}

void MeshMaterial::setShininess(float value)
{
	shiny = value;
}

void MeshMaterial::setKd(glm::vec3 value)
{
	kd = value;
}

void MeshMaterial::setKs(glm::vec3 value)
{
	ks = value;
}
