#pragma once

#include <cstddef>    // std::size_t
#include <functional> // std::function
#include <map>        // std::map
#include <memory>     // std::unique_ptr
#include <string>     // std::string
#include <vector>     // std::vector

#include "DialogWindow.h"
#include "KeyCode.h"
#include "MenuItem.h"

// Предварительное объявление NanoVG-контекста.
// Полный заголовок nanovg.h подключается в MenuSystem.cpp.
struct NVGcontext;

// Класс MenuSystem хранит состояние меню и отвечает за его отрисовку.
class MenuSystem {
private:
    std::unique_ptr<MenuItem> rootMenu; // корневой пункт меню

    // Все диалоговые окна: id окна -> объект окна.
    std::map<std::string, std::unique_ptr<DialogWindow>> dialogs;

    // Путь к текущему подменю.
    // Например, {1, 0} означает: второй пункт главного меню, затем первый пункт вложенного меню.
    std::vector<std::size_t> currentPath;

    // Выбранный пункт на каждом уровне меню.
    std::vector<std::size_t> selectedIndices;

    bool menuOpen = false;   // открыто ли меню
    bool dialogOpen = false; // открыто ли диалоговое окно

    std::string activeDialogId; // id активного диалогового окна

    // Индекс выбранной кнопки в активном диалоговом окне.
    std::size_t selectedDialogButtonIndex = 0;

    // Функция, которая выполняет действие по его строковому имени.
    // Она задаётся из Application вместо отдельного ActionManager.
    std::function<void(const std::string&)> actionExecutor;

public:
    MenuSystem();

    // Передаёт системе меню корневое меню.
    void setRootMenu(std::unique_ptr<MenuItem> menu);

    // Добавляет диалоговое окно.
    void addDialog(std::unique_ptr<DialogWindow> dialog);

    // Задаёт функцию выполнения действий.
    void setActionExecutor(std::function<void(const std::string&)> executor);

    // Открытие и закрытие меню.
    void open();
    void close();

    // Проверяет, открыто ли меню.
    bool isOpen() const;

    // Обрабатывает клавишу.
    void handleKey(KeyCode keyCode);

    // Навигация по меню.
    void moveUp();
    void moveDown();
    void enter();
    void goBack();
    void escape();

    // Работа с диалоговыми окнами.
    void openDialog(const std::string& dialogId);
    void closeDialog();

    // Отрисовка меню и активного диалога.
    void draw(NVGcontext* context, float screenWidth, float screenHeight);

private:
    // Возвращает текущее меню или подменю.
    MenuItem& getCurrentMenu();

    // Возвращает индекс выбранного пункта текущего меню.
    std::size_t& getCurrentSelectedIndex();

    // Обработка клавиш, когда открыто диалоговое окно.
    void handleDialogKey(KeyCode keyCode);

    // Отрисовка текущего меню.
    void drawCurrentMenu(NVGcontext* context, float screenWidth, float screenHeight);

    // Отрисовка активного диалогового окна.
    void drawDialog(NVGcontext* context, float screenWidth, float screenHeight);

    // Отрисовка одной кнопки меню.
    void drawMenuButton(
        NVGcontext* context,
        MenuItem& item,
        bool selected,
        float x,
        float y,
        float width,
        float height
    );

    // Отрисовка текста по центру прямоугольной области.
    void drawTextCentered(
        NVGcontext* context,
        const std::string& text,
        float x,
        float y,
        float width,
        float height,
        float fontSize
    );
};
