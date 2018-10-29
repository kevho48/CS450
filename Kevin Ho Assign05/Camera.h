#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
class Camera
{
	public:
		Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up, float fov, float nearPlane, float farPlane, int bufferWidth, int bufferHeight);
		void updateBufferSize(int width, int height);
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
		void rotateRight(float angle);
		void rotateUp(float angle);
		void forward(float distance);
		void strafeRight(float distance);

	private:
		glm::vec3 eye = glm::vec3(0, 0, 1);
		glm::vec3 lookAt = glm::vec3(0, 0, 0);
		glm::vec3 up = glm::vec3(0, 1, 0);
		float fov = glm::radians(100.0f);
		float nearPlane = 0.1;
		float farPlane = 1000.0;
		float bufferWidth = 500;
		float bufferHeight = 500;

		glm::mat4 viewMat;
		glm::mat4 projMat;

		void recomputeViewMat();
		void recomputeProjMat();
};