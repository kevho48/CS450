#pragma once

#include <string>
#include <cstring>
#include <sstream>
#include <glm/glm.hpp>

class Material
{
	public:
		Material();
		~Material();

		glm::vec3 getColor();
		float getKd();
		float getKs();
		float getShine();
		float getTransmittance();
		float getIndexRefraction();
		void read(std::string data);
		std::string toString();

	private:
		glm::vec3 diffuseColor = glm::vec3(1, 1, 1);
		float kd = 1.0;
		float ks = 0.0;
		float shininess = 1.0;
		float transmittance = 0.0;
		float refractionIndex = 1.0;
};

