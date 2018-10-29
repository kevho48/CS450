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

void Scene::setPixelInImageBuffer(int x, int y, glm::vec3 color)
{
	int index = (y * width + x) * 3;

	imageBuffer[index] = color.r;
	imageBuffer[index + 1] = color.g;
	imageBuffer[index + 2] = color.b;
}

void Scene::clearImageBuffer(glm::vec3 color)
{
	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			imageBuffer[(i * width + j) * 3] = color.r;
			imageBuffer[(i * width + j) * 3 + 1] = color.g;
			imageBuffer[(i * width + j) * 3 + 2] = color.b;
		}
	}
}

bool Scene::intersectRayWithScene(glm::vec3 rayStart, glm::vec3 rayDir, double minDist, HitRecord &bestRecord)
{
	bool anyHit = false;
	for (int i = 0; i < getSurfaceCnt(); i++)
	{
		bool omaeWaMoShindeiru = spheres[i]->getIntersection(rayStart, rayDir, bestRecord);
		if (omaeWaMoShindeiru)
		{
			if (bestRecord.getT() > minDist)
			{
				if (anyHit == false)
				{
					bestRecord.setT(spheres[i]->getIntersection(rayStart, rayDir, bestRecord));
					anyHit = true;
				}
				else
					bestRecord.setT(spheres[i]->getIntersection(rayStart, rayDir, bestRecord));
			}
		}
	}

	return anyHit;
}

void Scene::raytrace()
{
	clearImageBuffer(glm::vec3(0, 0, 0));
	int nx = width;
	int ny = height;
	int nearDist = 0;
	double aspectRatio = nx / ny;
	double imagePlaneHeight = 2.0;
	double imagePlaneWidth = imagePlaneHeight * aspectRatio;
	double left = -(imagePlaneWidth / 2.0);
	double right = imagePlaneWidth / 2.0;
	double bottom = -(imagePlaneHeight / 2.0);
	double top = imagePlaneHeight / 2.0;
	glm::vec3 U = glm::vec3(1, 0, 0);
	glm::vec3 V = glm::vec3(0, 1, 0);
	glm::vec3 W = glm::vec3(0, 0, 1);
	glm::vec3 E = glm::vec3(0, 0, 1.5);
	float d = 1.0;

	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			float u = left + (right - left) * (i + 0.5) / nx;
			float v = bottom + (top - bottom) * (j + 0.5) / ny;
			glm::vec3 rayDirection = u*U + v*V - d*W;
			glm::vec3 rayStart = E;
			HitRecord bestRecord;
			if (intersectRayWithScene(rayStart, rayDirection, nearDist, bestRecord))
				setPixelInImageBuffer(i, j, materials.at(bestRecord.getMatIndex())->getColor());
			else
				setPixelInImageBuffer(i, j, backgroundColor);
		}
	}

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