#pragma once

#include <cstddef> // std::size_t
#include <memory>  // std::unique_ptr
#include <string>  // std::string
#include <utility> // std::move
#include <vector>  // std::vector

#include "Color.h"

// Класс MenuItem описывает один пункт меню.
// Один и тот же класс используется и для обычного пункта, и для подменю.
class MenuItem {
private:
    std::string id;         // внутренний идентификатор пункта
    std::string text;       // текст, который отображается на экране
    std::string actionName; // имя действия, которое вызывается при выборе

    Color normalColor;      // обычный цвет пункта
    Color selectedColor;    // цвет выбранного пункта
    Color textColor;        // цвет текста

    float width = 360.0f;   // ширина кнопки меню
    float height = 52.0f;   // высота кнопки меню

    // Дочерние пункты меню.
    // Если children не пустой, этот пункт является подменю.
    std::vector<std::unique_ptr<MenuItem>> children;

public:
    // Конструктор задаёт цвета пункта по умолчанию.
    MenuItem() {
        normalColor = { 70, 70, 70, 255 };
        selectedColor = { 110, 110, 110, 255 };
        textColor = { 255, 255, 255, 255 };
    }

    // Копирование запрещено: один дочерний пункт не может иметь двух владельцев.
    // Это связано с тем, что children хранит std::unique_ptr.
    MenuItem(const MenuItem&) = delete;
    MenuItem& operator=(const MenuItem&) = delete;

    // Перемещение разрешено: объект можно безопасно передать новому владельцу.
    MenuItem(MenuItem&&) noexcept = default;
    MenuItem& operator=(MenuItem&&) noexcept = default;

    // Геттеры основных строковых полей.
    // const std::string& getId() const { return id; }
    const std::string& getText() const { return text; }
    const std::string& getActionName() const { return actionName; }

    // Сеттеры основных строковых полей.
    void setId(const std::string& newId) { id = newId; }
    void setText(const std::string& newText) { text = newText; }
    void setActionName(const std::string& newActionName) { actionName = newActionName; }

    // Геттеры цветов.
    const Color& getNormalColor() const { return normalColor; }
    const Color& getSelectedColor() const { return selectedColor; }
    const Color& getTextColor() const { return textColor; }

    // Сеттеры цветов.
    void setNormalColor(const Color& color) { normalColor = color; }
    void setSelectedColor(const Color& color) { selectedColor = color; }
    void setTextColor(const Color& color) { textColor = color; }

    // Размер кнопки меню.
    // Значения могут задаваться в menu.gleb через SIZE width height.
    float getWidth() const { return width; }
    float getHeight() const { return height; }

    void setSize(float newWidth, float newHeight) {
        width = newWidth;
        height = newHeight;
    }

    // Добавляет дочерний пункт и передаёт владение в children.
    void addChild(std::unique_ptr<MenuItem> child) {
        children.push_back(std::move(child));
    }

    // Получение дочернего пункта по индексу.
    // at() дополнительно проверяет выход за границы.
    MenuItem& getChild(std::size_t index) {
        return *children.at(index);
    }

    // Количество дочерних пунктов.
    std::size_t getChildrenCount() const {
        return children.size();
    }

    // Проверяет, является ли пункт подменю.
    bool hasChildren() const {
        return !children.empty();
    }

    // Проверяет, есть ли у пункта действие.
    bool hasAction() const {
        return !actionName.empty();
    }
};
