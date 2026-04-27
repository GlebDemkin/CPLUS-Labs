#pragma once

#include <glad/glad.h>

class VBO {
public:
    VBO(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);
    ~VBO();

    // Возвращает OpenGL id объекта
    GLuint id() const { return ID; }

private:
    GLuint ID = 0;
};