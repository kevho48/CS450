#include "MeshShader.h"

MeshShader::MeshShader(string vertexFilename, string fragmentFilename) 
	:Shader(vertexFilename, fragmentFilename)
{
	modelMatLoc = glGetUniformLocation(programID, "modelMat");
	viewMatLoc = glGetUniformLocation(programID, "viewMat");
	normalMatLoc = glGetUniformLocation(programID, "normalMat");
	projMatLoc = glGetUniformLocation(programID, "projMat");
	shinyLoc = glGetUniformLocation(programID, "material.shininess");
	kdLoc = glGetUniformLocation(programID, "material.kd");
	ksLoc = glGetUniformLocation(programID, "material.ks");
	lightPosLoc = glGetUniformLocation(programID, "light.pos");
	lightColorLoc = glGetUniformLocation(programID, "light.color");
}

void MeshShader::setModelTransform(glm::mat4 &modelMat)
{
	this->modelMat = modelMat;
	glm::mat4 normalMat = glm::transpose(glm::inverse(viewMat * modelMat));

	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	glUniformMatrix4fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));
}

void MeshShader::setViewAndProjection(Camera *camera)
{
	this->viewMat = camera->getViewMatrix();
	glm::mat4x4 normalMat = glm::transpose(glm::inverse(viewMat * modelMat));
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniformMatrix4fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));
}

void MeshShader::setMaterial(MeshMaterial *material)
{
	glUniform1f(shinyLoc, material->getShininess());
	glUniform3fv(kdLoc, 1, glm::value_ptr(material->getKd()));
	glUniform3fv(ksLoc, 1, glm::value_ptr(material->getKs()));
}

void MeshShader::setLight(Light *light)
{
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(light->getPos()));
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(light->getColor()));
}