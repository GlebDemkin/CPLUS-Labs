#include "EBO.h"

EBO::EBO(const void* data, GLsizeiptr size, GLenum usage) {
    // Создаём индексный буфер
    glGenBuffers(1, &ID);

    // Загружаем индексы в буфер
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

EBO::~EBO() {
    // Удаляем индексный буфер
    if (ID != 0) {
        glDeleteBuffers(1, &ID);
    }
}