#include "objects/Nobbin.h"

#include "game/GameWorld.h" // игровой мир и поиск пути
#include "objects/Player.h" // проверка столкновения с игроком

// Обновляет врага.
void Nobbin::update(GameWorld& world, float deltaTime) {
    // Задержка между шагами врага.
    constexpr float moveDelay = 0.42f;

    // Накапливаем время.
    moveTimer += deltaTime;

    // Если время шага ещё не пришло, враг стоит.
    if (moveTimer < moveDelay) {
        return;
    }

    // Сбрасываем таймер шага.
    moveTimer = 0.0f;

    // Запоминаем старую позицию.
    const Vec2i oldPosition = getPosition();

    // Получаем следующую клетку по пути к игроку.
    const Vec2i nextPosition = world.findNextStepToPlayer(oldPosition);

    // Если пути нет или двигаться некуда, враг остаётся на месте.
    if (nextPosition == oldPosition) {
        return;
    }

    // Обновляем направление по разнице координат.
    direction = directionFromDelta(
        { nextPosition.x - oldPosition.x, nextPosition.y - oldPosition.y },
        direction
    );

    // Перемещаем врага.
    setPosition(nextPosition);

    // По условию кучку золота могут подобрать и монстры.
    world.collectGoldPileAt(nextPosition);

    // Если враг пришёл в клетку игрока, игрок погибает.
    const Player* player = world.getPlayer();

    if (player != nullptr && player->getPosition() == nextPosition) {
        world.killPlayer();
    }
}