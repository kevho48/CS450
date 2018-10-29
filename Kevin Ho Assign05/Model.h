#ifndef MODEL_H
#define MODEL_H

/*
//
// NOTE: The code that follows is heavily inspired by code from https://learnopengl.com/.
// 
*/

#include <iostream>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Shader.h"
using namespace std;

class Model {
private:
	vector<Mesh*> meshes;	
	string directory = "";
	
	void cleanup() {
		for (int i = 0; i < meshes.size(); i++) {
			delete meshes[i];
		}
		meshes.clear();
	
		directory = "";
	};

	void processNode(aiNode *node, const aiScene *scene) {
		// Go through all meshes...
		for (int i = 0; i < node->mNumMeshes; i++) {
			// Grab the aiMesh
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			// Process the aiMesh and add it to our list of meshes
			meshes.push_back(processMesh(mesh, scene));
		}

		// Recursively do this to children...
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	};

	Mesh* processMesh(aiMesh *mesh, const aiScene *scene) {
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		
		// Load vertices
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			glm::vec3 position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;
			vertex.pos = position;

			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertex.normal = normal;

			if (mesh->mTextureCoords[0]) {
				glm::vec2 uv;
				uv.x = mesh->mTextureCoords[0][i].x;
				uv.y = mesh->mTextureCoords[0][i].y;
				vertex.texcoords = uv;
			}
			else {
				vertex.texcoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// Load indices
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		return new Mesh(vertices, indices);
	};
	
public:
	Model() {};
	~Model() {
		cleanup();
	};

	bool load(string path) {
		// Cleanup first
		cleanup();

		// Load model data
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "Error: " << importer.GetErrorString() << endl;
			return false;
		}

		// Get directory
		directory = path.substr(0, path.find_last_of('/'));

		// Process each node
		processNode(scene->mRootNode, scene);

		return true;
	};

	void draw() {		
		for (int i = 0; i < meshes.size(); i++) {
			meshes.at(i)->draw();			
		}
	};
};

#endif
