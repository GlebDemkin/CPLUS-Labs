#pragma once

#include "game/Vec2i.h"         // координаты клетки
#include "objects/ObjectType.h" // тип игрового объекта

class GameWorld; // игровой мир, нужен для update

// GameObject — абстрактный базовый класс для всех игровых сущностей.
class GameObject {
private:
    // Позиция объекта на сетке уровня.
    Vec2i position;

    // Активен ли объект.
    bool active = true;

public:
    // Создаёт объект в заданной клетке.
    explicit GameObject(Vec2i startPosition)
        : position(startPosition) {
    }

    // Виртуальный деструктор нужен для корректного удаления наследников.
    virtual ~GameObject() = default;

    // Возвращает тип объекта.
    virtual ObjectType getType() const = 0;

    // Обновляет объект.
    virtual void update(GameWorld& world, float deltaTime) = 0;

    // Возвращает позицию объекта.
    Vec2i getPosition() const {
        return position;
    }

    // Меняет позицию объекта.
    void setPosition(Vec2i newPosition) {
        position = newPosition;
    }

    // Проверяет, активен ли объект.
    bool isActive() const {
        return active;
    }

    // Деактивирует объект.
    void deactivate() {
        active = false;
    }
};