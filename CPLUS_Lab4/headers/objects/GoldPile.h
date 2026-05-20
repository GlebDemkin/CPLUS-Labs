#pragma once

#include "objects/GameObject.h" // базовый игровой объект

// Кучка золота появляется после падения мешка и может быть подобрана.
class GoldPile final : public GameObject {
public:
    // Создаёт кучку золота в заданной клетке.
    explicit GoldPile(Vec2i startPosition)
        : GameObject(startPosition) {
    }

    // Кучка золота пассивна: её сбор обрабатывается в GameWorld.
    void update(GameWorld&, float) override {
    }

    // Возвращает тип объекта.
    ObjectType getType() const override {
        return ObjectType::GoldPile;
    }
};