#pragma once

#include "game/Direction.h"     // направление полёта
#include "objects/GameObject.h" // базовый игровой объект

// Огненный шар летит по прямой и уничтожает первого встреченного врага.
class Fireball final : public GameObject {
private:
    // Направление полёта огненного шара.
    Direction direction;

    // Таймер движения.
    float moveTimer = 0.0f;

public:
    // Создаёт огненный шар в заданной клетке и направлении.
    Fireball(Vec2i startPosition, Direction startDirection);

    // Обновляет движение и столкновения.
    void update(GameWorld& world, float deltaTime) override;

    // Возвращает тип объекта.
    ObjectType getType() const override;

    // Возвращает направление полёта.
    Direction getDirection() const;
};