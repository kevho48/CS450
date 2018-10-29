#include "Object.h"

Object::Object()
{

}

void Object::setModel(Model *m)
{
	model = m;
}

void Object::translate(glm::vec3 t)
{
	glm::mat4x4 M = glm::translate(t);
	modelMat = M * modelMat;
	center = M * center;
}

void Object::rotate(float angle, glm::vec3 axis)
{
	glm::mat4x4 t1 = glm::translate(-glm::vec3(center));
	glm::mat4x4 r = glm::rotate(angle, axis);
	glm::mat4x4 t2= glm::translate(glm::vec3(center));

	modelMat = t2 * r * t1 * modelMat;
}

void Object::scale(glm::vec3 s)
{
	glm::mat4x4 goku = glm::translate(-glm::vec3(center));
	glm::mat4x4 vegeta = glm::scale(s);
	glm::mat4x4 vegito = glm::translate(glm::vec3(center));

	modelMat = vegito * vegeta * goku * modelMat;
}

void Object::draw(MeshShader *shader)
{
	if (model != NULL)
	{
		shader->setModelTransform(modelMat);
		model->draw();
	}
}

void Object::resetTransformations()
{
	modelMat = glm::mat4x4(1.0);
	center = glm::vec4(0, 0, 0, 1);
}