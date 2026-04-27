#pragma once

#include <glad/glad.h>

class VAO {
public:
    VAO();
    ~VAO();

    // Возвращает OpenGL id объекта
    GLuint id() const { return ID; }

private:
    GLuint ID = 0;
};