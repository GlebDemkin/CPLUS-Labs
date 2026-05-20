#pragma once

#include <memory>    // std::unique_ptr
#include <stdexcept> // std::runtime_error
#include <utility>   // std::move
#include <vector>    // std::vector

#include "game/TileType.h"      // тип клетки уровня
#include "game/Vec2i.h"         // координаты клетки
#include "objects/GameObject.h" // базовый игровой объект

// Level хранит данные, прочитанные из внешнего .gleb-файла уровня.
// Он не управляет игровым процессом, а только описывает стартовое состояние карты.
class Level {
private:
    // Размер карты уровня.
    int width = 15;
    int height = 10;

    // Номер уровня.
    int levelNumber = 1;

    // Количество врагов на уровне.
    int enemyCount = 1;

    // Начальная позиция игрока.
    Vec2i playerSpawn{ 7, 9 };

    // Точка появления врагов.
    Vec2i enemySpawn{ 14, 1 };

    // Сетка клеток уровня: земля или пустота.
    std::vector<TileType> tiles;

    // Объекты, созданные при загрузке уровня.
    std::vector<std::unique_ptr<GameObject>> objects;

public:
    // Создаёт уровень со стандартным размером.
    Level() {
        setSize(width, height);
    }

    // Возвращает ширину уровня.
    int getWidth() const {
        return width;
    }

    // Возвращает высоту уровня.
    int getHeight() const {
        return height;
    }

    // Возвращает номер уровня.
    int getLevelNumber() const {
        return levelNumber;
    }

    // Возвращает количество врагов.
    int getEnemyCount() const {
        return enemyCount;
    }

    // Возвращает начальную позицию игрока.
    Vec2i getPlayerSpawn() const {
        return playerSpawn;
    }

    // Возвращает точку появления врагов.
    Vec2i getEnemySpawn() const {
        return enemySpawn;
    }

    // Возвращает тип клетки.
    TileType getTile(Vec2i position) const {
        return tiles.at(indexOf(position));
    }

    // Меняет тип клетки.
    void setTile(Vec2i position, TileType tileType) {
        tiles.at(indexOf(position)) = tileType;
    }

    // Передаёт объекты из Level в GameWorld.
    std::vector<std::unique_ptr<GameObject>> takeObjects() {
        return std::move(objects);
    }

    // Устанавливает размер карты и заполняет её землёй.
    void setSize(int newWidth, int newHeight) {
        if (newWidth <= 0 || newHeight <= 0) {
            throw std::runtime_error("Level size must be positive");
        }

        width = newWidth;
        height = newHeight;

        tiles.assign(
            static_cast<std::size_t>(width * height),
            TileType::Earth
        );
    }

    // Устанавливает номер уровня.
    void setLevelNumber(int newLevelNumber) {
        levelNumber = newLevelNumber;
    }

    // Устанавливает количество врагов.
    void setEnemyCount(int newEnemyCount) {
        enemyCount = newEnemyCount;
    }

    // Устанавливает начальную позицию игрока.
    void setPlayerSpawn(Vec2i position) {
        playerSpawn = position;
    }

    // Устанавливает точку появления врагов.
    void setEnemySpawn(Vec2i position) {
        enemySpawn = position;
    }

    // Добавляет стартовый объект уровня.
    void addObject(std::unique_ptr<GameObject> object) {
        objects.push_back(std::move(object));
    }

private:
    // Переводит координаты клетки {x, y} в индекс vector,
    // потому что карта логически двумерная, а хранится одномерно.
    int indexOf(Vec2i position) const {
        if (
            position.x < 0 ||
            position.x >= width ||
            position.y < 0 ||
            position.y >= height
        ) {
            throw std::runtime_error("Level position is outside of map");
        }

        return position.y * width + position.x;
    }
};