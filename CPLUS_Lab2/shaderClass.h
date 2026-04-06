#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>

std::string get_file_contents(const char* filename);

class Shader {
public:
	GLuint ID;

	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();

	void Delete();
};