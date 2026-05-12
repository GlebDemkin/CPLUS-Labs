#pragma once

#include <string> // std::string
#include <vector> // std::vector

// Класс DialogButton описывает кнопку внутри диалогового окна.
class DialogButton {
private:
    std::string id;         // внутренний идентификатор кнопки
    std::string text;       // текст, который отображается на кнопке
    std::string actionName; // действие, которое выполняется при выборе кнопки

    float width = 180.0f;   // ширина кнопки
    float height = 48.0f;   // высота кнопки

public:
    // Геттеры строковых полей.
    const std::string& getId() const { return id; }
    const std::string& getText() const { return text; }
    const std::string& getActionName() const { return actionName; }

    // Размер кнопки.
    float getWidth() const { return width; }
    float getHeight() const { return height; }

    // Сеттеры строковых полей.
    void setId(const std::string& newId) { id = newId; }
    void setText(const std::string& newText) { text = newText; }
    void setActionName(const std::string& newActionName) { actionName = newActionName; }

    // Размер задаётся в menu.gleb через SIZE width height.
    void setSize(float newWidth, float newHeight) {
        width = newWidth;
        height = newHeight;
    }
};

// Класс DialogWindow описывает диалоговое окно.
class DialogWindow {
private:
    std::string id;    // внутренний идентификатор окна
    std::string title; // заголовок окна
    std::string text;  // основной текст окна

    float width = 600.0f;  // ширина окна
    float height = 300.0f; // высота окна

    std::vector<DialogButton> buttons; // кнопки диалогового окна

public:
    // Геттеры строковых полей.
    const std::string& getId() const { return id; }
    const std::string& getTitle() const { return title; }
    const std::string& getText() const { return text; }

    // Размер окна.
    float getWidth() const { return width; }
    float getHeight() const { return height; }

    // Возвращает кнопки окна.
    // Неконстантная версия нужна, чтобы MenuSystem мог работать с кнопками при отрисовке.
    std::vector<DialogButton>& getButtons() { return buttons; }

    // Сеттеры строковых полей.
    void setId(const std::string& newId) { id = newId; }
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setText(const std::string& newText) { text = newText; }

    // Размер задаётся в menu.gleb через SIZE width height.
    void setSize(float newWidth, float newHeight) {
        width = newWidth;
        height = newHeight;
    }

    // Добавляет кнопку в диалоговое окно.
    void addButton(const DialogButton& button) {
        buttons.push_back(button);
    }
};
