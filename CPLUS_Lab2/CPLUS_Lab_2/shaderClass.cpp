#include "shaderClass.h"

#include <fstream>
#include <stdexcept>
#include <vector>

std::string get_file_contents(const char* filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Failed to open shader file");
    }

    std::string contents;
    file.seekg(0, std::ios::end);
    contents.resize((size_t)file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&contents[0], contents.size());

    return contents;
}

static GLuint compileShader(GLenum type, const char* source) {
    // Создаём шейдер
    GLuint shader = glCreateShader(type);

    // Передаём исходный код шейдера
    glShaderSource(shader, 1, &source, NULL);

    // Компилируем шейдер
    glCompileShader(shader);

    // Проверяем успешность компиляции
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<char> log(logLength);
        glGetShaderInfoLog(shader, logLength, NULL, log.data());

        glDeleteShader(shader);
        throw std::runtime_error(log.data());
    }

    return shader;
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    // Читаем код шейдеров из файлов
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    // Компилируем вершинный и фрагментный шейдеры
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    // Создаём shader program
    ID = glCreateProgram();

    // Прикрепляем шейдеры к программе
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    // Линкуем программу
    glLinkProgram(ID);

    // Проверяем успешность линковки
    GLint success = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    // После линковки отдельные шейдеры больше не нужны
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success) {
        GLint logLength = 0;
        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<char> log(logLength);
        glGetProgramInfoLog(ID, logLength, NULL, log.data());

        glDeleteProgram(ID);
        ID = 0;

        throw std::runtime_error(log.data());
    }
}

Shader::~Shader() {
    // Удаляем shader program
    if (ID != 0) {
        glDeleteProgram(ID);
    }
}