#pragma once

#include "glm/glm.hpp"
#include <string>
#include <cstring>
#include <sstream>

class Light
{
	public:
		Light();								//does nothing
		Light(glm::vec3 pos);					//sets position
		Light(glm::vec3 pos, glm::vec3 color);	//sets position and color

		glm::vec3 getPos();
		glm::vec3 getColor();
		void read(std::string data);
		std::string toString();

		~Light();

	private:
		glm::vec3 position = glm::vec3(0,0,0);						//holds position, defaults at (0, 0, 0)
		glm::vec3 color = glm::vec3(1,1,1);							//holds color, defaults at (1, 1, 1)

};