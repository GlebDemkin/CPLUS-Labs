#include "Application.h"

#include <iostream>
#include <stdexcept>

Application::Application() {
    window_ = nullptr;
    renderer_ = nullptr;
}

Application::~Application() {
    shutdown();
}

bool Application::init() {
    // GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Окно
    window_ = glfwCreateWindow(800, 800, "Lab 2: Moving segment", NULL, NULL);

    if (window_ == NULL) {
        std::cerr << "Failed to create GLFW window\n";
        shutdown();
        return false;
    }

    glfwMakeContextCurrent(window_);

    // GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        shutdown();
        return false;
    }

    glViewport(0, 0, 800, 800);

    // Прозрачность следа
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    try {
        // Renderer создаём после OpenGL-контекста
        renderer_ = new SegmentRenderer();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        shutdown();
        return false;
    }

    timers_.lastTime = (float)glfwGetTime();

    return true;
}

Segment::ControlState Application::readControls() const {
    Segment::ControlState control;

    // Переводим клавиши в состояние управления
    control.left = glfwGetKey(window_, GLFW_KEY_LEFT) == GLFW_PRESS;
    control.right = glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS;
    control.up = glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS;
    control.down = glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS;

    return control;
}

void Application::run() {
    while (!glfwWindowShouldClose(window_)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - timers_.lastTime;
        timers_.lastTime = currentTime;

        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window_, true);
        }

        Segment::ControlState control = readControls();
        Segment::UpdateResult segmentResult = segment_.update(control, deltaTime);

        updateTrail(segmentResult, deltaTime);
        render();

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void Application::updateTrail(const Segment::UpdateResult& segmentResult, float deltaTime) {
    timers_.trailTimer += deltaTime;

    // Добавляем новую часть следа по таймеру
    if (segmentResult.moved && timers_.trailTimer >= trail_.spawnDelay()) {
        trail_.addPart(
            segmentResult.offset,
            segmentResult.expansionDirection,
            segmentResult.expansionScale
        );

        timers_.trailTimer = 0.0f;
    }

    trail_.update(deltaTime);
}

void Application::render() {
    // Очистка кадра
    glClearColor(42.0f / 255.0f, 42.0f / 255.0f, 53.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Сначала след
    for (const TrailPart& part : trail_.parts()) {
        float t = part.age / trail_.lifetime();
        float alpha = 1.0f - t;
        float currentWidth = segment_.width() + t * trail_.expansion() * part.expansionScale;

        Color trailColor = { 1.0f, 0.32f, 0.18f, alpha * 0.45f };

        renderer_->drawSegment(
            segment_,
            part.offset,
            currentWidth,
            part.expansionDirection,
            trailColor
        );
    }

    // Потом основной отрезок
    Color segmentColor = { 1.0f, 0.82f, 0.35f, 1.0f };

    renderer_->drawSegment(
        segment_,
        segment_.offset(),
        segment_.width(),
        1.0f,
        segmentColor
    );
}

void Application::shutdown() {
    // Renderer удаляем до окна
    if (renderer_ != nullptr) {
        delete renderer_;
        renderer_ = nullptr;
    }

    if (window_ != nullptr) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }

    glfwTerminate();
}