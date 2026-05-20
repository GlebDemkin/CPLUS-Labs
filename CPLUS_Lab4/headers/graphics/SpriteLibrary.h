#pragma once

#include <unordered_map> // std::unordered_map для палитры цветов

#include "game/Direction.h"          // направление объекта
#include "graphics/Color.h"          // цвет RGBA
#include "graphics/PixelSprite.h"    // символьный пиксельный спрайт

// Набор спрайтов для четырёх направлений.
struct DirectionalSprites {
    PixelSprite up;    // спрайт вверх
    PixelSprite down;  // спрайт вниз
    PixelSprite left;  // спрайт влево
    PixelSprite right; // спрайт вправо
};

// SpriteLibrary хранит все программные пиксельные спрайты игры.
class SpriteLibrary {
private:
    // Палитра: символ спрайта -> цвет.
    std::unordered_map<char, Color> palette;

    // Спрайт изумруда.
    PixelSprite emeraldSprite;

    // Спрайт мешка золота.
    PixelSprite goldBagSprite;

    // Спрайт кучки золота.
    PixelSprite goldPileSprite;

    // Спрайт огненного шара.
    PixelSprite fireballSprite;

    // Спрайты игрока по направлениям.
    DirectionalSprites playerSprites;

    // Спрайты врага по направлениям.
    DirectionalSprites nobbinSprites;

public:
    // Создаёт библиотеку и заполняет все спрайты.
    SpriteLibrary();

    // Возвращает палитру.
    const std::unordered_map<char, Color>& getPalette() const {
        return palette;
    }

    // Возвращает спрайт изумруда.
    const PixelSprite& getEmeraldSprite() const {
        return emeraldSprite;
    }

    // Возвращает спрайт мешка золота.
    const PixelSprite& getGoldBagSprite() const {
        return goldBagSprite;
    }

    // Возвращает спрайт кучки золота.
    const PixelSprite& getGoldPileSprite() const {
        return goldPileSprite;
    }

    // Возвращает спрайт огня.
    const PixelSprite& getFireballSprite() const {
        return fireballSprite;
    }

    // Возвращает спрайт игрока по направлению.
    const PixelSprite& getPlayerSprite(Direction direction) const;

    // Возвращает спрайт врага по направлению.
    const PixelSprite& getNobbinSprite(Direction direction) const;
};