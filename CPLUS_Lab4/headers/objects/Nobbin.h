#pragma once

#include "game/Direction.h"     // направление движения
#include "objects/GameObject.h" // базовый игровой объект

// Nobbin — враг. Он не роет землю и двигается только по готовым тоннелям.
class Nobbin final : public GameObject {
private:
    // Текущее направление врага для выбора спрайта.
    Direction direction = Direction::Left;

    // Таймер движения врага.
    float moveTimer = 0.0f;

public:
    // Создаёт врага в заданной клетке.
    explicit Nobbin(Vec2i startPosition)
        : GameObject(startPosition) {
    }

    // Обновляет движение врага.
    void update(GameWorld& world, float deltaTime) override;

    // Возвращает тип объекта.
    ObjectType getType() const {
        return ObjectType::Nobbin;
    }

    // Возвращает направление движения.
    Direction getDirection() const {
        return direction;
    }
};