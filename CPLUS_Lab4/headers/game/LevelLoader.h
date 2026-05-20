#pragma once

#include <string> // std::string для имени файла уровня

#include "game/Level.h" // класс уровня

// LevelLoader читает игровой уровень из текстового файла .gleb.
class LevelLoader {
public:
    // Загружает уровень из файла и возвращает готовый объект Level.
    Level loadFromFile(const std::string& filename) const;
};