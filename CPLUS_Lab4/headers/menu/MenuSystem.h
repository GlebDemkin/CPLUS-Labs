#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "menu/DialogWindow.h"
#include "menu/KeyCode.h"
#include "menu/MenuItem.h"

struct NVGcontext;

// MenuSystem хранит состояние меню и отвечает за отрисовку пунктов и диалогов.
class MenuSystem {
private:
    // Корневое меню, загруженное из .gleb-файла.
    std::unique_ptr<MenuItem> rootMenu;

    // Диалоговые окна, доступные по идентификатору.
    std::map<std::string, std::unique_ptr<DialogWindow>> dialogs;

    // Путь к текущему подменю: индексы выбранных веток от корня.
    std::vector<std::size_t> currentPath;

    // Выбранный пункт на каждом уровне вложенности меню.
    std::vector<std::size_t> selectedIndices;

    // Открыто ли меню сейчас.
    bool menuOpen = false;

    // Открыт ли поверх меню диалог.
    bool dialogOpen = false;

    // Идентификатор активного диалога.
    std::string activeDialogId;

    // Индекс выбранной кнопки активного диалога.
    std::size_t selectedDialogButtonIndex = 0;

    // Функция, которая передаёт действие меню в Application.
    std::function<void(const std::string&)> actionExecutor;

public:
    // Создаёт систему меню.
    MenuSystem();

    // Устанавливает корневое меню.
    void setRootMenu(std::unique_ptr<MenuItem> menu);

    // Добавляет диалоговое окно.
    void addDialog(std::unique_ptr<DialogWindow> dialog);

    // Устанавливает обработчик действий меню.
    void setActionExecutor(std::function<void(const std::string&)> executor);

    // Открывает меню.
    void open();

    // Закрывает меню и сбрасывает путь.
    void close();

    // Проверяет, открыто ли меню.
    bool isOpen() const;

    // Обрабатывает клавишу меню.
    void handleKey(KeyCode keyCode);

    // Перемещает выбор вверх.
    void moveUp();

    // Перемещает выбор вниз.
    void moveDown();

    // Выполняет выбранный пункт.
    void enter();

    // Возвращает на уровень выше.
    void goBack();

    // Обрабатывает Escape.
    void escape();

    // Открывает диалог по id.
    void openDialog(const std::string& dialogId);

    // Закрывает активный диалог.
    void closeDialog();

    // Рисует меню и активный диалог.
    void draw(NVGcontext* context, float screenWidth, float screenHeight);

private:
    // Возвращает текущее открытое меню.
    MenuItem& getCurrentMenu();

    // Возвращает выбранный индекс текущего меню.
    std::size_t& getCurrentSelectedIndex();

    // Обрабатывает клавиши активного диалога.
    void handleDialogKey(KeyCode keyCode);

    // Рисует текущее меню.
    void drawCurrentMenu(NVGcontext* context, float screenWidth, float screenHeight);

    // Рисует активный диалог.
    void drawDialog(NVGcontext* context, float screenWidth, float screenHeight);

    // Рисует одну кнопку меню.
    void drawMenuButton(
        NVGcontext* context,
        MenuItem& item,
        bool selected,
        float x,
        float y,
        float width,
        float height
    );

    // Рисует текст по центру прямоугольной области.
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
