#include "HitRecord.h"

HitRecord::HitRecord()
{

}

HitRecord::HitRecord(double t, glm::vec3 point, glm::vec3 normal, int matIndex)
{
	this->t = t;
	this->point = point;
	this->normal = normal;
	this->materialIndex = matIndex;
}

double HitRecord::getT()
{
	return t;
}

glm::vec3 HitRecord::getPoint()
{
	return point;
}

glm::vec3 HitRecord::getNormal()
{
	return normal;
}

int HitRecord::getMatIndex()
{
	return materialIndex;
}

void HitRecord::setT(double t)
{
	this->t = t;
}

void HitRecord::setPoint(glm::vec3 point)
{
	this->point = point;
}

void HitRecord::setNormal(glm::vec3 normal)
{
	this->normal = normal;
}

void HitRecord::setMatIndex(int index)
{
	this->materialIndex = index;
}

HitRecord::~HitRecord()
{

}
