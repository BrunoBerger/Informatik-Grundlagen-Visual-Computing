#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <streambuf>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include "util.h"
#include "compute.h"
#include "particles.h"

//_______________________________________________________MAIN________________________________________________________//

int main(void) {
	// create a window with the specified width, height and title and initialize OpenGL 
	GLFWwindow* window = initialize(640, 480, "OpenGL Starter Project");
	glCheckError();
	GLuint shaderProgram = createShaderProgram(
		ASSETS_PATH"/shaders/test.vert.glsl", 
		ASSETS_PATH"/shaders/frag.glsl");
	glCheckError();
	GLuint vao = createBuffers();
	GLuint computeShaderParticleSimulationProgram = createComputeShaderProgram(ASSETS_PATH"/shaders/simulateParticles.glsl");
	GLuint computeShaderImageWriteProgram = createComputeShaderProgram(ASSETS_PATH"/shaders/writeParticleToImage.glsl");


/*	glCheckError();
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0 + 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 640, 480, 0, GL_RGBA, GL_FLOAT, NULL);
	glCheckError();
	
	glUseProgram(computeShaderProgram);
	GLint imageLocation = glGetUniformLocation(computeShaderProgram, "img_output");
	glUniform1i(imageLocation, 0);
	glCheckError();
	*/


	/////////
	//Working texture
	unsigned int texture2;
	glActiveTexture(GL_TEXTURE0 + 1);
	// Generate white OpenGL texture.
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glCheckError();
	float* tex_data;
	// Allocate the needed space.
	int width;
	int height;
	width = height = 128;
	tex_data = new float[width * height * sizeof(unsigned char) * 4];
	for (int i = 0; i < (int)(width * height * sizeof(unsigned char) * 4); i++) {
		tex_data[i] = 0.4f;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, tex_data);
	glBindImageTexture(1, texture2, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
	glCheckError();
	//////

	glUseProgram(computeShaderImageWriteProgram);
	GLint imageLocation = glGetUniformLocation(computeShaderImageWriteProgram, "img_output");
	glUniform1i(imageLocation, 1);
	glCheckError();
	glActiveTexture(GL_TEXTURE0 + 1);
	glUseProgram(shaderProgram);
	
	//////// Atomic increment
	// declare and generate a buffer object name
	GLuint atomicsBuffer;
	glGenBuffers(1, &atomicsBuffer);
	// bind the buffer and define its initial storage capacity
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * 1, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, atomicsBuffer);
	// unbind the buffer (was commented out once...)
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	glCheckError();

	///////// SSBO init
	struct Particle
	{
		float position[2];
		float velocity[2];
	};
	Particle particles[100];
	for (int i = 0; i < 100; i++) {
		particles[i] = Particle();
		particles[i].position[0] = 0.5;
		particles[i].position[1] = 0.5;
		particles[i].velocity[0] = 0;
		particles[i].velocity[1] = 0;
	}

	std::cout << particles[59].position[1] << "particel pos \n";
	glUseProgram(computeShaderImageWriteProgram);
	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(particles), &particles, GL_DYNAMIC_DRAW);
	//the khronos docs put this line in
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glCheckError();


	//***** Render loop ********//
	// loop until the user presses ESC or the window is closed programatically
    while (!glfwWindowShouldClose(window)) {
		// clear the back buffer with the specified color and the depth buffer with 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

		//***********************************//
		//***** 1. Simulate particles ********//





		glCheckError();
		//********************************//
		//***** 2. Write to image ********//
		glUseProgram(computeShaderImageWriteProgram);
		glUniform1f(glGetUniformLocation(computeShaderImageWriteProgram, "testVar"), 0.5);

		int ssbo_binding = 1;
		int block_index = glGetProgramResourceIndex(computeShaderImageWriteProgram, GL_SHADER_STORAGE_BLOCK, "particle_buf");
		glCheckError();
		glShaderStorageBlockBinding(computeShaderImageWriteProgram, block_index, ssbo_binding);
		glCheckError();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, ssbo);
		glCheckError();


		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer);
		GLuint a = 0;
		glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &a);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

		//imageLocation = glGetUniformLocation(computeShaderProgram, "atominc");
		//glUniform1i(imageLocation, atomicsBuffer);
		
		glActiveTexture(GL_TEXTURE0 + 1);
		imageLocation = glGetUniformLocation(computeShaderImageWriteProgram, "img_output");
		//std::cout << "Location: " << imageLocation;
		glUniform1i(imageLocation, 1);
		
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, atomicsBuffer);
		
		//**********************//
		glDispatchCompute(ceil(640), ceil(480), 1);
		//**********************//

		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glCheckError();
		

		//******
		GLuint userCounters;
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer);
		glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &userCounters);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
		//std::cout << userCounters << "\n";



		//********************************//
		//***** 3. Render to image ********//
		glUseProgram(shaderProgram);
		glActiveTexture(GL_TEXTURE0 + 1);
		imageLocation = glGetUniformLocation(shaderProgram, "img_input");
		//std::cout << "Location: " << imageLocation;
		glUniform1i(imageLocation, 1);

		// render to back buffer
		render(shaderProgram, vao);
		glCheckError();

		// switch front and back buffers
		glfwSwapBuffers(window);
        glfwPollEvents();
    }

	// clean up all created objects
	cleanup(window, shaderProgram, vao);

	// program exits properly
    exit(EXIT_SUCCESS);
}