#include <string>
#include <cstring>
#include <sstream>
#include <glm/glm.hpp>

class Sphere
{
	public:
		Sphere();
		Sphere(glm::vec3 center, double radius);
		glm::vec3 getCenter();
		double getRadius();
		int getMaterialIndex();

		void setMaterialIndex(int matIndex);
		void read(std::string data);
		std::string toString();

		~Sphere();

	private:
		glm::vec3 position = glm::vec3(0, 0, 0);
		double radius = 1;
		int materialIndex = -1;
};

