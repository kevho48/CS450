#include <iostream>
#include <cstring>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

static void error_callback(int error, const char* description)
{
	cerr << "ERROR " << error << ": " << description << endl;
}

int main(int argc, char **argv)
{
	GLFWwindow* window = NULL;
	int windowWidth = 640;
	int windowHeight = 480;
	int framebufferWidth = windowWidth;
	int framebufferHeight = windowHeight;
	cout << "BEGIN OPENGL EXERCISE" << endl;
	
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(windowWidth, windowHeight, "SEIZURE", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	float x = 0;
	float y = 1;
	while (!glfwWindowShouldClose(window))
	{
		cout << "Rendering..." << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
		glClearColor(y, 0.0f, x, 1.0f);

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		glViewport(0, 0, framebufferWidth, framebufferHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(0));
	// only uncomment if you want seizure
		if (x >= 1)
			x = 0;
		else
			x++;
		if (y >= 1)
			y = 0;
		else
			y++;
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
