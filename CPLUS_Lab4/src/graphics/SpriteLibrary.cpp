#include "graphics/SpriteLibrary.h"

#include <algorithm> // std::reverse
#include <string>    // std::string
#include <vector>    // std::vector

// Создаёт спрайт из строк.
static PixelSprite makeSprite(std::initializer_list<const char*> rows) {
    std::vector<std::string> pixels;

    for (const char* row : rows) {
        pixels.emplace_back(row);
    }

    return PixelSprite(std::move(pixels));
}

// Отражает спрайт по горизонтали.
static PixelSprite mirrorHorizontal(const PixelSprite& source) {
    std::vector<std::string> mirrored = source.getPixels();

    for (std::string& row : mirrored) {
        std::reverse(row.begin(), row.end());
    }

    return PixelSprite(std::move(mirrored));
}

// Заполняет палитру и все игровые спрайты.
SpriteLibrary::SpriteLibrary() {
    // Палитра символов.
    palette = {
        {'.', {0, 0, 0, 0}},
        {'T', {0, 0, 0, 255}},
        {'K', {20, 18, 18, 255}},
        {'O', {236, 77, 22, 255}},
        {'Y', {255, 210, 35, 255}},
        {'y', {181, 105, 20, 255}},
        {'$', {25, 20, 16, 255}},
        {'R', {220, 34, 30, 255}},
        {'r', {140, 20, 20, 255}},
        {'G', {35, 220, 55, 255}},
        {'g', {16, 130, 36, 255}},
        {'L', {78, 255, 90, 255}},
        {'C', {95, 205, 235, 255}},
        {'W', {245, 245, 225, 255}},
        {'S', {95, 95, 105, 255}},
        {'F', {255, 60, 20, 255}},
        {'A', {255, 150, 28, 255}}
    };

    // Машина игрока вправо.
    PixelSprite playerRight = makeSprite({
        "................",
        ".....YYY........",
        "....YRRRY.......",
        "...YRCCCRY......",
        "..YRCCCCCRY.....",
        ".YRCCCCCCRYg....",
        ".RRRRRRRRRRGGg..",
        "RRRRRRRRRSGGGGGG",
        "RRRRRRRRSSgGGg..",
        ".RRRRRRSSGGg....",
        "..SSS...SSS.....",
        ".SKKKS.SKKKS....",
        ".SKKKS.SKKKS....",
        "..SSS...SSS.....",
        "................",
        "................"
    });

    // Левый спрайт получаем зеркально.
    playerSprites.right = playerRight;
    playerSprites.left = mirrorHorizontal(playerRight);

    // Машина игрока вверх: бур сверху.
    playerSprites.up = makeSprite({
        "................",
        ".......G........",
        "......GGG.......",
        "......GgG.......",
        ".....SSSSS......",
        "....YRRRRRY.....",
        "...YRCCCCCRY....",
        "...RRCCCCCRR....",
        "...RRYyyyYRR....",
        "...RRYyyyYRR....",
        "...RRYYYYYRR....",
        "...RRRRRRRRR....",
        "...SSSS.SSSS....",
        "....SS...SS.....",
        "................",
        "................"
    });

    // Машина игрока вниз: бур снизу.
    playerSprites.down = makeSprite({
        "................",
        "................",
        "....SS...SS.....",
        "...SSSS.SSSS....",
        "...RRRRRRRRR....",
        "...RRYYYYYRR....",
        "...RRYyyyYRR....",
        "...RRYyyyYRR....",
        "...RRCCCCCRR....",
        "...YRCCCCCRY....",
        "....YRRRRRY.....",
        ".....SSSSS......",
        "......GgG.......",
        "......GGG.......",
        ".......G........",
        "................"
    });

    // Враг вправо.
    PixelSprite nobbinRight = makeSprite({
        "................",
        ".....YgYY.......",
        "....YGgGGY......",
        "...YGGGWKGG.....",
        "....YGGGrr......",
        ".....GGGG.......",
        "......GG........",
        "....RRRRRR......",
        "...RRRRRRRR.....",
        "...rrrrrrrr.....",
        "....RRRRRr......",
        "...RR...RR......",
        "..RR.....RR.....",
        "................",
        "................",
        "................"
    });

    // Левый спрайт получаем зеркально.
    nobbinSprites.right = nobbinRight;
    nobbinSprites.left = mirrorHorizontal(nobbinRight);

    // Враг вниз: видны глаза и рот.
    nobbinSprites.down = makeSprite({
        "................",
        "...g.YYYYY.g....",
        "...gYGGGGGYg....",
        "....yWKGKWy.....",
        "....yGrrrGy.....",
        ".....GGGGG......",
        "......gGg.......",
        "....RRRRRRR.....",
        "..RRRRRRRRRRR...",
        ".R..rrrrrrr..R..",
        "....RRRrRRR.....",
        "...RR..r..RR....",
        "..RR.......RR...",
        "................",
        "................",
        "................"
    });

    // Враг вверх: виден затылок, лица нет.
    nobbinSprites.up = makeSprite({
        "................",
        "...g.YYYYY.g....",
        "...gYGGGGGYg....",
        "....yGGGGGy.....",
        "....yGGGGGy.....",
        ".....GGGGG......",
        "......GGG.......",
        "....RRRRRRR.....",
        "..RRRRRRRRRRR...",
        ".R..rrrrrrr..R..",
        "....RRRrRRR.....",
        "...RR.....RR....",
        "..RR.......RR...",
        "................",
        "................",
        "................"
    });

    // Изумруд.
    emeraldSprite = makeSprite({
        "................",
        ".......G........",
        "......GGG.......",
        ".....GLLLG......",
        "....GLLLLLG.....",
        "...GLLWLLLG.....",
        "..GLLWLLLLLG....",
        ".GLLLLLLLLLLG...",
        "..GLLLLLLLLG....",
        "...GLLLLLLG.....",
        "....GLLLLG......",
        ".....GLLG.......",
        "......GG........",
        ".......G........",
        "................",
        "................"
    });

    // Мешок золота со знаком доллара.
    goldBagSprite = makeSprite({
        "................",
        "......yyyy......",
        ".....yYYYYy.....",
        "....yYYYYYYy....",
        "......yyyy......",
        ".....YyyyyY.....",
        "....YYYYYYYY....",
        "...YYYY$$YYYY...",
        "..YYYY$Y$YYYYY..",
        "..YYYYY$YYYYY...",
        "..YYYY$Y$YYYYY..",
        "...YYYYYYYYYY...",
        "....yYYYYYYy....",
        ".....yyyyyy.....",
        "................",
        "................"
    });

    // Кучка золота из монет.
    goldPileSprite = makeSprite({
        "................",
        "................",
        "................",
        "................",
        ".......YY.......",
        "......YWWY......",
        "....YYyyyyYY....",
        "...YWWYyyYWWY...",
        "..YYyyYYYYyyYY..",
        ".YWWYyYWWYyYWWY.",
        ".YyyYYYYYYYYyyY.",
        "..YWWYyyYWWYyY..",
        "...YYYYYYYYYY...",
        "....yyyyyyyy....",
        "................",
        "................"
    });

    // Огонь / взрыв / fireball.
    fireballSprite = makeSprite({
        "................",
        "................",
        "................",
        ".......F........",
        ".....FAAAF......",
        "....FAAAAF......",
        "...FAAWWAAF.....",
        "...FAAWWAAAF....",
        "..FAAAWWAAAF....",
        "...FAAAAAAF.....",
        "....FAAAF.......",
        ".....FFF........",
        "................",
        "................",
        "................",
        "................"
    });
}

// Возвращает спрайт игрока по направлению.
const PixelSprite& SpriteLibrary::getPlayerSprite(Direction direction) const {
    switch (direction) {
    case Direction::Up:
        return playerSprites.up;
    case Direction::Down:
        return playerSprites.down;
    case Direction::Left:
        return playerSprites.left;
    case Direction::Right:
        return playerSprites.right;
    }

    return playerSprites.right;
}

// Возвращает спрайт врага по направлению.
const PixelSprite& SpriteLibrary::getNobbinSprite(Direction direction) const {
    switch (direction) {
    case Direction::Up:
        return nobbinSprites.up;
    case Direction::Down:
        return nobbinSprites.down;
    case Direction::Left:
        return nobbinSprites.left;
    case Direction::Right:
        return nobbinSprites.right;
    }

    return nobbinSprites.down;
}
