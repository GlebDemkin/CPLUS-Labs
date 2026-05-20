#pragma once

#include <algorithm> // std::clamp
#include <istream>   // std::istream

// Цвет в формате RGBA. Каждый канал обычно находится в диапазоне 0..255.
struct Color {
    int r = 255; // красный канал
    int g = 255; // зелёный канал
    int b = 255; // синий канал
    int a = 255; // прозрачность
};

// Позволяет читать цвет из .gleb-файла в формате: r g b a.
inline std::istream& operator>>(std::istream& input, Color& color) {
    input >> color.r >> color.g >> color.b >> color.a;
    return input;
}

// Ограничивает значение цветового канала допустимым диапазоном.
inline int clampColorChannel(int value) {
    return std::clamp(value, 0, 255);
}