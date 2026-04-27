#pragma once

#include <glad/glad.h>

class EBO {
public:
    EBO(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);
    ~EBO();

    // Возвращает OpenGL id объекта
    GLuint id() const { return ID; }

private:
    GLuint ID = 0;
};