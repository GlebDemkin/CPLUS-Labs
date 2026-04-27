#include "VAO.h"

VAO::VAO() {
    // Создаём VAO
    glGenVertexArrays(1, &ID);
}

VAO::~VAO() {
    // Удаляем VAO
    if (ID != 0) {
        glDeleteVertexArrays(1, &ID);
    }
}