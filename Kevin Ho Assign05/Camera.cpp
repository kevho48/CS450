#include "Camera.h"

Camera::Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up, float fov, float nearPlane, float farPlane, int bufferWidth, int bufferHeight)
{
	this->eye = eye;
	this->lookAt = lookAt;
	this->up = up;
	this->fov = fov;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->bufferWidth = bufferWidth;
	this->bufferHeight = bufferHeight;

	void recomputeViewMat();
	void recomputeProjMat();
}

void Camera::recomputeViewMat()
{
	viewMat = glm::lookAt(eye, lookAt, up);
}

void Camera::recomputeProjMat()
{
	projMat = glm::perspective(fov, (float)bufferWidth/(float)bufferHeight, nearPlane, farPlane);
}

void Camera::updateBufferSize(int width, int height)
{
	bufferWidth = (float)width;
	bufferHeight = (float)height;

	recomputeProjMat();
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMat;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projMat;
}

void Camera::rotateRight(float angle)
{
	glm::mat4x4 memes = glm::translate(-eye);
	glm::mat4x4 better = glm::rotate(angle, up);
	glm::mat4x4 use = glm::translate(eye);
	lookAt = glm::vec3(use * better * memes * glm::vec4(lookAt, 1.0f));

	recomputeViewMat();
}

void Camera::rotateUp(float angle)
{
	glm::vec3 forward = eye - lookAt;
	glm::vec3 right = glm::normalize(glm::cross(forward, up));
	
	glm::mat4x4 goku = glm::translate(-eye);
	glm::mat4x4 vegeta = glm::rotate(angle, right);
	glm::mat4x4 vegito = glm::translate(eye);
	lookAt = glm::vec3(vegito * vegeta * goku * glm::vec4(lookAt, 1.0f));

	recomputeViewMat();
}

void Camera::forward(float distance)
{
	glm::vec3 forward = glm::normalize(lookAt - eye) * distance;
	glm::mat4x4 translate = glm::translate(forward);
	eye = glm::vec3(translate * glm::vec4(eye, 1.0));

	recomputeViewMat();
}

void Camera::strafeRight(float distance)
{
	glm::vec3 forward = lookAt - eye;
	glm::vec3 right = glm::normalize(glm::cross(forward, up));
	right = right * distance;
	
	glm::mat4x4 translate = glm::translate(right);
	eye = glm::vec3(translate * glm::vec4(eye, 1.0));
	lookAt = glm::vec3(translate * glm::vec4(lookAt, 1.0));
	
	recomputeViewMat();
}