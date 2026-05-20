#pragma once

#include <memory>  // std::unique_ptr
#include <string>  // std::string
#include <utility> // std::move
#include <vector>  // std::vector

#include "game/Direction.h"     // направления движения
#include "game/GameState.h"     // состояние игры
#include "game/InputState.h"    // ввод игрока
#include "game/Level.h"         // загруженный уровень
#include "game/TileType.h"      // тип клетки
#include "game/Vec2i.h"         // координаты клетки
#include "objects/GameObject.h" // базовый игровой объект

class GoldBag; // мешок золота
class Player;  // игрок

// GameWorld хранит карту уровня, объекты, очки, жизни и основные взаимодействия.
class GameWorld {
private:
    // Пути к файлам уровней.
    std::vector<std::string> levelFiles;

    // Индекс текущего уровня в списке levelFiles.
    int currentLevelIndex = 0;

    // Размеры текущей карты.
    int width = 15;
    int height = 10;

    // Номер текущего уровня для HUD.
    int levelNumber = 1;

    // Стартовая позиция игрока.
    Vec2i playerSpawn{ 7, 9 };

    // Точка появления врагов.
    Vec2i enemySpawn{ 14, 1 };

    // Сетка клеток уровня.
    std::vector<TileType> tiles;

    // Все активные объекты игрового мира.
    std::vector<std::unique_ptr<GameObject>> objects;

    // Быстрый указатель на игрока внутри objects.
    Player* player = nullptr;

    // Состояние ввода игрока.
    InputState inputState;

    // Текущее состояние игровой логики.
    GameState state = GameState::Playing;

    // Количество жизней.
    int lives = 3;

    // Деньги / очки игрока.
    int money = 0;

    // Сколько врагов ещё нужно породить.
    int enemyReserve = 0;

    // Таймер появления врагов.
    float enemySpawnTimer = 0.0f;

    // Активна ли анимация смерти игрока.
    bool playerDeathAnimationActive = false;

    // Таймер анимации смерти.
    float playerDeathTimer = 0.0f;

    // Позиция, где игрок погиб.
    Vec2i playerDeathPosition{ 0, 0 };

    // Таймер перехода между уровнями.
    float levelCompleteTimer = 0.0f;

public:
    // Создаёт игровой мир.
    GameWorld() = default;

    // Задаёт список файлов уровней.
    void setLevelFiles(std::vector<std::string> files) {
        levelFiles = std::move(files);
    }

    // Начинает новую игру.
    void startNewGame();

    // Полностью перезапускает текущий уровень.
    void restartCurrentLevel();

    // Обновляет игровой мир.
    void update(float deltaTime);

    // Возвращает ввод игрока.
    InputState& getInputState() {
        return inputState;
    }

    // Возвращает ввод игрока только для чтения.
    const InputState& getInputState() const {
        return inputState;
    }

    // Возвращает состояние игры.
    GameState getState() const {
        return state;
    }

    // Возвращает номер уровня.
    int getLevelNumber() const {
        return levelNumber;
    }

    // Возвращает количество жизней.
    int getLives() const {
        return lives;
    }

    // Возвращает количество денег.
    int getMoney() const {
        return money;
    }

    // Возвращает ширину карты.
    int getWidth() const {
        return width;
    }

    // Возвращает высоту карты.
    int getHeight() const {
        return height;
    }

    // Возвращает игрока.
    Player* getPlayer() {
        return player;
    }

    // Возвращает игрока только для чтения.
    const Player* getPlayer() const {
        return player;
    }

    // Возвращает список объектов только для чтения.
    const std::vector<std::unique_ptr<GameObject>>& getObjects() const {
        return objects;
    }

    // Проверяет, идёт ли анимация смерти игрока.
    bool isPlayerDeathAnimationActive() const {
        return playerDeathAnimationActive;
    }

    // Возвращает позицию взрыва игрока.
    Vec2i getPlayerDeathPosition() const {
        return playerDeathPosition;
    }

    // Проверяет, идёт ли переход между уровнями.
    bool isLevelCompleteTransitionActive() const {
        return state == GameState::LevelCompleted;
    }

    // Проверяет, находится ли клетка внутри карты.
    bool isInside(Vec2i position) const {
        return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
    }

    // Возвращает тип клетки.
    TileType getTile(Vec2i position) const {
        return tiles.at(indexOf(position));
    }

    // Меняет тип клетки.
    void setTile(Vec2i position, TileType tileType) {
        tiles.at(indexOf(position)) = tileType;
    }

    // Проверяет, является ли клетка тоннелем.
    bool isTunnel(Vec2i position) const {
        return isInside(position) && getTile(position) == TileType::Empty;
    }

    // Проверяет, есть ли в клетке мешок, который блокирует проход.
    bool hasBlockingBagAt(Vec2i position) const;

    // Ищет мешок золота в клетке.
    GoldBag* findGoldBagAt(Vec2i position);

    // Пытается толкнуть мешок золота.
    bool tryPushGoldBag(Vec2i bagPosition, Direction direction);

    // Проверяет, может ли мешок упасть в клетку.
    bool canBagFallInto(Vec2i position) const;

    // Собирает объекты в клетке игрока.
    void collectObjectsAtPlayer();

    // Собирает кучку золота в указанной клетке.
    void collectGoldPileAt(Vec2i position);

    // Создаёт огненный шар.
    void spawnFireball(Vec2i position, Direction direction);

    // Добавляет кучку золота.
    void addGoldPile(Vec2i position);

    // Убивает врагов в клетке.
    void killEnemiesAt(Vec2i position, int reward);

    // Проверяет, есть ли враг в клетке.
    bool hasEnemyAt(Vec2i position) const;

    // Раздавливает объекты в клетке.
    void crushObjectsAt(Vec2i position);

    // Запускает смерть игрока.
    void killPlayer();

    // Ищет следующий шаг врага к игроку.
    Vec2i findNextStepToPlayer(Vec2i startPosition) const;

private:
    // Загружает текущий уровень.
    void loadCurrentLevel(bool resetMoneyAndLives);

    // Загружает следующий уровень.
    void loadNextLevel();

    // Создаёт врага при необходимости.
    void spawnEnemyIfNeeded(float deltaTime);

    // Удаляет неактивные объекты.
    void removeInactiveObjects();

    // Проверяет, пройден ли уровень.
    void checkLevelCompleted();

    // Запускает переход после прохождения уровня.
    void startLevelCompleteTransition();

    // Обновляет таймер перехода уровня.
    void updateLevelCompleteTransition(float deltaTime);

    // Завершает переход уровня.
    void finishLevelCompleteTransition();

    // Обновляет анимацию смерти игрока.
    void updatePlayerDeathAnimation(float deltaTime);

    // Завершает анимацию смерти игрока.
    void finishPlayerDeathAnimation();

    // Переводит координаты клетки в индекс vector.
    int indexOf(Vec2i position) const;

    // Считает активные объекты заданного типа.
    int countActiveObjects(ObjectType objectType) const;
};