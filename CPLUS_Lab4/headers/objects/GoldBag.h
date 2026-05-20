#pragma once

#include "objects/GameObject.h" // базовый игровой объект

// Мешок золота можно толкать горизонтально. При падении он опасен для игрока и врагов.
class GoldBag final : public GameObject {
private:
    // Падает ли мешок сейчас.
    bool falling = false;

    // Сколько клеток мешок уже пролетел вниз.
    int fallDistance = 0;

    // Таймер падения.
    float fallTimer = 0.0f;

public:
    // Создаёт мешок золота в заданной клетке.
    explicit GoldBag(Vec2i startPosition) 
        : GameObject(startPosition) {
    }


    // Обновляет падение мешка.
    void update(GameWorld& world, float deltaTime) override;

    // Возвращает тип объекта.
    ObjectType getType() const {
        return ObjectType::GoldBag;
    }

    // Проверяет, падает ли мешок.
    bool isFalling() const {
        return falling;
    }

    // Возвращает расстояние падения.
    int getFallDistance() const {
        return fallDistance;
    }

private:
    // Останавливает падение и при необходимости разбивает мешок.
    void stopFalling(GameWorld& world);
};