#pragma once

#include "game/Vec2i.h" // координаты клетки

// Направление движения объекта по сетке уровня.
enum class Direction {
    Up,    // вверх
    Down,  // вниз
    Left,  // влево
    Right  // вправо
};

// Превращает направление в смещение по клетке.
inline Vec2i directionToVector(Direction direction) {
    switch (direction) {
    case Direction::Up:
        return { 0, -1 }; // на одну клетку вверх

    case Direction::Down:
        return { 0, 1 }; // на одну клетку вниз

    case Direction::Left:
        return { -1, 0 }; // на одну клетку влево

    case Direction::Right:
        return { 1, 0 }; // на одну клетку вправо
    }

    return { 0, 0 }; // запасной вариант
}

// Определяет направление по разнице координат.
inline Direction directionFromDelta(const Vec2i& delta, Direction fallback) {
    if (delta.x > 0) {
        return Direction::Right; // смещение вправо
    }

    if (delta.x < 0) {
        return Direction::Left; // смещение влево
    }

    if (delta.y > 0) {
        return Direction::Down; // смещение вниз
    }

    if (delta.y < 0) {
        return Direction::Up; // смещение вверх
    }

    return fallback; // если смещения нет
}