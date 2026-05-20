#include "graphics/PixelSprite.h"

#include <stdexcept> // std::runtime_error

#include <nanovg.h> // функции NanoVG


// Возвращает ширину спрайта в символах.
int PixelSprite::getWidth() const {
    if (pixels.empty()) {
        return 0;
    }

    return static_cast<int>(pixels.front().size());
}


// Рисует символьный спрайт через NanoVG.
void PixelSprite::draw(
    NVGcontext* context,
    float x,
    float y,
    float size,
    const std::unordered_map<char, Color>& palette
) const {
    // Пустой спрайт не рисуем.
    if (pixels.empty()) {
        return;
    }

    // Размер спрайта в символах.
    const int spriteWidth = getWidth();
    const int spriteHeight = getHeight();

    // Размер одного пикселя спрайта на экране.
    const float pixelWidth = size / static_cast<float>(spriteWidth);
    const float pixelHeight = size / static_cast<float>(spriteHeight);

    // Проходим по строкам спрайта.
    for (int row = 0; row < spriteHeight; ++row) {
        // Все строки должны иметь одинаковую ширину.
        if (static_cast<int>(pixels.at(row).size()) != spriteWidth) {
            throw std::runtime_error("PixelSprite has rows with different widths");
        }

        // Проходим по символам строки.
        for (int column = 0; column < spriteWidth; ++column) {
            // Берём символ текущего пикселя.
            const char symbol = pixels.at(row).at(static_cast<std::size_t>(column));

            // Ищем цвет символа в палитре.
            const auto colorIterator = palette.find(symbol);

            // Если символа нет в палитре, пропускаем его.
            if (colorIterator == palette.end()) {
                continue;
            }

            // Получаем цвет символа.
            const Color& color = colorIterator->second;

            // Полностью прозрачный пиксель не рисуем.
            if (color.a == 0) {
                continue;
            }

            // Рисуем маленький прямоугольник.
            nvgBeginPath(context);
            nvgRect(
                context,
                x + static_cast<float>(column) * pixelWidth,
                y + static_cast<float>(row) * pixelHeight,
                pixelWidth + 0.5f,
                pixelHeight + 0.5f
            );

            // Задаём цвет прямоугольника.
            nvgFillColor(context, nvgRGBA(
                clampColorChannel(color.r),
                clampColorChannel(color.g),
                clampColorChannel(color.b),
                clampColorChannel(color.a)
            ));

            // Заливаем прямоугольник.
            nvgFill(context);
        }
    }
}