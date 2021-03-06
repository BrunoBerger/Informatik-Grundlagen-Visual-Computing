#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <streambuf>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 


//________________________________________________CALLBACK_FUNCTIONS_________________________________________________//

static void errorCallback(int error, const char* description) {
	fputs(description, stderr);
}
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// close window when ESC has been pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		//std::cout << "Mouse click at (" << xpos << " : " << ypos << ")\n";
	}
}
static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos) {}

//_________________________________________________INITIALIZATION____________________________________________________//

GLFWwindow* initialize(int width, int height, std::string title) {
	GLFWwindow* window;
	glfwSetErrorCallback(errorCallback);

	// initialize glfw window
	if (!glfwInit())
		exit(EXIT_FAILURE);

	// we want to use the opengl 3.3 core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// actually create the window
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	// make sure the window creation was successful
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// initialize glad for loading all OpenGL functions
	if (!gladLoadGL()) {
		printf("Something went wrong!\n");
		exit(-1);
	}

	// print some information about the supported OpenGL version
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	fprintf(stdout, "Renderer: %s\n", renderer);
	fprintf(stdout, "OpenGL version supported %s\n", version);

	// register user callbacks
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mousePositionCallback);

	// set the clear color of the window
	glClearColor(0.0f, 0.3f, 0.3f, 0.0f);

	return window;
}

GLuint createBuffers() {
	// specify the layout of the vertex data, being the vertex position followed by the vertex color
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 uv;
	};

	// we specify a triangle with red, green, blue at the tips of the triangle
	Vertex vertexData[] = {
		Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f)},
		Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f)},
		Vertex{glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(1.f, 1.f, 0.f)},

		Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f)},
		Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.f, 1.f, 0.f)},
		Vertex{glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f)}
	};

	// create the vertex array object that holds all vertex buffers
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create a vertex buffer that contains all vertex positions and copy the vertex positions into that buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// we need to tell the buffer in which format the data is and we need to explicitly enable it
	// first we specify the layout of the vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(struct Vertex, pos)));
	glEnableVertexAttribArray(0);
	// then we specify the layout of the vertex color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(struct Vertex, uv)));
	glEnableVertexAttribArray(1);

	return vao;
}

GLuint compileShader(std::string path, GLenum shaderType) {
	// grab the contents of the file and store the source code in a string
	std::ifstream filestream(path);
	std::string shaderSource((std::istreambuf_iterator<char>(filestream)),
		std::istreambuf_iterator<char>());

	// create and compile the shader
	GLuint shaderHandle = glCreateShader(shaderType);
	const char* shaderSourcePtr = shaderSource.c_str();
	glShaderSource(shaderHandle, 1, &shaderSourcePtr, NULL);
	glCompileShader(shaderHandle);
	glCheckError();

	// check if compilation was successful
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
		std::cerr << "Error while compiling shader\n" << infoLog << std::endl;
	}
	glCheckError();

	// return the shader handle
	return shaderHandle;
}
GLuint createShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath) {
	// create and compile shaders
	GLenum vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLenum fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	// create a shader program, attach both shaders and link them together
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for errors while linking the shaders together
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "Error while linking shaders\n" << infoLog << std::endl;
	}

	// after creating the shader program we don't need the two shaders anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// return the shader program handle
	return shaderProgram;
}
GLuint createComputeShaderProgram(std::string computeShaderPath) {
	GLenum computeShader = compileShader(computeShaderPath, GL_COMPUTE_SHADER);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, computeShader);
	glLinkProgram(shaderProgram);
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "Error while linking compute shader\n" << infoLog << std::endl;
	}
	glDeleteShader(computeShader);
	return shaderProgram;
}


//______________________________________________________RENDER_______________________________________________________//

void render(GLuint shaderProgram, GLuint vao) {
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

//______________________________________________________CLEANUP______________________________________________________//

void cleanup(GLFWwindow* window, GLuint& shaderProgram, GLuint& vao) {
	// do some custom cleanup here
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);

	// lastly destroy the window and terminate glfw
	glfwDestroyWindow(window);
	glfwTerminate();
}


