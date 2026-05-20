#pragma once

#include "objects/GameObject.h" // базовый игровой объект

// Изумруд автоматически собирается игроком при входе в клетку.
class Emerald final : public GameObject {
public:
    // Создаёт изумруд в заданной клетке.
    explicit Emerald(Vec2i startPosition)
        : GameObject(startPosition) {
    }

    // Изумруд пассивен: его сбор обрабатывается в GameWorld.
    void update(GameWorld&, float) override {
    }

    // Возвращает тип объекта.
    ObjectType getType() const override {
        return ObjectType::Emerald;
    }
};