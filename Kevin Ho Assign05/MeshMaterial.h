#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"


class MeshMaterial
{
	public:
		MeshMaterial();
		float getShininess();
		glm::vec3 getKd();
		glm::vec3 getKs();
		void setShininess(float value);
		void setKd(glm::vec3 value);
		void setKs(glm::vec3 value);

	private:
		glm::vec3 kd = glm::vec3(1, 1, 1);
		glm::vec3 ks = glm::vec3(0, 0, 0);
		float shiny = 1.0;

};