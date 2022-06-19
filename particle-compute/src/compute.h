#pragma once

#include "util.h"

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