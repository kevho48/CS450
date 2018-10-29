#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include "Light.h"
#include "Material.h"
#include "Sphere.h"


class Scene
{
	public:
		Scene();
		~Scene();

		int getLightCnt();
		int getSurfaceCnt();
		int getMaterialCnt();
		float* getImageBuffer();
		int getBufferWidth();
		int getBufferHeight();

		std::string toString();
		bool loadNFF(std::string filename, int windowWidth, int windowHeight);

	private:
		std::vector<Light*> lights;
		std::vector<Material*> materials;
		std::vector<Sphere*> spheres;
		glm::vec3 backgroundColor = glm::vec3(0, 0, 0);
		
		float* imageBuffer = NULL;
		int height = 0;
		int width = 0;

		void cleanup();
};