#include "objects/GoldBag.h"

#include "game/GameWorld.h" // игровой мир и взаимодействия с объектами

    
// Обновляет состояние мешка.
void GoldBag::update(GameWorld& world, float deltaTime) {
    // Задержка между шагами падения.
    constexpr float fallDelay = 0.16f;

    // Накапливаем время.
    fallTimer += deltaTime;

    // Если время падения ещё не пришло, ничего не делаем.
    if (fallTimer < fallDelay) {
        return;
    }

    // Сбрасываем таймер шага.
    fallTimer = 0.0f;

    // Текущая позиция мешка.
    const Vec2i current = getPosition();

    // Клетка под мешком.
    const Vec2i below{ current.x, current.y + 1 };

    // Если мешок может упасть вниз.
    if (world.canBagFallInto(below)) {
        falling = true;
        ++fallDistance;

        // Раздавливаем игрока или врага под мешком.
        world.crushObjectsAt(below);

        // Перемещаем мешок вниз.
        setPosition(below);
        return;
    }

    // Если падать нельзя, останавливаем падение.
    stopFalling(world);
}

// Останавливает падение мешка.
void GoldBag::stopFalling(GameWorld& world) {
    // Если мешок не падал, ничего не делаем.
    if (!falling) {
        return;
    }

    // Если мешок упал больше чем на одну клетку, он разбивается.
    if (fallDistance > 1) {
        world.addGoldPile(getPosition());
        deactivate();
    }

    // Сбрасываем состояние падения.
    falling = false;
    fallDistance = 0;
}