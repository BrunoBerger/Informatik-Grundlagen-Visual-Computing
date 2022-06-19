#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <streambuf>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include "util.h"
#include "compute.h"

//_______________________________________________________MAIN________________________________________________________//

int main(void) {	
	// create a window with the specified width, height and title and initialize OpenGL 
	GLFWwindow* window = initialize(640, 480, "OpenGL Starter Project");
	GLuint shaderProgram = createShaderProgram(
		ASSETS_PATH"/shaders/test.vert.glsl", 
		ASSETS_PATH"/shaders/test.frag.glsl");
	GLuint vao = createBuffers();

	GLuint computeShaderProgram = createComputeShaderProgram(ASSETS_PATH"/shaders/compute.glsl");



	int work_grp_cnt[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
	std::cout << "Max work groups per compute shader" <<
		" x:" << work_grp_cnt[0] <<
		" y:" << work_grp_cnt[1] <<
		" z:" << work_grp_cnt[2] << "\n";

	int work_grp_size[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
	std::cout << "Max work group sizes" <<
		" x:" << work_grp_size[0] <<
		" y:" << work_grp_size[1] <<
		" z:" << work_grp_size[2] << "\n";

	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	std::cout << "Max invocations count per work group: " << work_grp_inv << "\n";


	// loop until the user presses ESC or the window is closed programatically
    while (!glfwWindowShouldClose(window)) {
		// clear the back buffer with the specified color and the depth buffer with 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

		glUseProgram(computeShaderProgram);
		glDispatchCompute(ceil(640 / 8), ceil(480 / 4), 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glUseProgram(computeShaderProgram);

		// render to back buffer
		render(shaderProgram, vao);

		// switch front and back buffers
		glfwSwapBuffers(window);
        glfwPollEvents();
    }

	// clean up all created objects
	cleanup(window, shaderProgram, vao);

	// program exits properly
    exit(EXIT_SUCCESS);
}