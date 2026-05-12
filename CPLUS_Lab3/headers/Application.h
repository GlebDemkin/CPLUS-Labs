#pragma once

#include <memory> // std::unique_ptr
#include <string> // std::string

#include "MenuSystem.h"

struct GLFWwindow;
struct NVGcontext;

// Класс Application управляет окном, OpenGL, NanoVG и системой меню.
class Application {
private:
    // Удаляет GLFW-окно при очистке unique_ptr.
    struct GLFWwindowDeleter {
        void operator()(GLFWwindow* window) const;
    };

    // Удаляет NanoVG-контекст при очистке unique_ptr.
    struct NanoVGContextDeleter {
        void operator()(NVGcontext* context) const;
    };

private:
    std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window;
    std::unique_ptr<NVGcontext, NanoVGContextDeleter> nanoVGContext;

    std::unique_ptr<MenuSystem> menuSystem;

    bool glfwInitialized = false;

    int fontId = -1;

    int windowWidth = 1280;
    int windowHeight = 720;

public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // Запускает приложение и возвращает код завершения.
    int run();

private:
    // Инициализация частей приложения.
    void initialize();
    void initializeGLFW();
    void createWindow();
    void initializeGLAD();
    void initializeNanoVG();
    void initializeMenu();

    // Читает resources/menu.gleb.
    void loadMenuResources();

    // Главный цикл и отрисовка.
    void mainLoop();
    void render();

    // Выполняет действие по имени из menu.gleb.
    void executeAction(const std::string& actionName);

    // Закрывает приложение.
    void closeApplication();

    // GLFW вызывает эту функцию при нажатии клавиши.
    static void keyCallback(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods
    );

    // Обрабатывает код клавиши GLFW.
    void handleKeyPress(int glfwKey);

    // Освобождает ресурсы.
    void shutdown();
};
