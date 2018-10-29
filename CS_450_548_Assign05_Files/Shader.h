#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace std;

class Shader {
protected:
	GLuint programID = 0;

private:
	
	bool loadAllFileData(string filename, string &fileData) {
		ifstream infile(filename);

		if (infile.fail()) {
			return false;
		}

		fileData = "";
		string line = "";
		while (getline(infile, line))
			fileData += "\n" + line;
		infile.close();

		return true;
	};

	bool checkShader(GLuint shaderID) {
		GLint result = GL_FALSE;
		int infoLogLength;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		vector<char> shaderErrorMessage(max(infoLogLength, int(1)));
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
		cout << &shaderErrorMessage[0] << endl;
		return result;
	};

	bool checkProgram(GLuint localProgramID) {
		GLint result = GL_FALSE;
		int infoLogLength;
		glGetProgramiv(localProgramID, GL_LINK_STATUS, &result);
		glGetProgramiv(localProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
		vector<char> ProgramErrorMessage(max(infoLogLength, int(1)));
		glGetProgramInfoLog(localProgramID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		cout << &ProgramErrorMessage[0] << endl;

		return result;
	};

	bool compileShader(GLuint shaderID, string shaderData) {
		char const * srcPtr = shaderData.c_str();
		cout << shaderData << endl;
		glShaderSource(shaderID, 1, &srcPtr, NULL);
		glCompileShader(shaderID);

		// Check shader
		return checkShader(shaderID);
	};

	GLuint readShaders(string vertexFilename, string fragmentFilename) {

		// Read the vertex shader code from the file
		string vertexShaderCode;
		if (!loadAllFileData(vertexFilename, vertexShaderCode)) {
			cout << "ERROR: Vertex shader file could not be loaded! " << vertexFilename << endl;
			return 0;
		}

		// Read the fragment shader code from the file
		string fragmentShaderCode;
		if (!loadAllFileData(fragmentFilename, fragmentShaderCode)) {
			cout << "ERROR: Fragment shader file could not be loaded! " << vertexFilename << endl;
			return 0;
		}

		// Create the shaders
		GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Compile vertex shader
		cout << "Compiling vertex shader : " << vertexFilename << endl;
		if (!compileShader(vertShaderID, vertexShaderCode)) {
			cout << "ERROR: VERTEX SHADER COMPILATION FAILED!" << endl;
			return 0;
		}

		// Compile Fragment Shader
		cout << "Compiling fragment shader : " << fragmentFilename << endl;
		if (!compileShader(fragShaderID, fragmentShaderCode)) {
			cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED!" << endl;
			return 0;
		}

		// Attach shaders
		cout << "Linking program" << endl;
		GLuint localProgramID = glCreateProgram();
		glAttachShader(localProgramID, vertShaderID);
		glAttachShader(localProgramID, fragShaderID);

		// Actually link the program
		glLinkProgram(localProgramID);
		
		// Check the program
		if (!checkProgram(localProgramID)) {
			cout << "ERROR: PROGRAM LINKING FAILED!" << endl;
			return 0;
		}

		// We don't need the individual shaders anymore, so delete them
		glDeleteShader(vertShaderID);
		glDeleteShader(fragShaderID);

		return localProgramID;
	};

public:
	Shader(string vertexFilename, string fragmentFilename) {
		programID = readShaders(vertexFilename, fragmentFilename);
		if (!programID) {
			exit(EXIT_FAILURE);
		}
	};

	virtual ~Shader() {
		glUseProgram(0);			// Good idea to make sure not active
		glDeleteProgram(programID);		
	};

	virtual void activate() {
		glUseProgram(programID);
	};

	virtual void deactivate() {
		glUseProgram(0);
	};
};

#endif
