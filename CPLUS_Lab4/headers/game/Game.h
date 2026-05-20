#pragma once

#include <vector>
#include <string>

#include "game/Direction.h"     // направления движения
#include "game/GameWorld.h"     // игровой мир
#include "graphics/Renderer.h"  // отрисовщик игры

struct NVGcontext;  // контекст NanoVG

// Game — простой фасад игровой части.
class Game {
private:
    // Внутренний игровой мир.
    GameWorld world;

public:
    // Создаёт игру и задаёт список уровней.
    Game() {
        world.setLevelFiles({
            "resources/levels/level01.gleb",
            "resources/levels/level02.gleb",
            "resources/levels/level03.gleb"
        });
    }

    // Начинает новую игру с первого уровня.
    void startNewGame() {
        world.startNewGame();
    }

    // Полностью перезапускает текущий уровень.
    void restartCurrentLevel() {
        world.restartCurrentLevel();
    }

    // Обновляет игровой мир.
    void update(float deltaTime) {
        world.update(deltaTime);
    }

    // Рисует игровой мир.
    void draw(Renderer& renderer, NVGcontext* context, float screenWidth, float screenHeight) const {
        renderer.drawGame(context, world, screenWidth, screenHeight);
    }

    // Запрашивает движение игрока.
    void requestMove(Direction direction) {
        world.getInputState().requestMove(direction);
    }

    // Запрашивает выстрел игрока.
    void requestFire() {
        world.getInputState().requestFire();
    }

    // Возвращает состояние игры.
    GameState getState() const {
        return world.getState();
    }

    // Возвращает номер текущего уровня.
    int getLevelNumber() const {
        return world.getLevelNumber();
    }

    // Возвращает количество жизней.
    int getLives() const {
        return world.getLives();
    }

    // Возвращает количество денег.
    int getMoney() const {
        return world.getMoney();
    }
};