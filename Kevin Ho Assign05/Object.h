#pragma once

#include <glm/glm.hpp>
#include "Model.h"
#include "MeshShader.h"

class Object
{
	public:
		Object();
		void setModel(Model *m);
		void translate(glm::vec3 t);
		void rotate(float angle, glm::vec3 axis);
		void scale(glm::vec3 s);
		void draw(MeshShader *shader);
		void resetTransformations();

	private:
		Model *model = NULL;
		glm::mat4 modelMat;
		glm::vec4 center = glm::vec4(0, 0, 0, 1);
};