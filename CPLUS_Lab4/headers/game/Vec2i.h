#pragma once

// Целочисленная позиция на клеточном поле уровня.
// x — номер столбца, y — номер строки.
struct Vec2i {
    int x = 0; // координата по горизонтали
    int y = 0; // координата по вертикали
};

// Проверяет равенство двух позиций.
inline bool operator==(const Vec2i& left, const Vec2i& right) {
    return left.x == right.x && left.y == right.y;
}

// Проверяет неравенство двух позиций.
inline bool operator!=(const Vec2i& left, const Vec2i& right) {
    return !(left == right);
}