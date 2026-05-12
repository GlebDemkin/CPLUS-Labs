#include "Application.h"

#include <cstdlib>   // EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>  // std::cout, std::cerr
#include <stdexcept> // std::runtime_error
#include <utility>   // std::move

#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <nanovg.h>

#define NANOVG_GL3
#define NANOVG_GL_NO_HEADER
#include <nanovg_gl.h>

#include "KeyCode.h"
#include "MenuParser.h"

// Переводит код клавиши GLFW во внутренний тип KeyCode.
// Благодаря этому MenuSystem не зависит напрямую от GLFW.
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
// Эта функция вызывается автоматически, когда unique_ptr очищается.
void Application::GLFWwindowDeleter::operator()(GLFWwindow* window) const {
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
}

// Удаляет NanoVG-контекст.
// Эта функция вызывается автоматически, когда unique_ptr очищается.
void Application::NanoVGContextDeleter::operator()(NVGcontext* context) const {
    if (context != nullptr) {
        nvgDeleteGL3(context);
    }
}

Application::Application() = default;

Application::~Application() {
    shutdown();
}

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

void Application::initialize() {
    initializeGLFW();
    createWindow();
    initializeGLAD();
    initializeNanoVG();
    initializeMenu();
}

void Application::initializeGLFW() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwInitialized = true;

    // Настраиваем OpenGL 3.3 Core Profile.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // NanoVG использует stencil buffer для корректной отрисовки.
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
}

void Application::createWindow() {
    GLFWwindow* createdWindow = glfwCreateWindow(
        windowWidth,
        windowHeight,
        "CPLUS_Lab3 Menu System",
        nullptr,
        nullptr
    );

    if (createdWindow == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Передаём владение окном в unique_ptr.
    window.reset(createdWindow);

    // Делаем OpenGL-контекст этого окна текущим.
    glfwMakeContextCurrent(window.get());

    // Включаем вертикальную синхронизацию.
    glfwSwapInterval(1);

    // Сохраняем this внутри GLFW-окна.
    // Это нужно, чтобы из статического keyCallback попасть обратно в объект Application.
    glfwSetWindowUserPointer(window.get(), this);

    // Назначаем обработчик клавиатуры.
    glfwSetKeyCallback(window.get(), Application::keyCallback);
}

void Application::initializeGLAD() {
    // GLAD загружает адреса функций OpenGL.
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

void Application::initializeNanoVG() {
    // Создаём NanoVG-контекст для OpenGL 3.
    NVGcontext* createdContext = nvgCreateGL3(
        NVG_ANTIALIAS | NVG_STENCIL_STROKES
    );

    if (createdContext == nullptr) {
        throw std::runtime_error("Failed to create NanoVG context");
    }

    // Передаём владение NanoVG-контекстом в unique_ptr.
    nanoVGContext.reset(createdContext);

    // Загружаем шрифт для текста меню.
    fontId = nvgCreateFont(
        nanoVGContext.get(),
        "sans",
        "C:/Windows/Fonts/arial.ttf"
    );

    if (fontId == -1) {
        std::cerr << "Warning: failed to load font" << std::endl;
    }
}

void Application::initializeMenu() {
    // Создаём систему меню.
    // ActionManager теперь удалён: действия выполняются через executeAction().
    menuSystem = std::make_unique<MenuSystem>();

    // Передаём MenuSystem функцию, которая умеет выполнять действия по строковому имени.
    menuSystem->setActionExecutor([this](const std::string& actionName) {
        executeAction(actionName);
    });

    loadMenuResources();

    // После загрузки ресурсов сразу открываем меню.
    menuSystem->open();
}

void Application::loadMenuResources() {
    MenuParser parser;

    // Читаем внешний файл ресурсов.
    MenuResource resource = parser.parse("resources/menu.gleb");

    // Передаём главное меню в MenuSystem.
    menuSystem->setRootMenu(std::move(resource.rootMenu));

    // Передаём все диалоговые окна в MenuSystem.
    for (auto& pair : resource.dialogs) {
        menuSystem->addDialog(std::move(pair.second));
    }
}

void Application::mainLoop() {
    // Главный цикл работает, пока окно не попросили закрыть.
    while (!glfwWindowShouldClose(window.get())) {
        render();

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }
}

void Application::render() {
    int currentWindowWidth = 0;
    int currentWindowHeight = 0;
    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwGetWindowSize(window.get(), &currentWindowWidth, &currentWindowHeight);
    glfwGetFramebufferSize(window.get(), &framebufferWidth, &framebufferHeight);

    if (currentWindowWidth <= 0 || currentWindowHeight <= 0) {
        return;
    }

    // pixelRatio нужен NanoVG для корректной отрисовки на экранах с масштабированием.
    const float pixelRatio =
        static_cast<float>(framebufferWidth) /
        static_cast<float>(currentWindowWidth);

    glViewport(0, 0, framebufferWidth, framebufferHeight);

    // Очищаем экран перед отрисовкой нового кадра.
    glClearColor(0.10f, 0.10f, 0.13f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Начинаем кадр NanoVG.
    nvgBeginFrame(
        nanoVGContext.get(),
        static_cast<float>(currentWindowWidth),
        static_cast<float>(currentWindowHeight),
        pixelRatio
    );

    if (menuSystem != nullptr) {
        menuSystem->draw(
            nanoVGContext.get(),
            static_cast<float>(currentWindowWidth),
            static_cast<float>(currentWindowHeight)
        );
    }

    // Завершаем кадр NanoVG.
    nvgEndFrame(nanoVGContext.get());
}

void Application::executeAction(const std::string& actionName) {
    // Здесь собраны все действия, которые могут быть указаны в menu.gleb.
    // Это проще, чем отдельный ActionManager.
    if (actionName == "start_game") {
        menuSystem->openDialog("start_dialog");
    }
    else if (actionName == "open_about_dialog") {
        menuSystem->openDialog("about_dialog");
    }
    else if (actionName == "open_graphics_dialog") {
        menuSystem->openDialog("graphics_dialog");
    }
    else if (actionName == "open_sound_dialog") {
        menuSystem->openDialog("sound_dialog");
    }
    else if (actionName == "close_dialog") {
        menuSystem->closeDialog();
    }
    else if (actionName == "go_back") {
        menuSystem->goBack();
    }
    else if (actionName == "close_menu") {
        menuSystem->close();
    }
    else if (actionName == "exit_application") {
        closeApplication();
    }
    else {
        std::cerr << "Unknown action: " << actionName << std::endl;
    }
}

void Application::closeApplication() {
    if (window != nullptr) {
        glfwSetWindowShouldClose(window.get(), GLFW_TRUE);
    }
}

void Application::keyCallback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
) {
    // Эти параметры сейчас не используются.
    (void)scancode;
    (void)mods;

    // Обрабатываем только момент нажатия клавиши.
    if (action != GLFW_PRESS) {
        return;
    }

    // Достаём указатель на Application, который ранее сохранили в createWindow().
    Application* application = static_cast<Application*>(
        glfwGetWindowUserPointer(window)
    );

    if (application != nullptr) {
        application->handleKeyPress(key);
    }
}

void Application::handleKeyPress(int glfwKey) {
    // M открывает меню снова, если оно было закрыто.
    if (glfwKey == GLFW_KEY_M) {
        menuSystem->open();
        return;
    }

    const KeyCode keyCode = convertGLFWKeyToKeyCode(glfwKey);

    if (keyCode == KeyCode::Unknown) {
        return;
    }

    // Если меню открыто, передаём клавишу в MenuSystem.
    if (menuSystem != nullptr && menuSystem->isOpen()) {
        menuSystem->handleKey(keyCode);
        return;
    }

    // Если меню закрыто, Escape закрывает приложение.
    if (keyCode == KeyCode::Escape) {
        closeApplication();
    }
}

void Application::shutdown() {
    // Сначала удаляем логику меню.
    menuSystem.reset();

    // Потом удаляем графические ресурсы.
    nanoVGContext.reset();
    window.reset();

    // Завершаем GLFW только если он был успешно инициализирован.
    if (glfwInitialized) {
        glfwTerminate();
        glfwInitialized = false;
    }
}
