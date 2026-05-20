#pragma once

#include "game/Direction.h" // направление движения

// Состояние ввода для игры (хранит запросы). Application записывает команды, а Player их читает.
struct InputState {
    // Был ли запрошен шаг игрока.
    bool moveRequested = false;

    // Направление запрошенного шага.
    Direction moveDirection = Direction::Right;

    // Был ли запрошен выстрел.
    bool fireRequested = false;

    // Запоминает запрос движения.
    void requestMove(Direction direction) {
        moveRequested = true;
        moveDirection = direction;
    }

    // Запоминает запрос выстрела.
    void requestFire() {
        fireRequested = true;
    }

    // Очищает одноразовые команды после обработки кадра.
    void clearOneFrameActions() {
        moveRequested = false;
        fireRequested = false;
    }
};