#pragma once

#include <algorithm>
#include <istream>

// Цвет в формате RGBA.
// Каждый канал обычно находится в диапазоне 0..255.
struct Color
{
    int r = 255; // красный канал
    int g = 255; // зелёный канал
    int b = 255; // синий канал
    int a = 255; // прозрачность
};

// inline разрешает определять функцию в заголовочном файле, который подключается в разные .cpp-файлы.

// Перегрузка оператора >> позволяет читать объект Color напрямую из потока.
// Например, строка "60 110 220 255" будет записана в r, g, b, a.
//
// std::istream& input — поток ввода, например файл.
//
// Возвращаем input, чтобы работали цепочки чтения: input >> color1 >> color2.
inline std::istream& operator>>(std::istream& input, Color& color)
{
    input >> color.r >> color.g >> color.b >> color.a;
    return input;
}

// Нужно перед передачей цвета в NanoVG.
inline int clampColorChannel(int value)
{
    return std::clamp(value, 0, 255); // ограничивает значение цветого канала диапазоном от 0 до 255.
}