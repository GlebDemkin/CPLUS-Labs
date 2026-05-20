#include "objects/Fireball.h"

#include "game/GameWorld.h" // игровой мир

// Создаёт огненный шар.
Fireball::Fireball(Vec2i startPosition, Direction startDirection)
    : GameObject(startPosition), direction(startDirection) {
}

// Обновляет полёт огненного шара.
void Fireball::update(GameWorld& world, float deltaTime) {
    // Задержка между шагами огненного шара.
    constexpr float moveDelay = 0.07f;

    // Накапливаем время.
    moveTimer += deltaTime;

    // Если время шага ещё не пришло, ничего не делаем.
    if (moveTimer < moveDelay) {
        return;
    }

    // Сбрасываем таймер шага.
    moveTimer = 0.0f;

    // Получаем смещение по направлению.
    const Vec2i offset = directionToVector(direction);

    // Вычисляем следующую клетку.
    const Vec2i target{
        getPosition().x + offset.x,
        getPosition().y + offset.y
    };

    // Если шар вылетел за карту, удаляем его.
    if (!world.isInside(target)) {
        deactivate();
        return;
    }

    // Если шар упёрся в землю или мешок, удаляем его.
    if (world.getTile(target) == TileType::Earth || world.hasBlockingBagAt(target)) {
        deactivate();
        return;
    }

    // Если шар попал во врага, уничтожаем врага и удаляем шар.
    if (world.hasEnemyAt(target)) {
        world.killEnemiesAt(target, 200);
        deactivate();
        return;
    }

    // Если путь свободен, перемещаем шар.
    setPosition(target);
}

// Возвращает тип объекта.
ObjectType Fireball::getType() const {
    return ObjectType::Fireball;
}

// Возвращает направление полёта.
Direction Fireball::getDirection() const {
    return direction;
}