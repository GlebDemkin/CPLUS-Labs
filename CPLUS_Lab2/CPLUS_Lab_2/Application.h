#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Segment.h"
#include "Trail.h"
#include "SegmentRenderer.h"

class Application {
public:
    Application();
    ~Application();

    bool init();
    void run();

private:
    // Таймеры приложения
    struct Timers {
        float trailTimer = 0.0f; // время до новой части следа
        float lastTime = 0.0f;   // время прошлого кадра
    };

    GLFWwindow* window_;

    Segment segment_;
    Trail trail_;
    SegmentRenderer* renderer_;

    Timers timers_;

    Segment::ControlState readControls() const;
    void updateTrail(const Segment::UpdateResult& segmentResult, float deltaTime);
    void render();
    void shutdown();
};