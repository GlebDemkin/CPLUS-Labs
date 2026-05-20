#pragma once

#include "game/Direction.h"     // направление движения
#include "objects/GameObject.h" // базовый игровой объект

class GameWorld; // игровой мир, нужен для update и движения

// Игрок управляет машиной Diggermobile.
class Player final : public GameObject {
private:
    // Текущее направление игрока.
    Direction direction = Direction::Right;

    // Таймер задержки между шагами.
    float moveTimer = 0.0f;

    // Таймер перезарядки выстрела.
    float fireCooldown = 0.0f;

public:
    // Создаёт игрока в заданной клетке.
    explicit Player(Vec2i startPosition) 
        : GameObject(startPosition) {
    }

    // Обновляет движение и выстрел.
    void update(GameWorld& world, float deltaTime) override;

    // Возвращает тип объекта.
    ObjectType getType() const override {
        return ObjectType::Player;
    }

    // Возвращает направление игрока.
    Direction getDirection() const {
        return direction;
    }

    // Меняет направление игрока.
    void setDirection(Direction newDirection) {
        direction = newDirection;
    }   

    // Возрождает игрока в стартовой позиции.
    void respawn(Vec2i startPosition);

private:
    // Пытается сделать шаг в заданном направлении.
    void tryMove(GameWorld& world, Direction moveDirection);
};