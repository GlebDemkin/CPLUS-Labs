#pragma once

#include "graphics/SpriteLibrary.h" // библиотека всех спрайтов игры

struct NVGcontext; // контекст NanoVG
class GameWorld;   // игровой мир
class GameObject;  // базовый игровой объект

// Renderer отвечает только за отображение. Игровая логика находится в GameWorld и объектах.
class Renderer {
private:
    // Хранилище всех пиксельных спрайтов и палитры.
    SpriteLibrary sprites;

public:
    // Создаёт объект отрисовщика.
    Renderer();

    // Рисует весь игровой экран.
    void drawGame(NVGcontext* context, const GameWorld& world, float screenWidth, float screenHeight);

private:
    // Рисует верхнюю панель: уровень, жизни, деньги.
    void drawHud(NVGcontext* context, const GameWorld& world, float screenWidth);

    // Рисует карту уровня: землю, тоннели, объекты и игровые наложения.
    void drawLevel(NVGcontext* context, const GameWorld& world, float fieldX, float fieldY, float cellSize);

    // Рисует один игровой объект по его типу.
    void drawObject(NVGcontext* context, const GameObject& object, float fieldX, float fieldY, float cellSize);

    // Рисует один пиксельный спрайт.
    void drawSprite(NVGcontext* context, const PixelSprite& sprite, float x, float y, float size);

    // Рисует текст.
    void drawText(NVGcontext* context, const char* text, float x, float y, float size);

    // Рисует затемнённую центральную панель с заголовком и подписью.
    void drawCenteredOverlayPanel(
        NVGcontext* context,
        float screenWidth,
        float screenHeight,
        const char* title,
        const char* subtitle
    );
};