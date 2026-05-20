#include "app/Application.h"

#include <cstdlib>    // EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>   // std::cout, std::cerr
#include <stdexcept>  // std::runtime_error
#include <utility>    // std::move

#include <chrono>
#include <thread>

#include <glad/gl.h>  // функции OpenGL

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> // окно, клавиатура, OpenGL-контекст

#include <nanovg.h> // базовый NanoVG

#define NANOVG_GL3
#define NANOVG_GL_NO_HEADER
#include <nanovg_gl.h> // NanoVG для OpenGL 3

#include "menu/MenuParser.h" // парсер .gleb-меню


// Переводит клавишу GLFW в наш тип KeyCode.
static KeyCode convertGLFWKeyToKeyCode(int glfwKey) {
    switch (glfwKey) {
    case GLFW_KEY_UP:
        return KeyCode::Up;

    case GLFW_KEY_DOWN:
        return KeyCode::Down;

    case GLFW_KEY_ENTER:
    case GLFW_KEY_KP_ENTER:
        return KeyCode::Enter;

    case GLFW_KEY_DELETE:
    case GLFW_KEY_BACKSPACE:
        return KeyCode::Delete;

    case GLFW_KEY_ESCAPE:
        return KeyCode::Escape;

    default:
        return KeyCode::Unknown;
    }
}


// Удаляет окно GLFW.
void Application::GLFWwindowDeleter::operator()(GLFWwindow* window) const {
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
}


// Удаляет контекст NanoVG.
void Application::NanoVGContextDeleter::operator()(NVGcontext* context) const {
    if (context != nullptr) {
        nvgDeleteGL3(context);
    }
}


// Запускает приложение.
int Application::run() {
    try {
        initialize();
        mainLoop();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception) {
        std::cerr << "Application error: " << exception.what() << std::endl;
        shutdown();
        return EXIT_FAILURE;
    }
}


// Выполняет всю начальную настройку.
void Application::initialize() {
    initializeGLFW();
    createWindow();
    initializeGLAD();
    initializeNanoVG();
    initializeMenus();
}


// Инициализирует GLFW.
void Application::initializeGLFW() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwInitialized = true;

    // Версия OpenGL: 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Используем Core Profile.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Буфер трафарета нужен NanoVG.
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
}


// Создаёт окно приложения.
void Application::createWindow() {
    GLFWwindow* createdWindow = glfwCreateWindow(
        windowWidth,
        windowHeight,
        "CPLUS Lab3 Digger",
        nullptr,
        nullptr
    );

    if (createdWindow == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Передаём окно в unique_ptr.
    window.reset(createdWindow);

    // Делаем OpenGL-контекст текущим.
    glfwMakeContextCurrent(window.get());

    // Включаем вертикальную синхронизацию.
    glfwSwapInterval(1);

    // Связываем окно с объектом Application.
    glfwSetWindowUserPointer(window.get(), this);

    // Назначаем обработчик клавиатуры.
    glfwSetKeyCallback(window.get(), Application::keyCallback);
}


// Инициализирует GLAD.
void Application::initializeGLAD() {
    const int gladVersion = gladLoadGL(glfwGetProcAddress);

    if (gladVersion == 0) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    std::cout << "OpenGL loaded: "
        << GLAD_VERSION_MAJOR(gladVersion)
        << "."
        << GLAD_VERSION_MINOR(gladVersion)
        << std::endl;
}


// Создаёт контекст NanoVG.
void Application::initializeNanoVG() {
    NVGcontext* createdContext = nvgCreateGL3(
        NVG_ANTIALIAS | NVG_STENCIL_STROKES
    );

    if (createdContext == nullptr) {
        throw std::runtime_error("Failed to create NanoVG context");
    }

    // Передаём контекст в unique_ptr.
    nanoVGContext.reset(createdContext);

    // Загружаем стандартный шрифт Windows.
    fontId = nvgCreateFont(
        nanoVGContext.get(),
        "sans",
        "C:/Windows/Fonts/arial.ttf"
    );

    if (fontId == -1) {
        std::cerr << "Warning: failed to load font C:/Windows/Fonts/arial.ttf" << std::endl;
    }
}


// Загружает все меню.
void Application::initializeMenus() {
    mainMenu = loadMenu("resources/menus/main_menu.gleb");
    pauseMenu = loadMenu("resources/menus/pause_menu.gleb");
    gameOverMenu = loadMenu("resources/menus/game_over_menu.gleb");
    winMenu = loadMenu("resources/menus/win_menu.gleb");

    // При запуске открыто главное меню.
    mainMenu->open();
}


// Загружает одно меню из .gleb-файла.
std::unique_ptr<MenuSystem> Application::loadMenu(const std::string& filename) {
    MenuParser parser;
    MenuResource resource = parser.parse(filename);

    auto menu = std::make_unique<MenuSystem>();

    // Передаём меню способ выполнять действия.
    menu->setActionExecutor([this](const std::string& actionName) {
        executeAction(actionName);
        });

    // Устанавливаем корневое меню.
    menu->setRootMenu(std::move(resource.rootMenu));

    // Добавляем диалоговые окна.
    for (auto& pair : resource.dialogs) {
        menu->addDialog(std::move(pair.second));
    }

    return menu;
}


// Возвращает меню, активное сейчас.
MenuSystem* Application::getActiveMenu() {
    switch (appState) {
    case AppState::MainMenu:
        return mainMenu.get();

    case AppState::Paused:
        return pauseMenu.get();

    case AppState::GameOver:
        return gameOverMenu.get();

    case AppState::Win:
        return winMenu.get();

    case AppState::Playing:
        return nullptr;
    }

    return nullptr;
}


// Главный цикл программы.
void Application::mainLoop() {
    double previousTime = glfwGetTime();

    while (!glfwWindowShouldClose(window.get())) {
        const double currentTime = glfwGetTime();

        // Время между кадрами.
        const float deltaTime = static_cast<float>(currentTime - previousTime);
        previousTime = currentTime;

        update(deltaTime);
        render();

        // Показываем готовый кадр.
        glfwSwapBuffers(window.get());

        // Обрабатываем события окна.
        glfwPollEvents();

        // Небольшая пауза защищает от слишком быстрого цикла,
        // если вертикальная синхронизация не сработала.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


// Обновляет приложение.
void Application::update(float deltaTime) {
    if (appState != AppState::Playing) {
        return;
    }

    // Обновляем игру только во время игры.
    game.update(deltaTime);

    // Если игра закончилась поражением.
    if (game.getState() == GameState::GameOver) {
        appState = AppState::GameOver;
        gameOverMenu->open();
    }

    // Если игрок прошёл игру.
    else if (game.getState() == GameState::Win) {
        appState = AppState::Win;
        winMenu->open();
    }
}


// Рисует один кадр.
void Application::render() {
    int currentWindowWidth = 0;
    int currentWindowHeight = 0;
    int framebufferWidth = 0;
    int framebufferHeight = 0;

    // Получаем размеры окна.
    glfwGetWindowSize(window.get(), &currentWindowWidth, &currentWindowHeight);

    // Получаем размеры буфера кадра.
    glfwGetFramebufferSize(window.get(), &framebufferWidth, &framebufferHeight);

    if (currentWindowWidth <= 0 || currentWindowHeight <= 0) { return; }

    // Коэффициент для экранов с масштабированием.
    const float pixelRatio = static_cast<float>(framebufferWidth) / static_cast<float>(currentWindowWidth);

    // Настраиваем область OpenGL.
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    // Очищаем экран.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Начинаем кадр NanoVG.
    nvgBeginFrame(
        nanoVGContext.get(),
        static_cast<float>(currentWindowWidth),
        static_cast<float>(currentWindowHeight),
        pixelRatio
    );

    // Рисуем игру во всех игровых состояниях.
    if (appState == AppState::Playing ||
        appState == AppState::Paused ||
        appState == AppState::GameOver ||
        appState == AppState::Win
        ) {
        game.draw(
            renderer,
            nanoVGContext.get(),
            static_cast<float>(currentWindowWidth),
            static_cast<float>(currentWindowHeight)
        );
    }

    // Если мы в главном меню, просто заливаем фон.
    else {
        nvgBeginPath(nanoVGContext.get());
        nvgRect(
            nanoVGContext.get(),
            0.0f,
            0.0f,
            static_cast<float>(currentWindowWidth),
            static_cast<float>(currentWindowHeight)
        );
        nvgFillColor(nanoVGContext.get(), nvgRGBA(0, 0, 0, 255));
        nvgFill(nanoVGContext.get());
    }

    // Рисуем активное меню поверх.
    if (MenuSystem* activeMenu = getActiveMenu()) {
        activeMenu->draw(
            nanoVGContext.get(),
            static_cast<float>(currentWindowWidth),
            static_cast<float>(currentWindowHeight)
        );
    }

    // Завершаем кадр NanoVG.
    nvgEndFrame(nanoVGContext.get());
}


// Выполняет действие из меню.
void Application::executeAction(const std::string& actionName) {
    if (actionName == "start_game" || actionName == "restart_game") {
        mainMenu->close();
        pauseMenu->close();
        gameOverMenu->close();
        winMenu->close();

        game.startNewGame();
        appState = AppState::Playing;
    }

    else if (actionName == "resume_game") {
        pauseMenu->close();
        appState = AppState::Playing;
    }

    else if (actionName == "restart_level") {
        pauseMenu->close();
        game.restartCurrentLevel();
        appState = AppState::Playing;
    }

    else if (actionName == "main_menu") {
        pauseMenu->close();
        gameOverMenu->close();
        winMenu->close();

        appState = AppState::MainMenu;
        mainMenu->open();
    }

    else if (actionName == "open_controls_dialog") {
        if (MenuSystem* activeMenu = getActiveMenu()) {
            activeMenu->openDialog("controls_dialog");
        }
    }

    else if (actionName == "open_about_dialog") {
        if (MenuSystem* activeMenu = getActiveMenu()) {
            activeMenu->openDialog("about_dialog");
        }
    }

    else if (actionName == "close_dialog") {
        if (MenuSystem* activeMenu = getActiveMenu()) {
            activeMenu->closeDialog();
        }
    }

    else if (actionName == "exit_application") {
        closeApplication();
    }

    else {
        std::cerr << "Unknown action: " << actionName << std::endl;
    }
}


// Закрывает приложение.
void Application::closeApplication() {
    if (window != nullptr) {
        glfwSetWindowShouldClose(window.get(), GLFW_TRUE);
    }
}


// Статический обработчик клавиатуры GLFW.
void Application::keyCallback(GLFWwindow* window, int key, int, int action, int) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    // Получаем Application из окна.
    Application* application = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (application != nullptr) {
        application->handleKeyEvent(key, action);
    }
}


// Обрабатывает событие клавиатуры.
void Application::handleKeyEvent(int glfwKey, int action) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    // Во время игры клавиши идут в игру.
    if (appState == AppState::Playing) {
        handleGameKey(glfwKey);
        return;
    }

    // В меню клавиши идут в MenuSystem.
    MenuSystem* activeMenu = getActiveMenu();

    if (activeMenu != nullptr && activeMenu->isOpen()) {
        const KeyCode keyCode = convertGLFWKeyToKeyCode(glfwKey);

        if (keyCode != KeyCode::Unknown) {
            activeMenu->handleKey(keyCode);

            // Escape в паузе возвращает в игру.
            if (appState == AppState::Paused && !activeMenu->isOpen()) {
                appState = AppState::Playing;
            }

            return;
        }
    }
}


// Обрабатывает клавиши во время игры.
void Application::handleGameKey(int glfwKey) {
    switch (glfwKey) {
    case GLFW_KEY_UP:
        game.requestMove(Direction::Up);
        break;

    case GLFW_KEY_DOWN:
        game.requestMove(Direction::Down);
        break;

    case GLFW_KEY_LEFT:
        game.requestMove(Direction::Left);
        break;

    case GLFW_KEY_RIGHT:
        game.requestMove(Direction::Right);
        break;

    case GLFW_KEY_SPACE:
    case GLFW_KEY_Z:
        game.requestFire();
        break;

    case GLFW_KEY_ESCAPE:
        appState = AppState::Paused;
        pauseMenu->open();
        break;

    default:
        break;
    }
}


// Освобождает ресурсы.
void Application::shutdown() {
    mainMenu.reset();
    pauseMenu.reset();
    gameOverMenu.reset();
    winMenu.reset();

    nanoVGContext.reset();
    window.reset();

    if (glfwInitialized) {
        glfwTerminate();
        glfwInitialized = false;
    }
}