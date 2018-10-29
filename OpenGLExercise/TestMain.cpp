#include <iostream>
#include <cstring>
#include <thread>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

GLFWwindow *window = NULL;
int windowWidth = 500;
int windowHeight = 500;
int framebufferWidth = windowWidth;
int framebufferHeight = windowHeight;

GLint modelMatLoc = -1;
glm::mat4 modelMat;
GLint viewMatLoc = -1;
glm::mat4 viewMat;
GLint projMatLoc = -1;
glm::mat4 projMat;

string currentTransformString = "v";

GLuint programID = 0;
vector<GLfloat> vertex_positions_host = {
	-0.3f, -0.3f, 0.0f,
	0.3f, -0.3f, 0.0f,
	-0.3f, 0.3f, 0.0f,
	-0.3f, 0.3f, 0.0f,
	0.3f, -0.3f, 0.0f,
	0.3f, 0.3f, 0.0f
};

vector<GLfloat> vertex_only = {
	-0.3f, -0.3f, 0.0f,
	0.3f, -0.3f, 0.0f,
	-0.3f, 0.3f, 0.0f,
	0.3f, 0.3f, 0.0f
};

vector<GLuint> indices = { 0, 1, 2, 1, 3, 2 };

class Mesh {
private:
	vector<GLfloat> pos;
	vector<GLuint> elements;
	GLuint vertexPositionBuffer = 0;
	GLuint elementBuffer = 0;
	GLuint VAO = 0;

public:
	Mesh(	vector<GLfloat> &positions,
			vector<GLuint> &indices) {

		pos.insert(	pos.end(), 
					positions.begin(),
					positions.end());

		elements.insert(elements.end(),
						indices.begin(),
						indices.end());

		glGenBuffers(1, &vertexPositionBuffer);
		glBindBuffer(	GL_ARRAY_BUFFER,
						vertexPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER,
					sizeof(GLfloat)*pos.size(),
					pos.data(),
					GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,
					vertexPositionBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT,
								GL_FALSE,
								3*sizeof(float),
								0);

		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
						elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
						elements.size()*sizeof(GLuint),
						elements.data(),
						GL_STATIC_DRAW);

		glBindVertexArray(0);
	};

	~Mesh() {
		pos.clear();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vertexPositionBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &elementBuffer);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO);
	};

	void draw() {
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, pos.size()/3);
		glDrawElements(	GL_TRIANGLES, elements.size(),
						GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	};
};

GLuint loadShaders(	string vertexFilename,
					string fragmentFilename) {

	ifstream vertexFile(vertexFilename);
	if(vertexFile.fail()) { return 0; }
	string VertexShaderCode;
	while(!vertexFile.eof()) {
		string line;
		getline(vertexFile, line);
		VertexShaderCode += line + "\n";
	}
	vertexFile.close();

	ifstream fragmentFile(fragmentFilename);
	if(fragmentFile.fail()) { return 0; }
	string FragmentShaderCode;
	while(!fragmentFile.eof()) {
		string line;
		getline(fragmentFile, line);
		FragmentShaderCode += line + "\n";
	}
	fragmentFile.close();

	cout << "VERTEX SHADER:" << endl;
	cout << VertexShaderCode << endl;
	cout << "FRAGMENT SHADER:" << endl;
	cout << FragmentShaderCode << endl;

	GLuint VertexShaderID = glCreateShader(
								GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(
								GL_FRAGMENT_SHADER);

	char const *VertexSourcePointer = VertexShaderCode.c_str();
	char const *FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, 
					&VertexSourcePointer, NULL);
	glShaderSource(FragmentShaderID, 1,
					&FragmentSourcePointer, NULL);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	GLuint ProgramID = glCreateProgram();

	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	glLinkProgram(ProgramID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void printRM(string name, glm::mat4 &M) {
	cout << name << ":" << endl;
	for(int row = 0; row < 4; row++) {
		for(int col = 0; col < 4; col++) {
			cout << M[col][row] << ", ";
		}
		cout << endl;
	}
}

static void error_callback(int error, const char* desc) {
	cerr << "ERROR " << error << ": ";
	cerr << desc << endl;
}

static void key_callback(GLFWwindow *window,
							int key, int scancode,
							int action, int mods) {

	if(action == GLFW_PRESS) {
		if(key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	if(action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch(key) {
		case GLFW_KEY_Q:
			modelMat = glm::rotate(glm::radians(5.0f),
									glm::vec3(0,0,1))*modelMat;
			currentTransformString = "R(5)*" 
									+ currentTransformString;
			break;
		case GLFW_KEY_E:
			modelMat = glm::rotate(glm::radians(-5.0f),
									glm::vec3(0,0,1))*modelMat;
			currentTransformString = "R(-5)*"
									+ currentTransformString;
			break;
		case GLFW_KEY_SPACE:
			modelMat = glm::mat4(1.0);
			currentTransformString = "v";
			break;
		case GLFW_KEY_F:
			modelMat = glm::scale(glm::vec3(0.8,1,1))*modelMat;
			currentTransformString = "S(0.8,1.0)*"
									+ currentTransformString;
			break;
		case GLFW_KEY_G:
			modelMat = glm::scale(glm::vec3(1.25,1,1))*modelMat;
			currentTransformString = "S(1.25,1.0)*"
									+ currentTransformString;
			break;
		case GLFW_KEY_W:
			modelMat = glm::translate(glm::vec3(0,0.1,0))*modelMat;
			currentTransformString = "T(0,0.1)*"
									+ currentTransformString;
			break;
		case GLFW_KEY_S:
			modelMat = glm::translate(glm::vec3(0,-0.1,0))*modelMat;
			currentTransformString = "T(0,-0.1)*"
									+ currentTransformString;
			break;
		default:
			break;
		}

		printRM("Model", modelMat);
		cout << currentTransformString << endl;
	}
}

int main(int argc, char **argv) {
	cout << "BEGIN OPENGL EXERCISE" << endl;

	glfwSetErrorCallback(error_callback);

	if(!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
					GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(windowWidth, windowHeight,
								"Title of Window",
								NULL, NULL);

	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	glewExperimental = true;
	GLenum err = glewInit();
	glGetError();
	if(GLEW_OK != err) {
		cout << "ERROR: GLEW could not start: ";
		cout << glewGetErrorString(err) << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	cout << "GLEW version: ";
	cout << glewGetString(GLEW_VERSION) << endl;

	GLint glMajor, glMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
	glGetIntegerv(GL_MINOR_VERSION, &glMinor);
	cout << "OpenGL context version: ";
	cout << glMajor << "." << glMinor << endl;
	cout << "Supported GLSL version: ";
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << endl;

	//Mesh *mesh = new Mesh(vertex_positions_host);
	Mesh *mesh = new Mesh(vertex_only, indices);

	programID = loadShaders("../Basic.vs",
							"../Basic.ps");
	if(!programID) {
		cout << "ERROR: Cannot load shaders!" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	modelMatLoc = glGetUniformLocation(programID,
										"modelMat");
	viewMatLoc = glGetUniformLocation(programID,
										"viewMat");

	projMatLoc = glGetUniformLocation(programID,
										"projMat");


	glClearColor(0.0f, 0.0f, 0.7f, 1.0f);

	string filename = "../test.jpg";
	int width, height, nrComponents;
	unsigned int textureID = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data != NULL)
	{
		glGenTextures(1, &textureID);
		GLenum format;
		if (nrComponents == 3)
			format = GL_RGB;
		else
			if (nrComponents == 4)
				format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		cout << "ERROR: Cannot load Texture!" << endl;
		stbi_image_free(data);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLint uniformTextureID = glGetUniformLocation(programID, "diffuseTexture");

	while(!glfwWindowShouldClose(window)) {
		//cout << "Rendering..." << endl;
		glfwGetFramebufferSize(window, 
								&framebufferWidth,
								&framebufferHeight);
		glViewport(0,0,framebufferWidth, framebufferHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(programID);

		glUniform1i(uniformTextureID, 0);

		glUniformMatrix4fv(	modelMatLoc, 1, GL_FALSE,
							glm::value_ptr(modelMat));

		viewMat = glm::lookAt(glm::vec3(1.0,0,1.0),
								glm::vec3(0,0,0),
								glm::vec3(0,1,0));
		glUniformMatrix4fv(viewMatLoc,1, GL_FALSE,
							glm::value_ptr(viewMat));

		float fov = glm::radians(90.0f);
		float aspectRatio = ((float)framebufferWidth)/((float)framebufferHeight);
		float near = 0.01;
		float far = 1000.0;
		projMat = glm::perspective(fov, aspectRatio,
									near, far);
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,
							glm::value_ptr(projMat));

		// DRAW GLORIOUS THINGS HERE
		mesh->draw();


		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(15));

	}

	delete mesh;

	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &(textureID));

	glUseProgram(0);
	glDeleteProgram(programID);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
