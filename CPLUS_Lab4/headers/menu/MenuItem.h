#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "graphics/Color.h"

// Один пункт меню. Он может быть обычной кнопкой или подменю.
class MenuItem {
private:
    // Идентификатор пункта из .gleb-файла.
    std::string id;

    // Текст, который показывается пользователю.
    std::string text;

    // Название действия, которое выполнит Application.
    std::string actionName;

    // Цвет обычного состояния кнопки.
    Color normalColor;

    // Цвет выбранного состояния кнопки.
    Color selectedColor;

    // Цвет текста кнопки.
    Color textColor;

    // Размер кнопки меню.
    float width = 360.0f;
    float height = 52.0f;

    // Дочерние пункты, если пункт является подменю.
    std::vector<std::unique_ptr<MenuItem>> children;

public:
    // Создаёт пункт меню с базовыми цветами.
    MenuItem() {
        normalColor = { 70, 70, 70, 255 };
        selectedColor = { 110, 110, 110, 255 };
        textColor = { 255, 255, 255, 255 };
    }

    // Копирование запрещено, потому что внутри есть unique_ptr.
    MenuItem(const MenuItem&) = delete;
    MenuItem& operator=(const MenuItem&) = delete;

    // Перемещение разрешено для передачи пунктов между контейнерами.
    MenuItem(MenuItem&&) noexcept = default;
    MenuItem& operator=(MenuItem&&) noexcept = default;

    // Возвращает идентификатор пункта.
    const std::string& getId() const { return id; }

    // Возвращает текст пункта.
    const std::string& getText() const { return text; }

    // Возвращает действие пункта.
    const std::string& getActionName() const { return actionName; }

    // Устанавливает идентификатор пункта.
    void setId(const std::string& newId) { id = newId; }

    // Устанавливает текст пункта.
    void setText(const std::string& newText) { text = newText; }

    // Устанавливает действие пункта.
    void setActionName(const std::string& newActionName) { actionName = newActionName; }

    // Возвращает цвет обычной кнопки.
    const Color& getNormalColor() const { return normalColor; }

    // Возвращает цвет выбранной кнопки.
    const Color& getSelectedColor() const { return selectedColor; }

    // Возвращает цвет текста.
    const Color& getTextColor() const { return textColor; }

    // Устанавливает цвет обычной кнопки.
    void setNormalColor(const Color& color) { normalColor = color; }

    // Устанавливает цвет выбранной кнопки.
    void setSelectedColor(const Color& color) { selectedColor = color; }

    // Устанавливает цвет текста.
    void setTextColor(const Color& color) { textColor = color; }

    // Возвращает ширину кнопки.
    float getWidth() const { return width; }

    // Возвращает высоту кнопки.
    float getHeight() const { return height; }

    // Устанавливает размер кнопки.
    void setSize(float newWidth, float newHeight) {
        width = newWidth;
        height = newHeight;
    }

    // Проверяет, есть ли у пункта подменю.
    bool hasChildren() const { return !children.empty(); }

    // Проверяет, есть ли у пункта действие.
    bool hasAction() const { return !actionName.empty(); }

    // Возвращает количество дочерних пунктов.
    std::size_t getChildrenCount() const { return children.size(); }

    // Возвращает дочерний пункт для изменения.
    MenuItem& getChild(std::size_t index) { return *children.at(index); }

    // Возвращает дочерний пункт только для чтения.
    const MenuItem& getChild(std::size_t index) const { return *children.at(index); }

    // Добавляет дочерний пункт в подменю.
    void addChild(std::unique_ptr<MenuItem> child) {
        children.push_back(std::move(child));
    }
};
