#ifndef MESH_H
#define MESH_H

/*
//
// NOTE: The code that follows is heavily inspired by code from https://learnopengl.com/.
// 
*/

#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
using namespace std;

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

class Mesh {
private:
	vector<Vertex> vertices;
	vector<GLuint> elements;
	GLuint VBO = 0;
	GLuint EBO = 0;
	GLuint VAO = 0;
	
public:
	Mesh(vector<Vertex> verts,
		vector<GLuint> indices) {

		vertices.insert(vertices.end(), verts.begin(), verts.end());
		elements.insert(elements.end(), indices.begin(), indices.end());
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// 0 = pos (3 elements)
		// 1 = normal (3 elements)
		// 2 = texcoords (2 elements)
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			elements.size() * sizeof(GLuint),
			elements.data(),
			GL_STATIC_DRAW);

		glBindVertexArray(0);
	};

	~Mesh() {
		vertices.clear();
		elements.clear();
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &EBO);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO);
	};

	void draw() {		
		glBindVertexArray(VAO);		
		glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	};
};

#endif
