#include "SegmentRenderer.h"

namespace {
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
}

SegmentRenderer::SegmentRenderer()
    // Поля Shader, VBO и EBO создаём сразу с параметрами через список инициализации.
    : shaderProgram_("segment.vert", "segment.frag"),
    vbo_(nullptr, 12 * sizeof(GLfloat), GL_DYNAMIC_DRAW),
    ebo_(indices, sizeof(indices))
{
    // Связываем VAO, VBO и EBO
    glBindVertexArray(vao_.id());
    glBindBuffer(GL_ARRAY_BUFFER, vbo_.id());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_.id());

    // Описываем формат вершины: 3 числа float на одну вершину
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Получаем адреса uniform-переменных
    offsetLocation_ = glGetUniformLocation(shaderProgram_.id(), "offset");
    colorLocation_ = glGetUniformLocation(shaderProgram_.id(), "color");
}

void SegmentRenderer::setSegmentVertices(
    const Segment& segment,
    float currentWidth,
    float expansionDirection
) {
    GeoVector begin = segment.begin();
    GeoVector end = segment.end();
    float baseWidth = segment.width();

    float bottomY;
    float topY;

    if (expansionDirection >= 0.0f) {
        // Расширение вверх
        bottomY = begin.y;
        topY = begin.y + currentWidth;
    }
    else {
        // Расширение вниз, но на уровне основного отрезка
        bottomY = begin.y + baseWidth - currentWidth;
        topY = begin.y + baseWidth;
    }

    vertices_[0] = begin.x;
    vertices_[1] = bottomY;
    vertices_[2] = 0.0f;

    vertices_[3] = end.x;
    vertices_[4] = bottomY;
    vertices_[5] = 0.0f;

    vertices_[6] = end.x;
    vertices_[7] = topY;
    vertices_[8] = 0.0f;

    vertices_[9] = begin.x;
    vertices_[10] = topY;
    vertices_[11] = 0.0f;
}

void SegmentRenderer::drawSegment(
    const Segment& segment,
    const GeoVector& position,
    float currentWidth,
    float expansionDirection,
    const Color& color
) {
    // Обновляем вершины под нужную ширину и направление расширения
    setSegmentVertices(segment, currentWidth, expansionDirection);

    glUseProgram(shaderProgram_.id());

    glBindVertexArray(vao_.id());
    glBindBuffer(GL_ARRAY_BUFFER, vbo_.id());

    // Загружаем новые вершины в VBO
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_), vertices_);

    // Передаём позицию отрезка
    glUniform2f(offsetLocation_, position.x, position.y);

    // Передаём цвет
    glUniform4f(
        colorLocation_,
        color.red,
        color.green,
        color.blue,
        color.alpha
    );

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}