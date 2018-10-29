#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	cleanup();
}

int Scene::getLightCnt()
{
	return lights.size();
}

int Scene::getSurfaceCnt()
{
	return spheres.size();
}

int Scene::getMaterialCnt()
{
	return materials.size();
}

float* Scene::getImageBuffer()
{
	return imageBuffer;
}

int Scene::getBufferWidth()
{
	return width;
}

int Scene::getBufferHeight()
{
	return height;
}

std::string Scene::toString()
{
	std::string output = "b " + std::to_string(backgroundColor.r) + " "
								+std::to_string(backgroundColor.g) + " "
								+ std::to_string(backgroundColor.b) + "\n";
	
	for (unsigned int i = 0; i < lights.size(); i++)
		output += lights[i]->toString() + "\n";
	for (unsigned int i = 0; i < spheres.size(); i++)
		output += spheres[i]->toString() + "\n";
	for (unsigned int i = 0; i < materials.size(); i++)
		output += materials[i]->toString() + "\n";

	return output;
}

bool Scene::loadNFF(std::string filename, int windowWidth, int windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	std::ifstream file;
	file.open(filename);
	if (file.fail())
	{
		std::cout << "FILE FAILED" << std::endl;
		return false;
	}
		
	cleanup();

	int currentMatIndex = -1;

	std::string line = "";	
	while (!file.eof())
	{
		getline(file, line);
		
		std::stringstream ss(line);

		std::string extra;

		ss >> extra;
		
		if (extra == "l")
		{
			Light* l = new Light;
			l->read(line);
			lights.push_back(l);
		}
		else
		{
			if (extra == "s")
			{
				Sphere* s = new Sphere;
				s->setMaterialIndex(currentMatIndex);
				s->read(line);
				spheres.push_back(s);
			}
			else
			{
				if (extra == "f")
				{
					Material* m = new Material;
					m->read(line);
					materials.push_back(m);
					currentMatIndex++;
				}
				else
				{
					if (extra == "b")
					{
						ss >> backgroundColor.r;
						ss >> backgroundColor.g;
						ss >> backgroundColor.b;
					}
				}
			}
		}
	}

	imageBuffer = new float[3 * width * height];

	return true;
}

void Scene::cleanup()
{
	for (unsigned int i = 0; i < lights.size(); i++)
		delete lights[i];
	lights.clear();

	for (unsigned int i = 0; i < spheres.size(); i++)
		delete spheres[i];
	spheres.clear();

	for (unsigned int i = 0; i < materials.size(); i++)
		delete materials[i];
	materials.clear();
	
	if (imageBuffer != NULL)
	{
		delete imageBuffer;
		imageBuffer = NULL;
	}
}