#pragma once

#include "glm\glm.hpp"

#define MIN_DISTANCE_T 0.001

class HitRecord
{
	public:
		HitRecord();
		HitRecord(double t, glm::vec3 point, glm::vec3 normal, int matIndex);

		double getT();
		glm::vec3 getPoint();
		glm::vec3 getNormal();
		int getMatIndex();
		void setT(double t);
		void setPoint(glm::vec3 point);
		void setNormal(glm::vec3 normal);
		void setMatIndex(int index);
		
		~HitRecord();

		
	private:
		double t = -1;
		glm::vec3 point = glm::vec3(0, 0, 0);
		glm::vec3 normal = glm::vec3(0, 0, 1);
		int materialIndex = 0;
};