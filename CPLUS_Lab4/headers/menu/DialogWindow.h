#pragma once

#include <string>
#include <vector>

// Кнопка внутри диалогового окна.
// Хранит текст, действие и размер кнопки.
class DialogButton {
private:
    // Идентификатор кнопки из .gleb-файла.
    std::string id;

    // Текст, который отображается на кнопке.
    std::string text;

    // Название действия, которое выполнит Application.
    std::string actionName;

    // Размер кнопки на экране.
    float width = 180.0f;
    float height = 48.0f;

public:
    // Возвращает идентификатор кнопки.
    const std::string& getId() const { return id; }

    // Возвращает текст кнопки.
    const std::string& getText() const { return text; }

    // Возвращает название действия кнопки.
    const std::string& getActionName() const { return actionName; }

    // Возвращает ширину кнопки.
    float getWidth() const { return width; }

    // Возвращает высоту кнопки.
    float getHeight() const { return height; }

    // Устанавливает идентификатор кнопки.
    void setId(const std::string& newId) { id = newId; }

    // Устанавливает текст кнопки.
    void setText(const std::string& newText) { text = newText; }

    // Устанавливает действие кнопки.
    void setActionName(const std::string& newActionName) { actionName = newActionName; }

    // Устанавливает размер кнопки.
    void setSize(float newWidth, float newHeight) {
        width = newWidth;
        height = newHeight;
    }
};

// Диалоговое окно, которое может быть описано во внешнем .gleb-файле.
class DialogWindow {
private:
    // Идентификатор диалога.
    std::string id;

    // Заголовок диалогового окна.
    std::string title;

    // Основной текст диалогового окна.
    std::string text;

    // Размер диалогового окна.
    float width = 600.0f;
    float height = 300.0f;

    // Кнопки внутри диалога.
    std::vector<DialogButton> buttons;

public:
    // Возвращает идентификатор диалога.
    const std::string& getId() const { return id; }

    // Возвращает заголовок диалога.
    const std::string& getTitle() const { return title; }

    // Возвращает основной текст диалога.
    const std::string& getText() const { return text; }

    // Возвращает ширину диалога.
    float getWidth() const { return width; }

    // Возвращает высоту диалога.
    float getHeight() const { return height; }

    // Возвращает список кнопок для изменения и отрисовки.
    std::vector<DialogButton>& getButtons() { return buttons; }

    // Устанавливает идентификатор диалога.
    void setId(const std::string& newId) { id = newId; }

    // Устанавливает заголовок диалога.
    void setTitle(const std::string& newTitle) { title = newTitle; }

    // Устанавливает основной текст диалога.
    void setText(const std::string& newText) { text = newText; }

    // Устанавливает размер диалога.
    void setSize(float newWidth, float newHeight) {
        width = newWidth;
        height = newHeight;
    }

    // Добавляет кнопку в диалог.
    void addButton(const DialogButton& button) {
        buttons.push_back(button);
    }
};
