#pragma once

#include <glad/glad.h>

#include "Segment.h"
#include "GeoVector.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

struct Color {
    float red;
    float green;
    float blue;
    float alpha;
};

class SegmentRenderer {
public:
    SegmentRenderer();

    void drawSegment(
        const Segment& segment,
        const GeoVector& position,
        float currentWidth,
        float expansionDirection,
        const Color& color
    );

private:
    Shader shaderProgram_;
    VAO vao_;
    VBO vbo_;
    EBO ebo_;

    GLfloat vertices_[12];

    GLint offsetLocation_;
    GLint colorLocation_;

    void setSegmentVertices(
        const Segment& segment,
        float currentWidth,
        float expansionDirection
    );
};