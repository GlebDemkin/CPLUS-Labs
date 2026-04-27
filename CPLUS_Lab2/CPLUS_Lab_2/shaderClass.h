#pragma once

#include <glad/glad.h>
#include <string>


// Читает содержимое файла
std::string get_file_contents(const char* filename);

class Shader {
public:
    Shader(const char* vertexFile, const char* fragmentFile);
    ~Shader();

    GLuint id() const { return ID; }

private:
    GLuint ID = 0;
};