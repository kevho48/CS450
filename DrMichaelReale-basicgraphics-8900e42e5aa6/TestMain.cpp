#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>
#include <GL/glew.h>					// NOTE: You MUST include GLEW before any other OpenGL-related libraries!
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"					// NOTE: Need to compile, hence the quotes
#include "glm/gtc/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION		// NOTE: This line should only appear in ONE C++ file
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION	// NOTE: This line should only appear in ONE C++ file
#include "stb_image_write.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;

/*///////////////////////////////////////////////////////////////////////////////////////////////////

This code will allow you to set pixel values directly on the window/screen.
Basically, you will write values to a 2D array (mybuffer).

This 2D array will be copied to a texture, which will then be applied to a quad that fills the
window/screen.

///////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////
WARNING: The shaders are for OpenGL 3.3
///////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////
GLOBALS
///////////////////////////////////////////////////////////////////////////////*/

GLfloat *mybuffer;						// Size of this is [textureWidth*textureHeight*3]

double mouseX = 0;						// Mouse variables
double mouseY = 0;
bool leftMouseButtonDown = false;

// Texture and window width/height //

// NOTE: Window and framebuffer width/height will change, BUT texture width/height will not
// ALSO NOTE: The window and framebuffer sizes MAY NOT BE the SAME!!! (Looking at you Apple...)
int windowWidth = 640;
int windowHeight = 480;

int framebufferWidth = windowWidth;
int framebufferHeight = windowHeight;

int textureWidth = windowWidth;
int textureHeight = windowHeight;

// OpenGL rendering //

GLuint renderedTexture;					// Texture ID we will render to
GLuint renderToTextureProgramID;		// Shader program to render texture to quad

GLuint quad_vertexbuffer;				// Vertex and texture coordinate buffers for quad we will draw to screen
GLuint quad_texcoordbuffer;
GLuint quad_VAO;						// Vertex Array Object (VAO) of quad we will draw to screen
										
// GLFW //

GLFWwindow* window;						// Handle for window

/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - Setup and OpenGL-related
///////////////////////////////////////////////////////////////////////////////*/

// Loads shader code; taken from: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// Returns the shader program ID
GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path);
	if (FragmentShaderStream.is_open()){
		string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	cout << "Compiling vertex shader : " << vertex_file_path << endl;
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	//cout << VertexSourcePointer << endl;
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> VertexShaderErrorMessage(max(InfoLogLength, int(1)));
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	cout << &VertexShaderErrorMessage[0] << endl;

	// Compile Fragment Shader
	cout << "Compiling fragment shader : " << fragment_file_path << endl;
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	//cout << FragmentSourcePointer << endl;
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> FragmentShaderErrorMessage(max(InfoLogLength, int(1)));
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	cout << &FragmentShaderErrorMessage[0] << endl;

	// Attach shaders
	cout << "Linking program" << endl;
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	// Actually link the program
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	cout << &ProgramErrorMessage[0] << endl;

	// We don't need the individual shaders anymore, so delete them
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

// Our own function to set up all our OpenGL-specific stuff
void initOpenGL() {
	
	// Set display color
	glClearColor(0.0, 1.0, 1.0, 0.0);

	// TEXTURE SETUP //////////////////////////////////////////////////////////////////////////////

	// For the color buffer, we will create a 2D color texture
	// First, create the texture ID
	glGenTextures(1, &renderedTexture);

	// Bind this texture to be the current 2D texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Allocate an empty 2D texture
	// Texel = texture pixel
	glTexImage2D(GL_TEXTURE_2D,		// Target
		0,							// Mipmap level (don't worry about this)
		GL_RGB,						// Internal format of texels (Red-Green-Blue color model)
		textureWidth,				// Width and height of texture
		textureHeight,
		0,							// Border around texture (setting to 0 because we don't want one)
		GL_RGB,						// Format of texels (must match internal format)
		GL_UNSIGNED_BYTE,			// Data type of each texels
		0);							// If we wanted to copy in data right away, we would do it here (for now, passing in NULL)

	// Determine texture filtering.
	// Basically has to do with what texel we use when our texture coordinates are between texels.
	// In this case, we're using "Nearest Neighbor", which is awful under most circumstances.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Unbind texture (just to be polite)
	glBindTexture(GL_TEXTURE_2D, 0);

	// SHADER SETUP CODE //////////////////////////////////////////////////////////////////////////

	// Load shader program for rendering textured quad to screen
	renderToTextureProgramID = loadShaders("../shaders/RTT.vs", "../shaders/RTT.ps");

	// Get position of uniform variable for texture and set to first active texture unit
	GLuint uniformTextureID = glGetUniformLocation(renderToTextureProgramID, "textureID");
	glUniform1i(uniformTextureID, 0);
	
	// OUR SETUP CODE //////////////////////////////////////////////////////////////////////////

	// Create our own color buffer	
	mybuffer = new GLfloat[3 * textureWidth*textureHeight];

	// TEXTURED QUAD SETUP ////////////////////////////////////////////////////////////////////////

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	static const GLfloat g_quad_texcoords_buffer_data[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	// Create Vertex Array Object (VAO)
	glGenVertexArrays(1, &quad_VAO);
	glBindVertexArray(quad_VAO);

	// Create buffer for vertex coordinates
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Create buffer for texture coordinates
	glGenBuffers(1, &quad_texcoordbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_texcoordbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_texcoords_buffer_data), g_quad_texcoords_buffer_data, GL_STATIC_DRAW);
	
	// Enable attribute array 0 and 2
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	// Bind the vertex buffer as current buffer
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);

	// Define how to read quad_vertexbuffer
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // number of components (3 = 3D point)
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Bind the vertex buffer as current buffer
	glBindBuffer(GL_ARRAY_BUFFER, quad_texcoordbuffer);

	// Define how to read quad_texcoordbuffer
	glVertexAttribPointer(
		2,                  // attribute 2
		2,                  // number of components (2 = U,V coordinates)
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Unbind
	glBindVertexArray(0);
}

// Cleanup OpenGL stuff
void cleanupOpenGL() {

	// Delete texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &renderedTexture);

	// Delete shader program
	glUseProgram(0);
	glDeleteProgram(renderToTextureProgramID);

	// Delete vertex and texture coordinate buffers for quad
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &quad_vertexbuffer);
	glDeleteBuffers(1, &quad_texcoordbuffer);

	// Delete Vertex Array Object (VAO)
	glDeleteVertexArrays(1, &quad_VAO);

	// Delete mybuffer
	delete[] mybuffer;
	mybuffer = NULL;
}

// This function will set a pixel value WITHOUT checking the coordinates
void setPixelinBuffer(int x, int y, GLfloat r, GLfloat g, GLfloat b) {

	int index = (y*textureWidth + x) * 3;

	mybuffer[index] = r;
	mybuffer[index + 1] = g;
	mybuffer[index + 2] = b;
}

// This will fill mybuffer with a single color
void clearMyBuffer(GLfloat r, GLfloat g, GLfloat b) {

	int i, j;
	for (i = 0; i < textureHeight; i++) {
		for (j = 0; j < textureWidth; j++) {
			setPixelinBuffer(j, i, r, g, b);			
		}
	}
}

// This function will perform a HORRIFICALLY inefficient check each time
// to make sure we're not writing to a location outside of our buffer.
void setPixelinBufferCheck(int x, int y, GLfloat r, GLfloat g, GLfloat b) {

	// NOTE: This is a HORRIBLY inefficient way to check for out of bounds cases, but we'll do this for now...

	if (x >= 0 && y >= 0 && x < textureWidth && y < textureHeight) {
		setPixelinBuffer(x, y, r, g, b);
	}
}

// This function will copy the contents of mybuffer into a texture.
void copyInBufferData() {

	// Bind the texture (since we're about to modify it)
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Replace texture with our own buffer
	// NOTE: This does NOT reallocate the texture; it only copies in data
	glTexSubImage2D(GL_TEXTURE_2D,					// Target
		0,								// Mipmap level
		0, 0,							// Starting (x,y) coordinates
		textureWidth, textureHeight,	// Size of area to replace
		GL_RGB,							// Format of texels
		GL_FLOAT,						// Type of pixel data you are passing in
		mybuffer);						// Pointer to pixel data we want to copy to texture

	// Unbind texture (just to be polite)
	glBindTexture(GL_TEXTURE_2D, 0);
}

// This function will render a textured quad to the screen.
void renderWindow() {

	// RENDER TO SCREEN ///////////////////////////////////////////////////////////////////////////

	// Use default framebuffer (window) and set viewport
	// WARNING: The viewport uses ACTUAL PIXELS, so do NOT use window size here!
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float ratio = framebufferWidth / (float)framebufferHeight;
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	// Clear window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use shader program for rendering a textured quad to the screen
	glUseProgram(renderToTextureProgramID);
	
	// Using the first 2D texture (not using multi-texturing, so this is moot, but best to be on the safe side)
	glActiveTexture(GL_TEXTURE0);

	// Use renderedTexture as the 2D texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Bind VAO
	glBindVertexArray(quad_VAO);
		
	// Draw the object!
	// (6 vertices = 2 triangles)
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Unbind VAO
	glBindVertexArray(0);

	// Turn off shader program
	glUseProgram(0);
}

void saveScreenshot(string filename) {

	int channelCnt = 4;
	unsigned char *imageData = new unsigned char[channelCnt * framebufferWidth*framebufferHeight];
	glReadPixels(0, 0, framebufferWidth, framebufferHeight,
		GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	// Flip data
	for (int i = 0; i < framebufferHeight / 2; i++) {
		int firstRow = i*framebufferWidth * channelCnt;
		int otherRow = (framebufferHeight - 1 - i)*framebufferWidth * channelCnt;

		int byteCntInRow = framebufferWidth * channelCnt;

		for (int j = 0; j < byteCntInRow; j++) {
			unsigned char tmp = imageData[firstRow + j];
			imageData[firstRow + j] = imageData[otherRow + j];
			imageData[otherRow + j] = tmp;
		}
	}

	stbi_write_png(	filename.c_str(), framebufferWidth, framebufferHeight, channelCnt, 
					imageData, sizeof(unsigned char) * channelCnt * framebufferWidth);
	delete[] imageData;
}

void drawToBuffer() {

	// I'm just filling in the lower-left hand corner with a red square	
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			setPixelinBuffer(i, j, 1.0, 0.0, 0);
		}
	}

	// ...and the upper-left hand corner with a green square
	for (int i = 0; i < 100; i++) {
		for (int j = (textureHeight - 1); j >= (textureHeight - 100); j--) {
			setPixelinBuffer(i, j, 0, 1.0, 0);
		}
	}

	// ...and the upper-right hand corner with a blue square
	for (int i = (textureWidth - 1); i >= (textureWidth - 100); i--) {
		for (int j = (textureHeight - 1); j >= (textureHeight - 100); j--) {
			setPixelinBuffer(i, j, 0, 0, 1.0);
		}
	}


	// ...and then draw a white square that follows the mouse cursor (if left mouse button is down)
	if (leftMouseButtonDown) {
		// Rescale, because the window may have changed size
		int mX = (mouseX * textureWidth) / windowWidth;
		int mY = (mouseY * textureHeight) / windowHeight;

		for (int i = mX; i < (mX + 30); i++) {
			for (int j = mY; j < (mY + 30); j++) {
				setPixelinBufferCheck(i, j, 1, 1, 1);
			}
		}
	}
}

/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - GLFW Callbacks
///////////////////////////////////////////////////////////////////////////////*/

// Callback for errors
static void error_callback(int error, const char* description) {
	cerr << description << endl;
}

// Callback for key press, release, and repeat
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Callback for mouse MOVEMENT
static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {

	cout << "MOUSE POS: " << xpos << " " << ypos << endl;
	mouseX = xpos;
	// NOTE: Must flip Y mouse coordinate
	mouseY = windowHeight - 1 - ypos;
}

// Callback for mouse BUTTONS
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			cout << "MOUSE LEFT DOWN" << endl;
			leftMouseButtonDown = true;
		}
		else if (action == GLFW_RELEASE) {
			cout << "MOUSE LEFT UP" << endl;
			leftMouseButtonDown = false;
		}
	}
}

// Callback when the window changes size
void window_size_callback(GLFWwindow* window, int width, int height) {
	windowWidth = width;
	windowHeight = height;
}

// Callback when the framebuffer changes size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	framebufferWidth = width;
	framebufferHeight = height;	
}

/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - Main
///////////////////////////////////////////////////////////////////////////////*/

int main(void)
{
	// GLFW setup //

	// Set error callback
	glfwSetErrorCallback(error_callback);

	// (Try to) initialize GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	
	// MAC-SPECIFIC: Convince Mac that, yes, we want to use OpenGL version 3.3 (and NOT 2.1)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create our GLFW window
	window = glfwCreateWindow(windowWidth, windowHeight, "your waifu is trash", NULL, NULL);

	// Were we able to make it?
	if (!window) {
		// Kill GLFW and exit program
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Get current framebuffer size, since that could be different from the requested window size
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	
	// We want to draw to this window, so make the OpenGL context associated with this window our current context.
	glfwMakeContextCurrent(window);

	// Basically, turning VSync on (so we will wait until the screen is updated once before swapping the back and front buffers
	glfwSwapInterval(1);

	// Set our keyboard callback function, so that we can process keyboard input!
	glfwSetKeyCallback(window, key_callback);

	// Set our mouse callback function for when the mouse MOVES
	glfwSetCursorPosCallback(window, mouse_position_callback);

	// Set our mouse BUTTON callback function
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Set our callback for when the window changes size
	glfwSetWindowSizeCallback(window, window_size_callback);

	// Set our callback for when the framebuffer changes size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLEW setup //

	// MAC-SPECIFIC: Some issues occur with using OpenGL core and GLEW; so, we'll use the experimental version of GLEW
	glewExperimental = true;
	
	// (Try to) initalize GLEW
	GLenum err = glewInit();
	
	// Call glGetError() to clear it out (can occur after glewInit() for some reason)
	glGetError();
	
	if (GLEW_OK != err) {
		// We couldn't start GLEW, so we've got to go.
		// Kill GLFW and get out of here
		cout << "ERROR: GLEW could not start: " << glewGetErrorString(err) << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	cout << "GLEW initialized; version " << glewGetString(GLEW_VERSION) << endl;

	GLint glMajor, glMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
	glGetIntegerv(GL_MINOR_VERSION, &glMinor);
	cout << "OpenGL context version: " << glMajor << "." << glMinor << endl;
	printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Initialize OpenGL-related stuff
	initOpenGL();

	// NOTE: This code does nothing except create a 4x4 identity matrix and print the first row-column value (which is one).
	// This code is here to make sure GLM is working properly.
	glm::mat4 Model = glm::mat4(1.0f);
	cout << Model[0][0] << endl;

	cout << "cos of pi: " << glm::cos(glm::pi<double>()) << endl;

	// Keep going until the window should close (explicit request in program, user clicks "X", etc.)
	while (!glfwWindowShouldClose(window)) {

		// Clear mybuffer with a black background
		clearMyBuffer(0.2,0.5,0.3);

		// Draw to mybuffer
		drawToBuffer();

		// Copy whatever we've drawn on mybuffer into a texture 
		copyInBufferData();

		// Render textured quad to window
		renderWindow();

		// Swap the back and front buffers!
		glfwSwapBuffers(window);

		// See if any events need to be handled (keyboard, screen resize, mouse, etc.)
		glfwPollEvents();
	}

	// Take a screenshot before we go on to glory...	
	saveScreenshot("screenshot.png");
		
	// Cleanup OpenGL-related stuff
	cleanupOpenGL();

	// Destroy window and stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	// Exit happily!
	exit(EXIT_SUCCESS);
}
