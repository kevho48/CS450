#pragma once

#include <glm/glm.hpp>
#include <sstream>
#include <string>

#include "Camera.h"
#include "MeshMaterial.h"
#include "Light.h"

#include "Shader.h"


class MeshShader : public Shader
{
	public:
		MeshShader(string vertexFilename, string fragmentFilename);
		void setModelTransform(glm::mat4 &modelMat);
		void setViewAndProjection(Camera *camera);
		void setMaterial(MeshMaterial *material);
		void setLight(Light *light);

	private:
		GLint modelMatLoc = -1;
		GLint viewMatLoc = -1;
		GLint normalMatLoc = -1;
		GLint projMatLoc = -1;
		GLint shinyLoc = -1;
		GLint kdLoc = -1;
		GLint ksLoc = -1;
		GLint lightPosLoc = -1;
		GLint lightColorLoc = -1;

		glm::mat4 modelMat;
		glm::mat4 viewMat;
};
