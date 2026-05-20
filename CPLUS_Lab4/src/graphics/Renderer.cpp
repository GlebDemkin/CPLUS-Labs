#include "graphics/Renderer.h"

#include <algorithm> // std::min
#include <cstdio>    // std::snprintf

#include <nanovg.h> // функции NanoVG

#include "game/GameWorld.h"     // игровой мир
#include "objects/Emerald.h"    // изумруд
#include "objects/Fireball.h"   // огненный шар
#include "objects/GameObject.h" // базовый объект
#include "objects/GoldBag.h"    // мешок золота
#include "objects/GoldPile.h"   // кучка золота
#include "objects/Nobbin.h"     // враг
#include "objects/Player.h"     // игрок

// Создаёт отрисовщик.
Renderer::Renderer() = default;

// Рисует весь игровой экран.
void Renderer::drawGame(NVGcontext* context, const GameWorld& world, float screenWidth, float screenHeight) {
    // Заливаем весь экран чёрным фоном.
    nvgBeginPath(context);
    nvgRect(context, 0.0f, 0.0f, screenWidth, screenHeight);
    nvgFillColor(context, nvgRGBA(0, 0, 0, 255));
    nvgFill(context);

    // Рисуем верхнюю панель.
    drawHud(context, world, screenWidth);

    // Размеры отступов и области под игру.
    const float hudHeight = 72.0f;
    const float sidePadding = 40.0f;
    const float bottomPadding = 30.0f;

    // Доступная область под игровое поле.
    const float availableWidth = screenWidth - 2.0f * sidePadding;
    const float availableHeight = screenHeight - hudHeight - bottomPadding;

    // Размер клетки выбирается так, чтобы вся карта поместилась на экран.
    const float cellSize = std::min(
        availableWidth / static_cast<float>(world.getWidth()),
        availableHeight / static_cast<float>(world.getHeight())
    );

    // Размер игрового поля в пикселях.
    const float fieldWidth = cellSize * static_cast<float>(world.getWidth());
    const float fieldHeight = cellSize * static_cast<float>(world.getHeight());

    // Положение игрового поля на экране.
    const float fieldX = (screenWidth - fieldWidth) / 2.0f;
    const float fieldY = hudHeight + (availableHeight - fieldHeight) / 2.0f;

    // Рисуем землю и тоннели.
    drawLevel(context, world, fieldX, fieldY, cellSize);

    // Предметы рисуются перед персонажами.
    for (const auto& object : world.getObjects()) {
        if (!object->isActive()) {
            continue;
        }

        const ObjectType type = object->getType();

        if (type == ObjectType::Emerald || type == ObjectType::GoldBag || type == ObjectType::GoldPile) {
            drawObject(context, *object, fieldX, fieldY, cellSize);
        }
    }

    // Потом рисуем врагов.
    for (const auto& object : world.getObjects()) {
        if (object->isActive() && object->getType() == ObjectType::Nobbin) {
            drawObject(context, *object, fieldX, fieldY, cellSize);
        }
    }

    // Потом рисуем игрока, если сейчас не идёт взрыв.
    for (const auto& object : world.getObjects()) {
        if (object->isActive() && object->getType() == ObjectType::Player && !world.isPlayerDeathAnimationActive()) {
            drawObject(context, *object, fieldX, fieldY, cellSize);
        }
    }

    // Если игрок погибает, вместо машинки рисуется огонь.
    if (world.isPlayerDeathAnimationActive()) {
        const Vec2i deathPosition = world.getPlayerDeathPosition();
        const float deathX = fieldX + static_cast<float>(deathPosition.x) * cellSize;
        const float deathY = fieldY + static_cast<float>(deathPosition.y) * cellSize;

        drawSprite(context, sprites.getFireballSprite(), deathX, deathY, cellSize);
    }

    // Огненный шар рисуется поверх персонажей.
    for (const auto& object : world.getObjects()) {
        if (object->isActive() && object->getType() == ObjectType::Fireball) {
            drawObject(context, *object, fieldX, fieldY, cellSize);
        }
    }

    // Рисуем рамку игрового поля.
    nvgBeginPath(context);
    nvgRect(context, fieldX, fieldY, fieldWidth, fieldHeight);
    nvgStrokeColor(context, nvgRGBA(255, 220, 0, 255));
    nvgStrokeWidth(context, 2.0f);
    nvgStroke(context);

    // При прохождении уровня выводим центральную панель.
    if (world.getState() == GameState::LevelCompleted) {
        drawCenteredOverlayPanel(
            context,
            screenWidth,
            screenHeight,
            "УРОВЕНЬ ПРОЙДЕН",
            "Переход на следующий уровень..."
        );
    }
}

// Рисует верхнюю панель с информацией.
void Renderer::drawHud(NVGcontext* context, const GameWorld& world, float screenWidth) {
    // Фон панели.
    nvgBeginPath(context);
    nvgRect(context, 0.0f, 0.0f, screenWidth, 64.0f);
    nvgFillColor(context, nvgRGBA(18, 18, 22, 255));
    nvgFill(context);

    // Жёлтая нижняя линия.
    nvgBeginPath(context);
    nvgRect(context, 0.0f, 63.0f, screenWidth, 2.0f);
    nvgFillColor(context, nvgRGBA(255, 210, 0, 255));
    nvgFill(context);

    // Формируем строку интерфейса.
    char buffer[128];

    std::snprintf(
        buffer,
        sizeof(buffer),
        "УРОВЕНЬ: %02d        ЖИЗНИ: %d        ДЕНЬГИ: %d",
        world.getLevelNumber(),
        world.getLives(),
        world.getMoney()
    );

    // Выводим текст интерфейса.
    drawText(context, buffer, 35.0f, 22.0f, 26.0f);
}


// Рисует клетки карты.
void Renderer::drawLevel(NVGcontext* context, const GameWorld& world, float fieldX, float fieldY, float cellSize) {
    const float fieldWidth = cellSize * static_cast<float>(world.getWidth());
    const float fieldHeight = cellSize * static_cast<float>(world.getHeight());

    // Сначала одним прямоугольником заливаем всё поле цветом тоннеля.
    nvgBeginPath(context);
    nvgRect(context, fieldX, fieldY, fieldWidth, fieldHeight);
    nvgFillColor(context, nvgRGBA(3, 6, 18, 255));
    nvgFill(context);

    for (int y = 0; y < world.getHeight(); ++y) {
        for (int x = 0; x < world.getWidth(); ++x) {
            if (world.getTile({ x, y }) != TileType::Earth) {
                continue;
            }

            const float screenX = fieldX + static_cast<float>(x) * cellSize;
            const float screenY = fieldY + static_cast<float>(y) * cellSize;

            // Внешний тёмно-коричневый квадрат земли.
            nvgBeginPath(context);
            nvgRect(context, screenX, screenY, cellSize, cellSize);
            nvgFillColor(context, nvgRGBA(76, 42, 18, 255));
            nvgFill(context);

            // Внутренний коричневый квадрат.
            const float padding = cellSize * 0.08f;

            nvgBeginPath(context);
            nvgRect(
                context,
                screenX + padding,
                screenY + padding,
                cellSize - 2.0f * padding,
                cellSize - 2.0f * padding
            );
            nvgFillColor(context, nvgRGBA(125, 73, 28, 255));
            nvgFill(context);

            // Жёлтые квадратные камушки, чтобы земля не выглядела однообразно.
            const float dotSize = cellSize * 0.10f;

            nvgBeginPath(context);

            nvgRect(
                context,
                screenX + cellSize * 0.22f,
                screenY + cellSize * 0.26f,
                dotSize,
                dotSize
            );

            nvgRect(
                context,
                screenX + cellSize * 0.65f,
                screenY + cellSize * 0.38f,
                dotSize,
                dotSize
            );

            nvgRect(
                context,
                screenX + cellSize * 0.40f,
                screenY + cellSize * 0.70f,
                dotSize,
                dotSize
            );

            if ((x + y) % 2 == 0) {
                nvgRect(
                    context,
                    screenX + cellSize * 0.76f,
                    screenY + cellSize * 0.74f,
                    dotSize,
                    dotSize
                );
            }

            nvgFillColor(context, nvgRGBA(235, 185, 65, 210));
            nvgFill(context);
        }
    }
}

// Рисует один игровой объект.
void Renderer::drawObject(NVGcontext* context, const GameObject& object, float fieldX, float fieldY, float cellSize) {
    // Переводим позицию клетки в экранные координаты.
    const Vec2i position = object.getPosition();
    const float screenX = fieldX + static_cast<float>(position.x) * cellSize;
    const float screenY = fieldY + static_cast<float>(position.y) * cellSize;

    // Выбираем спрайт по типу объекта.
    switch (object.getType()) {
    case ObjectType::Player: {
        const Player& player = static_cast<const Player&>(object);
        drawSprite(context, sprites.getPlayerSprite(player.getDirection()), screenX, screenY, cellSize);
        break;
    }

    case ObjectType::Nobbin: {
        const Nobbin& nobbin = static_cast<const Nobbin&>(object);
        drawSprite(context, sprites.getNobbinSprite(nobbin.getDirection()), screenX, screenY, cellSize);
        break;
    }

    case ObjectType::Emerald:
        drawSprite(context, sprites.getEmeraldSprite(), screenX, screenY, cellSize);
        break;

    case ObjectType::GoldBag:
        drawSprite(context, sprites.getGoldBagSprite(), screenX, screenY, cellSize);
        break;

    case ObjectType::GoldPile:
        drawSprite(context, sprites.getGoldPileSprite(), screenX, screenY, cellSize);
        break;

    case ObjectType::Fireball:
        drawSprite(context, sprites.getFireballSprite(), screenX, screenY, cellSize);
        break;
    }
}

// Рисует пиксельный спрайт.
void Renderer::drawSprite(NVGcontext* context, const PixelSprite& sprite, float x, float y, float size) {
    sprite.draw(context, x, y, size, sprites.getPalette());
}

// Рисует текст.
void Renderer::drawText(NVGcontext* context, const char* text, float x, float y, float size) {
    nvgFontFace(context, "sans");
    nvgFontSize(context, size);
    nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(context, nvgRGBA(255, 220, 0, 255));
    nvgText(context, x, y, text, nullptr);
}

// Рисует затемнённую центральную панель.
void Renderer::drawCenteredOverlayPanel(
    NVGcontext* context,
    float screenWidth,
    float screenHeight,
    const char* title,
    const char* subtitle
) {
    // Затемняем фон.
    nvgBeginPath(context);
    nvgRect(context, 0.0f, 0.0f, screenWidth, screenHeight);
    nvgFillColor(context, nvgRGBA(0, 0, 0, 170));
    nvgFill(context);

    // Размеры панели.
    const float panelWidth = 620.0f;
    const float panelHeight = 210.0f;
    const float panelX = (screenWidth - panelWidth) / 2.0f;
    const float panelY = (screenHeight - panelHeight) / 2.0f;

    // Фон панели.
    nvgBeginPath(context);
    nvgRoundedRect(context, panelX, panelY, panelWidth, panelHeight, 18.0f);
    nvgFillColor(context, nvgRGBA(25, 22, 18, 245));
    nvgFill(context);

    // Рамка панели.
    nvgBeginPath(context);
    nvgRoundedRect(context, panelX, panelY, panelWidth, panelHeight, 18.0f);
    nvgStrokeColor(context, nvgRGBA(255, 205, 0, 255));
    nvgStrokeWidth(context, 3.0f);
    nvgStroke(context);

    // Текст панели.
    nvgFontFace(context, "sans");
    nvgTextAlign(context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    nvgFontSize(context, 36.0f);
    nvgFillColor(context, nvgRGBA(255, 220, 0, 255));
    nvgText(context, screenWidth / 2.0f, panelY + 76.0f, title, nullptr);

    nvgFontSize(context, 24.0f);
    nvgFillColor(context, nvgRGBA(235, 235, 235, 255));
    nvgText(context, screenWidth / 2.0f, panelY + 136.0f, subtitle, nullptr);
}