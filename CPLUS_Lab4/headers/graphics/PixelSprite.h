#pragma once

#include <string>        // std::string для строк спрайта
#include <unordered_map> // std::unordered_map для палитры символов
#include <utility>       // std::move для передачи строк спрайта
#include <vector>        // std::vector для хранения строк спрайта

#include "graphics/Color.h" // цвет RGBA

struct NVGcontext; // контекст NanoVG

// Пиксельный спрайт хранится как матрица символов.
// Каждый символ переводится в цвет через палитру и рисуется маленьким квадратом.
class PixelSprite {
private:
    // Строки спрайта.
    std::vector<std::string> pixels;

public:
    // Создаёт пустой спрайт.
    PixelSprite() = default;

    // Создаёт спрайт из строк символов.
    explicit PixelSprite(std::vector<std::string> spritePixels)
        : pixels(std::move(spritePixels)) {
    }

    // Возвращает ширину спрайта в символах.
    int getWidth() const;

    // Возвращает высоту спрайта в символах.
    int getHeight() const {
        return static_cast<int>(pixels.size());
    }

    // Возвращает строки спрайта.
    const std::vector<std::string>& getPixels() const {
        return pixels;
    }

    // Рисует спрайт через NanoVG.
    void draw(
        NVGcontext* context,
        float x,
        float y,
        float size,
        const std::unordered_map<char, Color>& palette
    ) const;
};