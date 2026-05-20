#pragma once

#include <memory>  // std::unique_ptr
#include <string>  // std::string

#include "app/AppState.h"        // состояние приложения
#include "game/Game.h"           // игровая логика
#include "graphics/Renderer.h"   // отрисовка
#include "menu/MenuSystem.h"     // меню

struct GLFWwindow;  // окно GLFW
struct NVGcontext;  // контекст NanoVG

// Application управляет окном, главным циклом, меню и переключением состояний.
class Application {
private:
    // Удаляет окно GLFW.
    // Структура, объект которой можно вызвать как функцию.
    struct GLFWwindowDeleter {
        void operator()(GLFWwindow* window) const;
    };

    // Удаляет контекст NanoVG.
    struct NanoVGContextDeleter {
        void operator()(NVGcontext* context) const;
    };

private:
    // Окно приложения.
    // std::unique_ptr<ТипОбъекта, СпособУдаления>
    std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window;

    // Контекст для рисования NanoVG.
    std::unique_ptr<NVGcontext, NanoVGContextDeleter> nanoVGContext;

    // Главное меню.
    // std::unique_ptr<ТипОбъекта>
    std::unique_ptr<MenuSystem> mainMenu;

    // Меню паузы.
    std::unique_ptr<MenuSystem> pauseMenu;

    // Меню проигрыша.
    std::unique_ptr<MenuSystem> gameOverMenu;

    // Меню победы.
    std::unique_ptr<MenuSystem> winMenu;

    // Основная игра.
    Game game;

    // Отрисовщик игры и интерфейса.
    Renderer renderer;

    // Текущее состояние приложения.
    AppState appState = AppState::MainMenu;

    // Флаг успешной инициализации GLFW.
    bool glfwInitialized = false;

    // Идентификатор шрифта NanoVG.
    int fontId = -1;

    // Размеры окна.
    int windowWidth = 1280;
    int windowHeight = 720;

public:
    // Создаёт объект приложения.
    Application() = default;

    // Освобождает ресурсы приложения.
    ~Application() { shutdown(); }

    // Запрещает копирование приложения.
    Application(const Application&) = delete;

    // Запрещает присваивание приложения.
    Application& operator=(const Application&) = delete;

    // Запускает приложение.
    int run();

private:
    // Инициализирует все подсистемы.
    void initialize();

    // Инициализирует GLFW.
    void initializeGLFW();

    // Создаёт окно.
    void createWindow();

    // Инициализирует GLAD.
    void initializeGLAD();

    // Инициализирует NanoVG.
    void initializeNanoVG();

    // Загружает все меню.
    void initializeMenus();

    // Загружает одно меню из файла.
    std::unique_ptr<MenuSystem> loadMenu(const std::string& filename);

    // Возвращает активное меню.
    MenuSystem* getActiveMenu();

    // Выполняет главный цикл.
    void mainLoop();

    // Рисует один кадр.
    void render();

    // Обновляет состояние приложения.
    void update(float deltaTime);

    // Выполняет действие из меню.
    void executeAction(const std::string& actionName);

    // Закрывает приложение.
    void closeApplication();

    // Обрабатывает клавиши GLFW.
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Обрабатывает событие клавиатуры.
    void handleKeyEvent(int glfwKey, int action);

    // Обрабатывает клавиши во время игры.
    void handleGameKey(int glfwKey);

    // Освобождает ресурсы.
    void shutdown();
};