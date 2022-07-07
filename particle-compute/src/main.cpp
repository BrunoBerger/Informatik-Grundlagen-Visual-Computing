#include "util.h"
#include <random>
#include <time.h>

const int IMG_WIDTH = 640;
const int IMG_HEIGHT = 480;
const int N_PARTICLES = 2000;

double attractorXPos = IMG_WIDTH / 2;
double attractorYPos = IMG_HEIGHT / 2;

int main(void) {
	// create a window with the specified width, height and title and initialize OpenGL 
	GLFWwindow* window = initialize(IMG_WIDTH, IMG_HEIGHT, "Particle Simulation");
	glCheckError();
	GLuint shaderProgram = createShaderProgram(
		ASSETS_PATH"/shaders/test.vert.glsl", 
		ASSETS_PATH"/shaders/frag.glsl");
	glCheckError();
	GLuint vao = createBuffers();
	GLuint computeShaderParticleSimulationProgram = createComputeShaderProgram(ASSETS_PATH"/shaders/simulateParticles.glsl");
	GLuint computeShaderImageWriteProgram = createComputeShaderProgram(ASSETS_PATH"/shaders/writeParticleToImage.glsl");

	/////////
	//Background texture
	unsigned int texture2;
	glActiveTexture(GL_TEXTURE0 + 1);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glCheckError();
	float* tex_data;
	int width = IMG_WIDTH;
	int height = IMG_HEIGHT;
	//width = height = 100; // Accivate to "zoom in"
	tex_data = new float[width * height * sizeof(unsigned char) * 4];
	for (int i = 0; i < (int)(width * height * sizeof(unsigned char) * 4); i++)
		tex_data[i] = 0.2f; // Base background color

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, tex_data);
	glBindImageTexture(1, texture2, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
	glCheckError();

	
	glUseProgram(computeShaderImageWriteProgram);
	GLint imageLocation = glGetUniformLocation(computeShaderImageWriteProgram, "img_output");
	glUniform1i(imageLocation, 1);
	glCheckError();
	glActiveTexture(GL_TEXTURE0 + 1);
	glUseProgram(shaderProgram);


	// Creating random particles
	struct Particle
	{
		GLfloat position[2];
		GLfloat velocity[2];
		GLdouble dist = 0;
	};
	Particle particles[N_PARTICLES];
	srand(time(NULL)); // Seed the time
	for (int i = 0; i < N_PARTICLES; i++) {
		particles[i] = Particle();
		// Option1: Place randomly in frame
		particles[i].position[0] = rand() % IMG_WIDTH;
		particles[i].position[1] = rand() % IMG_HEIGHT;
		// Option2: Place in center
		//particles[i].position[0] = IMG_WIDTH/2.0;
		//particles[i].position[1] = IMG_HEIGHT/2.0;

		particles[i].velocity[0] = (((float)rand() / (float)RAND_MAX)-0.5)*3;
		particles[i].velocity[1] = (((float)rand() / (float)RAND_MAX)-0.5)*3;
		//std::cout << "particel vel (" << particles[i].velocity[0] << " : " << particles[i].velocity[1] << ")\n";
	}

	///////// SSBO init
	glUseProgram(computeShaderImageWriteProgram);
	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(particles), &particles, GL_DYNAMIC_DRAW);
	//the khronos docs put this line in
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glCheckError();


	std::cout << "^^These 3 Errors are OK\n\n=========================\n"
		<< "Simulating " << N_PARTICLES << " particles, in a " 
		<< IMG_WIDTH << "x" << IMG_HEIGHT << "window. "
		<< "You can change these constants at the top of the main.cpp file\n"
		<< "You can hold SPACE to bind the attraction point to your cursor!" << std::endl;
	//***** Render loop ********//
	// loop until the user presses ESC or the window is closed programatically
    while (!glfwWindowShouldClose(window)) {
		// clear the back buffer with the specified color and the depth buffer with 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Fill background again, uncomment for "trail effect"
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, tex_data);
        

		//***********************************//
		//***** 1. Simulate particles ********//
		glUseProgram(computeShaderParticleSimulationProgram);

		// If space ispressed place atttractor at cursor, otherwise at point defined at top
		double xpos, ypos;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			glfwGetCursorPos(window, &xpos, &ypos);
		}
		else {
			xpos = attractorXPos;
			ypos = attractorYPos;
		}
		glUniform2f(glGetUniformLocation(computeShaderParticleSimulationProgram, "cursorPos"), xpos, IMG_HEIGHT-ypos);

		int ssbo_binding_2 = 1;
		int block_index_2 = glGetProgramResourceIndex(computeShaderParticleSimulationProgram, GL_SHADER_STORAGE_BLOCK, "particle_buf");
		glShaderStorageBlockBinding(computeShaderParticleSimulationProgram, block_index_2, ssbo_binding_2);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_2, ssbo);


		glDispatchCompute(N_PARTICLES, 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		//********************************//
		//***** 2. Write to image ********//
		glUseProgram(computeShaderImageWriteProgram);


		// Update ssbo data
		int ssbo_binding = 1;
		int block_index = glGetProgramResourceIndex(computeShaderImageWriteProgram, GL_SHADER_STORAGE_BLOCK, "particle_buf");
		glShaderStorageBlockBinding(computeShaderImageWriteProgram, block_index, ssbo_binding);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, ssbo);

		glActiveTexture(GL_TEXTURE0 + 1);
		imageLocation = glGetUniformLocation(computeShaderImageWriteProgram, "img_output");
		glUniform1i(imageLocation, 1);
		
		glDispatchCompute(N_PARTICLES, 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		//********************************//
		//***** 3. Render to image ********//
		glUseProgram(shaderProgram);
		glActiveTexture(GL_TEXTURE0 + 1);
		imageLocation = glGetUniformLocation(shaderProgram, "img_input");
		//std::cout << "Location: " << imageLocation;
		glUniform1i(imageLocation, 1);

		// render to back buffer and switch with front buffer
		render(shaderProgram, vao);
		glfwSwapBuffers(window);
        glfwPollEvents();
		glCheckError();
    }

	// clean up all created objects
	cleanup(window, shaderProgram, vao);
    exit(EXIT_SUCCESS);
}