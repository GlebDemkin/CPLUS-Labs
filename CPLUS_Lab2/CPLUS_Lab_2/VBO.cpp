#include "VBO.h"

VBO::VBO(const void* data, GLsizeiptr size, GLenum usage) {
    // Создаём буфер вершин
    glGenBuffers(1, &ID);

    // Загружаем данные в буфер
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VBO::~VBO() {
    // Удаляем буфер вершин
    if (ID != 0) {
        glDeleteBuffers(1, &ID);
    }
}