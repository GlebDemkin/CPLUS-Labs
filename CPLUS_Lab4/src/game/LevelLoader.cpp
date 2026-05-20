#include "game/LevelLoader.h"

#include <fstream>   // std::ifstream
#include <memory>    // std::make_unique
#include <stdexcept> // std::runtime_error
#include <string>    // std::string

#include "objects/Emerald.h"  // изумруд
#include "objects/GoldBag.h"  // мешок золота
#include "objects/GoldPile.h" // кучка золота

// Загружает уровень из .gleb-файла.
Level LevelLoader::loadFromFile(const std::string& filename) const {
    // Открываем файл уровня.
    std::ifstream input(filename);

    // Проверяем, открылся ли файл.
    if (!input.is_open()) {
        throw std::runtime_error("Failed to open level file: " + filename);
    }

    // Создаём объект уровня.
    Level level;

    // Текущий считанный токен.
    std::string token;

    // Читаем файл по словам.
    while (input >> token) {
        // Комментарий в файле уровня.
        if (token == "#") {
            std::string ignoredLine;
            std::getline(input, ignoredLine);
        }

        // Номер уровня.
        else if (token == "LEVEL") {
            int number = 1;
            input >> number;
            level.setLevelNumber(number);
        }

        // Размер карты.
        else if (token == "SIZE") {
            int width = 0;
            int height = 0;
            input >> width >> height;
            level.setSize(width, height);
        }

        // Стартовая позиция игрока.
        else if (token == "PLAYER") {
            Vec2i position;
            input >> position.x >> position.y;
            level.setPlayerSpawn(position);
        }

        // Точка появления врагов.
        else if (token == "ENEMY_SPAWN") {
            Vec2i position;
            input >> position.x >> position.y;
            level.setEnemySpawn(position);
        }

        // Количество врагов.
        else if (token == "ENEMY_COUNT") {
            int count = 0;
            input >> count;
            level.setEnemyCount(count);
        }

        // Начало карты уровня.
        else if (token == "MAP") {
            // Читаем строки карты.
            for (int y = 0; y < level.getHeight(); ++y) {
                std::string row;
                input >> row;

                // Проверяем ширину строки.
                if (static_cast<int>(row.size()) != level.getWidth()) {
                    throw std::runtime_error("Invalid row width in level file: " + filename);
                }

                // Разбираем символы строки.
                for (int x = 0; x < level.getWidth(); ++x) {
                    const char symbol = row.at(static_cast<std::size_t>(x));
                    const Vec2i position{ x, y };

                    // Земля.
                    if (symbol == '#') {
                        level.setTile(position, TileType::Earth);
                    }

                    // Пустота / тоннель.
                    else if (symbol == '.') {
                        level.setTile(position, TileType::Empty);
                    }

                    // Игрок.
                    else if (symbol == 'P') {
                        level.setTile(position, TileType::Empty);
                        level.setPlayerSpawn(position);
                    }

                    // Изумруд в земле.
                    else if (symbol == 'E') {
                        level.setTile(position, TileType::Earth);
                        level.addObject(std::make_unique<Emerald>(position));
                    }

                    // Мешок золота в земле.
                    else if (symbol == '$') {
                        level.setTile(position, TileType::Earth);
                        level.addObject(std::make_unique<GoldBag>(position));
                    }

                    // Кучка золота в тоннеле.
                    else if (symbol == 'G') {
                        level.setTile(position, TileType::Empty);
                        level.addObject(std::make_unique<GoldPile>(position));
                    }

                    // Неизвестный символ.
                    else {
                        throw std::runtime_error(
                            "Unknown map symbol in level file: " + std::string(1, symbol)
                        );
                    }
                }
            }
        }

        // Конец файла уровня.
        else if (token == "END") {
            break;
        }

        // Неизвестная команда файла.
        else {
            throw std::runtime_error("Unexpected token in level file: " + token);
        }
    }

    // Возвращаем готовый уровень.
    return level;
}